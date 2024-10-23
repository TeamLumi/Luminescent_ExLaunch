#pragma once

#include "externals/il2cpp-api.h"
#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"
#include "externals/UnityEngine/Transform.h"
#include "externals/Dpr/Battle/View/Systems/BattleViewUISystem.h"
#include "externals/Dpr/Battle/View/Systems/BattleViewSystem.h"

namespace Dpr::Battle::View {
    struct BattleViewCore : ILClass<BattleViewCore, 0x04c5ca30> {
        struct Fields : SmartPoint::AssetAssistant::SingletonMonoBehaviour::Fields {
            void* _shadowCastMaterial;
            UnityEngine::Transform::Object* _cluster;
            Dpr::Battle::View::Systems::BattleViewSystem::Object* _ViewSystem_k__BackingField;
            Dpr::Battle::View::Systems::BattleViewUISystem::Object* _UISystem_k__BackingField;
        };

        static inline StaticILMethod<0x04c676c0, BattleViewCore> Method$BattleViewCore_get_Instance {};

        static inline Dpr::Battle::View::BattleViewCore::Object* get_Instance() {
            return SmartPoint::AssetAssistant::SingletonMonoBehaviour::get_Instance(Method$BattleViewCore_get_Instance);
        }
    };
}
