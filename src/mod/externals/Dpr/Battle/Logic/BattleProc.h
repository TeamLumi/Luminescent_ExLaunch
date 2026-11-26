#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Dpr/Battle/Logic/BATTLE_SETUP_PARAM.h"
#include "externals/Dpr/Battle/Logic/MainModule.h"
#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"
#include "externals/UnityEngine/Transform.h"

namespace Dpr::Battle::Logic {
    struct BattleProc : ILClass<BattleProc, game::Address::Dpr_Battle_Logic_BattleProc_TypeInfo> {
        struct FinalizeCoroutine_d_34 : ILClass<FinalizeCoroutine_d_34> {
            struct Fields {
                int32_t __1__state;
                Il2CppObject* __2__current;
                Dpr::Battle::Logic::BattleProc::Object* __4__this;
                void* __8__1; // Dpr_Battle_Logic_BattleProc___c__DisplayClass34_0_o*
                int32_t _evolveCheckTargetIndices_5__2;
            };
        };

        struct Fields : SmartPoint::AssetAssistant::SingletonMonoBehaviour::Fields {
            UnityEngine::Transform::Object* cluster;
            Dpr::Battle::Logic::MainModule::Object* mainModule;
            void* updateCore; // Dpr_Battle_Logic_BattleProc_UpdateCoreFunc_o*
            int32_t subSeq;
            bool isFinalized;
        };

        struct StaticFields {
            Dpr::Battle::Logic::BATTLE_SETUP_PARAM::Object* setupParam;
            bool _isInitialized;
            bool _isEnd;
            void* onEndFunc;
            void* onInitializedFunc;
            bool isDebugLoopStop;
        };
    };
}
