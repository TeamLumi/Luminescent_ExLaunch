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

        inline void set_material(UnityEngine::Material::Object* value) {
            external<void>(0x0269a4a0, this, value);
        }
    };
}
