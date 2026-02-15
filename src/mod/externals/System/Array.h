#pragma once

#include "externals/il2cpp-api.h"

#include "externals/DPData/PoffinData.h"
#include "externals/System/Predicate.h"

namespace System {
    struct Array : ILClass<Array> {
        struct Fields {};

        // System::Object
        template <typename T>
        static inline int32_t FindIndex(T::Array* array, System::Predicate::Object* match) {
            return external<int32_t>(0x026c51f0, array, match, *T::Method$$System_Array_FindIndex);
        }

        // DPData::PoffinData
        static inline int32_t FindIndex(DPData::PoffinData::Array* array, System::Predicate::Object* match) {
            return external<int32_t>(0x026c4730, array, match, *DPData::PoffinData::Method$$System_Array_FindIndex);
        }
    };
}
