#pragma once

#include "memory/string.h"
#include "ui/base/element.h"
#include "selectable.h"

namespace ui {
    ELEMENT(Combo) {
        nn::string label;
        nn::string placeholder;
        ImGuiComboFlags flags = ImGuiComboFlags_None;

        bool isValid()
        override{
                return !label.empty() && !placeholder.empty();
        }

        bool beginDraw()
        override{
                return ImGui::BeginCombo(label.c_str(), placeholder.c_str(), flags);
        }

        void endDraw()
        override{
                ImGui::EndCombo();
        }

        ELEMENT_SUPPORTS_CHILD(Selectable);
    };
}
