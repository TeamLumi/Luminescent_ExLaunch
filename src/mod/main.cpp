#include "exlaunch.hpp"
#include "fs/fs_mount.hpp"
#include "socket/Socket.h"
#include "logger/logger.h"
#include "ui/ui.h"
#include "oe.h"
#include "checks.hpp"
#include "features/features.h"
#include "save/save.h"
#include "nn/err.h"
#include "features/activated_features.h"
#include "data/features.h"
#include "data/utils.h"

void logFeatures() {
    Logger::log("Main Features:\n");
    for (int i=0; i<FEATURE_COUNT; i++)
        if (IsActivatedFeature(i))
            Logger::log(" %s\n", FEATURES[i]);
    Logger::log("\n");

    Logger::log("Debug Features:\n");
    for (int i=0; i<DEBUG_FEATURE_COUNT; i++)
        if (IsActivatedDebugFeature(i))
            Logger::log(" %s\n", DEBUG_FEATURES[i]);
    Logger::log("\n");

    Logger::log("Item Features:\n");
    for (int i=0; i<ITEM_FEATURE_COUNT; i++)
        if (IsActivatedItemFeature(i))
            Logger::log(" %s\n", ITEM_FEATURES[i]);
    Logger::log("\n");

    Logger::log("Key Item Features:\n");
    for (int i=0; i<KEY_ITEM_FEATURE_COUNT; i++)
        if (IsActivatedKeyItemFeature(i))
            Logger::log(" %s\n", KEY_ITEM_FEATURES[i]);
    Logger::log("\n");

    Logger::log("Small Patch Features:\n");
    for (int i=0; i<SMALL_PATCH_FEATURE_COUNT; i++)
        if (IsActivatedSmallPatchFeature(i))
            Logger::log(" %s\n", SMALL_PATCH_FEATURES[i]);
    Logger::log("\n");

    Logger::log("Save Data Features:\n");
    for (int i=0; i<SAVE_FEATURE_COUNT; i++)
        if (IsActivatedSaveFeature(i))
            Logger::log(" %s\n", SAVE_FEATURES[i]);
    Logger::log("\n");

    Logger::log("Battle Features:\n");
    for (int i=0; i<BATTLE_FEATURE_COUNT; i++)
        if (IsActivatedBattleFeature(i))
            Logger::log(" %s\n", BATTLE_FEATURES[i]);
    Logger::log("\n");

    Logger::log("Frontier Features:\n");
    for (int i=0; i<FRONTIER_FEATURE_COUNT; i++)
        if (IsActivatedFrontierFeature(i))
            Logger::log(" %s\n", FRONTIER_FEATURES[i]);
    Logger::log("\n");
}

static Socket gSocket {};
HOOK_DEFINE_TRAMPOLINE(MainInitHook){
    static void Callback(){
        R_ABORT_UNLESS(nn::fs::MountSdCardForDebug("sd"));

        if (gSocket.init(LOGGER_IP, LOGGER_PORT).isFailure()) {
            Logger::log("Failed to connect to logging server!\n");
        } else {
            Logger::addListener([](const char* message) {
                gSocket.sendMessage(message);
            });
            Logger::log("Connected to logging server!\n");
        }
        Logger::log(MODULE_NAME " Loaded!\n");

        nn::oe::DisplayVersion display_version{};
        nn::oe::GetDisplayVersion(&display_version);
        Logger::log("Detected version: %s\n", display_version.name);


        // Load activated features
        exl_save_main();
        exl_features_main(); // Features JSON is read here

        if (IsActivatedDebugFeature(array_index(DEBUG_FEATURES, "Feature Logging")))
            logFeatures();

        Orig();
    }
};

HOOK_DEFINE_REPLACE(NoAction) {
    static void Callback() {

    }
};

extern "C" void exl_main(void* x0, void* x1) {
    exl::hook::Initialize();

    if (!is_game_version(DIAMOND, "1.3.0")) {
        nn::err::ApplicationErrorArg err(nn::err::MakeErrorCode(nn::err::ErrorCodeCategoryType::unk1, 0x420), "Unsupported game version!",
                                         "Please make sure you are running Pokemon Brilliant Diamond version 1.3.0.",
                                         nn::settings::LanguageCode::Make(nn::settings::Language::Language_English));
        nn::err::ShowApplicationError(err);
        svcSleepThread(2000000000);
        EXL_ABORT(0, "Unsupported game version!");
    }

    // We alloc before this is called
    NoAction::InstallAtSymbol("_ZN2nn2fs12SetAllocatorEPFPvmEPFvS1_mE");

    // We initialize nn::socket before the game does; make the game not call it
    NoAction::InstallAtOffset(0x01733d88);
    NoAction::InstallAtOffset(0x01733d78);

    MainInitHook::InstallAtSymbol("nnMain");

#if DEBUG_BUILD
    exl_imgui_main();
    exl_debug_menu_main();
#endif
}

extern "C" NORETURN void exl_exception_entry() {
    EXL_ABORT(0x420);
}
