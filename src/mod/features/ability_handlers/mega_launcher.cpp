#include "exlaunch.hpp"

#include "features/ability_handlers/ability_handlers.h"

#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventFactor.h"
#include "externals/Dpr/Battle/Logic/FX32.h"
#include "externals/Dpr/Battle/Logic/WAZADATA.h"

#include "data/moves.h"
#include "data/utils.h"

using namespace Dpr::Battle::Logic;

HOOK_DEFINE_REPLACE(Tokusei$$handler_MegaLauncher_Pow) {
    static void Callback(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
        system_load_typeinfo(0x89e3);

        Common::getClass()->initIfNeeded();

        if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
            return;

        auto waza = Common::GetEventVar(args, EventVar::Label::WAZAID);
        switch (waza)
        {
            case array_index(MOVES, "Water Pulse"):
            case array_index(MOVES, "Aura Sphere"):
            case array_index(MOVES, "Dark Pulse"):
            case array_index(MOVES, "Dragon Pulse"):
            case array_index(MOVES, "Flash Cannon"):
            case array_index(MOVES, "Origin Pulse"):
            case array_index(MOVES, "Terrain Pulse"):
            case array_index(MOVES, "Armor Cannon"):
                Common::MulEventVar(args, EventVar::Label::WAZA_POWER_RATIO, FX32::CONST_1_5);
                break;
        }
    }
};

void InstallHooks_Ability_MegaLauncher() {
    Tokusei$$handler_MegaLauncher_Pow::InstallAtOffset(0x0195e950);
}