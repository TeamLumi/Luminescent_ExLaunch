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
                    .fields = external<UnityEngine::Color::Fields>(0x026b5e20, this, name)
            };
        }

        inline void SetColor(System::String::Object* name, UnityEngine::Color::Object value) {
            UnityEngine::Color::Fields valueProxy = { .r = value.fields.r, .g = value.fields.g, .b = value.fields.b, .a = value.fields.a };
            external<void>(0x026b6020, this, name, valueProxy);
        }

        inline bool HasProperty(System::String::Object* name) {
            return external<bool>(0x026b6c70, this, name);
        }
    };
}