#include "exlaunch.hpp"

#include "features/ability_handlers/ability_handlers.h"

#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventFactor.h"
#include "externals/Dpr/Battle/Logic/WAZADATA.h"

#include "data/types.h"
#include "data/utils.h"

using namespace Dpr::Battle::Logic;

HOOK_DEFINE_REPLACE(Tokusei$$handler_HayateNoTsubasa) {
    static void Callback(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
        system_load_typeinfo(0x8990);
        Common::getClass()->initIfNeeded();

        if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
            return;

        if (Common::GetEventVar(args, EventVar::Label::WAZA_TYPE) != array_index(TYPES, "Flying"))
            return;

        auto prio = Common::GetEventVar(args, EventVar::Label::WAZA_PRI);
        Common::RewriteEventVar(args, EventVar::Label::WAZA_PRI, prio + 1);
    }
};

void InstallHooks_Ability_GaleWings() {
    Tokusei$$handler_HayateNoTsubasa::InstallAtOffset(0x0195ebd0);
}