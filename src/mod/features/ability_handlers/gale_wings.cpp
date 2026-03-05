#include "exlaunch.hpp"

#include "features/ability_handlers/ability_handlers.h"

void InstallHooks_Ability_GaleWings() {
    // Treat HP as always full so gale wings is like Gen 6
    exl::patch::CodePatcher p(0x0195ec70);
    p.WriteInst(exl::armv8::inst::Movz(exl::armv8::reg::X0, 1));
}