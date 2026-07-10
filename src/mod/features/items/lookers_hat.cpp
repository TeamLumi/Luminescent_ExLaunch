#include "exlaunch.hpp"

#include "data/items.h"
#include "data/species.h"
#include "data/utils.h"
#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"

// Speed boost: Hook BTL_POKEPARAM::GetValue to multiply base agility by 1.5x
// for Pikachu holding Looker's Hat. This feeds into Event_CalcAgility before
// trick room, paralysis, and event handlers, so all interactions work correctly.
HOOK_DEFINE_TRAMPOLINE(LookersHat_SpeedBoost) {
    static int32_t Callback(Dpr::Battle::Logic::BTL_POKEPARAM::Object* __this, int32_t vid, void* method) {
        int32_t result = Orig(__this, vid, method);
        if (vid == (int32_t)Dpr::Battle::Logic::BTL_POKEPARAM::ValueID::BPP_AGILITY &&
            (int32_t)__this->GetMonsNo() == array_index(SPECIES, "Pikachu") &&
            (int32_t)__this->GetItem() == array_index(ITEMS, "Looker's Hat")) {
            result = result * 3 / 2;
        }
        return result;
    }
};

// Crit boost: Hook BTL_POKEPARAM::GetCriticalRank to add +1 crit stage
// for Pikachu holding Looker's Hat. Downstream calc::Rounddown(rank, 3) caps it.
HOOK_DEFINE_TRAMPOLINE(LookersHat_CritBoost) {
    static uint32_t Callback(Dpr::Battle::Logic::BTL_POKEPARAM::Object* __this, void* method) {
        uint32_t result = Orig(__this, method);
        if ((int32_t)__this->GetMonsNo() == array_index(SPECIES, "Pikachu") &&
            (int32_t)__this->GetItem() == array_index(ITEMS, "Looker's Hat")) {
            result += 1;
        }
        return result;
    }
};

void exl_items_lookers_hat_main() {
    LookersHat_SpeedBoost::InstallAtOffset(0x01fdd010);  // BTL_POKEPARAM::GetValue
    LookersHat_CritBoost::InstallAtOffset(0x01FE0C10);   // BTL_POKEPARAM::GetCriticalRank
};
