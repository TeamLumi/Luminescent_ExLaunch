include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/switch/SwitchTools.cmake)
set(__SWITCH_TOOLS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

add_custom_target(all_modules)

function(add_module index name)
    cmake_parse_arguments(MODULE_ARGS "" "" "INCLUDE;SOURCE;SOURCE_SHALLOW;EMBED_BINARIES" ${ARGN})

    set(ALL_INCLUDE)
    foreach(include ${MODULE_ARGS_INCLUDE})
        list(APPEND ALL_INCLUDE ${include})
    endforeach()

    set(ALL_SOURCE)
    foreach(source ${MODULE_ARGS_SOURCE})
        file(GLOB_RECURSE SOURCE_FILES ${source}/*.c ${source}/*.cpp ${source}/*.s)
        list(APPEND ALL_INCLUDE ${source})
        list(APPEND ALL_SOURCE ${SOURCE_FILES})
    endforeach()

    foreach(source ${MODULE_ARGS_SOURCE_SHALLOW})
        file(GLOB SOURCE_FILES ${source}/*.c ${source}/*.cpp ${source}/*.s)
        list(APPEND ALL_INCLUDE ${source})
        list(APPEND ALL_SOURCE ${SOURCE_FILES})
    endforeach()

    set(ALL_EMBED_BINARIES)
    foreach(embed_binary ${MODULE_ARGS_EMBED_BINARIES})
        list(APPEND ALL_EMBED_BINARIES ${embed_binary})
    endforeach()

    add_custom_target(${name})
    add_dependencies(all_modules ${name})

    set_property(
        TARGET ${name}
        PROPERTY MODULE_INCLUDE ${ALL_INCLUDE}
    )
    set_property(
        TARGET ${name}
        PROPERTY MODULE_SOURCE ${ALL_SOURCE}
    )
    set_property(
        TARGET ${name}
        PROPERTY EMBED_BINARIES ${ALL_EMBED_BINARIES}
    )
    set_property(
        TARGET ${name}
        PROPERTY SUBSDK "subsdk${index}"
    )

    create_releases_main(${name})
endfunction()

function(add_module_binary module binary)
    cmake_parse_arguments(BINARY_ARGS "" "LINKER_SCRIPT;SPECS_TEMPLATE" "INCLUDE;SOURCE;EMBED_BINARIES" ${ARGN})

    if (NOT DEFINED BINARY_ARGS_LINKER_SCRIPT)
        set(BINARY_ARGS_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/module/module.ld)
    endif()

    if (NOT DEFINED BINARY_ARGS_SPECS_TEMPLATE)
        set(BINARY_ARGS_SPECS_TEMPLATE ${CMAKE_CURRENT_SOURCE_DIR}/module/module.specs.template)
    endif()

    if (NOT TARGET ${module})
        message(FATAL_ERROR "Module ${module} not found")
    endif()

    set(ALL_INCLUDE)
    foreach(include ${BINARY_ARGS_INCLUDE})
        list(APPEND ALL_INCLUDE ${include})
    endforeach()

    set(ALL_SOURCE)
    foreach(source ${BINARY_ARGS_SOURCE})
        file(GLOB_RECURSE SOURCE_FILES ${source}/*.c ${source}/*.cpp ${source}/*.asm)
        list(APPEND ALL_SOURCE ${SOURCE_FILES})
    endforeach()

    set(ALL_EMBED_BINARIES)
    foreach(embed_binary ${BINARY_ARGS_EMBED_BINARIES})
        list(APPEND ALL_EMBED_BINARIES ${embed_binary})
    endforeach()

    set(binary ${module}_${binary})

    get_target_property(PARENT_INCLUDE ${module} MODULE_INCLUDE)
    get_target_property(PARENT_SOURCE ${module} MODULE_SOURCE)
    get_target_property(PARENT_EMBED_BINARIES ${module} EMBED_BINARIES)

    add_executable(${binary} ${ALL_SOURCE} ${PARENT_SOURCE})
    target_include_directories(${binary} PUBLIC ${ALL_INCLUDE} ${PARENT_INCLUDE})
    target_embed_binaries(${binary} ${ALL_EMBED_BINARIES} ${PARENT_EMBED_BINARIES})

    string(LENGTH ${module} MODULE_NAME_LEN)
    target_compile_definitions(
        ${binary}
        PUBLIC
            EXL_PROGRAM_ID=0
            NNSDK=1
            EXL_LOAD_KIND=Module
            EXL_LOAD_KIND_ENUM=EXL_LOAD_KIND_MODULE
            MODULE_NAME="${module}"
            MODULE_NAME_LEN=${MODULE_NAME_LEN}
            LOGGER_IP="${LOGGER_IP}"
            LOGGER_PORT=${LOGGER_PORT}
    )

    set(SUBSDK_LD_PATH ${BINARY_ARGS_LINKER_SCRIPT})
    if (MINGW OR WIN32)
        string(REGEX REPLACE "^/([a-zA-Z])/" "\\1:/" SUBSDK_LD_PATH "${SUBSDK_LD_PATH}")
    endif ()

    configure_file(
            ${BINARY_ARGS_SPECS_TEMPLATE} ${CMAKE_CURRENT_BINARY_DIR}/${binary}.specs
    )

    get_target_property(SUBSDK_NAME ${module} SUBSDK)
    create_nso(${binary} ${SUBSDK_NAME})

    add_dependencies(${module} ${binary}_nso)
endfunction()

function(add_module_package module binary variant title_id game)
    cmake_parse_arguments(PACKAGE_ARGS "" "NPDM_TEMPLATE" "" ${ARGN})
    if (NOT DEFINED PACKAGE_ARGS_NPDM_TEMPLATE)
        set(PACKAGE_ARGS_NPDM_TEMPLATE ${CMAKE_CURRENT_SOURCE_DIR}/module/npdm.json.template)
    endif()
    set(binary ${module}_${binary})
    set(variant ${module}_${variant})
    if (NOT TARGET ${binary}_nso)
        message(FATAL_ERROR "Module binary ${binary} not found")
    endif()

    string(TOLOWER "0x${title_id}" CONFIG_TITLE_ID)
    configure_file(${PACKAGE_ARGS_NPDM_TEMPLATE} ${CMAKE_CURRENT_BINARY_DIR}/${variant}.npdm.json)
    create_npdm(${variant})

    get_target_property(SUBSDK_NAME ${module} SUBSDK)
    set(SHARED_NSO ${CMAKE_CURRENT_BINARY_DIR}/${binary}_out/${SUBSDK_NAME})
    set(PACKAGED_NSO ${CMAKE_CURRENT_BINARY_DIR}/${variant}_out/${SUBSDK_NAME})
    add_custom_command(
            OUTPUT ${PACKAGED_NSO}
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/${variant}_out"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SHARED_NSO}" "${PACKAGED_NSO}"
            DEPENDS ${binary}_nso ${SHARED_NSO}
            VERBATIM
    )
    add_custom_target(${variant}_nso DEPENDS ${PACKAGED_NSO})

    add_custom_target(${variant}_all ALL DEPENDS ${variant}_npdm ${variant}_nso)
    add_dependencies(${module} ${variant}_all)
    create_releases(${module} ${game} ${variant} ${title_id})
endfunction()

function(create_releases_main module)
    add_custom_target(${module}_release_all)
    add_custom_target(${module}_release_variants)
    add_custom_target(${module}_zip_all)

    function(create_release target with_ftp path)
        add_custom_target(${module}_release_${target} DEPENDS ${module}_release_variants)

        add_dependencies(${module}_release_all ${module}_release_${target})

        add_custom_target(${module}_zip_${target} DEPENDS ${module}_release_${target})
        add_custom_command(
                TARGET ${module}_zip_${target} POST_BUILD
                COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/zips/
                COMMAND zip -r "${CMAKE_CURRENT_BINARY_DIR}/zips/${module}_${target}.zip" *
                WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${module}_releases/${target}/"
        )

        add_dependencies(${module}_zip_all ${module}_zip_${target})

        if (${with_ftp})
            add_custom_target(
                ${module}_ftp_${target}
                DEPENDS ${module}_release_${target}
                COMMAND python3 "${CMAKE_CURRENT_SOURCE_DIR}/scripts/send_patch.py" ${FTP_IP} ${FTP_PORT} ${FTP_USER} ${FTP_PASS} "${CMAKE_CURRENT_BINARY_DIR}/${module}_releases/${target}/"
                USES_TERMINAL
            )
        else()
            add_custom_target(
                ${module}_cp_${target}
                DEPENDS ${module}_release_${target}
                COMMAND cp -r "${CMAKE_CURRENT_BINARY_DIR}/${module}_releases/${target}/*" "${path}"
            )
        endif()
    endfunction()

    create_release(atmosphere TRUE "")
    create_release(modmanager TRUE "")
    create_release(ryujinx FALSE "~/.config/Ryujinx/")
    create_release(yuzu FALSE "~/.local/share/yuzu/")
endfunction()

function(create_releases module game_name variant title_id)
    add_custom_target(${variant}_release_all)
    add_dependencies(${module}_release_variants ${variant}_release_all)

    function(create_release target folder)
        add_custom_target(${variant}_release_${target}
                COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_BINARY_DIR}/${variant}_out" "${CMAKE_CURRENT_BINARY_DIR}/${variant}_releases/${target}/${folder}"
                COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_BINARY_DIR}/${variant}_out" "${CMAKE_CURRENT_BINARY_DIR}/${module}_releases/${target}/${folder}"
                DEPENDS ${variant}_all
                VERBATIM
        )

        add_dependencies(${variant}_release_all ${variant}_release_${target})
    endfunction()

    create_release(atmosphere "atmosphere/contents/${title_id}/exefs/")
    create_release(modmanager "mods/${game_name}/${module}/contents/${title_id}/exefs/")
    create_release(ryujinx "mods/contents/${title_id}/${module}/exefs/")
    create_release(yuzu "load/${title_id}/${module}/exefs/")
endfunction()

function(create_npdm variant)
    add_custom_target(
            ${variant}_npdm
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/${variant}_out"
            COMMAND ${npdmtool} "${CMAKE_CURRENT_BINARY_DIR}/${variant}.npdm.json" "${CMAKE_CURRENT_BINARY_DIR}/${variant}_out/main.npdm"
            VERBATIM
    )
endfunction()

function(create_nso variant subsdk)
    add_custom_command(
            OUTPUT ${CMAKE_BINARY_DIR}/${variant}_out/${subsdk}
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/${variant}_out"
            COMMAND ${elf2nso} $<TARGET_FILE:${variant}> "${CMAKE_CURRENT_BINARY_DIR}/${variant}_out/${subsdk}"
            DEPENDS ${variant}
            VERBATIM
    )

    # Add the respective NSO target and set the required linker flags for the original target.
    add_custom_target(${variant}_nso DEPENDS ${CMAKE_BINARY_DIR}/${variant}_out/${subsdk})
    set_target_properties(${variant} PROPERTIES LINK_FLAGS "-specs=${CMAKE_CURRENT_BINARY_DIR}/${variant}.specs -g3 ${ARCH} -nostartfiles")
endfunction()
