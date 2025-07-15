#pragma once

#include "memory/string.h"
#include "ui/base/element.h"
#include "ui/base/combo.h"
#include "ui/base/selectable.h"
#include "nn/fs/fs_directories.hpp"
#include "nn/result.h"
#include "logger/logger.h"
#include "externals/UnityEngine/Transform.h"
#include "externals/UnityEngine/_Object.h"
#include "externals/UnityEngine/UI/ListPool.h"
#include "externals/UnityEngine/SceneManagement/SceneManager.h"
#include "externals/Dpr/UI/UIManager.h"
#include "externals/Dpr/Battle/View/BattleViewCore.h"

namespace ui {
    ELEMENT(ElementInspector) {
        // TODO: Consider an nn::vector for roots?
        UnityEngine::Transform* root;
        bool showChildren = true;
        bool showComponents = false;

        void draw() override {
            if (root != nullptr) {
                drawElement(root);
            } else {
                auto scene = UnityEngine::SceneManagement::SceneManager::GetActiveScene();
                auto name = scene.get_name()->asCString();
                if (ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
                    auto objs = scene.GetRootGameObjects();
                    for (auto& o : *objs) {
                        drawElement(o.get_transform());
                    }
                    ImGui::TreePop();
                }

                auto manager = Dpr::UI::UIManager::get_Instance();
                drawElement(manager->fields._activeRoot);
            }
        }

    private:
        void drawElement(UnityEngine::Transform* element, const nn::string& prefix = "") {
            auto name = element->cast<UnityEngine::_Object>()->GetName();
            auto str = prefix + name->asCString();
            auto childCount = element->get_childCount();

            // Checks if Object is a character (child of "Character Objects")
            bool showCoordinates = element->GetParent() != nullptr &&
                    element->GetParent()->cast<UnityEngine::_Object>() != nullptr &&
                    element->GetParent()->cast<UnityEngine::_Object>()->GetName() != nullptr &&
                    strcmp(element->GetParent()->cast<UnityEngine::_Object>()->GetName()->asCString().c_str(), "Character Objects") == 0;

            if (ImGui::TreeNodeEx(str.c_str(), ImGuiTreeNodeFlags_DefaultOpen * (childCount < 3))) {  // Default open if the element has less than 3 children
                auto list = UnityEngine::UI::ListPool::Get(UnityEngine::UI::ListPool::Method$$Component$$Get);
                System::RuntimeTypeHandle::Object handle{};
                handle.fields.value = &UnityEngine::Component::getClass()->_1.byval_arg;
                element->cast<UnityEngine::Component>()->GetComponents(System::Type::GetTypeFromHandle(handle), list);
                auto componentCount = list->fields._size;

                if (showChildren) {
                    if (showCoordinates) {
                        auto position = element->get_localPosition();
                        auto positionStr = "X: " + nn::to_string((int32_t)position.fields.x) + " Z: " + nn::to_string((int32_t)position.fields.z);
                        ImGui::TreeNodeEx(positionStr.c_str(), 0);
                    }

                    for (int i = 0; i < childCount; i++) {
                        auto child = element->GetChild(i);
                        drawElement(child, nn::to_string(i) + ": ");
                    }
                }

                if (showChildren && showComponents && childCount > 0 && componentCount > 0) {
                    ImGui::Separator();
                }

                if (showComponents) {
                    for (int i = 0; i < list->fields._size; i++) {
                        auto component = list->fields._items->m_Items[i];
                        ImGui::Text("%s", component->klass->_1.name);
                    }
                }

                ImGui::TreePop();
                UnityEngine::UI::ListPool::Release<System::Collections::Generic::List$$Component>(list, UnityEngine::UI::ListPool::Method$$Component$$Release);
            }
        }
    };
}
