#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/View/BTLV_WAZA_INFO.h"
#include "externals/Dpr/Battle/View/UI/BUIButtonBase.h"
#include "externals/Dpr/UI/UIText.h"
#include "externals/System/Nullable.h"
#include "externals/UnityEngine/GameObject.h"
#include "externals/UnityEngine/UI/Image.h"

namespace Dpr::Battle::View::UI {
    struct BUIWazaButton : ILClass<BUIWazaButton> {
        struct Fields : BUIButtonBase::Fields {
            Dpr::UI::UIText::Object* _ppText;
            Dpr::UI::UIText::Object* _maxPpText;
            UnityEngine::GameObject::Object* _effectiveObj;
            UnityEngine::UI::Image::Object* _effectiveBG;
            UnityEngine::UI::Image::Object* _effectiveImage;
            Dpr::UI::UIText::Object* _effectiveText;
            System::Nullable<Dpr::Battle::View::BTLV_WAZA_INFO>::Object _Info_k__BackingField;
        };

        static_assert(offsetof(Fields, _Info_k__BackingField) == 0x78);

        static inline StaticILMethod<0x04c6b760> Method$$Submit {};

        inline bool Submit() {
            return external<bool>(0x021d47f0, this, Method$$Submit);
        }
    };
}
