#pragma once

#include "externals/il2cpp-api.h"

#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"
#include "externals/System/String.h"
#include "externals/UnityEngine/GameObject.h"
#include "externals/UnityEngine/RectTransform.h"
#include "externals/UnityEngine/UI/Image.h"
#include "externals/UnityEngine/UI/LayoutGroup.h"

namespace SmartPoint::Components {
    struct DebugMenu : ILClass<DebugMenu> {
        struct Fields : SmartPoint::AssetAssistant::SingletonMonoBehaviour::Fields {
            UnityEngine::GameObject::Object* _template;
            UnityEngine::RectTransform::Object* informationWindow;
            void* topCanvas;
            void* topCanvasGroup;
            void* informationText;
            void* _caption;
            UnityEngine::UI::Image::Object* _image;
            bool _isOpaque;
        };

        struct MenuInstance : ILClass<MenuInstance> {
            struct Fields {
                void* _ownerCell_k__BackingField;
                System::String::Object* _caption;
                void* _scrollView;
                UnityEngine::GameObject::Object* _gameObject;
                int32_t _currentIndex;
                MenuInstance::Object* _parent;
                void* _canvas;
                void* _canvasGroup;
                UnityEngine::UI::LayoutGroup::Object* _layoutGroup;
                void* _cells;
            };
        };

        static inline SmartPoint::Components::DebugMenu::MenuInstance::Object* Create(System::String::Object* caption, SmartPoint::Components::DebugMenu::MenuInstance::Object* parent) {
            return external<SmartPoint::Components::DebugMenu::MenuInstance::Object*>(0x02092420, caption, parent);
        }
    };
}
