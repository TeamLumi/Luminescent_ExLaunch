#pragma once

#include "externals/il2cpp-api.h"
#include "externals/DG/Tweening/Sequence.h"
#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "externals/Dpr/Battle/View/Systems/MsgShowParam.h"
#include "externals/Dpr/Battle/View/UI/BUIActionList.h"
#include "externals/Dpr/UI/Cursor.h"
#include "externals/Dpr/UI/UIWindow.h"
#include "externals/UnityEngine/CanvasGroup.h"
#include "externals/UnityEngine/MonoBehaviour.h"

namespace Dpr::Battle::View::Systems {
    struct BattleViewUISystem : ILClass<BattleViewUISystem> {
        struct Fields : UnityEngine::MonoBehaviour::Fields {
            void* _decoImage; // UnityEngine_UI_RawImage_o*
            void* _statusWindows; // Dpr_Battle_View_UI_BUIStatusWindow_array*
            void* _ballSprites; // UnityEngine_Sprite_array*
            void* _ballLargeSprites; // UnityEngine_Sprite_array*
            Dpr::Battle::View::UI::BUIActionList::Object* _actionList;
            void* _wazaList; // Dpr_Battle_View_UI_BUIWazaList_o*
            void* _wazaDescription; // Dpr_Battle_View_UI_BUIWazaDescription_o*
            void* _tokuseiNear; // Dpr_Battle_View_UI_BUITokuseiPlate_o*
            void* _tokuseiFar; // Dpr_Battle_View_UI_BUITokuseiPlate_o*
            void* _ballPlateNear; // Dpr_Battle_View_UI_BUIBallPlate_o*
            void* _ballPlateFar; // Dpr_Battle_View_UI_BUIBallPlate_o*
            void* _pokeBallList; // Dpr_Battle_View_UI_BUIPokeBallList_o*
            void* _safariBall; // Dpr_Battle_View_UI_BUISafariBall_o*
            void* _targetSelect; // Dpr_Battle_View_UI_BUITargetSelect_o*
            void* _commTime; // Dpr_Battle_View_UI_BUICommTime_o*
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
            MsgShowParam::Object _showParam;
            void* _messagePramQueue; // System_Collections_Generic_Queue_ValueTuple_MessageTextParseDataModel__MsgShowParam___o*
            UnityEngine::Coroutine::Object* _coMsgQueue;
            System::Action::Object* _endMsgAction;
            void* _msgWindow; // Dpr_MsgWindow_MsgWindow_o*
            System::NullablePrimitive<int32_t>::Object _yesnoResult;
            Dpr::UI::UIWindow::Object* _uiWindow;
            void* _pokeWinParam; // Dpr_UI_PokemonBattleWindow_Param_o*
            UnityEngine::Coroutine::Object* _coCloseMenuUI;
            void* _pokeSelParam; // Dpr_Battle_Logic_PokeSelParam_o*
            void* _pokeSelResult; // Dpr_Battle_Logic_PokeSelResult_o*
            void* _actionParam; // Dpr_Battle_Logic_BattleViewBase_SelectActionParam_o*
            Dpr::Battle::Logic::BTL_POKEPARAM::Object* _swapWaitPokemon;
            int32_t preHUDEventId;
            uint32_t _outMemberIndex;
            uint16_t _selectItem;
            int32_t _selectWaza;
            int32_t _selectPoke;
            void* _autoPilot; // Dpr_Battle_View_Systems_BUIAutoPilot_o*
            Dpr::Battle::Logic::BTL_POKEPARAM::Object* _currentPoke;
            DG::Tweening::Sequence::Object* _decoballSequence;
            int32_t SeqEndWaitSafetyCount;
            int32_t seqDotDispWaitTime;
            UnityEngine::CanvasGroup::Object* _uiCanvasGroup;
        };

        static_assert(sizeof(Fields) == 0x150);
    };
}
