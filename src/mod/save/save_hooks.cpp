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

CustomSaveData* getCustomSaveData() {
    if (gCustomSaveData == nullptr)
        gCustomSaveData = (CustomSaveData*)nn_malloc(sizeof(CustomSaveData));
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
    loadSymbolEncountersFromJson(saveFile);

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
        getSymbolEncountersAsJson(),
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

void exl_save_main() {
    exl_migration_main();

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
