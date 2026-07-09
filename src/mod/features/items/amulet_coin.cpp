#include "exlaunch.hpp"

#include "data/items.h"
#include "data/utils.h"
#include "externals/Dpr/Battle/Logic/MainModule.h"
#include "externals/Pml/PokePara/CoreParam.h"
#include "externals/Pml/PokeParty.h"

// The Amulet Coin/Luck Incense handlers are held-item event handlers, so they only run for
// party members that actually entered the battle; a benched holder never sets the per-client
// money double-up flag. FixRegularMoney inlines the flag scan that multiplies the prize, so
// set the player's flag right before it runs if anyone in the party holds one of the items.
// SetMoneyDblUp just writes a 1 into the per-client flag array, so an in-battle holder that
// already set it stays at the vanilla 2x — no stacking.
HOOK_DEFINE_TRAMPOLINE(MainModule_FixRegularMoney) {
    static uint32_t Callback(Dpr::Battle::Logic::MainModule::Object* __this) {
        uint8_t clientID = __this->fields.m_myClientID;
        Pml::PokeParty::Object* party = __this->GetSrcParty(clientID, false);
        if (party != nullptr) {
            for (uint32_t i = 0; i < party->fields.m_memberCount; i++) {
                auto poke = (Pml::PokePara::CoreParam::Object*)party->GetMemberPointer(i);
                uint16_t item = poke->GetItem();
                if (item == array_index(ITEMS, "Amulet Coin") ||
                    item == array_index(ITEMS, "Luck Incense")) {
                    __this->SetMoneyDblUp(clientID);
                    break;
                }
            }
        }

        return Orig(__this);
    }
};

void exl_items_amulet_coin_main() {
    MainModule_FixRegularMoney::InstallAtOffset(0x02036e50);
};
