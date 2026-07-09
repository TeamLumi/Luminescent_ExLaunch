#include "exlaunch.hpp"

#include "data/items.h"
#include "data/utils.h"
#include "externals/Dpr/Battle/Logic/MainModule.h"
#include "externals/Dpr/Battle/Logic/MoneyDblUpCause.h"
#include "externals/Pml/PokePara/CoreParam.h"
#include "externals/Pml/PokeParty.h"

// The Amulet Coin/Luck Incense handlers are held-item event handlers, so they only run for
// party members that actually entered the battle; a benched holder never sets the STD money
// double-up cause flag. FixRegularMoney inlines the cause-flag scan that multiplies the
// prize, so set the flag right before it runs if anyone in the player's party holds one of
// the items. SetMoneyDblUp just writes a 1 into the cause slot, so an in-battle holder that
// already set it stays at the vanilla 2x — no stacking.
HOOK_DEFINE_TRAMPOLINE(MainModule_FixRegularMoney) {
    static uint32_t Callback(Dpr::Battle::Logic::MainModule::Object* __this) {
        Pml::PokeParty::Object* party = __this->GetSrcParty(__this->fields.m_myClientID, false);
        for (uint32_t i = 0; i < party->fields.m_memberCount; i++) {
            auto poke = (Pml::PokePara::CoreParam::Object*)party->GetMemberPointer(i);
            uint16_t item = poke->GetItem();
            if (item == array_index(ITEMS, "Amulet Coin") ||
                item == array_index(ITEMS, "Luck Incense")) {
                __this->SetMoneyDblUp(Dpr::Battle::Logic::MoneyDblUpCause::MONEY_DBLUP_STD);
                break;
            }
        }

        return Orig(__this);
    }
};

void exl_items_amulet_coin_main() {
    MainModule_FixRegularMoney::InstallAtOffset(0x02036e50);
};
