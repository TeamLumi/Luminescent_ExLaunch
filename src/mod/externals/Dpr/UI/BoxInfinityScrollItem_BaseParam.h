#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::UI {
    struct BoxInfinityScrollItem_BaseParam : ILClass<BoxInfinityScrollItem_BaseParam> {
        struct Fields {
            int32_t paramIndex;
            void* itemParams; // System_Collections_Generic_List_BoxInfinityScrollItem_BaseParam_ItemParam__o*
            int32_t displayMode;
        };
    };
}