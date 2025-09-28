#pragma once

#include "externals/il2cpp.h"

#include "externals/DPData/CONFIG.h"
#include "externals/Dpr/UI/Cursor.h"
#include "externals/Dpr/UI/UIWindow.h"
#include "externals/Dpr/UI/SettingMenuItem.h"
#include "externals/System/Collections/Generic/List.h"
#include "externals/System/Collections/IEnumerator.h"
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
            Cursor::Object* _cursor;
            UnityEngine::UI::ScrollRect::Object* _scrollRect;
            int32_t _viewMenuItemNum;
            UnityEngine::RectTransform::Object* _messageWindowRoot;
            int32_t _selectIndex;
            System::Collections::Generic::List$$SettingMenuItem::Object* _activeItems;
            int32_t _scrollIndex;
            DPData::CONFIG::Object _tempConfig;
            void* _voiceInstance;
        };

        struct VirtualInvokeData_OpPlayOpenWindowAnimation {
            typedef System::Collections::IEnumerator::Object*(*Il2CppMethodPointer)(SettingWindow::Object*, int32_t, UnityEngine::Events::UnityAction::Object*, const MethodInfo*);
            Il2CppMethodPointer methodPtr;
            const MethodInfo* method;
        };

        struct VirtualInvokeData_OpenMessageWindow {
            typedef System::Collections::IEnumerator::Object*(*Il2CppMethodPointer)(SettingWindow::Object*, Dpr::MsgWindow::MsgWindowParam::Object*, const MethodInfo*);
            Il2CppMethodPointer methodPtr;
            const MethodInfo* method;
        };

        struct VTable {
            VirtualInvokeData _0_Equals;
            VirtualInvokeData _1_Finalize;
            VirtualInvokeData _2_GetHashCode;
            VirtualInvokeData _3_ToString;
            VirtualInvokeData _4_OnCreate;
            VirtualInvokeData _5_OnDestroy;
            VirtualInvokeData _6_OnTransition;
            VirtualInvokeData _7_OnTransitionComplete;
            VirtualInvokeData _8_OnOpen;
            VirtualInvokeData _9_PlayOpenWindowAnimation;
            VirtualInvokeData_OpPlayOpenWindowAnimation _10_OpPlayOpenWindowAnimation;
            VirtualInvokeData _11_GetWindowAnimationConnectId;
            VirtualInvokeData _12_OpPlayCloseWindowAnimationAndWaiting;
            VirtualInvokeData _13_EnableMainCameraByUiMode;
            VirtualInvokeData _14_OnCloseKeyguide;
            VirtualInvokeData_OpenMessageWindow _15_OpenMessageWindow;
            VirtualInvokeData _16_CloseMessageWindow;
            VirtualInvokeData _17_IsActiveMessageWindow;
            VirtualInvokeData _18_CreateContextMenuYesNo;
            VirtualInvokeData _19_CreateContextMenuYesNo;
            VirtualInvokeData _20_OnAddContextMenuYesNoItemParams;
        };

        static inline StaticILMethod<0x04c7fc20, SettingMenuItem> Method$$OnMenuItemValueChaged {};
        static inline StaticILMethod<0x04c7fc28, SettingMenuItem> Method$$OnUpdate {};

        inline System::Collections::IEnumerator::Object* virtual_OpPlayOpenWindowAnimation(int32_t prevWindowId, UnityEngine::Events::UnityAction::Object* onOpend) {
            return (*(this->instance()->klass->vtable)._10_OpPlayOpenWindowAnimation.methodPtr)
                    (this->instance(), prevWindowId, onOpend, this->instance()->klass->vtable._10_OpPlayOpenWindowAnimation.method);
        }

        inline void virtual_OpenMessageWindow(Dpr::MsgWindow::MsgWindowParam::Object* messageParam) {
            (*(this->instance()->klass->vtable)._15_OpenMessageWindow.methodPtr)
                    (this->instance(), messageParam, this->instance()->klass->vtable._15_OpenMessageWindow.method);
        }

        inline bool SetSelectIndex(int32_t selectIndex, bool isInitialize) {
            return external<bool>(0x01d41700, this, selectIndex, isInitialize);
        }

        inline bool SetSelectValue(int32_t selectValue) {
            return external<bool>(0x01d41a70, this, selectValue);
        }

        inline void OpenDescriptionMessageWindow() {
            external<void>(0x01d41c30, this);
        }
    };
}
