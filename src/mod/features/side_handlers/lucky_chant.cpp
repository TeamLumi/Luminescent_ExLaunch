#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/Handler/Side.h"
#include "side_handlers.h"

using namespace Dpr::Battle::Logic;

HOOK_DEFINE_REPLACE(Dpr_Battle_Logic_Handler_Side_handler_side_Omajinai) {
    static void Callback(EventFactor::EventHandlerArgs::Object** args, uint8_t mySide) {
        uint8_t pokeID = Common::GetEventVar(args, EventVar::Label::POKEID_DEF);
        if (Common::PokeIDtoSide(args, &pokeID) != mySide)
            return;

        Common::RewriteEventVar(args, EventVar::Label::FAIL_FLAG, 1);
    }
};

void InstallHooks_SideEffect_LuckyChant() {
    Dpr_Battle_Logic_Handler_Side_handler_side_Omajinai::InstallAtOffset(0x01939c40);
}