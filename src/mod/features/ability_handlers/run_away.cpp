#include "exlaunch.hpp"

#include "features/ability_handlers/ability_handlers.h"

#include "externals/Dpr/Battle/Logic/BTL_CLIENT.h"
#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "externals/Dpr/Battle/Logic/CantEscapeCode.h"

#include "data/abilities.h"
#include "data/utils.h"

using namespace Dpr::Battle::Logic;

HOOK_DEFINE_TRAMPOLINE(CheckForbidChangeEscapeCommon) {
    static Dpr::Battle::Logic::CantEscapeCode Callback(BTL_CLIENT *__this, BTL_POKEPARAM *procPoke, uint8_t *pokeID, uint16_t *tokuseiID) {
        // Run Away allows switch outs
        if (procPoke->GetValue(BTL_POKEPARAM::ValueID::BPP_TOKUSEI) == array_index(ABILITIES, "Run Away"))
            return Dpr::Battle::Logic::CantEscapeCode::CANTESC_NULL;
        else
            return Orig(__this, procPoke, pokeID, tokuseiID);
    }
};

void InstallHooks_Ability_RunAway() {
    CheckForbidChangeEscapeCommon::InstallAtOffset(0x01f574e0);
}