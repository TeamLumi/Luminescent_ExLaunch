#pragma once
#include "externals/il2cpp-api.h"
#include "externals/Dpr/Battle/View/UI/BUIButtonBase.h"
#include "externals/UnityEngine/UI/Image.h"
#include "BUIButtonBase.h"

namespace Dpr::Battle::View::UI {
    struct BUIActionSelectButton : ILClass<BUIActionSelectButton> {
        struct Fields : BUIButtonBase<BUIActionSelectButton> {
            void* _buttonMsgLabels;
            UnityEngine::UI::Image::Object* _lightFrame;
            UnityEngine::UI::Image::Object* _iconImage;
            int32_t _buttonType;
        };

        static inline StaticILMethod<0x04c6b748> Method$$Submit {};


        inline bool Submit() {
            return external<bool>(0x021d47f0, this, Method$$Submit);
        }
    };


}

namespace System::Collections::Generic {
    struct List$$BUIActionSelectButton : List<List$$BUIActionSelectButton, Dpr::Battle::View::UI::BUIActionSelectButton> {

    };
}
