#include "exlaunch.hpp"

#include "data/abilities.h"
#include "data/utils.h"
#include "externals/Dpr/Battle/Logic/BTL_CLIENT.h"
#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "externals/Dpr/Battle/Logic/CantEscapeCode.h"
#include "externals/FlagWork.h"

using namespace Dpr::Battle::Logic;

HOOK_DEFINE_TRAMPOLINE(CheckForbidChangeEscapeCommon) {
    static Dpr::Battle::Logic::CantEscapeCode Callback(BTL_CLIENT *__this, BTL_POKEPARAM *procPoke, uint8_t *pokeID, uint16_t *tokuseiID) {
        // Disallow escaping if can't escape flag is on
        if (FlagWork::GetFlag(FlagWork_Flag::FLAG_CANT_ESCAPE))
        {
            return Dpr::Battle::Logic::CantEscapeCode::CANTESC_TOOSENBOU;
        }

        // Nigeashi (Run Away) allows switch outs
        if (procPoke->GetValue(0x12) == array_index(ABILITIES, "Run Away"))
        {
            return Dpr::Battle::Logic::CantEscapeCode::CANTESC_NULL;
        }
        else
        {
            return Orig(__this, procPoke, pokeID, tokuseiID);
        }
    }
};

void exl_ability_changes_main() {
    CheckForbidChangeEscapeCommon::InstallAtOffset(0x01f574e0);

    // Treat HP as always full so gale wings is like Gen 6
    exl::patch::CodePatcher p(0x0195ec70);
    p.WriteInst(exl::armv8::inst::Movz(exl::armv8::reg::X0, 1));
}
