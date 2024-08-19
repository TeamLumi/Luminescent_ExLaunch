#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/Handler/Side.h"
#include "externals/Dpr/Battle/Logic/SICKCONT.h"
#include "externals/Dpr/Battle/Logic/TemptTargetCause.h"
#include "externals/Dpr/Battle/Logic/TemptTargetPriority.h"
#include "side_handlers.h"

using namespace Dpr::Battle::Logic;

HOOK_DEFINE_REPLACE(Dpr_Battle_Logic_Handler_Side_handler_SpotLight_TemptTarget) {
    static void Callback(EventFactor::EventHandlerArgs::Object** args, uint8_t mySide) {
        uint8_t pokeID = Common::GetEventVar(args, EventVar::Label::POKEID_ATK);

        if (Common::PokeIDtoSide(args, &pokeID) != mySide)
            return;

        BTL_SICKCONT::Object cont = (*args)->fields.pBattleEnv->GetSideEffectStatus(mySide, Common::GetSubID(args))->GetContParam();
        Common::RewriteEventVar(args, EventVar::Label::POKEID_DEF, SICKCONT::GetPokeID(&cont));
        Common::RewriteEventVar(args, EventVar::Label::TEMPT_TARGET_PRIORITY, (int32_t)TemptTargetPriority::WAZA_2);
        Common::RewriteEventVar(args, EventVar::Label::TEMPT_TARGET_CAUSE, (int32_t)TemptTargetCause::OTHER);
    }
};

void InstallHooks_SideEffect_Spotlight() {
    Dpr_Battle_Logic_Handler_Side_handler_SpotLight_TemptTarget::InstallAtOffset(0x0193bd10);
}