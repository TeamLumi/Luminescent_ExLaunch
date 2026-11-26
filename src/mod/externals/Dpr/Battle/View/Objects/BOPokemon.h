#pragma once

#include "externals/il2cpp-api.h"
#include "externals/BattlePokemonEntity.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/Dpr/Battle/View/Objects/MotionTimingData.h"
#include "externals/Dpr/Battle/View/Objects/BattleViewCharacter.h"

namespace Dpr::Battle::View::Objects {
    struct BOPokemon : ILClass<BOPokemon> {
        struct Fields : Dpr::Battle::View::Objects::BattleViewCharacter::Fields {
            BattlePokemonEntity::Object* _entity;
            Pml::PokePara::PokemonParam::Object* _param;
            int32_t _lastPlayAnimationState;
            Dpr::Battle::View::Objects::MotionTimingData::Object _motionTimingData;
            void* _motionReplaceData; //XLSXContent_BattleDataTable_SheetMotionReplaceData_o*
            bool m_isPlayPinchSoundRTPC;
            bool m_isPlayPinchSound;
            void* _iPtrMigawariObject; //Dpr_ObjectEntity_o*
            void* _iPtrDigudaStone; //Dpr_ObjectEntity_o*
            bool _IsEnableFloat_k__BackingField;
            bool _IsVisibleDigudaStone_k__BackingField;
            bool _HitBackFlg_k__BackingField;
            bool _IsVisibleMigawari_k__BackingField;
            bool _IsVisibleTame_k__BackingField;
        };

        inline uint8_t get_IsGChange() {
            return external<uint8_t>(game::Address::Dpr_Battle_View_Objects_BOPokemon_get_IsGChange, this);
        }
    };
}