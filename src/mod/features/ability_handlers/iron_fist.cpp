#include "exlaunch.hpp"

#include "features/ability_handlers/ability_handlers.h"

#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventFactor.h"
#include "externals/Dpr/Battle/Logic/FX32.h"
#include "externals/Dpr/Battle/Logic/WAZADATA.h"

using namespace Dpr::Battle::Logic;

HOOK_DEFINE_REPLACE(Tokusei$$handler_TetunoKobusi) {
    static void Callback(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
        system_load_typeinfo(0x8a7a);

        Common::getClass()->initIfNeeded();

        if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
            return;

        auto waza = Common::GetEventVar(args, EventVar::Label::WAZAID);
        if (WAZADATA::GetFlag(waza, Pml::WazaData::WazaFlag::PUNCH))
            Common::MulEventVar(args, EventVar::Label::WAZA_POWER_RATIO, FX32::CONST_1_5);
    }
};

void InstallHooks_Ability_IronFist() {
    Tokusei$$handler_TetunoKobusi::InstallAtOffset(0x01943660);
}