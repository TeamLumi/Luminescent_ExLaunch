#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "externals/Dpr/Battle/Logic/Common.h"
#include "data/species.h"
#include "data/utils.h"
#include "move_handlers.h"

using namespace Dpr::Battle::Logic;

HOOK_DEFINE_REPLACE(Dpr_Battle_Logic_Handler_Waza_handler_IjigenRush) {
    static void Callback(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
        if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
            return;

        BTL_POKEPARAM::Object* bpp = Common::GetPokeParam(args, pokeID);
        uint16_t dexID = bpp->GetMonsNo();
        if (dexID != array_index(SPECIES, "Hoopa")) {
            Common::RewriteEventVar_FAIL_CAUSE(args, WazaFailCause::IJIGEN_OTHER);
            return;
        }

        if (bpp->fields.m_formNo != 1) {
            Common::RewriteEventVar_FAIL_CAUSE(args, WazaFailCause::IJIGEN_FOOPA);
            return;
        }
    }
};

void InstallHooks_Move_HyperspaceFury() {
    Dpr_Battle_Logic_Handler_Waza_handler_IjigenRush::InstallAtOffset(0x01822b60);
}