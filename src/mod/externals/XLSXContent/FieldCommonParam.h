#pragma once

#include "externals/il2cpp-api.h"
#include "externals/UnityEngine/ScriptableObject.h"

namespace XLSXContent {
    struct FieldCommonParam : ILClass<FieldCommonParam> {
        struct Sheetdata : ILClass<Sheetdata> {
            struct Fields {
                float param;
            };
        };

        struct Fields : UnityEngine::ScriptableObject::Fields {
            XLSXContent::FieldCommonParam::Sheetdata::Array* data;
        };

        inline Sheetdata::Object* get_Item(int32_t index) {
            return external<Sheetdata::Object*>(0x017d4a70, this, index);
        }
    };
}
