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
            external<void>(0x02699f30, this, properties, materialIndex);
        }

        inline UnityEngine::Material::Array* get_materials() {
            return external<UnityEngine::Material::Array*>(0x0269a3b0, this);
        }

        inline void set_materials(UnityEngine::Material::Array* value) {
            external<void>(0x0269a400, this, value);
        }

        inline UnityEngine::Material::Array* get_sharedMaterials() {
            return external<UnityEngine::Material::Array*>(0x0269a590, this);
        }

        inline void set_sharedMaterials(UnityEngine::Material::Array* value) {
            external<void>(0x0269a5e0, this, value);
        }

        inline void set_material(UnityEngine::Material::Object* value) {
            external<void>(0x0269a4a0, this, value);
        }

        inline void set_sharedMaterial(UnityEngine::Material::Object* value) {
            external<void>(0x0269a540, this, value);
        }

        inline void set_enabled(bool value) {
            external<void>(0x0269a090, this, value);
        }
    };
}
