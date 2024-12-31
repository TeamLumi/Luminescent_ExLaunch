#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/UI/UIText.h"
#include "externals/KeyguideID.h"
#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/UnityEngine/UI/Image.h"

struct KeyguideItem : ILClass<KeyguideItem> {
    struct Param : ILClass<Param, 0x04c59fd8> {
        struct Fields {
            KeyguideID keyguideId;
        };

        inline void ctor() {
            external<void>(0x01aec4d0, this);
        }
    };

    struct Fields : UnityEngine::MonoBehaviour::Fields {
        UnityEngine::UI::Image::Object* _button;
        Dpr::UI::UIText::Object* _text;
    };
};

namespace System::Collections::Generic {
    struct List$$KeyguideItem_Param : List<List$$KeyguideItem_Param, KeyguideItem::Param> {
        static inline StaticILMethod<0x04c89da0> Method$$Clear {};
        static inline StaticILMethod<0x04c89d98, KeyguideItem::Param> Method$$Add {};
    };
}