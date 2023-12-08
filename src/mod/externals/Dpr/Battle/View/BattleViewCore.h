#pragma once

#include "externals/il2cpp-api.h"
#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"
#include "externals/UnityEngine/Transform.h"
#include "externals/Dpr/Battle/View/Systems/BattleViewSystem.h"
#include "externals/Dpr/Battle/View/Systems/BattleViewUISystem.h"

namespace Dpr::Battle::View {
    struct BattleViewCore : ILClass<BattleViewCore, 0x04c5ca30> {
        struct Fields : SmartPoint::AssetAssistant::SingletonMonoBehaviour::Fields {
            void* _shadowCastMaterial;
            UnityEngine::Transform::Object* _cluster;
            Dpr::Battle::View::Systems::BattleViewSystem::Object* _ViewSystem_k__BackingField;
            Dpr::Battle::View::Systems::BattleViewUISystem::Object* _UISystem_k__BackingField;
        };

        static inline Dpr::Battle::View::BattleViewCore::Object* instance() {
            return SmartPoint::AssetAssistant::SingletonMonoBehaviour::get_Instance(SmartPoint::AssetAssistant::SingletonMonoBehaviour::Method$$BattleViewCore$$get_Instance);
        }
    };
}
