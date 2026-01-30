#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/ScriptableObject.h"

namespace XLSXContent {
    struct MapAttributeExTable : ILClass<MapAttributeExTable> {
        struct SheetData : ILClass<SheetData> {
            struct Fields {
                int32_t AttributeEx;
                int32_t month;
                int32_t day;
                int32_t add_rate;
                int32_t add_hatching;
            };
        };

        struct Fields : UnityEngine::ScriptableObject::Fields {
            XLSXContent::MapAttributeExTable::SheetData::Array* Data;
        };
    };
}
