#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/View/UI/BattleViewUICanvasBase.h"
#include "externals/Dpr/UI/TypePanel.h"
#include "externals/Dpr/UI/UIText.h"
#include "externals/Dpr/UI/WazaCategoryPanel.h"

namespace Dpr::Battle::View::UI {
    struct BUIWazaDescription : ILClass<BUIWazaDescription> {
        struct Fields : BattleViewUICanvasBase::Fields {
            UnityEngine::UI::Image::Object* _bgImage;
            Dpr::UI::TypePanel::Object* _typePanel;
            TMPro::TextMeshProUGUI::Object* _wazaNameText;
            TMPro::TextMeshProUGUI::Object* _ppCountText;
            TMPro::TextMeshProUGUI::Object* _ppMaxText;
            Dpr::UI::WazaCategoryPanel::Object* _wazaCategory;
            Dpr::UI::UIText::Object* _powerText;
            Dpr::UI::UIText::Object* _dexText;
            TMPro::TextMeshProUGUI::Object* _descriptionText;
            UnityEngine::RectTransform::Object* _connectorLeft;
            UnityEngine::RectTransform::Object* _connectorVirtival;
            UnityEngine::RectTransform::Object* _connectorRight;
            UnityEngine::UI::Image::Array* _connectorImages;
        };

        static_assert(offsetof(Fields, _connectorImages) == 0xB8);
    };
}
