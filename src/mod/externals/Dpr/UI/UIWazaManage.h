#pragma once

#include "externals/il2cpp.h"

#include "externals/Dpr/UI/PokemonSick.h"
#include "externals/Dpr/UI/UIMsgWindowController.h"
#include "externals/Dpr/UI/UIText.h"
#include "externals/Dpr/UI/UIWindow.h"
#include "externals/Dpr/UI/WazaManageCondition.h"
#include "externals/Dpr/UI/WazaManagePokemonStausPanel.h"
#include "externals/Dpr/UI/WazaManageWazaStatusPanel.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/System/Action.h"
#include "externals/UnityEngine/GameObject.h"
#include "externals/UnityEngine/RectTransform.h"
#include "externals/UnityEngine/UI/Image.h"

namespace Dpr::UI {
    struct UIWazaManage : ILClass<UIWazaManage> {
        struct __c__DisplayClass35_0 : ILClass<__c__DisplayClass35_0, game::Address::Dpr_UI_UIWazaManage_c_DisplayClass35_0_TypeInfo> {
            struct Fields {
                Pml::PokePara::PokemonParam::Object* pokemonParam;
            };

            inline void ctor() {
                external<void>(game::Address::Dpr_UI_UIWazaManage_c_DisplayClass35_0_ctor, this);
            }
        };

        struct __c__DisplayClass39_0 : ILClass<__c__DisplayClass39_0, game::Address::Dpr_UI_UIWazaManage_c_DisplayClass39_0_TypeInfo> {
            struct Fields {
                int32_t selectWazaNo;
                int32_t learnWazaNo;
                int32_t unlearnWazaNo;
                Dpr::UI::UIWazaManage::Object* __4__this;
                System::Action::Object* __9__4;
                System::Action::Object* __9__3;
            };

            inline void ctor() {
                external<void>(game::Address::Dpr_UI_UIWazaManage_c_DisplayClass39_0_ctor, this);
            }
        };

        struct Param : ILStruct<Param> {
            struct Fields {
                int32_t BootType;
                bool IsOpenFromFieldScript;
                Pml::PokePara::PokemonParam::Object* PokemonParam;
                int32_t LearnWazaNo;
                System::Action::Object* ResultCallback;
            };
        };

        struct Fields : Dpr::UI::UIWindow::Fields {
            UnityEngine::GameObject::Object* bgObject;
            UnityEngine::GameObject::Object* withWazaBgObject;
            void* categoryTabs; // Dpr_UI_PokemonStatusTab_array*
            UnityEngine::GameObject::Array* statusPanelObjects;
            UnityEngine::UI::Image::Array* categoryTabCornerImages;
            WazaManageWazaStatusPanel::Array* wazaStatusPanels;
            UnityEngine::UI::Image::Object* pokemonInfoMonsterBallImage;
            UnityEngine::RectTransform::Object* pokemonInfoSelectArrowRoot;
            Dpr::UI::UIText::Object* pokemonInfoName;
            Dpr::UI::UIText::Object* pokemonInfoLevel;
            UnityEngine::UI::Image::Object* pokemonInfoSex;
            UnityEngine::UI::Image::Object* pokemonInfoLanguage;
            Dpr::UI::PokemonSick::Object* pokemonInfoSick;
            WazaManagePokemonStausPanel::Object* battlePokemonStatusPanel;
            WazaManagePokemonStausPanel::Object* contestPokemonStatusPanel;
            WazaManageCondition::Object* statusPanelCondition;
            Dpr::UI::UIMsgWindowController::Object* msgWindowController;
            void* activeTabs; // System_Collections_Generic_List_PokemonStatusTab__o*
            int32_t _animStateIn;
            int32_t _animStateOut;
            int32_t selectTabIndex;
            Dpr::UI::UIWazaManage::Param::Object param;
        };

        static_assert(offsetof(Fields, param) == 0x100);

        static inline StaticILMethod<game::Address::Method_Dpr_UI_UIWazaManage_c_DisplayClass39_0_OnUpdate_b_2> Method$$OnUpdate$$b__2 {};
        static inline StaticILMethod<game::Address::Method_Dpr_UI_UIWazaManage_OnUpdate_b_39_0> Method$$OnUpdate$$b__39_0 {};
        static inline StaticILMethod<game::Address::Method_Dpr_UI_UIWazaManage_OnUpdate_b_39_1> Method$$OnUpdate$$b__39_1 {};

        static inline StaticILMethod<game::Address::Method_Dpr_UI_UIManager_CreateUIWindow_UIWazaManage, Dpr::UI::UIWazaManage> Method$$CreateUIWindow {};

        inline void Open(Dpr::UI::UIWazaManage::Param::Object param, MethodInfo* method) {
            external<void>(game::Address::Dpr_UI_UIWazaManage_Open, this, param, method);
        }

        inline void UpdateSelectTab(float deltaTime) {
            external<void>(game::Address::Dpr_UI_UIWazaManage_UpdateSelectTab, this, deltaTime);
        }

        inline void Close(UnityEngine::Events::UnityAction::Object* onClosed_) {
            external<void>(game::Address::Dpr_UI_UIWazaManage_Close, this, onClosed_);
        }
    };
}
