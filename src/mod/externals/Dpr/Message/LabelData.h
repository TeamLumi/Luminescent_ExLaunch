#pragma once

#include "externals/il2cpp-api.h"
#include "externals/System/String.h"

namespace Dpr::Message {
    struct LabelData : ILClass<LabelData> {
        struct Fields {
            int32_t labelIndex;
            int32_t arrayIndex;
            System::String::Object* labelName;
            void* styleInfo;
            System::Int32_array* attributeValueArray;
            void* tagDataArray;
            void* wordDataArray;
        };
    };
}

