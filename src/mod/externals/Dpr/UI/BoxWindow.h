#pragma once
#include "externals/il2cpp-api.h"
#include "BoxStatusPanel.h"
#include "BoxTray.h"
#include "externals/DG/Tweening/Tween.h"
#include "externals/System/Action.h"
#include "externals/UnityEngine/Material.h"
#include "externals/Dpr/UI/Cursor.h"
#include "externals/Dpr/UI/ContextMenuWindow.h"
#include "externals/Dpr/UI/PokemonStatusWindow.h"
#include "Keyguide.h"
#include "externals/System/Collections/IEnumerator.h"
#include "externals/System/Func.h"


namespace Dpr::UI {
    struct BoxWindow : ILClass<BoxWindow> {
        struct __c__DisplayClass200_0 : ILClass<__c__DisplayClass200_0> {
            struct Fields {
                BoxWindow::Object* __4__this;
                int32_t trayNum;
                UnityEngine::Events::UnityAction::Object* __9__11;
            };

            static inline StaticILMethod<0x04c7ea18> Method$$__OpOpen__b__1 {};
        };

        struct __c__DisplayClass204_0 : ILClass<__c__DisplayClass204_0, 0x04c619b0> {
            struct Fields {
                bool isModelClosed;
            };

            inline void ctor() {
                external<void>(0x01a212d0, this);
            }
        };

        struct __c__DisplayClass296_0 : ILClass<__c__DisplayClass296_0, 0x04c61880> {
            struct Fields {
                UnityEngine::Events::UnityAction::Object* onClosed;
                BoxWindow::Object* __4__this;
            };

            static inline StaticILMethod<0x04c7eaf0> Method$$__OpenContextMenu__b__0 {};

            inline void ctor() {
                external<void>(0x01a22910, this);
            }
        };

        struct __OpOpen_d__200 : ILClass<__OpOpen_d__200> {
            struct Fields {
                int32_t __1__state;
                Il2CppObject* __2__current;
                BoxWindow::Object* __4__this;
                int32_t prevWindowId;
                __c__DisplayClass200_0::Object* __8__1;
            };
        };

        struct __OpClose_d__204 : ILClass<__OpClose_d__204, 0x04c617d0> {
            struct Fields {
                int32_t __1__state;
                Il2CppObject* __2__current;
                BoxWindow::Object* __4__this;
                __c__DisplayClass204_0::Object* __8__1;
                int32_t nextWindowId;
                UnityEngine::Events::UnityAction::Object* onClosed_;
            };

            inline void ctor(int32_t __1__state) {
                external<void>(0x01a252d0, this, __1__state);
            }
        };

        struct NaviParam : ILClass<NaviParam, 0x04c616f8> {
            struct Fields {
                int32_t itemType;
                int32_t index;
            };

            inline void ctor() {
                external<void>(0x01a27540, this);
            }
        };

        struct OpenParam : ILClass<OpenParam, 0x04c5ef90> {
            struct Fields {
                int32_t dispMode;
                int32_t tray;
                int32_t index;
                int32_t teamIndex;
                bool isSelectParty;
                int32_t openType;
                int32_t selectCount;
                int32_t targetLevel;
                bool isEnableDying;
                bool isEnableEgg;
                bool isEnableTeam;
                bool isEnableParty;
                bool isShowSelectCount;
                bool isEnableKeyboard;
                bool isOpenFromBattleTeam;
                bool isDisableUseful;
                bool isDisableDuplicate;
                bool isDontDuckOffBGM;
                bool isExternalTrade;
                bool isGMS;
                System::Int32_array* targetsPokeNo;
                System::Int32_array* selectNgPokeNos;
                System::String::Object* tradeName;
                void* selected;
                void* searchData;
            };

            inline void ctor() {
                external<void>(0x01a27970, this);
            }
        };

        struct __c__DisplayClass282_0 : ILClass<__c__DisplayClass282_0> {
            struct Fields {
                BoxWindow::Object* __4__this;
                BoxWindow::OpenParam::Object* reopenParam;
                PokemonStatusWindow::Object* statusParams;
            };
        };

        static inline StaticILMethod<0x04c7ecf0> Method$$Dpr_UI_BoxWindow_OnUpdate {};
        static inline StaticILMethod<0x04c6bd48, Dpr::UI::ContextMenuItem::Object*> Method$$Dpr_UI_BoxWindow_OnContextMenu {};

        struct Fields : Dpr::UI::UIWindow::Fields {
            UnityEngine::UI::Image::Object* _displayTitle; // 0x70
            UIText::Object* _displayModeText; // 0x78
            UnityEngine::UI::Image::Object* _operationTitle; // 0x80
            UIText::Object* _operationTypeText; // 0x88
            UnityEngine::Color32::Array* _operationTypeColors; // 0x90
            UnityEngine::RectTransform::Object* _partyTrayRoot; // 0x98
            void* _partyTrayCanvasGroup; // 0xA0
            UnityEngine::UI::Image::Array* _partyArrows; // 0xA8
            void* _partyNormal; // 0xB0 // Todo Implement Dpr::UI::BoxParty::Object*
            void* _battleTeamScroll; // 0xB8 // Todo Implement Dpr::UI::BoxInfinityScroll::Object*
            UnityEngine::RectTransform::Object* _boxTrayRoot; // 0xC0
            void* _boxTrayCanvasGroup; // 0xC8
            UnityEngine::UI::Image::Array* _trayArrows; // 0xD0
            void* _trayScroll; // 0xD8 // Todo Implement Dpr::UI::BoxInfinityScroll::Object*
            UnityEngine::RectTransform::Object* _naviPartyRoot; // 0xE0
            UnityEngine::RectTransform::Object* _naviTrayRoot; // 0xE8
            void* _multiNavis; // 0xF0
            void* _naviPartySelect; // 0xF8
            UIText::Array* _leftButtonTexts; // 0x100
            UnityEngine::UI::Image::Object* _searchButtunImage; // 0x108
            UnityEngine::UI::Image::Object* _searchIconImage; // 0x110
            UnityEngine::UI::Image::Object* _boxButtunImage; // 0x118
            UnityEngine::UI::Image::Object* _boxIconImage; // 0x120
            void* _boxListPanel; // 0x128 // Todo Implement Dpr::UI::BoxListPanel::Object*
            void* _swapRootPrefab; // 0x130
            UnityEngine::RectTransform::Object* _contextMenuRoot; // 0x138
            Dpr::UI::Cursor::Object* _cursor; // 0x140
            UnityEngine::Transform::Object* _cursorBody; // 0x148
            UnityEngine::UI::Image::Object* _rangeCursor; // 0x150
            UnityEngine::UI::Image::Object* _rangePlate; // 0x158
            void* _cursorCanvas; // 0x160
            void* _modelViewSide; // 0x168
            void* _modelViewTrade; // 0x170
            UnityEngine::RectTransform::Object* _modelSideRoot; // 0x178
            UnityEngine::RectTransform::Object* _modelTradeRoot; // 0x180
            void* _searchPanel; // 0x188
            void* _searchSubPanel; // 0x190
            void* _searchDescription; // 0x198
            UnityEngine::GameObject::Object* _tradeInfo; // 0x1A0
            UIText::Object* _traderName; // 0x1A8
            void* _namePlate; // 0x1B0
            void* _numberPlate; // 0x1B8
            BoxStatusPanel::Object* _statusPanel; // 0x1C0
            void* _markSettingPanel; // 0x1C8
            UnityEngine::GameObject::Object* _selectCountPlate; // 0x1D0
            UIText::Object* _selectCountText; // 0x1D8
            void* _wallPaperSelector; // 0x1E0
            void* _topCanvasGroup; // 0x1E8
            void* _bottomCanvasGroup; // 0x1F0
            UnityEngine::UI::Image::Object* _darkScreen; // 0x1F8
            UnityEngine::Transform::Object* _effectRoot; // 0x200
            int32_t _animStateIn; // 0x208
            int32_t _animStateOut; // 0x20C
            int32_t _controlType; // 0x210
            int32_t _operationType; // 0x214
            int32_t _displayMode; // 0x218
            int32_t _statusType; // 0x21C
            void* _battleTeam; // 0x220
            BoxTray::Object* _boxTray; // 0x228
            void* _navigator; // 0x230
            void* _swapIcon; // 0x238
            void* _currentPokeParam; // 0x240
            System::String::Object* _currentMsgID; // 0x248
            int32_t _currentTrayIndex; // 0x250
            int32_t _currentTeamIndex; // 0x254
            int32_t _wallNo; // 0x258
            void* _lastNaviBoxTrayItem; // 0x260
            void* _fromNavigator; // 0x268
            void* _returnNavigator; // 0x270
            int32_t _fromTrayIndex; // 0x278
            int32_t _fromTeamIndex; // 0x27C
            BoxInfinityScrollItem::Object* _fromTray; // 0x280
            void* _hideIconIndexes; // 0x288
            void* _effectIndexes; // 0x290
            bool _isRepeatCancel; // 0x298
            int32_t _rangeX; // 0x29C
            int32_t _rangeY; // 0x2A0
            int32_t _rangeWidth; // 0x2A4
            int32_t _rangeHeight; // 0x2A8
            void* _rangeBeginNavigator; // 0x2B0
            void* _swapIcons; // 0x2B8
            void* _fromNavigators; // 0x2C0
            void* _toNavigators; // 0x2C8
            void* _existSwapIndexes; // 0x2D0
            int32_t _searchType; // 0x2D8
            void* _searchData; // 0x2E0
            bool _isDuckOn; // 0x2E8
            void* _param; // 0x2F0
            void* _selected; // 0x2F8
            System::Action::Object* _onSelected; // 0x300
            System::Action::Object* _onDecide; // 0x308
            System::Action::Object* _onConfirm; // 0x310
            System::Action::Object* _onComplete; // 0x318
            System::Action::Object* _onCancelSelect; // 0x320
            int32_t _targetLangId; // 0x328
            UnityEngine::Coroutine::Object* _coOpen; // 0x330
            UnityEngine::Coroutine::Object* _coClose; // 0x338
            void* _modelView; // 0x340
            void* _messageParam; // 0x348
            void* _keyguideParam; // 0x350
            UnityEngine::Material::Object* _matSearchButton; // 0x358
            UnityEngine::Material::Object* _matBoxButton; // 0x360
            void* _twSequence; // 0x368
            Dpr::UI::ContextMenuWindow::Object* _contextMenu; // 0x370
            float _waitSave; // 0x378
            void* _formChangeEffectInstance; // 0x380
            bool isCancelFormChange; // 0x388
            void* _tradeParam; // 0x390
            void* _tradeInfoCanvasGroup; // 0x398
            void* _networkDialog; // 0x3A0
            bool _isPhaseProcDone; // 0x3A8
            bool _isTradeInfoEnable; // 0x3A9
            float _tradeTimeoutCount; // 0x3AC
            bool _isTradeCheckIllegal; // 0x3B0
            float _modelOpenPosX; // 0x3B4
            float _modelClosePosX; // 0x3B8
            float _modelTradeOpenPosX; // 0x3BC
            float _modelTradeClosePosX; // 0x3C0
            float _partyOpenPosX; // 0x3C4
            float _partyClosePosX; // 0x3C8
            float _boxTrayPosX; // 0x3CC
            float _boxTraySlideX; // 0x3D0
            float _trayCellSizeX; // 0x3D4
            float _trayCellSizeY; // 0x3D8
            bool _isControlEnable; // 0x3DC
            bool _isForceClosing; // 0x3DD
        };

        static_assert(offsetof(Fields, _isForceClosing) == 0x3CD);

        struct VirtualInvokeData_OpPlayOpenWindowAnimation {
            typedef System::Collections::IEnumerator::Object*(*Il2CppMethodPointer)(UIWindow::Object*,
                                                                                    int32_t, UnityEngine::Events::UnityAction::Object*, const MethodInfo*);
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
            VirtualInvokeData _14_OnCloseKeyGuide;
            VirtualInvokeData _15_OpenMessageWindow;
            VirtualInvokeData _16_CloseMessageWindow;
            VirtualInvokeData _17_IsActiveMessageWindow;
            VirtualInvokeData _18_CreateContextMenuYesNo;
            VirtualInvokeData _19_CreateContextMenuYesNo;
            VirtualInvokeData _20_OnAddContextMenuYesNoItemParams;
        };

        static_assert(sizeof(VTable) == 0x150, "Size of VTable is not 0x150\n");
        static_assert(offsetof(VTable, _10_OpPlayOpenWindowAnimation) == 0xa0,
                      "Offset of _10_OpPlayOpenWindowAnimation is not 0xa0\n");

        inline void Open(int32_t prevWindowId, bool isDuckOn) {
            external<void>(0x01cb6080, this, prevWindowId, isDuckOn);
        }

        inline void Close(UnityEngine::Events::UnityAction::Object* onClosed, int32_t nextWindowId) {
            external<void>(0x01cb5cc0, this, onClosed, nextWindowId);
        }

        inline void Awake() {
            external<void>(0x01cb6020, this);
        }

        inline void SetupKeyguide() {
            external<void>(0x01cb7e30, this);
        }

        inline void OpenStatusWindow(Dpr::UI::PokemonStatusWindow::Param::Object* statusParams,
                                     Dpr::UI::BoxWindow::OpenParam::Object* reopenParams) {
            external<void>(0x01cc5ad0, this, statusParams, reopenParams);
        }

        inline void OpenContextMenu(System::Collections::Generic::List$$ContextMenuItem_Param::Object* menuItems,
                                    System::Func::Object* onClicked, UnityEngine::Events::UnityAction::Object* onClosed,
                                    int32_t selectIndex, uint32_t seDecide) {
            external<void>(0x01cc4ab0, this, menuItems, onClicked, onClosed, selectIndex, seDecide);

        }
    };
}