#pragma once

#include "externals/il2cpp-api.h"

#include "externals/ContextMenuID.h"
#include "externals/Dpr/UI/UIText.h"
#include "externals/System/Collections/Generic/List.h"
#include "externals/System/String.h"
#include "externals/UnityEngine/Color.h"
#include "externals/UnityEngine/MonoBehaviour.h"

namespace Dpr::UI {
    struct ContextMenuItem : ILClass<ContextMenuItem, game::Address::Dpr_UI_ContextMenuItem_TypeInfo> {
        struct Param : ILClass<Param, game::Address::Dpr_UI_ContextMenuItem_Param_TypeInfo, game::Address::Dpr_UI_ContextMenuItem_Param_Array_TypeInfo> {
            struct Fields {
                ContextMenuID menuId;
                System::String::Object* messageFile;
                System::String::Object* messageLabel;
                int32_t messageIndex;
                System::String::Object* text;
                int32_t colorType;
            };

            inline void ctor() {
                external<void>(game::Address::Dpr_UI_ContextMenuItem_Param_ctor, this);
            }
        };

        struct Fields : UnityEngine::MonoBehaviour::Fields {
            Dpr::UI::UIText::Object* _text;
            UnityEngine::Color::Array* _fontColors;
            Dpr::UI::ContextMenuItem::Param::Object* _param;
            int32_t _index;
        };
    };
}

namespace System::Collections::Generic {
    struct List$$ContextMenuItem : List<List$$ContextMenuItem, Dpr::UI::ContextMenuItem> {
        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_List_ContextMenuItem_Add, Dpr::UI::ContextMenuItem> Method$$Add {};
    };

    struct List$$ContextMenuItem_Param : List<List$$ContextMenuItem_Param, Dpr::UI::ContextMenuItem::Param> {
        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_List_ContextMenuItem_Param_Add, Dpr::UI::ContextMenuItem::Param> Method$$Add {};
    };
}