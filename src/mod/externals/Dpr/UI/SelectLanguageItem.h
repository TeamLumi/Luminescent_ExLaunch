#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Dpr/UI/SelectOpeningItem.h"

namespace Dpr::UI {
    struct SelectLanguageItem : ILClass<SelectLanguageItem> {
        struct Fields : Dpr::UI::SelectOpeningItem::Fields {
            int32_t _langId;
            bool _useKanji;
        };
    };
}

namespace System::Collections::Generic {
    struct List$$SelectLanguageItem : List<List$$SelectLanguageItem, Dpr::UI::SelectLanguageItem> {
        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_List_SelectLanguageItem_Add> Method$$Add {};
    };
}