#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BattleViewBase.h"
#include "externals/Dpr/Battle/View/UI/BUIActionList.h"
#include "externals/Dpr/Battle/View/UI/BUIPokeBallList.h"
#include "externals/Dpr/Battle/View/UI/BUIWazaList.h"
#include "externals/Dpr/UI/Cursor.h"
#include "externals/UnityEngine/Coroutine.h"
#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/UnityEngine/Sprite.h"

namespace Dpr::Battle::View::Systems {
    struct BattleViewUISystem : ILClass<BattleViewUISystem> {
        struct SwitchActionListCoroutine_d__198 : ILClass<SwitchActionListCoroutine_d__198> {
            struct Fields {
                int32_t __1__state;
                Il2CppObject* __2__current;
                BattleViewUISystem::Object* __4__this;
                bool isShow;
                bool withStatusWindow;
                bool withDecoraton;
            };
        };

        struct Fields : UnityEngine::MonoBehaviour::Fields {
            void* _decoImage; // UnityEngine_UI_RawImage_o*
            void* _statusWindows; // Dpr_Battle_View_UI_BUIStatusWindow_array*
            UnityEngine::Sprite::Array* _ballSprites;
            UnityEngine::Sprite::Array* _ballLargeSprites;
            Dpr::Battle::View::UI::BUIActionList::Object* _actionList;
            Dpr::Battle::View::UI::BUIWazaList::Object* _wazaList;
            void* _wazaDescription;
            void* _tokuseiNear;
            void* _tokuseiFar;
            void* _ballPlateNear;
            void* _ballPlateFar;
            Dpr::Battle::View::UI::BUIPokeBallList::Object* _pokeBallList;
            void* _safariBall;
            void* _targetSelect;
            void* _commTime;
            Dpr::UI::Cursor::Object* _cursor;
            float _cursorWait;
            float _executeWait;
            int32_t _tutorialBallCount;
            int32_t _tutorialDrugCount;
            int32_t _tutorialMoney;
            bool _isMenuUIEnd;
            bool _isFirstTime;
            bool _isPlaySound;
            bool _isShowDeco;
            bool _isMsgEnd;
            bool _isMessageSleep;
            bool _isMessageOpen;
            float _msgTimer;
            void* _showParam;
            void* _messagePramQueue;
            UnityEngine::Coroutine::Object* _coMsgQueue;
            System::Action::Object* _endMsgAction;
            void* _msgWindow;
            void* _yesNoResult;
            void* _uiWindow;
            void* _pokeWinParam;
            UnityEngine::Coroutine::Object* _coCloseMenuUI;
            void* _pokeSelParam;
            void* _pokeSelResult;
            Dpr::Battle::Logic::BattleViewBase::SelectActionParam::Object* _actionParam;
            Dpr::Battle::Logic::BTL_POKEPARAM::Object* _swapWaitPokemon;
            int32_t preHUDEventId;
            uint32_t _outMemberIndex;
            uint16_t _selectItem;
            int32_t _selectWaza;
            int32_t _selectPoke;
            void* _autoPilot;
            Dpr::Battle::Logic::BTL_POKEPARAM::Object* _currentPoke;
            void* _decoballSequence;
            int32_t SeqEndWaitSafetyCount;
            int32_t seqDotDispWaitTime;
            void* _uiCanvasGroup;
        };

        static_assert(offsetof(Fields, _uiCanvasGroup) == 0x148);

        inline void PlaySe(uint32_t eventId) {
            external<void>(0x01e7c5f0, this, eventId);
        }

        inline bool IsOpenMessage() {
            return external<bool>(0x01e77c40, this);
        }

        inline bool IsStatusWindowTarasit() {
            return external<bool>(0x01e7b350, this);
        }

        inline void SetCurrentPokemonStatus() {
            external<void>(0x01e7a6f0, this);
        }

        inline void Apply(int32_t interpFrame) {
            external<void>(0x01e7a540, this, interpFrame);
        }

        inline void SwitchShowStatusWindowsCore(bool isShow, bool isForceBallHide) {
            external<void>(0x01e79f50, this, isShow, isForceBallHide);
        }

        inline void SwitchShowDecoImage(bool isShow) {
            external<void>(0x01e765e0, this, isShow);
        }

        inline void SwitchShowActionList(bool isShow, bool withStatusWindow, bool withDecoraton) {
            external<void>(0x01e763f0, this, isShow, withStatusWindow, withDecoraton);
        }

        inline bool IsOpenedStatus() {
            return external<bool>(0x01e76340, this);
        }
    };
}
