#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Contest/AContestPlayerData.h"
#include "externals/UnityEngine/Sprite.h"
#include "externals/UnityEngine/Vector3.h"
#include "externals/XLSXContent/PokemonInfo.h"

namespace Dpr::Contest {
    struct ContestPlayerEntity : ILClass<ContestPlayerEntity> {
        struct Fields {
            UnityEngine::Vector3::Object RESET_BALL_POS;
            XLSXContent::PokemonInfo::SheetCatalog::Object* catalog;
            UnityEngine::_Object::Object* assetPokeModel;
            void* iPtrSealEffects; // System_Collections_Generic_List_BtlvEffectInstance__o*
            void* iPtrLightEffect; // Dpr_Battle_View_Objects_BtlvEffectInstance_o*
            void* wazaPoke; // Dpr_Battle_View_Objects_BOPokemon_o*
            void* boTrainer; // Dpr_Battle_View_Objects_BOTrainer_o*
            void* boPokemon; // Dpr_Battle_View_Objects_BOPokemon_o*
            void* ballEntity; // Dpr_ObjectEntity_o*
            Dpr::Contest::AContestPlayerData::Object* playerData;
            UnityEngine::Sprite::Object* pmIconSpr;
            UnityEngine::Sprite::Object* wazaTypeIconSpr;
            UnityEngine::Vector3::Object defaultTrainerPos;
            UnityEngine::Vector3::Object defaultPokemonPos;
            int32_t trainerType;
            int32_t colorID;
            System::String::Object* characterModelPath;
            System::String::Object* pokeModelPath;
            System::String::Object* ballModelPath;
            System::String::Object* pokeIconPath;
        };

        static_assert(offsetof(Fields, pokeIconPath) == 0xA0);

        static inline StaticILMethod<0x04c7c628, UnityEngine::Sprite::Object*> Method$$AppendLoadPokemonIcon_b_99 {};
    };
}
