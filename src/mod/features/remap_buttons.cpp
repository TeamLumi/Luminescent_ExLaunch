#include "exlaunch.hpp"
#include "externals/il2cpp.h"
#include "externals/GameController.h"
#include "externals/FieldManager.h"
#include "externals/FlagWork.h"
#include "externals/FieldPoketch.h"
#include "logger/logger.h"

HOOK_DEFINE_REPLACE(RemapPoketchToPlus) {
    static bool Callback(FieldManager* __this) {
        system_load_typeinfo(game::TypeInfo::FieldManager_ControlPoketch);
        if (FlagWork::GetFlag(FlagWork_Flag::FE_POKETCH_GET) &&
            !FlagWork::GetSysFlag(FlagWork_SysFlag::SYS_FLAG_POKETCH_HOOK) &&
            !FlagWork::GetSysFlag(FlagWork_SysFlag::SYS_INPUT_OFF)) {

            system_load_typeinfo(game::TypeInfo::FieldInput_Push);
            GameController::getClass()->initIfNeeded();
            FieldPoketch::getClass()->initIfNeeded();

            if (((GameController::getClass()->static_fields->push) & GameController::ButtonMask::Plus) != 0) {
                FieldPoketch::OnPushedAppearButton();
            } else {
                FieldPoketch::RestorePoketchWindow();
            }

            return FieldPoketch::IsControlPoketch();
        }

        return false;
    }
};

HOOK_DEFINE_REPLACE(UnionWarp) {
    static bool Callback() {
        return false;
    }
};

static int last = 0;
HOOK_DEFINE_TRAMPOLINE(LogButton) {
    static void Callback(void* p1, float p2) {
        Orig(p1, p2);

        int current = GameController::getClass()->static_fields->on;
        if (current != last) {
            if (current != 0) {
                Logger::log("Button: %x\n", current);
            }
            last = current;
        }
    }
};

void exl_remap_main() {
    // Remap Poketch to Plus
    RemapPoketchToPlus::InstallAtOffset(game::Address::FieldManager_ControlPoketch);
    exl::patch::CodePatcher p(game::Address::Dpr_UI_PoketchWindow_OnCreate + 0x250);
    p.WriteInst(exl::armv8::inst::Movz(exl::armv8::reg::X1, GameController::ButtonMask::Plus));

    // Disable Y to warp to union room
    UnionWarp::InstallAtOffset(game::Address::FieldManager_OpenUnionRoomWarp);

    // Log button presses
//    LogButton::InstallAtOffset(0x0179a970);

    // Remap Plus/Minus to Y for registered items
    p.Seek(game::Address::FieldManager_UseShotCut + 0x558);
    p.Write(0x39407108);  // ldrb w8, [x8, #0x1c]
    p.Write(0x721d011f);  // tst w8, #8
    p.Seek(game::Address::FieldManager_UseShotCut + 0x5C0);
    p.Write(0x39406108);  // ldrb w8, [x8, #0x18]
    p.Write(0x721d011f);  // tst w8, #8
    p.Seek(game::Address::FieldManager_UseShotCut + 0x6B4);
    p.Write(0x39408108);    // ldrb w8, [x8, #0x20]
    p.Write(0x721d011f);    // tst w8, #8
}
