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

bool SaveSystem_Load(bool fromBackup, PlayerWork::Object* playerWork) {
    Logger::log("[SaveSystem$$Load] Mounting Save Data...\n");
    Dpr::NX::SaveSystem::getClass()->static_fields->_Instance->MountSaveData();

    Logger::log("[SaveSystem$$Load] Loading JSON...\n");
    nn::json saveFile = FsHelper::loadJsonFileFromPath(fromBackup ? backupSaveName : mainSaveName);

    Logger::log("[SaveSystem$$Load] Converting to PlayerWork...\n");
    System::String::Object* jsonString = System::String::Create(saveFile.dump().c_str());

    Logger::log("[SaveSystem$$Load] Making a temp PlayerWork...\n");
    System::RuntimeTypeHandle::Object handle{};
    handle.fields.value = &PlayerWork::getClass()->_1.byval_arg;
    auto tempPlayerWork = reinterpret_cast<PlayerWork::Object*>(UnityEngine::JsonUtility::FromJson(jsonString, System::Type::GetTypeFromHandle(handle)));

    Logger::log("[SaveSystem$$Load] Copying the temp PlayerWork to the real one...\n");
    memcpy(&playerWork->fields._saveData.fields, &tempPlayerWork->fields._saveData.fields, 0x7b8);

    // TODO: Return false if the file doesn't exist or this failed somehow?
    return true;
}

bool VerifySaveData() {
    // Don't actually verify save data for now
    return true;
}

HOOK_DEFINE_TRAMPOLINE(PlayerWork$$CustomLoadOperation) {
    static bool Callback(PlayerWork::Object* playerWork) {
        system_load_typeinfo(0x6a91);
        system_load_typeinfo(0x6ad4);
        Dpr::NX::SaveSystem::getClass()->initIfNeeded();

        bool loadResult = SaveSystem_Load(playerWork->fields._isBackupSave, playerWork);

        if (!loadResult)
            playerWork->fields._loadResult = Dpr::NX::SaveSystem::SaveDataExists() ? PlayerWork::LoadResult::FAILED : PlayerWork::LoadResult::NOT_EXIST;
        else if (!VerifySaveData())
            playerWork->fields._loadResult = PlayerWork::LoadResult::CORRUPTED;
        else
            playerWork->fields._loadResult = PlayerWork::LoadResult::SUCCESS;

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

HOOK_DEFINE_REPLACE(SaveSystem$$Save) {
    static bool Callback(System::Byte_array* data, bool writeMain, bool writeBackup) {
        system_load_typeinfo(0x757e);
        Dpr::NX::SaveSystem::getClass()->initIfNeeded();

        Logger::log("[SaveSystem$$Save] Mounting Save Data...\n");
        Dpr::NX::SaveSystem::getClass()->static_fields->_Instance->MountSaveData();

        Logger::log("[SaveSystem$$Save] Get PlayerWork instance...\n");
        auto playerWork = PlayerWork::get_instance();

        Logger::log("[SaveSystem$$Save] Convert to JSON...\n");
        System::String::Object* jsonData = UnityEngine::JsonUtility::ToJson(reinterpret_cast<Il2CppObject*>(playerWork), false);

        if (writeMain) {
            Logger::log("[SaveSystem$$Save] Saving SaveData.bin...\n");
            auto result = FsHelper::writeFileToPath(jsonData->asCString().data(), jsonData->asCString().size(), mainSaveName);
            if (!result.isSuccess())
            {
                Logger::log("[SaveSystem$$Save] The save was unsuccessful!\n");
                return false;
            }
        }

        if (writeBackup) {
            Logger::log("[SaveSystem$$Save] Saving Backup.bin...\n");
            auto result = FsHelper::writeFileToPath(jsonData->asCString().data(), jsonData->asCString().size(), backupSaveName);
            if (!result.isSuccess())
            {
                Logger::log("[SaveSystem$$Save] The save was unsuccessful!\n");
                return false;
            }
        }

        return FsHelper::Commit(saveMountName).isSuccess();
    }
};

HOOK_DEFINE_REPLACE(PlayerWork$$VerifySaveData) {
    static bool Callback(System::Byte_array* playerWork) {
        return true;
    }
};

void exl_save_main() {
    // Loading
    PlayerWork$$CustomLoadOperation::InstallAtOffset(0x02ceb850);

    // Saving
    PlayerWork$$CustomSaveOperation::InstallAtOffset(0x02cebf00);
    SaveSystem$$Save::InstallAtOffset(0x01ddf3e0);

    // Verifying
    PlayerWork$$VerifySaveData::InstallAtOffset(0x02ceba00);
}
