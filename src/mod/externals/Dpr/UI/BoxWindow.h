#pragma once

#include "externals/il2cpp-api.h"

#include "externals/DG/Tweening/Sequence.h"
#include "externals/Dpr/MsgWindow/MsgWindowParam.h"
#include "externals/Dpr/UI/BoxInfinityScroll.h"
#include "externals/Dpr/UI/BoxListPanel.h"
#include "externals/Dpr/UI/BoxMultiNavigator.h"
#include "externals/Dpr/UI/BoxParty.h"
#include "externals/Dpr/UI/BoxStatusPanel.h"
#include "externals/Dpr/UI/PokemonIcon.h"
#include "externals/Dpr/UI/UINavigator.h"
#include "externals/Dpr/UI/UIText.h"
#include "externals/Dpr/UI/UIWindow.h"
#include "externals/Effect/EffectInstance.h"
#include "externals/System/Collections/Generic/List.h"
#include "externals/UnityEngine/Canvas.h"
#include "externals/UnityEngine/CanvasGroup.h"
#include "externals/UnityEngine/Color32.h"
#include "externals/UnityEngine/Coroutine.h"
#include "externals/UnityEngine/Material.h"
#include "externals/UnityEngine/RectTransform.h"
#include "externals/UnityEngine/Transform.h"
#include "externals/UnityEngine/UI/Image.h"

namespace Dpr::UI {
    struct BoxWindow : ILClass<BoxWindow, 0x04c5ca38> {
        struct Fields : UIWindow::Fields {
            UnityEngine::UI::Image::Object* _displayTitle;
            UIText::Object* _displayModeText;
            UnityEngine::UI::Image::Object* _operationTitle;
            UIText::Object* _operationTypeText;
            UnityEngine::Color32::Array* _operationTypeColors;
            UnityEngine::RectTransform::Object* _partyTrayRoot;
            UnityEngine::CanvasGroup::Object* _partyTrayCanvasGroup;
            UnityEngine::UI::Image::Array* _partyArrows;
            BoxParty::Object* _partyNormal;
            BoxInfinityScroll::Object* _battleTeamScroll;
            UnityEngine::RectTransform::Object* _boxTrayRoot;
            UnityEngine::CanvasGroup::Object* _boxTrayCanvasGroup;
            UnityEngine::UI::Image::Array* _trayArrows;
            BoxInfinityScroll::Object* _trayScroll;
            UnityEngine::RectTransform::Object* _naviPartyRoot;
            UnityEngine::RectTransform::Object* _naviTrayRoot;
            BoxMultiNavigator::Array* _multiNavis;
            UINavigator::Object* _naviPartySelect;
            UIText::Array* _leftButtonTexts;
            UnityEngine::UI::Image::Object* _searchButtunImage;
            UnityEngine::UI::Image::Object* _searchIconImage;
            UnityEngine::UI::Image::Object* _boxButtunImage;
            UnityEngine::UI::Image::Object* _boxIconImage;
            BoxListPanel::Object* _boxListPanel;
            PokemonIcon::Object* _swapRootPrefab;
            UnityEngine::RectTransform::Object* _contextMenuRoot;
            struct Dpr_UI_Cursor_o* _cursor;
            UnityEngine::Transform::Object* _cursorBody;
            UnityEngine::UI::Image::Object* _rangeCursor;
            UnityEngine::UI::Image::Object* _rangePlate;
            UnityEngine::Canvas::Object* _cursorCanvas;
            struct Dpr_UI_PokemonModelView_o* _modelViewSide;
            struct Dpr_UI_PokemonModelView_o* _modelViewTrade;
            UnityEngine::RectTransform::Object* _modelSideRoot;
            UnityEngine::RectTransform::Object* _modelTradeRoot;
            struct Dpr_UI_BoxSearchPanel_o* _searchPanel;
            struct Dpr_UI_BoxTextSelector_o* _searchSubPanel;
            struct Dpr_UI_BoxSearchDescription_o* _searchDescription;
            UnityEngine::GameObject::Object* _tradeInfo;
            UIText::Object* _traderName;
            struct Dpr_UI_BoxNamePlate_o* _namePlate;
            struct Dpr_UI_BoxSelectNumberPlate_o* _numberPlate;
            BoxStatusPanel::Object* _statusPanel;
            struct Dpr_UI_BoxMarkingSettingPanel_o* _markSettingPanel;
            UnityEngine::GameObject::Object* _selectCountPlate;
            UIText::Object* _selectCountText;
            struct Dpr_UI_BoxWallPaperSelector_o* _wallPaperSelector;
            UnityEngine::CanvasGroup::Object* _topCanvasGroup;
            UnityEngine::CanvasGroup::Object* _bottomCanvasGroup;
            UnityEngine::UI::Image::Object* _darkScreen;
            UnityEngine::Transform::Object* _effectRoot;
            int32_t _animStateIn;
            int32_t _animStateOut;
            int32_t _controlType;
            int32_t _operationType;
            int32_t _displayMode;
            int32_t _statusType;
            BoxParty::Object* _battleTeam;
            struct Dpr_UI_BoxTray_o* _boxTray;
            UINavigator::Object* _navigator;
            PokemonIcon::Object* _swapIcon;
            Pml::PokePara::PokemonParam::Object* _currentPokeParam;
            System::String::Object* _currentMsgID;
            int32_t _currentTrayIndex;
            int32_t _currentTeamIndex;
            int32_t _wallNo;
            UINavigator::Object* _lastNaviBoxTrayItem;
            UINavigator::Object* _fromNavigator;
            UINavigator::Object* _returnNavigator;
            int32_t _fromTrayIndex;
            int32_t _fromTeamIndex;
            BoxInfinityScrollItem::Object* _fromTray;
            System::Collections::Generic::List$$int32_t::Object* _hideIconIndexes;
            System::Collections::Generic::List$$int32_t::Object* _effectIndexes;
            bool _isRepeatCancel;
            int32_t _rangeX;
            int32_t _rangeY;
            int32_t _rangeWidth;
            int32_t _rangeHeight;
            UINavigator::Object* _rangeBeginNavigator;
            void* _swapIcons; // System_Collections_Generic_List_PokemonIcon__o*
            void* _fromNavigators; // System_Collections_Generic_List_UINavigator__o*
            void* _toNavigators; // System_Collections_Generic_List_UINavigator__o*
            void* _existSwapIndexes; // System_Collections_Generic_HashSet_int__o*
            int32_t _searchType;
            struct Dpr_UI_BoxWindow_SEARCH_DATA_o* _searchData;
            bool _isDuckOn;
            struct Dpr_UI_BoxWindow_OpenParam_o* _param;
            void* _selected; // System_Collections_Generic_List_BoxWindow_SelectedPokemon__o*
            System::Action::Object* _onSelected;
            System::Action::Object* _onDecide;
            System::Action::Object* _onConfirm;
            System::Action::Object* _onComplete;
            System::Action::Object* _onCancelSelect;
            int32_t _targetLangId;
            UnityEngine::Coroutine::Object* _coOpen;
            UnityEngine::Coroutine::Object* _coClose;
            struct Dpr_UI_PokemonModelView_o* _modelView;
            Dpr::MsgWindow::MsgWindowParam::Object* _messageParam;
            struct Dpr_UI_Keyguide_Param_o* _keyguideParam;
            UnityEngine::Material::Object* _matSearchButton;
            UnityEngine::Material::Object* _matBoxButton;
            DG::Tweening::Sequence::Object* _twSequence;
            struct Dpr_UI_ContextMenuWindow_o* _contextMenu;
            float _waitSave;
            Effect::EffectInstance::Object* _formChangeEffectInstance;
            bool isCancelFormChange;
            struct Dpr_UI_BoxWindow_TradeParam_o* _tradeParam;
            UnityEngine::CanvasGroup::Object* _tradeInfoCanvasGroup;
            struct Dpr_UI_DialogWindow_o* _networkDialog;
            bool _isPhaseProcDone;
            bool _isTradeInfoEnable;
            float _tradeTimeoutCount;
            bool _isTradeCheckIllegal;
            float _modelOpenPosX;
            float _modelClosePosX;
            float _modelTradeOpenPosX;
            float _modelTradeClosePosX;
            float _partyOpenPosX;
            float _partyClosePosX;
            float _boxTrayPosX;
            float _boxTraySlideX;
            float _trayCellSizeX;
            float _trayCellSizeY;
            bool _isControlEnable;
            bool _isForceClosing;
        };

        inline void Open(int32_t prevWindowId, bool isDuckOn) {
            external<void>(0x01cb6080, this, prevWindowId, isDuckOn);
        }
    };
}