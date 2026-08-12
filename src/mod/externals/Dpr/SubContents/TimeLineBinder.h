#pragma once

#include <externals/EnvironmentSettings.h>

#include "externals/il2cpp-api.h"

#include "externals/BattlePokemonEntity.h"
#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/UnityEngine/Transform.h"

namespace Dpr::SubContents {
    struct TimeLineBinder : ILClass<TimeLineBinder> {
        struct Fields : UnityEngine::MonoBehaviour::Fields {
            void* director; // UnityEngine_Playables_PlayableDirector_o *
            UnityEngine::Transform::Object* Camera;
            void* BindAssets; // System_Collections_Generic_List_BindModel__o *
            void* BindEffects; // System_Collections_Generic_List_BindModelEffect__o *
            void* BindSounds; // System_Collections_Generic_List_BindModelSound__o *
            EnvironmentSettings::Object* EnvSettings;
            void* MyEnvironmentController; // EnvironmentController_o *
            void* PrevEnvController; // EnvironmentController_o *
            void* timeLineAsset; // UnityEngine_Timeline_TimelineAsset_o *
            void* ExternalAssets; // System_Collections_Generic_Dictionary_string__TimeLineBinder_IBindData__o *
            void* PokeAssets; // System_Collections_Generic_Dictionary_int__Object__o *
            System::Action::Object* OnTimeLineCallBack;
            void* OnComplete; // System_Action_PlayableDirector__o *
            BattlePokemonEntity::Object* battlePokeEntity;
            int32_t Button01;
            int32_t Button02;
            bool isActive;
            int32_t StartIndex;
            int32_t Button10;
            Dpr::PatcheelPattern::Object* ppp;
            uint32_t ppval;
            int32_t Button11;
            float DebugTime;
            int32_t Button010;
        };
    };
}
