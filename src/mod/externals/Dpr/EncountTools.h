#pragma once
#include "Battle/Logic/BATTLE_SETUP_PARAM.h"
#include "externals/il2cpp-api.h"
#include "externals/Pml/PokeParty.h"
#include "externals/Pml/PokePara/PokemonParam.h"

namespace Dpr {
    struct EncountTools : ILClass<EncountTools, 0x4c59e90> {

        static inline uint16_t SetWildPokemonItem(Pml::PokePara::PokemonParam* pp, int32_t itemRnd, int32_t rnd1 , int32_t rnd2) {
            return external<uint16_t>(0x02c3aab0, pp, itemRnd, rnd1, rnd2);
        }
        
        static inline Pml::PokeParty::Object* CreateSimpleParty(int32_t monsNo0, int32_t level0, int32_t monsNo1, int32_t level1, Pml::PokeParty::Object* party, uint16_t sex, uint16_t seikaku, uint16_t sex1, uint16_t seikaku1, uint16_t form0, uint16_t form1, uint8_t talentVNum) {
            return external<Pml::PokeParty::Object*>(0x02c3a940, monsNo0, level0, monsNo1, level1, party, sex, seikaku, sex1, seikaku1, form0, form1, talentVNum);
        }

        static inline void SetupBattleWild(Dpr::Battle::Logic::BATTLE_SETUP_PARAM::Object battleSetupParam, Pml::PokeParty::Object* iPtrEnemyParty, int32_t arenaID, int32_t mapAttrib, int32_t weatherType, bool isSwim, bool isFishing, int32_t partnerID, bool isCaptureDemo, int32_t safariBallNum, bool isSymbol, bool isMitu, System::String::Object* overlapBgm, int32_t overlapSetupEffectId, bool isCantUseBall) {
            return external<void>(0x02c3abc0, battleSetupParam, iPtrEnemyParty, arenaID, mapAttrib, weatherType, isSwim, isFishing, partnerID, isCaptureDemo, safariBallNum, isSymbol, isMitu, overlapBgm, overlapSetupEffectId, isCantUseBall);
        }

    };
}


