#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/UI/UIWindow.h"
#include "externals/KeyguideItem.h"
#include "externals/UnityEngine/Coroutine.h"
#include "externals/UnityEngine/RectTransform.h"

namespace Dpr::UI {
    struct Keyguide : ILClass<Keyguide> {
        struct Param : ILClass<Param, game::Address::Dpr_UI_Keyguide_Param_TypeInfo> {
            struct Fields {
                System::Collections::Generic::List$$KeyguideItem_Param* itemParams;
            };

            inline void ctor() {
                external<void>(game::Address::Dpr_UI_Keyguide_Param_ctor, this);
            }
        };

        struct Fields : Dpr::UI::UIWindow::Fields {
            UnityEngine::RectTransform::Object* _guideRoot;
            KeyguideItem::Object* _itemPrefab;
            UnityEngine::Coroutine::Object* _coClose;
            bool _isOpen;
        };

        inline void Open(Param::Object* param) {
            external<void>(game::Address::Dpr_UI_Keyguide_Open, this, param);
        }
    };
}