#pragma once

#include "externals/il2cpp-api.h"

#include "externals/BattlePokemonEntity.h"
#include "externals/Dpr/Battle/View/Objects/BOPokemon.h"
#include "externals/UnityEngine/Color.h"
#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/UnityEngine/Transform.h"

namespace Dpr::SubContents {
    struct TimeLineMotion : ILClass<TimeLineMotion> {
        struct Fields : UnityEngine::MonoBehaviour::Fields {
            void* playerEntity; // BattlePlayerEntity_o*
            BattlePokemonEntity::Object* pokeEntity;
            Dpr::Battle::View::Objects::BOPokemon::Object* boPokemon;
            UnityEngine::Color::Object AddColor;
            float LoopSec;
            UnityEngine::Transform::Object* Waist;
            void* heightList; // System_Collections_Generic_List_float__o*
            float HeightLimit;
            uint32_t Pattern;
            bool updatePattern;
            float prevScale;
            int32_t DebugIndex;
            int32_t Button01;
        };

        static_assert(offsetof(Fields, Button01) == 0x5C);
        static_assert(sizeof(Fields) == 0x60);
    };
}
