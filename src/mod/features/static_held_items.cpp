#include "exlaunch.hpp"
#include "externals/Dpr/EncountTools.h"
#include "externals/PlayerWork.h"
#include "externals/Pml/PokePara/CoreParam.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/Pml/PokeParty.h"
#include "externals/UnityEngine/Random.h"
#include "externals/Pml/PokePara/EggCheckType.h"
#include "data/abilities.h"
#include "data/items.h"
#include "data/utils.h"

#include "util.h"
#include "Logger/logger.h"



using namespace Pml;

HOOK_DEFINE_INLINE(SetWildPokemonItem) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {

        const int32_t ITEM1_BASERATE = 50;
        const int32_t ITEM1_BOOSTEDRATE = 60;
        const int32_t ITEM2_BASERATE = 20;
        const int32_t ITEM2_BOOSTEDRATE = 20;

        int32_t item1Rate;
        int32_t item2Rate;

        system_load_typeinfo(0x3f8c);

        PokeParty::Object *party = PlayerWork::get_playerParty();
        auto firstPoke = (PokePara::CoreParam *) party->GetMemberPointer(0);

        if (!firstPoke->IsEgg(PokePara::EggCheckType::BOTH_EGG)) {
            int32_t ability = firstPoke->GetTokuseiNo();
            switch (ability) {
                case array_index(ABILITIES, "Super Luck"):
                case array_index(ABILITIES, "Compound Eyes"):
                    item1Rate = ITEM1_BOOSTEDRATE;
                    item2Rate = ITEM2_BOOSTEDRATE;
                    break;
                default:
                    item1Rate = ITEM1_BASERATE;
                    item2Rate = ITEM2_BASERATE;
                    break;
            }
        }

        auto __this = (PokeParty::Object*) ctx->X[0];
        auto idx = (uint32_t) ctx->W[1];

        PokePara::PokemonParam* enemyPoke = __this->GetMemberPointer(0);
        int32_t randomRoll = UnityEngine::Random::Range(0,100);

        uint16_t setItem = Dpr::EncountTools::SetWildPokemonItem(enemyPoke, randomRoll, item1Rate, item2Rate);
        Logger::log("[SetWildPokemonItem] Item set to %s\n", ITEMS[setItem]);

        ctx->X[0] = (uint64_t) __this->GetMemberPointer(idx);
    }
};

void exl_static_held_items_main() {
    SetWildPokemonItem::InstallAtOffset(0x0179f85c);
}


