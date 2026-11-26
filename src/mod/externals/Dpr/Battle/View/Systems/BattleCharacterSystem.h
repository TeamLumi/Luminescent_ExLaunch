#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Dpr/Battle/View/Objects/BOPokemon.h"

namespace Dpr::Battle::View::Systems {
    struct BattleCharacterSystem : ILClass<BattleCharacterSystem> {
        struct Fields {
            void* _viewSystem; //Dpr::Battle::View::Systems::BattleViewSystem::Object*
            void* _trainers; //Dpr_Battle_View_Objects_BOTrainer_array*
            Dpr::Battle::View::Objects::BOPokemon::Array* _pokemons;
        };

        inline Dpr::Battle::View::Objects::BOPokemon::Object * GetPokemonModel(int32_t vPos) {
            return external<Dpr::Battle::View::Objects::BOPokemon::Object *>(game::Address::Dpr_Battle_View_Systems_BattleCharacterSystem_GetPokemonModel, this, vPos);
        }
    };
}