#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/View/UI/BattleViewUICanvasBase.h"
#include "externals/Dpr/Battle/View/UI/BUIActionSelectButton.h"
#include "externals/Dpr/Battle/View/UI/BUIButtonBase.h"
#include "externals/System/Action.h"
#include "externals/TMPro/TextMeshProUGUI.h"
#include "externals/UnityEngine/UI/Image.h"

namespace Dpr::Battle::View::UI {
    struct BUIPokeBallList : ILClass<BUIPokeBallList> {
        struct Fields : Dpr::Battle::View::UI::BattleViewUICanvasBase::Fields {
            void* _prevButton;
            void* _nextButton;
            UnityEngine::UI::Image::Object* _ballIcon;
            UnityEngine::UI::Image::Object* _originalIcon;
            TMPro::TextMeshProUGUI::Object* _ballNameText;
            TMPro::TextMeshProUGUI::Object* _ballCountText;
            TMPro::TextMeshProUGUI::Object* _descriptionText;
            void* _useButton;
            void* _balls;
            void* _ballSprites;
            System::Action::Object* _callBack;
            int32_t firstIndex;
            bool isAction;
            void* _sprites;
            uint16_t _Result_k__BackingField;
        };

        static_assert(offsetof(Fields, _Result_k__BackingField) == 0xC0);

        inline void SetInvalid() {
            external<void>(0x01d229d0, this);
        }
    };
}

