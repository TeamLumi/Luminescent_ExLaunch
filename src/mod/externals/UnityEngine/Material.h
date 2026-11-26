#pragma once

#include "externals/il2cpp-api.h"
#include "externals/System/String.h"
#include "externals/UnityEngine/_Object.h"
#include "externals/UnityEngine/Color.h"

namespace UnityEngine {
    struct Material : ILClass<Material> {
        struct Fields : UnityEngine::_Object::Fields {
            //TODO
        };

        inline UnityEngine::Color::Object GetColor(System::String::Object* name) {
            return {
                .fields = external<UnityEngine::Color::Fields>(game::Address::UnityEngine_Material_GetColor, this, name)
            };
        }

        inline void SetColor(System::String::Object* name, UnityEngine::Color::Object value) {
            UnityEngine::Color::Fields valueProxy = { .r = value.fields.r, .g = value.fields.g, .b = value.fields.b, .a = value.fields.a };
            external<void>(game::Address::UnityEngine_Material_SetColor, this, name, valueProxy);
        }

        inline float GetFloat(System::String::Object* name) {
            return external<float>(game::Address::UnityEngine_Material_GetFloat, this, name);
        }

        inline void SetFloat(System::String::Object* name, float value) {
            external<void>(game::Address::UnityEngine_Material_SetFloat, this, name, value);
        }

        inline bool HasProperty(System::String::Object* name) {
            return external<bool>(game::Address::UnityEngine_Material_HasProperty, this, name);
        }

        inline System::String::Array* GetShaderKeywords() {
            return external<System::String::Array*>(game::Address::UnityEngine_Material_GetShaderKeywords, this);
        }
    };
}