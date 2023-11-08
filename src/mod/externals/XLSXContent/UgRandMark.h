#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/Primitives.h"
#include "externals/System/String.h"
#include "externals/UnityEngine/ScriptableObject.h"

namespace XLSXContent {
    struct UgRandMark : ILClass<UgRandMark, 0x04c657b0> {
        struct Sheettable : ILClass<Sheettable> {
            struct Fields {
                int32_t id;
                System::String::Object* FileName;
                int32_t size;
                int32_t min;
                int32_t max;
                int32_t smax;
                int32_t mmax;
                int32_t lmax;
                int32_t llmax;
                int32_t watermax;
                System::Int32_array* typerate;
            };
        };

        struct Fields : UnityEngine::ScriptableObject::Fields {
            Sheettable::Array* table;
        };
    };
}
