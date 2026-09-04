#include "exlaunch.hpp"

#include "externals/PlayerWork.h"
#include "externals/System/Primitives.h"
#include "save/save.h"
#include "save/migration/save_migration.h"

#include "memory/json.h"
#include "fs/fs_files.hpp"
#include "externals/Dpr/NX/SaveSystem.h"
#include "helpers/fsHelper.h"
#include "externals/UnityEngine/JsonUtility.h"
#include "err.h"
#include "externals/UnityEngine/Switch/Notification.h"

static CustomSaveData* gCustomSaveData = nullptr;

constexpr uintptr_t cmd32_Offset = 0x1d264c;
constexpr uintptr_t cmd51_Offset = 0x1d2780;
constexpr uintptr_t cmd57_Offset = 0x1d296c;
constexpr uintptr_t cmd61_Offset = 0x1d2a84;
constexpr uintptr_t ISaveDataInfoReader_Read_Offset = 0x1d8028;
constexpr uint8_t saveDataSpaceUser = 1;
constexpr int64_t targetDataSize = 0x1000000; // 16 MiB
constexpr int64_t targetJournalSize = 0x1000000; // 16 MiB

CustomSaveData* getCustomSaveData() {
    if (gCustomSaveData == nullptr) {
        void* allocation = nn_malloc(sizeof(CustomSaveData));
        gCustomSaveData = new (allocation) CustomSaveData();
    }
    return gCustomSaveData;
}

nn::json LoadJSONSave(bool fromBackup) {
    Dpr::NX::SaveSystem::getClass()->static_fields->_Instance->MountSaveData();
    Logger::log("Loading JSON save data...\n");
    return FsHelper::loadJsonFileFromPath(fromBackup ? CustomSaveData::backupSaveName : CustomSaveData::mainSaveName);
}

bool SaveSystem_Load(PlayerWork::Object* playerWork, nn::json saveFile) {
    if (saveFile != nullptr && !saveFile.is_discarded()) {
        // Retrieve the PlayerWork structure from the parsed Json
        System::String::Object* jsonString = System::String::Create(saveFile["playerWork"].dump().c_str());

        System::RuntimeTypeHandle::Object handle{};
        handle.fields.value = &PlayerWork::getClass()->_1.byval_arg;
        auto tempPlayerWork = reinterpret_cast<PlayerWork::Object*>(UnityEngine::JsonUtility::FromJson(jsonString, System::Type::GetTypeFromHandle(handle)));

        // Copy the saveData of our temporary converted PlayerWork into the real PlayerWork
        memcpy(&playerWork->fields._saveData.fields, &tempPlayerWork->fields._saveData.fields, sizeof(PlayerWork::SaveData::Fields));

        Logger::log("[SaveSystem$$Load] JSON save data loaded.\n");
        return true;
    }
    else {
        return false;
    }
}

bool VerifySaveData() {
    // Don't actually verify save data for now
    return true;
}

void FailedLoad() {
    nn::err::ApplicationErrorArg err(nn::err::MakeErrorCode(
            nn::err::ErrorCodeCategoryType::unk1, 0x421),
                                     "Failed to load save file!","The save file was not parsed successfully!",
                                     nn::settings::LanguageCode::Make(nn::settings::Language::Language_English));
    nn::err::ShowApplicationError(err);
    EXL_ABORT(0);
}

void LoadCustomSaveData(nn::json& saveFile) {
    Logger::log("[LoadCustomSaveData] Loading custom data...\n");

    // ADD THE LOADING OF NEW STRUCTS HERE
    loadMainFromJson(saveFile);
    loadPlayerColorVariationFromJson(saveFile);
    loadDexFormsFromJson(saveFile);
    loadAYouFromJson(saveFile);
    loadExtraSettingsFromJson(saveFile);

    Logger::log("[LoadCustomSaveData] Custom data loaded successfully.\n");
}

nn::json WriteCustomSaveData() {
    Logger::log("[WriteCustomSaveData] Converting custom data...\n");
    nn::json lumiObject = nn::json::object();

    // ADD THE SAVING OF NEW STRUCTS HERE
    nn::vector<nn::json> saveFunctions = {
        getMainAsJson(),
        getPlayerColorVariationAsJson(),
        getDexFormsAsJson(),
        getAYouAsJson(),
        getExtraSettingsAsJson(),
    };

    Logger::log("[WriteCustomSaveData] Custom data converted successfully.\n");

    for (const auto& jsonStructure : saveFunctions) {
        lumiObject.update(jsonStructure);
    }

    return {{"lumi", lumiObject}};
}

HOOK_DEFINE_REPLACE(PlayerWork$$CustomLoadOperation) {
    static bool Callback(PlayerWork::Object* playerWork) {
        system_load_typeinfo(0x6a91);
        system_load_typeinfo(0x6ad4);
        Dpr::NX::SaveSystem::getClass()->initIfNeeded();

        // Initialize version as "Vanilla" prior to loading.
        getCustomSaveData()->main.Initialize();

        auto saveFile = LoadJSONSave(playerWork->fields._isBackupSave);
        bool loadResult = SaveSystem_Load(playerWork, saveFile);

        if (!loadResult) {
            if (Dpr::NX::SaveSystem::SaveDataExists()) {
                playerWork->fields._loadResult = PlayerWork::LoadResult::FAILED;
                FailedLoad(); // EXL_ABORT
            }
            else {
                playerWork->fields._loadResult = PlayerWork::LoadResult::NOT_EXIST;
                Logger::log("No save exists.\n");
            }
        }
        else if (!VerifySaveData()) {
            playerWork->fields._loadResult = PlayerWork::LoadResult::CORRUPTED;
            FailedLoad(); // EXL_ABORT
        }
        else {
            playerWork->fields._loadResult = PlayerWork::LoadResult::SUCCESS;
            LoadCustomSaveData(saveFile);
        }

        migrate(playerWork);
        playerWork->fields._isBackupSave = false;
        return true;
    }
};

HOOK_DEFINE_REPLACE(PlayerWork$$CustomSaveOperation) {
    static bool Callback(PlayerWork::Object* playerWork) {
        Dpr::NX::SaveSystem::Save(nullptr, playerWork->fields._isMainSave, playerWork->fields._isBackupSave);

        playerWork->fields._isMainSave = true;
        playerWork->fields._isBackupSave = false;
        return true;
    }
};

HOOK_DEFINE_REPLACE(PlayerWork$$CustomSaveAsyncOperation) {
    static bool Callback(PlayerWork::Object* playerWork) {
        system_load_typeinfo(0x6a92);
        playerWork->OnPreSave();
        Dpr::NX::SaveSystem::SaveAsync(nullptr, playerWork->fields._isMainSave, playerWork->fields._isBackupSave);

        playerWork->fields._isMainSave = true;
        playerWork->fields._isBackupSave = false;
        return true;
    }
};

HOOK_DEFINE_REPLACE(SaveSystem$$Save) {
    static bool Callback(System::Byte_array* data, bool writeMain, bool writeBackup) {
        system_load_typeinfo(0x757e);
        Dpr::NX::SaveSystem::getClass()->initIfNeeded();
        Dpr::NX::SaveSystem::getClass()->static_fields->_Instance->MountSaveData();
        auto playerWork = PlayerWork::get_instance();

        System::String::Object* jsonData = UnityEngine::JsonUtility::ToJson(reinterpret_cast<Il2CppObject*>(playerWork), false);

        // Isolate the PlayerWork section into a key to easily retrieve it.
        nn::string writeString = {R"({"playerWork":)" + jsonData->asCString()};

        // Append all Custom Data separate from the PlayerWork structure.
        writeString.append("," + WriteCustomSaveData().dump().erase(0,1));

        if (writeMain) {
            auto result = FsHelper::writeFileToPath(writeString.data(), writeString.size(), CustomSaveData::mainSaveName);
            if (!result.isSuccess()) {
                Logger::log("[SaveSystem$$Save] Failed to save!\n");
                return false;
            }
        }

        if (writeBackup) {
            auto result = FsHelper::writeFileToPath(writeString.data(), writeString.size(), CustomSaveData::backupSaveName);
            if (!result.isSuccess()) {
                Logger::log("[SaveSystem$$Save] Failed to save!\n");
                return false;
            }
        }

        return FsHelper::Commit(CustomSaveData::saveMountName).isSuccess();
    }
};

HOOK_DEFINE_REPLACE(SaveSystem$$SaveAsync) {
    static void Callback(System::Byte_array* data, bool writeMain, bool writeBackup) {
        system_load_typeinfo(0x757c);
        Dpr::NX::SaveSystem::getClass()->initIfNeeded();

        auto saveSystem = Dpr::NX::SaveSystem::getClass()->static_fields->_Instance;

        if (!saveSystem->fields._isBusy) {
            saveSystem->fields._isBusy = true;
            UnityEngine::Switch::Notification::EnterExitRequestHandlingSection();
            saveSystem->fields._writeMain = writeMain;
            saveSystem->fields._writeBackup = writeBackup;
            saveSystem->fields._saveRequest = true;
            saveSystem->StartThread();
        }
    }
};

HOOK_DEFINE_REPLACE(PlayerWork$$VerifySaveData) {
    static bool Callback(System::Byte_array* playerWork) {
        return true;
    }
};

void ReleaseObject(void* obj) {
    if (obj != nullptr) {
        auto vtable = *static_cast<uintptr_t**>(obj);
        reinterpret_cast<void(*)(void*)>(vtable[1])(obj);
    }
}

uint64_t GetSaveDataId(void* proxy) {
    ReaderStorage reader{};
    OutReader out{&reader};

    Result res = reinterpret_cast<Result(*)(void*, OutReader*, uint8_t)
    >(exl::util::modules::GetSDKOffset(cmd61_Offset))(proxy, &out, saveDataSpaceUser);

    if (R_FAILED(res) || reader.object == nullptr)
        return 0;

    uint64_t saveDataId = 0;

    SaveDataInfo info[16]{};
    BufferDesc description{info, sizeof(info)};
    int64_t count = 0;

    res = reinterpret_cast<Result(*)(void*, int64_t*, BufferDesc*)>(
        exl::util::modules::GetSDKOffset(ISaveDataInfoReader_Read_Offset)
    )(reader.object, &count, &description);

    if (R_SUCCEEDED(res)) {
        count = std::min<int64_t>(count, std::size(info));

        for (int64_t i = 0; i < count; i++) {
            if (info[i].applicationId == DIAMOND) {
                saveDataId = info[i].saveDataId;
                break;
            }
        }
    }

    ReleaseObject(reader.object);
    return saveDataId;
}

HOOK_DEFINE_TRAMPOLINE(IFileSystemProxy$$CMD_51) {
    static Result Callback(void* proxy, void* outFileSystem, uint8_t spaceId, void* saveDataAttribute) {

        SaveDataExtraData extraData{};

        BufferDesc desc{
            &extraData,
            sizeof(extraData)
        };
        uint64_t saveDataId = GetSaveDataId(proxy);
        if (saveDataId == 0) {
            Logger::log("[IFileSystemProxy$$CMD_51] SaveDataId was not found.\n");
            return Orig(proxy, outFileSystem, spaceId, saveDataAttribute);
        }
        Logger::log("[IFileSystemProxy$$CMD_51] Proxy=%p SaveID=0x%llx\n", proxy, saveDataId);

        // Retrieve current SaveDataExtraData
        Result res = reinterpret_cast<Result(*)(void*, BufferDesc*, uint8_t, uint64_t)
        >(exl::util::modules::GetSDKOffset(cmd57_Offset))(proxy, &desc, saveDataSpaceUser, saveDataId);

        if (R_FAILED(res)) {
            Logger::log("[IFileSystemProxy$$CMD_51] Cmd_57 failed with Res: 0x%08x.\n", res);
            return Orig(proxy, outFileSystem, spaceId, saveDataAttribute);
        }

        Logger::log("[IFileSystemProxy$$CMD_51] Current Sizes: 0x%llx, 0x%llx\n", extraData.dataSize, extraData.journalSize);

        // Verify if extension is required
        if (extraData.dataSize < targetDataSize || extraData.journalSize < targetJournalSize) {
            res = reinterpret_cast<Result(*)(void*, uint8_t, uint64_t, int64_t, int64_t)
            >(exl::util::modules::GetSDKOffset(cmd32_Offset)
            )(proxy, saveDataSpaceUser, saveDataId, targetDataSize, targetJournalSize);
        }

        if (R_FAILED(res)) {
            Logger::log("[IFileSystemProxy$$CMD_51] Cmd_32 (Extension) failed.\n");
            return Orig(proxy, outFileSystem, spaceId, saveDataAttribute);
        }

        return Orig(proxy, outFileSystem, spaceId, saveDataAttribute);
    }
};

void exl_save_main() {
    exl_migration_main();

    // Hardware Save Size Patch
    IFileSystemProxy$$CMD_51::InstallAtSDKOffset(cmd51_Offset);

    // Loading
    PlayerWork$$CustomLoadOperation::InstallAtOffset(0x02ceb850);

    // Saving
    PlayerWork$$CustomSaveOperation::InstallAtOffset(0x02cebf00);
    PlayerWork$$CustomSaveAsyncOperation::InstallAtOffset(0x02cec400);
    SaveSystem$$Save::InstallAtOffset(0x01ddf3e0);
    SaveSystem$$SaveAsync::InstallAtOffset(0x01ddf680);

    // Verifying
    PlayerWork$$VerifySaveData::InstallAtOffset(0x02ceba00);
}
