#pragma once

#include "externals/il2cpp-api.h"

#include "externals/DPData/PoffinData.h"
#include "externals/Dpr/Message/MessageMsgFile.h"
#include "externals/Dpr/UI/ContextMenuItem.h"
#include "externals/Dpr/UI/Cursor.h"
#include "externals/Dpr/UI/UIScrollView.h"
#include "externals/Dpr/UI/UIText.h"
#include "externals/Dpr/UI/UIWindow.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/System/Action.h"
#include "externals/System/Primitives.h"
#include "externals/UnityEngine/Coroutine.h"
#include "externals/UnityEngine/GameObject.h"
#include "externals/UnityEngine/MonoBehaviour.h"

namespace Dpr::UI {
    struct UIPofinCase : ILClass<UIPofinCase> {
        struct ItemButton : ILClass<ItemButton> {
            struct Fields : UnityEngine::MonoBehaviour::Fields {
                UIText::Object* nameText;
                UIText::Object* levelText;
                UnityEngine::UI::Image::Object* newImage;
                int32_t index;
                DPData::PoffinData::Object _Data_k__BackingField;
            };

            inline UnityEngine::RectTransform::Object* GetRectTransform() {
                return external<UnityEngine::RectTransform::Object*>(game::Address::Dpr_UI_UIPofinCase_ItemButton_GetRectTransform, this);
            }
        };

        struct DisplayClass35_0 : ILClass<DisplayClass35_0, game::Address::Dpr_UI_UIPofinCase_c_DisplayClass35_0_TypeInfo> {
            struct Fields {
                UIPofinCase::Object* __4__this;
                DPData::PoffinData::Object poffinData;
                System::Action::Object* __9__6;
                UnityEngine::Events::UnityAction::Object* __9__4;
                System::Action::Object* __9__2;
            };

            static_assert(offsetof(Fields, __9__2) == 0x28);

            static inline StaticILMethod<game::Address::Method_Dpr_UI_UIPofinCase_c_DisplayClass35_0_ShowItemContextMenu_b_1, Dpr::UI::ContextMenuItem, bool> Method$$ShowItemContextMenu_b__1 {};

            static inline MethodInfo* Method$$SelectPoffinUIOnClicked = nullptr;
            static MethodInfo* getMethod$$SelectPoffinUIOnClicked(Il2CppMethodPointer method) {
                if (Method$$SelectPoffinUIOnClicked == nullptr)
                    Method$$SelectPoffinUIOnClicked = (*Method$$ShowItemContextMenu_b__1)->copyWith(method);
                return Method$$SelectPoffinUIOnClicked;
            };

            inline void ctor() {
                external<void>(game::Address::Dpr_UI_UIPofinCase_c_DisplayClass35_0_ctor, this);
            }
        };

        struct __c : ILClass<__c, game::Address::Dpr_UI_UIPofinCase_c_TypeInfo> {
            struct StaticFields {
                UIPofinCase::__c::Object* __9;
                UnityEngine::Events::UnityAction::Object* __9__35_0;
                UnityEngine::Events::UnityAction::Object* __9__35_3;
                UnityEngine::Events::UnityAction::Object* __9__40_0;
            };
        };

        struct Fields : Dpr::UI::UIWindow::Fields {
            void* categorySelector; // Dpr_UI_UIPofinCase_CategorySelector_o*
            void* tasteLump; // Dpr_UI_UIPofinCase_TasteLump_o*
            UIScrollView::Object* scrollView;
            Cursor::Object* cursor;
            UIText::Object* subText;
            UnityEngine::GameObject::Object* noDataTextObject;
            UnityEngine::GameObject::Array* noDataDisableObjects;
            Pml::PokePara::PokemonParam::Object* selectedPokemonParam;
            System::Action::Object* onResultCallback;
            void* poffinResultDatas; // XLSXContent_PoffinResult_SheetSheet1_array*
            DPData::PoffinData::Array* poffinDatas;
            int32_t selectedIndex;
            int32_t selectPartyIndex;
            float scrollPos;
            ItemButton::Object* currentItemButton;
            Dpr::Message::MessageMsgFile::Object* poffinMainMsgFile;
            UnityEngine::Coroutine::Object* removeNewIconCoroutine;
            void* poffin3DModel; // Dpr_Demo_Demo_ModelViewer_o*
            void* conditionParam; // Dpr_Demo_ConditionParam_o*
        };

        static_assert(offsetof(Fields, conditionParam) == 0xE8);

        static inline StaticILMethod<game::Address::Method_Dpr_UI_UIPofinCase_c_ShowItemContextMenu_b_35_0, Dpr::UI::UIWindow>    Method$$ShowItemContextMenu_b__35_0 {};

        static inline StaticILMethod<game::Address::Method_Dpr_UI_UIManager_CreateUIWindow_UIPofinCase, Dpr::UI::UIPofinCase> Method$$CreateUIWindow {};

        inline void Open(int32_t prevWindowId) {
            external<void>(game::Address::Dpr_UI_UIPofinCase_Open, this, prevWindowId);
        }

        inline void Close(UnityEngine::Events::UnityAction::Object* onClosed_, int32_t nextWindowId) {
            external<void>(game::Address::Dpr_UI_UIPofinCase_Close, this, onClosed_, nextWindowId);
        }
    };
}