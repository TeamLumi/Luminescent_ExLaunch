#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/String.h"
#include "externals/UnityEngine/_Object.h"

namespace UnityEngine {
    struct Shader : ILClass<Shader> {
        struct Fields : UnityEngine::_Object::Fields {
            // TODO
        };

        static inline int32_t PropertyToID(System::String::Object* name) {
            return external<int32_t>(0x02996620, name);
        }
    };
}