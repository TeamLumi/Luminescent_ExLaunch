#pragma once

#include "externals/il2cpp-api.h"
#include "externals/UnityEngine/Behaviour.h"

namespace UnityEngine {
    struct Animator : ILClass<Animator> {
        struct Fields : Behaviour::Fields {

        };

        inline int32_t GetInteger(int32_t id) {
            return external<int32_t>(0x02c07b70, this, id);
        }

        inline void SetInteger(System::String::Object* name, int32_t value) {
            external<void>(0x02c07c10, this, name, value);
        }
    };
}
