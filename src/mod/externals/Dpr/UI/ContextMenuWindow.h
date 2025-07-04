#pragma once

#include "externals/il2cpp-api.h"

#include "externals/ContextMenuID.h"
#include "externals/Dpr/UI/ContextMenuItem.h"
#include "externals/Dpr/UI/Cursor.h"
#include "externals/Dpr/UI/UIWindow.h"
#include "externals/System/Func.h"
#include "externals/UnityEngine/RectTransform.h"
#include "externals/UnityEngine/UI/Image.h"
#include "externals/UnityEngine/Vector2.h"
#include "externals/UnityEngine/Vector3.h"
#include "externals/UnityEngine/UI/VerticalLayoutGroup.h"

namespace Dpr::UI {
    struct ContextMenuWindow : ILClass<ContextMenuWindow, 0x04c5dae0> {
        struct Param : ILClass<Param, 0x04c5e778> {
            struct Fields {
                int32_t windowType;
                ContextMenuItem::Param::Array* itemParams;
                UnityEngine::Vector2::Object pivot;
                UnityEngine::Vector3::Object position;
                int32_t selectIndex;
                float minItemWidth;
                int32_t cancelIndex;
                bool useCancel;
                bool useLoopAndRepeat;
                bool isInputEnable;
                UnityEngine::Events::UnityAction::Object* onSetupMessageArgs;
                uint32_t seDecide;
                uint32_t seCancel;
            };

            inline void ctor() {
                external<void>(0x01ec23d0, this);
            }
        };

        struct Fields : Dpr::UI::UIWindow::Fields {
            UnityEngine::RectTransform::Object* _windowRoot;
            UnityEngine::RectTransform::Object* _contentRoot;
            ContextMenuItem::Object* _itemPrefab;
            Cursor::Object* _cursor;
            UnityEngine::UI::Image::Object* _imageBg;
            void* _windowParams; // Dpr_UI_ContextMenuWindow_MenuWindowParam_array*
            int32_t _animStateIn;
            int32_t _animStateOut;
            System::Collections::Generic::List$$ContextMenuItem::Object* _items;
            int32_t _selectIndex;
            UnityEngine::UI::VerticalLayoutGroup::Object* _verticalLayoutGroup;
            System::Func::Object* onClicked;
            UnityEngine::Events::UnityAction::Object* onSelectChanged;
            ContextMenuWindow::Param::Object* _param;
            UnityEngine::Vector3::Object _position;
        };

        static inline StaticILMethod<0x04c8ff48, Dpr::UI::ContextMenuWindow> Method$$CreateUIWindow {};

        inline void ctor() {
            external<void>(0x01ec1770, this);
        }

        inline void Open(Param::Object* param) {
            external<void>(0x01ec0bc0, this, param);
        }
    };
}