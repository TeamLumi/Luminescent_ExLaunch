#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Dpr/UI/SelectLanguageItem.h"
#include "externals/Dpr/UI/SelectOpeningBase.h"
#include "externals/UnityEngine/Events/UnityAction.h"
#include "externals/UnityEngine/RectTransform.h"
#include "externals/Dpr/UI/ContextMenuItem.h"

namespace Dpr::UI {
    struct SelectLanguageWindow : ILClass<SelectLanguageWindow> {
        struct Fields : Dpr::UI::SelectOpeningBase::Fields {
            UnityEngine::RectTransform::Object* _content;
            void* _cursor; // struct Dpr_UI_Cursor_o*
            System::Collections::Generic::List$$SelectLanguageItem::Object* _items;
            int32_t _selectIndex;
            int32_t _reloadAllMsbtState;
            UnityEngine::Events::UnityAction::Object* onReloadLanguage;
        };

        inline bool SetSelectIndex(int32_t index, bool isInitialized) {
            return external<bool>(0x01d3c220, this, index, isInitialized);
        }

        static inline StaticILMethod<0x04c7fb88> Method$$SelectLanguageWindow__OpenConfirmMessageWindow__b__18_0 {};
        static inline StaticILMethod<0x04c6bdc0, Dpr::UI::ContextMenuItem::Object*> Method$$SelectLanguageWindow__OpenConfirmMessageWindow__b__18_1 {};
    };
}