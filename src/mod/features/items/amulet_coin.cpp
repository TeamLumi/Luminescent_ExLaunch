#include "exlaunch.hpp"

#include "data/items.h"
#include "data/utils.h"
#include "externals/PlayerWork.h"
#include "externals/Pml/PokePara/CoreParam.h"
#include "externals/Pml/PokeParty.h"

// The Amulet Coin/Luck Incense handlers are held-item event handlers, so they only run for
// party members that actually entered the battle; a benched holder never sets the per-client
// money double-up flag. Scan the saved party when the prize ratio is computed and apply the
// doubling once if anyone holds one of the items.
HOOK_DEFINE_TRAMPOLINE(MainModule_calcMoneyDblUpRatio) {
    static int32_t Callback(int64_t __this) {
        int32_t ratio = Orig(__this);

        if (ratio == 1) {
            Pml::PokeParty::Object* party = PlayerWork::get_playerParty();
            for (uint32_t i = 0; i < party->fields.m_memberCount; i++) {
                auto poke = (Pml::PokePara::CoreParam::Object*)party->GetMemberPointer(i);
                uint16_t item = poke->GetItem();
                if (item == array_index(ITEMS, "Amulet Coin") ||
                    item == array_index(ITEMS, "Luck Incense")) {
                    return 2;
                }
            }
        }

        return ratio;
    }
};

void exl_items_amulet_coin_main() {
    MainModule_calcMoneyDblUpRatio::InstallAtOffset(0x02036f30);
};
