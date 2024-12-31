#pragma once

#include "externals/il2cpp-api.h"

#include "externals/ContextMenuID.h"
#include "externals/Dpr/UI/UIText.h"
#include "externals/System/Collections/Generic/List.h"
#include "externals/System/String.h"
#include "externals/UnityEngine/Color.h"
#include "externals/UnityEngine/MonoBehaviour.h"

namespace Dpr::UI {
    struct ContextMenuItem : ILClass<ContextMenuItem, 0x04c61bc8> {
        struct Param : ILClass<Param, 0x04c5e788> {
            struct Fields {
                ContextMenuID menuId;
                System::String::Object* messageFile;
                System::String::Object* messageLabel;
                int32_t messageIndex;
                System::String::Object* text;
                int32_t colorType;
            };

            inline void ctor() {
                external<void>(0x01ebee30, this);
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
    struct List$$ContextMenuItem_Param : List<List$$ContextMenuItem_Param, Dpr::UI::ContextMenuItem::Param, 0x04c5efb8> {
        static inline StaticILMethod<0x04c896c0, Dpr::UI::ContextMenuItem::Param> Method$$Add {};
        static inline StaticILMethod<0x04c896b8> Method$$ctor {};
        static inline StaticILMethod<0x04c68940> Method$$ToArray {};

        inline Dpr::UI::ContextMenuItem::Param::Array* ToArray() {
            return external<Dpr::UI::ContextMenuItem::Param::Array*>(0x02a3e720, this, *Method$$ToArray);
        }
    };
}