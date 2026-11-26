#include "exlaunch.hpp"

void exl_battle_camera_fix_main() {
    exl::patch::CodePatcher p(game::Address::Dpr_Battle_View_Systems_BattleViewSystem_ResetDefaultCamera + 0x128);
    p.WriteInst(exl::armv8::inst::Nop());
};