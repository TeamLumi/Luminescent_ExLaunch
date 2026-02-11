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
#include "logger/logger.h"



using namespace Pml;

HOOK_DEFINE_INLINE(SetWildPokemonItem) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {

        system_load_typeinfo(0x3f8c);

        auto __this = (PokeParty::Object*) ctx->X[0];
        auto idx = (uint32_t) ctx->W[1];

        PokePara::PokemonParam* enemyPoke = __this->GetMemberPointer(0);
        int32_t randomRoll = UnityEngine::Random::Range(0,100);

        uint16_t setItem = Dpr::EncountTools::SetWildPokemonItem(enemyPoke, randomRoll, 0, 0);
        Logger::log("[SetWildPokemonItem] Item set to %s\n", ITEMS[setItem]);

        ctx->X[0] = (uint64_t) __this->GetMemberPointer(idx);
    }
};

void exl_static_held_items_main() {
    SetWildPokemonItem::InstallAtOffset(0x0179f85c);
}


