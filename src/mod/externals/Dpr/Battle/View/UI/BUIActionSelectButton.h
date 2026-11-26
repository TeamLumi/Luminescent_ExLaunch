#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/View/UI/BUIButtonBase.h"
#include "externals/UnityEngine/UI/Image.h"

namespace Dpr::Battle::View::UI {
    struct BUIActionSelectButton : ILClass<BUIActionSelectButton> {
        struct Fields : BUIButtonBase::Fields {
            void* _buttonMsgLabels;
            UnityEngine::UI::Image::Object* _lightFrame;
            UnityEngine::UI::Image::Object* _iconImage;
            int32_t _buttonType;
        };

        static inline StaticILMethod<game::Address::Method_Dpr_Battle_View_UI_BUIButtonBase_BUIActionSelectButton_Submit> Method$$Submit {};

        inline bool Submit() {
            return external<bool>(game::Address::Dpr_Battle_View_UI_BUIButtonBase_BUIActionSelectButton_Submit, this, Method$$Submit);
        }
    };


}

namespace System::Collections::Generic {
    struct List$$BUIActionSelectButton : List<List$$BUIActionSelectButton, Dpr::Battle::View::UI::BUIActionSelectButton> {

    };
}
