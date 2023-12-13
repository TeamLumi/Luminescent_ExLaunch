#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/View/UI/BUIButtonBase.h"
#include "externals/UnityEngine/GameObject.h"
#include "externals/UnityEngine/UI/Image.h"

namespace Dpr::Battle::View::UI {
    struct BUIActionSelectButton : ILClass<BUIActionSelectButton> {
        struct Fields : Dpr::Battle::View::UI::BUIButtonBase::Fields {
            void* _buttonMsgLabels; // System_Collections_Generic_Dictionary_BUIActionList_ActionButtonType__string__o*
            UnityEngine::UI::Image::Object* _lightFrame;
            UnityEngine::UI::Image::Object* _iconImage;
            int32_t _buttonType;
        };
    };
}

namespace System::Collections::Generic {
    struct List$$BUIActionSelectButton : List<List$$BUIActionSelectButton, Dpr::Battle::View::UI::BUIActionSelectButton> {
    };
}