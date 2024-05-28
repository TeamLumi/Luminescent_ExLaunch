#pragma once
#include "externals/il2cpp-api.h"
#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/UnityEngine/RectTransform.h"
#include "externals/UnityEngine/UI/Image.h"
#include "externals/UnityEngine/Events/UnityAction.h"
#include "externals/Dpr/UI/UIText.h"
#include "externals/System/Collections/Generic/List.h"
#include "externals/System/MulticastDelegate.h"

struct XMenuTopItem : ILClass<XMenuTopItem> {
    struct Fields : UnityEngine::MonoBehaviour::Fields {
        UnityEngine::RectTransform::Object* _root;
        UnityEngine::UI::Image::Object* _icon;
        UnityEngine::UI::Image::Object* _newIcon;
        UnityEngine::UI::Image::Object* _imageShadow;
        void* _name;
        void* _spriteIcons;
        void* _animator;
        int32_t _itemType;
        void* _duplicate;
    };

};

namespace System::Collections::Generic {
    struct List$$XMenuTopItem : List<List$$XMenuTopItem, XMenuTopItem> {

        static inline StaticILMethod<0x04c8b3a8> Method$$Add {};
        static inline StaticILMethod<0x04c8b3b0> Method$$Clear {};
    };
}

namespace UnityEngine::Events {
    struct UnityAction$$XMenuTopItem : ILClass<UnityAction$$XMenuTopItem> {
        struct Fields : ::System::MulticastDelegate::Fields {

        };
    };
}
