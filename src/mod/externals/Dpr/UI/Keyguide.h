#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/UI/UIWindow.h"
#include "externals/KeyguideItem.h"
#include "externals/UnityEngine/Coroutine.h"
#include "externals/UnityEngine/RectTransform.h"

namespace Dpr::UI {
    struct Keyguide : ILClass<Keyguide> {
        struct Param : ILClass<Param, 0x04c59fd0> {
            struct Fields {
                System::Collections::Generic::List$$KeyguideItem_Param* itemParams;
            };

            inline void ctor() {
                external<void>(0x01c4ef80, this);
            }
        };

        struct Fields : Dpr::UI::UIWindow::Fields {
            UnityEngine::RectTransform::Object* _guideRoot;
            KeyguideItem::Object* _itemPrefab;
            UnityEngine::Coroutine::Object* _coClose;
            bool _isOpen;
        };

        inline void Open(Param::Object* param) {
            external<void>(0x01c4f000, this, param);
        }
    };
}