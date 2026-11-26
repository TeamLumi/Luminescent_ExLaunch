#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/UI/SelectOpeningItem.h"

namespace Dpr::UI {
    struct SelectPlayerVisualItem : ILClass<SelectPlayerVisualItem> {
        struct Fields : Dpr::UI::SelectOpeningItem::Fields {
            bool sex;
            int32_t colorId;
        };
    };
}

namespace System::Collections::Generic {
    struct List$$SelectPlayerVisualItem : List<List$$SelectPlayerVisualItem, Dpr::UI::SelectPlayerVisualItem> {
        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_List_SelectPlayerVisualItem_Add> Method$$Add {};
    };
}