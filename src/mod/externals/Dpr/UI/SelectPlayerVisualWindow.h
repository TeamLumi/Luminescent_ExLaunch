#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/UI/Cursor.h"
#include "externals/Dpr/UI/SelectPlayerVisualItem.h"
#include "externals/Dpr/UI/SelectOpeningBase.h"
#include "externals/UnityEngine/Events/UnityAction.h"
#include "externals/UnityEngine/RectTransform.h"

namespace Dpr::UI {
    struct SelectPlayerVisualWindow : ILClass<SelectPlayerVisualWindow> {
        struct Fields : Dpr::UI::SelectOpeningBase::Fields {
            UnityEngine::RectTransform::Object* _content;
            Dpr::UI::Cursor::Object* _cursor;
            System::Collections::Generic::List$$SelectPlayerVisualItem::Object* _items;
            int32_t _selectIndex;
        };

        inline bool SetSelectIndex(int32_t index, bool isInitialized) {
            return external<bool>(0x01d3e050, this, index, isInitialized);
        }
    };
}