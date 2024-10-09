#include "exlaunch.hpp"

#include "externals/PlayerWork.h"
#include "externals/System/Primitives.h"
#include "save/save.h"
#include "save/migration/save_migration.h"

#include "memory/json.h"
#include "fs/fs_files.hpp"
#include "externals/Dpr/NX/SaveSystem.h"
#include "result.h"
#include "helpers/fsHelper.h"
#include "externals/UnityEngine/JsonUtility.h"
#include "externals/UnityEngine/PlayerPrefs.h"
#include "externals/GameManager.h"
#include "err.h"

static CustomSaveData* gCustomSaveData = nullptr;

CustomSaveData* getCustomSaveData() {
    if (gCustomSaveData == nullptr)
        gCustomSaveData = (CustomSaveData*)nn_malloc(sizeof(CustomSaveData));
    return gCustomSaveData;
}

bool SaveSystem_Load(bool fromBackup, PlayerWork::Object* playerWork) {
    Dpr::NX::SaveSystem::getClass()->static_fields->_Instance->MountSaveData();

    Logger::log("[SaveSystem$$Load] Loading JSON save data...\n");
    nn::json saveFile = FsHelper::loadJsonFileFromPath(fromBackup ? CustomSaveData::backupSaveName : CustomSaveData::mainSaveName);
    if (saveFile != nullptr && !saveFile.is_discarded()) {
        System::String::Object* jsonString = System::String::Create(saveFile["playerWork"].dump().c_str());

        Logger::log("[SaveSystem$$Load] Converting to PlayerWork...\n");
        System::RuntimeTypeHandle::Object handle{};
        handle.fields.value = &PlayerWork::getClass()->_1.byval_arg;
        auto tempPlayerWork = reinterpret_cast<PlayerWork::Object*>(UnityEngine::JsonUtility::FromJson(jsonString, System::Type::GetTypeFromHandle(handle)));

        memcpy(&playerWork->fields._saveData.fields, &tempPlayerWork->fields._saveData.fields, 0x7b8);

        Logger::log("[SaveSystem$$Load] JSON save data loaded.\n");
        return true;
    }

    else {
        return false;
    }

    // TODO: Return false if the file doesn't exist or this failed somehow?
}

bool VerifySaveData() {
    // Don't actually verify save data for now
    return true;
}

void FailedLoad(PlayerWork::LoadResult res) {
    nn::err::ApplicationErrorArg err(nn::err::MakeErrorCode(nn::err::ErrorCodeCategoryType::unk1, 0x421), "Failed to load save file!",
                                     res == PlayerWork::LoadResult::NOT_EXIST ? "Save data was not found" : "The save file was not parsed successfully",
                                     nn::settings::LanguageCode::Make(nn::settings::Language::Language_English));
    nn::err::ShowApplicationError(err);
}

void LoadCustomSaveData(bool isBackup) {
    loadMain(isBackup);

    Logger::log("CustomSaveData Loaded.\n");
}

HOOK_DEFINE_TRAMPOLINE(PlayerWork$$CustomLoadOperation) {
    static bool Callback(PlayerWork::Object* playerWork) {
        system_load_typeinfo(0x6a91);
        system_load_typeinfo(0x6ad4);
        Dpr::NX::SaveSystem::getClass()->initIfNeeded();

        bool loadResult = SaveSystem_Load(playerWork->fields._isBackupSave, playerWork);


        if (!loadResult) {
            playerWork->fields._loadResult = Dpr::NX::SaveSystem::SaveDataExists() ? PlayerWork::LoadResult::FAILED : PlayerWork::LoadResult::NOT_EXIST;
            FailedLoad(playerWork->fields._loadResult);
            EXL_ABORT(0);
        }
        else if (!VerifySaveData())
            playerWork->fields._loadResult = PlayerWork::LoadResult::CORRUPTED;
        else {
            playerWork->fields._loadResult = PlayerWork::LoadResult::SUCCESS;
            LoadCustomSaveData(playerWork->fields._isBackupSave);
        }

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

void WriteCustomSaveData(nn::string* writeString) {
    writeString->append(R"(,"lumi:")");

    // Main
    writeString->append(saveMain().dump());

    // AYou
    //writeString->append(getCustomSaveData()->ayou.ToJson());

    // Complete our JSON structure after adding the additional entries.
    writeString->append(R"(})");
}

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

        // Isolate the PlayerWork section into a key to easily retrieve it.
        nn::string writeString = {R"({"playerWork":)" + jsonData->asCString()};

        // Append all Custom Data separate from the PlayerWork structure.
        WriteCustomSaveData(&writeString);

        if (writeMain) {
            auto result = FsHelper::writeFileToPath(writeString.data(), writeString.size(), CustomSaveData::mainSaveName);
            if (!result.isSuccess())
            {
                Logger::log("[SaveSystem$$Save] The save was unsuccessful!\n");
                return false;
            }
        }

        if (writeBackup) {
            auto result = FsHelper::writeFileToPath(writeString.data(), writeString.size(), CustomSaveData::backupSaveName);
            if (!result.isSuccess())
            {
                Logger::log("[SaveSystem$$Save] The save was unsuccessful!\n");
                return false;
            }
        }

        return FsHelper::Commit(CustomSaveData::saveMountName).isSuccess();
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
