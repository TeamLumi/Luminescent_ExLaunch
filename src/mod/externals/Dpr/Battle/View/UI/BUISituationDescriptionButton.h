#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Dpr/Battle/View/UI/BUIButtonBase.h"
#include "externals/TMPro/TextMeshProUGUI.h"

namespace Dpr::Battle::View::UI {
    struct BUISituationDescriptionButton : ILClass<BUISituationDescriptionButton> {
        struct Fields : Dpr::Battle::View::UI::BUIButtonBase::Fields {
            TMPro::TextMeshProUGUI::Object* _contentsText;
        };
    };
}

namespace System::Collections::Generic {
    struct List$$BUISituationDescriptionButton : List<List$$BUISituationDescriptionButton, Dpr::Battle::View::UI::BUISituationDescriptionButton> {
        static inline StaticILMethod<0x04c890f0> Method$$RemoveRange {};
    };
}