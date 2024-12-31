#pragma once
#include "externals/il2cpp-api.h"

namespace Dpr::Battle::Logic {
    struct Setup : ILClass<Setup> {

        static inline void BTL_SETUP_BattleInst(Dpr::Battle::Logic::BATTLE_SETUP_PARAM::Object* pSetupParam,
                                                Pml::PokeParty::Object* pPlayerParty,
                                                int32_t instTrainerEnemy1, Pml::PokeParty::Object* instEnemy1Party,
                                                void* instEnemy1SealTIDs, int32_t instTrainerEnemy2,
                                                Pml::PokeParty::Object* instEnemy2Party, void* instEnemy2SealTIDs,
                                                BTL_FIELD_SITUATION::Object* pFieldSituation, int32_t rule) {

            external<void>(0x01f6ffa0, pSetupParam, pPlayerParty, instTrainerEnemy1, instEnemy1Party,
                           instEnemy1SealTIDs, instTrainerEnemy2, instEnemy2Party, instEnemy2SealTIDs,
                           pFieldSituation, rule);
        }

        static inline void BATTLE_SETUP_FIELD_SITUATION_Init(BTL_FIELD_SITUATION::Object* sit) {
            external<void>(0x01f6e080, sit);
        }
    };
}