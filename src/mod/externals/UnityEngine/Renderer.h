#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/Component.h"
#include "externals/UnityEngine/Material.h"
#include "externals/UnityEngine/MaterialPropertyBlock.h"

namespace UnityEngine {
    struct Renderer : ILClass<Renderer> {
        struct Fields : UnityEngine::Component::Fields {
        };

        inline void SetPropertyBlock(UnityEngine::MaterialPropertyBlock::Object* properties, int32_t materialIndex) {
            external<void>(game::Address::UnityEngine_Renderer_SetPropertyBlock, this, properties, materialIndex);
        }

        inline UnityEngine::Material::Array* get_materials() {
            return external<UnityEngine::Material::Array*>(game::Address::UnityEngine_Renderer_get_materials, this);
        }

        inline void set_materials(UnityEngine::Material::Array* value) {
            external<void>(game::Address::UnityEngine_Renderer_set_materials, this, value);
        }

        inline UnityEngine::Material::Array* get_sharedMaterials() {
            return external<UnityEngine::Material::Array*>(game::Address::UnityEngine_Renderer_get_sharedMaterials, this);
        }

        inline void set_sharedMaterials(UnityEngine::Material::Array* value) {
            external<void>(game::Address::UnityEngine_Renderer_set_sharedMaterials, this, value);
        }

        inline void set_material(UnityEngine::Material::Object* value) {
            external<void>(game::Address::UnityEngine_Renderer_set_material, this, value);
        }

        inline void set_sharedMaterial(UnityEngine::Material::Object* value) {
            external<void>(game::Address::UnityEngine_Renderer_set_sharedMaterial, this, value);
        }

        inline void set_enabled(bool value) {
            external<void>(game::Address::UnityEngine_Renderer_set_enabled, this, value);
        }
    };
}
