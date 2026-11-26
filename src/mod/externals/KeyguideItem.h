#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/UI/UIText.h"
#include "externals/KeyguideID.h"
#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/UnityEngine/UI/Image.h"

struct KeyguideItem : ILClass<KeyguideItem> {
    struct Param : ILClass<Param, game::Address::KeyguideItem_Param_TypeInfo> {
        struct Fields {
            KeyguideID keyguideId;
        };

        inline void ctor() {
            external<void>(game::Address::KeyguideItem_Param_ctor, this);
        }
    };

    struct Fields : UnityEngine::MonoBehaviour::Fields {
        UnityEngine::UI::Image::Object* _button;
        Dpr::UI::UIText::Object* _text;
    };
};

namespace System::Collections::Generic {
    struct List$$KeyguideItem_Param : List<List$$KeyguideItem_Param, KeyguideItem::Param> {
        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_List_KeyguideItem_Param_Add> Method$$Add{};
    };
}