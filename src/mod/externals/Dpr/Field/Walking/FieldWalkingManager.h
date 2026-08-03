#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Dpr/Field/Walking/WalkingAIManager.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/System/String.h"
#include "externals/UnityEngine/Vector3.h"

namespace Dpr::Field::Walking {
    struct FieldWalkingManager : ILClass<FieldWalkingManager> {
        struct Fields : Dpr::Field::Walking::WalkingAIManager::Fields {
            int32_t prevArea;
            Pml::PokePara::PokemonParam::Object* _PartnerPokeParam_k__BackingField;
            System::String::Object* PartnerNPC_ObjectName;
            UnityEngine::Vector3::Object _EntryPoint_k__BackingField;
            void* PokeAssets; // System_Collections_Generic_Dictionary_int__Object__o*
            void* Controller; // Dpr_Field_Walking_WalkingCharacterController_o*
            void* pokeTalkModel; // Dpr_Field_Walking_FieldPokeTalkModel_o*
            bool isCancel;
            bool isForceEnter;
            void* talkList; // System_Collections_Generic_List_FieldWalkingPokeTalk_SheetSheet1__o*
            void* kinomiTable; // XLSXContent_FieldWalkingKinomiTable_o*
            void* seikakuTable; // System_Collections_Generic_List_FieldWalkingKinomiSeikakuTable_SheetSheet1__o*
            bool isEvent;
            bool isBattleRetrurnCreate;
            void* deleteTween; // DG_Tweening_Tween_o*
            void* ChangePos; // DG_Tweening_Tweener_o*
        };

        void SetPartnerPoke(Pml::PokePara::PokemonParam::Object* poke) {
            return external<void>(0x01cd62e0, this, poke);
        }

        void CheckPartnerPokeChange(Pml::PokePara::PokemonParam::Object* param, bool isDelete) {
            return external<void>(0x01cd6dc0, this, param, isDelete);
        }
    };
}