#pragma once

#include <vector>
#include "ui/base/element.h"
#include "window.h"
#include "helpers.h"
#include "imgui_backend/imgui_impl_nvn.hpp"

namespace ui {
    ELEMENT(Root) {
        bool displayed = false;

        bool beginDraw() override {
            InputHelper::updatePadState();
            auto& io = ImGui::GetIO();
            io.MouseDrawCursor = InputHelper::isInputToggled();

            if (displayed || InputHelper::isInputToggled()) {
                displayed = true;
                ImguiNvnBackend::newFrame();
                ImGui::NewFrame();
                return true;
            }
            return false;
        }

        void endDraw() override {
            ImGui::Render();
            ImguiNvnBackend::renderDrawData(ImGui::GetDrawData());
        }

        ELEMENT_SUPPORTS_CHILD(Window);
    };
}
