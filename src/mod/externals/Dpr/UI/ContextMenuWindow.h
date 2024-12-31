#pragma once
#include "externals/il2cpp-api.h"
#include "externals/Dpr/UI/Cursor.h"
#include "externals/Dpr/UI/ContextMenuItem.h"
#include "externals/Dpr/UI/UIWindow.h"

namespace Dpr::UI {
    struct ContextMenuWindow : ILClass<ContextMenuWindow> {
        struct Param : ILClass<Param, 0x04c5e778> {
            struct Fields {
                int32_t windowType;
                Dpr::UI::ContextMenuItem::Param::Array* itemParams;
                UnityEngine::Vector2::Object pivot;
                UnityEngine::Vector3::Object position;
                int32_t selectIndex;
                float minItemWidth;
                int32_t cancelIndex;
                bool useCancel;
                bool useLoopAndRepeat;
                bool isInputEnable;
                void* onSetupMessageArgs;
                uint32_t seDecide;
                uint32_t seCancel;
            };

            inline void ctor() {
                external<void>(0x01ec23d0, this);
            }
        };

        struct Fields : UIWindow::Fields {
            UnityEngine::RectTransform::Object* _windowRoot;
            UnityEngine::RectTransform::Object* _contentRoot;
            ContextMenuItem::Object* _itemPrefab;
            Cursor::Object* _cursor;
            UnityEngine::UI::Image::Object* _imageBg;
            void* _windowParams;
            int32_t animStateIn;
            int32_t animStateOut;
            void* _items;
            int32_t _selectIndex;
            void* _verticalLayoutGroup;
            void* onClicked;
            UnityEngine::Events::UnityAction::Object* onSelectChanged;
            Param::Object* _param;
            UnityEngine::Vector3::Object _position;
        };

        inline void Open(ContextMenuWindow::Param::Object* param) {
            external<void>(0x01ec0bc0, this, param);
        }
    };
}