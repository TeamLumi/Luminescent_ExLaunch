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
#include "memory/nn_allocator.h"

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
    exl_save_main();
    exl_features_main();
}

extern "C" NORETURN void exl_exception_entry() {
    EXL_ABORT(0x420);
}
