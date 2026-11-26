#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/UI/PoketchAppBase.h"
#include "externals/Dpr/UI/PoketchButton.h"
#include "externals/Dpr/UI/UIInputButton.h"
#include "externals/Dpr/UI/UIWindow.h"
#include "externals/UnityEngine/Color32.h"
#include "externals/UnityEngine/Events/UnityAction.h"
#include "externals/UnityEngine/RectTransform.h"
#include "externals/UnityEngine/UI/Image.h"
#include "externals/UnityEngine/Vector3.h"

namespace Dpr::UI {
    struct PoketchWindow : ILClass<PoketchWindow> {
        struct _OpOpen_d__94 : ILClass<_OpOpen_d__94> {
            struct Fields {
                int32_t __1__state;
                Il2CppObject* __2__current;
                Dpr::UI::PoketchWindow::Object* __4__this;
                int32_t prevWindowId;
            };
        };

        enum class TouchState: int32_t {
            None = 0,
            Touch = 1,
            Hold = 2,
            Release = 3,
            Push = 4,
        };

        struct Fields : public Dpr::UI::UIWindow::Fields {
            void* _canvasGroup;
            void* _imageBg;
            UnityEngine::RectTransform::Object* _root;
            UnityEngine::RectTransform::Object* _rootTopShutter;
            UnityEngine::RectTransform::Object* _rootBottomShutter;
            Dpr::UI::PoketchButton::Object* _changeButton;
            UnityEngine::UI::Image::Object* _cursor;
            void* _appBG;
            UnityEngine::UI::Image::Array* _bodyImages;
            void* _bodySprites;
            UnityEngine::UI::Image::Array* _numImages;
            void* _numSprites;
            System::Collections::Generic::List$$PoketchAppBase::Object* _poketchAppList;
            float _resizeDuration;
            float _changeDuration;
            float _closeWait;
            float _changeWait;
            float _smallScale;
            float _largeScale;
            UnityEngine::Vector3::Object _smallPos;
            UnityEngine::Vector3::Object _largePos;
            float _toSmallDelay;
            float _toLargeDelay;
            UnityEngine::Color32::Object _lightColor;
            UnityEngine::Color32::Array* _bgColors;
            float _cursolMoveValue;
            float _voiceWait;
            Dpr::UI::UIInputButton::Object* _buttonR;
            Dpr::UI::UIInputButton::Object* _buttonSR;
            bool _isSizeChanging;
            bool _isBackLight;
            bool _isTouch;
            bool _isSelecting;
            Dpr::UI::PoketchButton::Object* _preButton;
            void* _appChangeCoroutine;
            float _appCloseTime;
            void* _twSequence;
            void* _openCoroutine;
            void* _closeCoroutine;
            int32_t _voiceMonsNo;
            void* _voiceInstance;
            void* _voiceData;
            float _voiceTimer;
            float _cursorMinX;
            float _cursorMaxX;
            float _cursorMinY;
            float _cursorMaxY;
            int32_t _CurrentAppIndex_k__BackingField;
            bool _IsLarge_k__BackingField;
            bool _IsPauseContinue_k__BackingField;
            bool _IsCloseOnce_k__BackingField;
            int32_t AppWidth;
            int32_t AppHeight;
            float _cursorX;
            float _cursorY;
            TouchState _touchState;
        };

        static_assert(sizeof(Fields) == 424);

        static inline StaticILMethod<game::Address::Method_Dpr_UI_PoketchWindow_OnCreate_b_90_0> Method$$OnCreate_b_90 {};

        static inline MethodInfo* Method$$GoBackAction = nullptr;
        static MethodInfo* getMethod$$GoBackAction(Il2CppMethodPointer method) {
            if (Method$$GoBackAction == nullptr)
                Method$$GoBackAction = (*Method$$OnCreate_b_90)->copyWith(method);
            return Method$$GoBackAction;
        };

        inline void SelectApp(bool forward) {
            external<void>(game::Address::Dpr_UI_PoketchWindow_SelectApp, this, forward);
        }

        inline void OnInputPrev(int32_t button, int32_t state) {
            external<void>(game::Address::Dpr_UI_PoketchWindow_OnInputPrev, this, button, state);
        }

        inline bool IsInRange(Dpr::UI::PoketchButton* btn, float x, float y) {
            return external<bool>(game::Address::Dpr_UI_PoketchWindow_IsInRange, this, btn, x, y);
        }

        inline void Close(UnityEngine::Events::UnityAction::Object* onClosed_) {
            external<void>(game::Address::Dpr_UI_PoketchWindow_Close, this, onClosed_);
        }

        inline void ChangePoketchSize(bool notReleaseUncontrol, UnityEngine::Events::UnityAction::Object* callback) {
            external<void>(game::Address::Dpr_UI_PoketchWindow_ChangePoketchSize, this, notReleaseUncontrol, callback);
        }

        inline void SetCursorPosition(float posX, float posY) {
            external<void>(game::Address::Dpr_UI_PoketchWindow_SetCursorPosition, this, posX, posY);
        }
    };
}
