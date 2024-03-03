#pragma once

#include "externals/il2cpp-api.h"
#include "externals/UnityEngine/ScriptableObject.h"

namespace XLSXContent {
    struct MonohiroiTable : ILClass<MonohiroiTable> {
        struct SheetMonoHiroi : ILClass<SheetMonoHiroi> {
            struct Fields {
                uint16_t ID;
                System::Byte_array* Ratios;
            };
        };

        struct Fields : UnityEngine::ScriptableObject::Fields {
            SheetMonoHiroi::Array* MonoHiroi;
        };

    };


};