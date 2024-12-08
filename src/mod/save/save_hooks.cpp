#include "exlaunch.hpp"

#include "externals/PlayerWork.h"
#include "externals/System/Primitives.h"
#include "save/save.h"
#include "save/migration/save_migration.h"

#include "memory/json.h"
#include "fs/fs_files.hpp"
#include "externals/Dpr/NX/SaveSystem.h"
#include "externals/Dpr/NX/UserAccount.h"
#include "helpers/fsHelper.h"
#include "externals/UnityEngine/JsonUtility.h"
#include "externals/GameManager.h"
#include "err.h"
#include "externals/UnityEngine/Switch/Notification.h"

static constexpr long TEST_SIZE = 200000000;

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
    loadMainFromJson(saveFile);
    loadPlayerColorVariationFromJson(saveFile);
    loadDexFormsFromJson(saveFile);
    loadAYouFromJson(saveFile);
    Logger::log("[LoadCustomSaveData] Custom data loaded successfully.\n");
}

nn::json WriteCustomSaveData() {
    Logger::log("[WriteCustomSaveData] Converting custom data...\n");
    nn::json lumiObject = nn::json::object();
    nn::vector<nn::json> saveFunctions = {
        getMainAsJson(),
        getPlayerColorVariationAsJson(),
        getDexFormsAsJson(),
        getAYouAsJson(),
    };
    Logger::log("[WriteCustomSaveData] Custom data converted successfully.\n");

    for (const auto& jsonStructure : saveFunctions) {
        lumiObject.update(jsonStructure);
    }

    return {{"lumi", lumiObject}};
}

HOOK_DEFINE_INLINE(SDKTest) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        Logger::log("[SDK] X8 Offset is %X.\n", ctx->X[8]);
        ctx->X[4] = ctx->X[20];
    }
};

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

HOOK_DEFINE_REPLACE(SDK$$Log) {
    static void Callback(Result result, int32_t param_2, int32_t param_3, const char* logName, void* param_5, const char* msg, ...) {
        va_list args;
        va_start(args, msg);
        Logger::log("[S][%s]", logName);
        Logger::log(msg, args);
        Logger::log("\n");
        va_end(args);
    }
};

HOOK_DEFINE_REPLACE(SDK$$LogError) {
    static void Callback(Result result, const char* msg) {
        Logger::log("[S][ERROR] %s\n", msg);
    }
};

HOOK_DEFINE_REPLACE(IsEnableAccessLog) {
    static int32_t Callback(uint param_1) {
        return 1;
    }
};

static constexpr int64_t VANILLA_SAVE_SIZE = 4194304;

HOOK_DEFINE_INLINE(SaveAvailableSize) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        Logger::log("[S][AvailableSize] Overriding Max Values\n");
        ctx->X[8] = VANILLA_SAVE_SIZE*5;
    }
};



HOOK_DEFINE_TRAMPOLINE(AvailableSizeCheck) {
    static void Callback(long* param_1, uint32_t param_2, ulong param_3) {
        Logger::log("[S][AvailableSize] Accessed.\n");
        Orig(param_1, param_2, param_3);
    }
};

HOOK_DEFINE_TRAMPOLINE(JAvailableSizeCheck) {
    static void Callback(long* param_1, uint32_t param_2, ulong param_3) {
        Logger::log("[S][JAvailableSize] Accessed.\n");
        Orig(param_1, param_2, param_3);
    }
};

HOOK_DEFINE_TRAMPOLINE(FUN_210270) {
    static int Callback(void* param_1) {
        Logger::log("[S][FUN_210270] Accessed.\n");
        return Orig(param_1);
    }
};

HOOK_DEFINE_TRAMPOLINE(FUN_210370) {
    static int Callback(void* param_1, int32_t param_2, long param_3, long param_4, long param_5) {
        Logger::log("[S][FUN_210370] Accessed.\n");
        return Orig(param_1, param_2, param_3, param_4, param_5);
    }
};

HOOK_DEFINE_TRAMPOLINE(FUN_20E8B0) {
    static uint Callback(long* param_1, int32_t param_2, ulong param_3, ulong param_4, ulong param_5) {
        Logger::log("[S][FUN_20E8B0] Accessed.\n");
        return Orig(param_1, param_2, param_3, param_4, param_5);
    }
};

HOOK_DEFINE_TRAMPOLINE(CommandExtend) {
    static Result Callback(void* __this, void* param_1, uchar param_2, void* param_3, long param_4, long param_5) {
        Logger::log("[S][CommandExtend] Accessed.\n");
        Result res = Orig(__this, param_1, param_2, param_3, param_4, param_5);
        Logger::log("[S][CommandExtend] Returning %d / %x.\n", res, res);
        return 0;
    }
};

HOOK_DEFINE_TRAMPOLINE(ExtendApplication) {
    static uint Callback(long* param_1, uint32_t applicationID, void* param_3, uint32_t saveDataType, void* uid, long param_6, long param_7) {
        Logger::log("[S][Extend Application] Accessed.\n");
        return Orig(param_1, applicationID, param_3, saveDataType, uid, param_6, param_7);
    }
};

HOOK_DEFINE_TRAMPOLINE(ResizeFile) {
    static int Callback(void* __this, void* param_1, void* param_2, long param_3) {
        Logger::log("[S][ResizeFile] Accessed.\n");
        return Orig(__this, param_1, param_2, param_3);
    }
};

HOOK_DEFINE_TRAMPOLINE(Flush) {
    static Result Callback(uint32_t FileHandle) {
        Logger::log("[S][Flush] Accessed.\n");
        return Orig(FileHandle);
    }
};

HOOK_DEFINE_TRAMPOLINE(EnsureSave) {
    static Result Callback(long* param_1, uint32_t ApplicationId, void* param_3, void* uid) {
        Logger::log("[S][EnsureSave] Accessed.\n");
        return Orig(param_1, ApplicationId, param_3, uid);
    }
};

HOOK_DEFINE_INLINE(Init) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        constexpr long offset = 0x0020f850;

        Logger::log("[Init] Pre-User.\n");
        Dpr::NX::UserAccount::Open();
        Logger::log("[Init] Account Initialized.\n");

        Dpr::NX::UserAccount::getClass()->initIfNeeded();
        auto uid = Dpr::NX::UserAccount::getClass()->static_fields->UserId;
        nn::account::Uid actualUID {};
        actualUID.uid[0] = uid.fields._data0;
        actualUID.uid[1] = uid.fields._data1;
        Logger::log("[Init] Post-User.\n");

        Logger::log("[Init] Calling SDK offset.\n");
        auto ExtendSaveData = reinterpret_cast<Result(*)(nn::account::Uid*, long, long)>(exl::util::modules::GetSDKOffset(offset));
        Result res = ExtendSaveData(&actualUID, TEST_SIZE, TEST_SIZE);
        Logger::log("[Init] Offset Called.\n");
        if (res) {
            Logger::log("[Init] Extend Failed.\n");
        }

        else {
            Logger::log("[Init] Extend Successful.\n");
        }
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

    SDK$$Log::InstallAtSDKOffset(0x0056c510);
    SDK$$LogError::InstallAtSDKOffset(0x00575f70);
    IsEnableAccessLog::InstallAtSDKOffset(0x00575f80);
    SaveAvailableSize::InstallAtSDKOffset(0x001f51d4);
    SaveAvailableSize::InstallAtSDKOffset(0x001f56c4);


    AvailableSizeCheck::InstallAtSDKOffset(0x00577e00);
    JAvailableSizeCheck::InstallAtSDKOffset(0x00577e10);
    ResizeFile::InstallAtSDKOffset(0x001c6bac);

    FUN_210270::InstallAtSDKOffset(0x00210270);
    FUN_210370::InstallAtSDKOffset(0x00210370);
    FUN_20E8B0::InstallAtSDKOffset(0x0020e8b0);
    CommandExtend::InstallAtSDKOffset(0x0002d9e0);
    ExtendApplication::InstallAtSDKOffset(0x0020e5c0);
    Flush::InstallAtSDKOffset(0x001bb0d0);
    //Init::InstallAtOffset(0x02cd16fc);
    EnsureSave::InstallAtSDKOffset(0x0020dd50);
}
