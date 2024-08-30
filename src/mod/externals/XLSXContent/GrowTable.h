#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/Primitives.h"
#include "externals/UnityEngine/ScriptableObject.h"

namespace XLSXContent {
    struct GrowTable : ILClass<GrowTable> {
        struct SheetData : ILClass<SheetData> {
            struct Fields {
                System::UInt32_array* exps;
            };
        };

        struct Fields : UnityEngine::ScriptableObject::Fields {
            XLSXContent::GrowTable::SheetData::Array* Data;
        };
    };
}