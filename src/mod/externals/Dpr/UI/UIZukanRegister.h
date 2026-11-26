#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/UI/UIWindow.h"
#include "externals/Dpr/UI/ZukanDescriptionPanel.h"
#include "externals/UnityEngine/RectTransform.h"
#include "externals/Dpr/UI/UIMsgWindowController.h"

namespace Dpr::UI {
    struct UIZukanRegister : ILClass<UIZukanRegister> {
        enum class AddMemberResult : int32_t {
            Party = 0,
            Box = 1,
            Cancel = 2
        };

        struct Fields : Dpr::UI::UIWindow::Fields {
            int32_t _animStateIn;
            int32_t _animStateOut;
            Dpr::UI::ZukanDescriptionPanel::Object* descriptionPanel;
            UnityEngine::RectTransform::Object* descriptionHideModelViewPositionRectTransform;
            UnityEngine::RectTransform::Object* contextMenuPositionRectTransform;
            Dpr::UI::UIMsgWindowController::Object* msgWindowController;
            int32_t bootType;
            bool isWaitUpdate;
            bool isRegisterNew;
            bool isNotAddMember;
            bool isSkipAddMemberProc;
            Pml::PokePara::PokemonParam::Object* pokemonParam;
            int32_t addMemberResult;
            System::Action::Object* OnComplete;
        };

        static_assert(offsetof(Fields, OnComplete) == 0xa0);

        static inline StaticILMethod<game::Address::Method_Dpr_UI_UIManager_CreateUIWindow_UIZukanRegister, Dpr::UI::UIZukanRegister> Method$$CreateUIWindow {};

        inline void add_OnComplete(System::Action::Object* value) {
            external<void>(game::Address::Dpr_UI_UIZukanRegister_add_OnComplete, this, value);
        }

        inline void Open(Pml::PokePara::PokemonParam::Object* pokemonParam, bool isSkipAddMemberProc, int32_t prevWindowId) {
            external<void>(game::Address::Dpr_UI_UIZukanRegister_Open, this, pokemonParam, isSkipAddMemberProc, prevWindowId);
        }

        inline void OpenAddMemberOnly(Pml::PokePara::PokemonParam::Object* pokemonParam, bool isNotAddMember, int32_t prevWindowId) {
            external<void>(game::Address::Dpr_UI_UIZukanRegister_OpenAddMemberOnly, this, pokemonParam, isNotAddMember, prevWindowId);
        }
    };
}