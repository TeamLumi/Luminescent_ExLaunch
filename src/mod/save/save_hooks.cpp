#include "exlaunch.hpp"

#include "externals/PlayerWork.h"
#include "externals/System/Primitives.h"
#include "save/save.h"
#include "save/migration/save_migration.h"

#include "externals/ViewerSettings.h"
#include "externals/SmartPoint/Components/PlayerPrefsProvider.h"
#include "memory/json.h"
#include "fs/fs_files.hpp"
#include "externals/System/Convert.h"
#include "externals/Dpr/NX/SaveSystem.h"
#include "result.h"
#include "helpers/fsHelper.h"
#include "externals/UnityEngine/JsonUtility.h"
#include "externals/UnityEngine/PlayerPrefs.h"
#include "externals/System/Runtime/InteropServices/Marshal.h"
#include "externals/GameManager.h"
#include "externals/FlagWork.h"
#include "externals/FlagWork_Enums.h"

static CustomSaveData* gCustomSaveData = nullptr;
static bool onLoadInjection; // Flag to ensure injection doesn't happen more than once.
static bool isBackup; // Stores backup status during initialization to pass into post-load PlayerWork injection.
static bool migrationRequired; // Controls where LoadBoxes() and LinkBoxes() are called.

static constexpr const char* mainSaveName = "SaveData:/SaveData.bin";
static constexpr const char* backupSaveName = "SaveData:/Backup.bin";
static constexpr const char* saveMountName = "SaveData";

static constexpr long vanillaByteSize = 979108;

CustomSaveData* getCustomSaveData() {
    if (gCustomSaveData == nullptr)
        gCustomSaveData = (CustomSaveData*)nn_malloc(sizeof(CustomSaveData));

    return gCustomSaveData;
}

bool CustomLoadOperation(PlayerWork::Object* playerWork) {
    system_load_typeinfo(0x6a91);
    system_load_typeinfo(0x6ad4);
    Dpr::NX::SaveSystem::getClass()->initIfNeeded();
    Dpr::NX::SaveSystem::getClass()->static_fields->_Instance->MountSaveData();

    if (Dpr::NX::SaveSystem::SaveDataExists()) {
        Logger::log("[CustomLoadOperation] Loading JSON...\n");
        nn::json saveFile = FsHelper::loadJsonFileFromPath(playerWork->fields._isBackupSave ? backupSaveName : mainSaveName);
        Logger::log("[CustomLoadOperation] Converting to PlayerWork.\n");
        System::String::Object* jsonString = System::String::Create(saveFile.dump().c_str());

        System::RuntimeTypeHandle::Object handle{};
        handle.fields.value = &PlayerWork::getClass()->_1.byval_arg;

        auto tempPlayerWork = reinterpret_cast<PlayerWork::Object*>(UnityEngine::JsonUtility::FromJson(
                jsonString, System::Type::GetTypeFromHandle(handle)));

        memcpy(&playerWork->fields._saveData.fields, &tempPlayerWork->fields._saveData.fields, 0x7b8);

        playerWork->fields._loadResult = PlayerWork::LoadResult::SUCCESS;
        playerWork->fields._isBackupSave = false;

        Logger::log("[CustomLoadOperation] Overwrite Successful\n");
    }

    else {
        playerWork->fields._loadResult = PlayerWork::LoadResult::NOT_EXIST;
        playerWork->fields._isBackupSave = false;
    }

    return playerWork->fields._loadResult == PlayerWork::LoadResult::SUCCESS;
}
HOOK_DEFINE_TRAMPOLINE(PatchExistingSaveData__Load) {
    static bool Callback(PlayerWork::Object* playerWork) {
        bool success = CustomLoadOperation(playerWork);//

        isBackup = playerWork->fields._isBackupSave;

//        if (success)
//        {
//            // Start as vanilla save
//            getCustomSaveData()->main.version = ModVersion::Vanilla;
//
//            // Load version data
//            loadMain(isBackup);
//
//            // Check if migration is required (Related to Box Expansion)
//            migrationRequired = getCustomSaveData()->main.version < CURRENT_VERSION;
//
//            /* Load all other data
//             * (Lumi Boxes loaded in separate function where migration is not required) */
//            loadZukan(isBackup);
//            loadWorks(isBackup);
//            loadFlags(isBackup);
//            loadSysFlags(isBackup);
//            loadTrainers(isBackup);
//            loadItems(isBackup);
//            loadBerries(isBackup);
//            loadColorVariations(isBackup);
//            //if (migrationRequired) loadBoxes(isBackup);
//
//            // Perform migration loop
//            migrate(playerWork);
//
//            /* Put our custom-length data into PlayerWork for the game to access
//             * (Lumi Boxes linked in separate function where migration is not required) */
//            linkZukan(playerWork);
//            linkWorks(playerWork);
//            linkFlags(playerWork);
//            linkSysFlags(playerWork);
//            linkTrainers(playerWork);
//            linkItems(playerWork);
//            linkBerries(playerWork);
//            linkColorVariations(playerWork);
//            //if (migrationRequired) linkBoxes(playerWork);
//        }
        playerWork->fields._isBackupSave = false;

        return true;
    }
};

//void injectPlayerWork() {
//    auto method = SmartPoint::Components::PlayerPrefsProvider<PlayerWork>
//            ::Method$SmartPoint_Components_PlayerPrefsProvider_PlayerWork_get_instance;
//    auto playerWork = (PlayerWork::Object*) SmartPoint::Components::PlayerPrefsProvider<PlayerWork>
//            ::get_Instance(method);
//    loadBoxes(isBackup);
//    linkBoxes(playerWork);
//    onLoadInjection = true; // Safeguard to prevent further injection until game is relaunched.
//}


HOOK_DEFINE_REPLACE(SaveSystem$$Save) {
    static bool Callback(System::Byte_array *data, bool writeMain, bool writeBackup) {
        system_load_typeinfo(0x757e);
        Dpr::NX::SaveSystem::getClass()->initIfNeeded();
        Dpr::NX::SaveSystem::getClass()->static_fields->_Instance->MountSaveData();

        auto playerWork =
                SmartPoint::Components::PlayerPrefsProvider<PlayerWork>::get_Instance(
                        SmartPoint::Components::PlayerPrefsProvider<PlayerWork>::
                                Method$SmartPoint_Components_PlayerPrefsProvider_PlayerWork_get_instance);

        System::String::Object* jsonData = UnityEngine::JsonUtility::ToJson(
                reinterpret_cast<Il2CppObject*>(playerWork), false);

        if (writeMain) {
            FsHelper::writeFileToPath(jsonData->asCString().data(), jsonData->asCString().size(),
                                      mainSaveName);
        }

        if (writeBackup) {
            FsHelper::writeFileToPath(jsonData->asCString().data(), jsonData->asCString().size(),
                                      backupSaveName);
        }

        return nn::fs::Commit(saveMountName).isSuccess();
    }
};

HOOK_DEFINE_REPLACE(PatchExistingSaveData__Save) {
    static void Callback(PlayerWork::Object* playerWork) {
        bool isMain = playerWork->fields._isMainSave;
        isBackup = playerWork->fields._isBackupSave;

        // Remove the custom-length PlayerWork data with the vanilla save's
        unlinkZukan(playerWork);
        unlinkWorks(playerWork);
        unlinkFlags(playerWork);
        unlinkSysFlags(playerWork);
        unlinkTrainers(playerWork);
        unlinkItems(playerWork);
        unlinkBerries(playerWork);
        unlinkColorVariations(playerWork);
        //unlinkBoxes(playerWork);

#ifndef DEBUG_DISABLE_SAVE  // Allow disabling the saving to test the save migration code
        // Save version data to file
//        saveMain(isMain, isBackup);

        // Save rest of data to files
//        saveZukan(isMain, isBackup);
//        saveWorks(isMain, isBackup);
//        saveFlags(isMain, isBackup);
//        saveSysFlags(isMain, isBackup);
//        saveTrainers(isMain, isBackup);
//        saveItems(isMain, isBackup);
//        saveBerries(isMain, isBackup);
//        saveColorVariations(isMain, isBackup);
        //saveBoxes(isMain, isBackup);

#endif

        // Save base save file
        Dpr::NX::SaveSystem::Save(nullptr, isMain, isBackup);

        // Re-replace the PlayerWork data with our custom-length one
//        relinkZukan(playerWork);
//        relinkWorks(playerWork);
//        relinkFlags(playerWork);
//        relinkSysFlags(playerWork);
//        relinkTrainers(playerWork);
//        relinkItems(playerWork);
//        relinkBerries(playerWork);
//        relinkColorVariations(playerWork);
        //relinkBoxes(playerWork);
    }
};

HOOK_DEFINE_REPLACE(PatchExistingSaveData__Verify) {
    static bool Callback(System::Byte_array* playerWork) {
        return true;
    }
};

HOOK_DEFINE_TRAMPOLINE(FieldCanvas$$Start) {
    static void Callback(void* __this) {
        if (!onLoadInjection && !migrationRequired) {
            //injectPlayerWork();
        }
        Orig(__this);
    }
};

void exl_save_main() {
    PatchExistingSaveData__Load::InstallAtOffset(0x02ceb850);
    PatchExistingSaveData__Save::InstallAtOffset(0x02cebf00);
    PatchExistingSaveData__Verify::InstallAtOffset(0x02ceba00);
    //FieldCanvas$$Start::InstallAtOffset(0x01784b90);
    PlayerWork$$get_UniqueID::InstallAtOffset(0x02ced560);
    SaveSystem$$Save::InstallAtOffset(0x01ddf3e0);

    // Backup save patches
    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p(0);
    auto inst = nn::vector<exl::patch::Instruction> {
        { 0x02ceb9dc, Nop() },
    };
    p.WriteInst(inst);
}
