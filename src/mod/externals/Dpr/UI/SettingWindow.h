#pragma once

#include "externals/il2cpp.h"
#include "externals/DPData/CONFIG.h"
#include "externals/Dpr/UI/UIWindow.h"
#include "externals/Dpr/UI/SettingMenuItem.h"
#include "externals/System/Collections/Generic/List.h"
#include "externals/UnityEngine/UI/ScrollRect.h"

namespace Dpr::UI {
    struct SettingWindow : ILClass<SettingWindow> {
        struct _OpOpen_d__11 : ILClass<_OpOpen_d__11> {
            struct Fields {
                int32_t __1__state;
                Il2CppObject* __2__current;
                Dpr::UI::SettingWindow::Object* __4__this;
                int32_t prevWindowId;
            };
        };

        struct Fields : Dpr::UI::UIWindow::Fields {
            void* _cursor;
            UnityEngine::UI::ScrollRect::Object* _scrollRect;
            int32_t _viewMenuItemNum;
            UnityEngine::RectTransform::Object* _messageWindowRoot;
            int32_t _selectIndex;
            System::Collections::Generic::List$$SettingMenuItem::Object* _activeItems;
            int32_t _scrollIndex;
            DPData::CONFIG::Object _tempConfig;
            void* _voiceInstance;
        };
    };
}
