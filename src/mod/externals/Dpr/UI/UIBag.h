#pragma once

#include "externals/il2cpp.h"

#include "externals/ContextMenuID.h"
#include "externals/Dpr/Item/ItemInfo.h"
#include "externals/Dpr/UI/PokemonPartyItem.h"
#include "externals/Dpr/UI/UIMsgWindowController.h"
#include "externals/Dpr/UI/UIWindow.h"
#include "externals/Dpr/UI/BagItemPanel.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/Pml/PokeParty.h"
#include "externals/System/Action.h"
#include "externals/UnityEngine/RectTransform.h"
#include "externals/UnityEngine/Vector2.h"
#include "externals/UnityEngine/Vector3.h"

namespace Dpr::UI {
    struct UIBag : ILClass<UIBag> {
        struct __c__DisplayClass127_0 : ILClass<__c__DisplayClass127_0> {
            struct Fields {
                Dpr::UI::UIBag::Object* __4__this;
                Dpr::Item::ItemInfo::Object* item;
                void* useItemAction;
                void* equipItemAction;
                void* __9__12;
                void* __9__17;
                void* __9__13;
                void* __9__19;
            };

            static inline StaticILMethod<0x04c80068> Method$$ShowItemContextMenu_EndUseAction {};
        };

        struct __c__DisplayClass127_1 : ILClass<__c__DisplayClass127_1> {
            struct Fields {
                uint32_t fieldFuncType;
                Dpr::UI::UIBag::__c__DisplayClass127_0::Object* CS___8__locals1;
            };
        };

        struct __c__DisplayClass134_0 : ILClass<__c__DisplayClass134_0> {
            inline void _UseWazaMachine_b__3(int32_t index) {
                external<void>(0x01be0fa0, this, index);
            }
        };

        struct __c__DisplayClass135_0 : ILClass<__c__DisplayClass135_0> {
            struct Fields{

            };
        };

        struct __c__DisplayClass144_0 : ILClass<__c__DisplayClass144_0, 0x04c62840> {
            struct Fields {
                Dpr::UI::UIBag::Object* __4__this;
                Pml::PokePara::PokemonParam::Object* pokemonParam;
                Dpr::UI::PokemonPartyItem::Object* pokemonPartyItem;
            };

            static inline StaticILMethod<0x04c801a0, int32_t> Method$$DisplayClass144_0_UseFormChange {};

            static inline MethodInfo* Method$$OnSelectedIncenseBurner = nullptr;
            static MethodInfo* getMethod$$OnSelectedIncenseBurner(Il2CppMethodPointer method) {
                if (Method$$OnSelectedIncenseBurner == nullptr)
                    Method$$OnSelectedIncenseBurner = (*Method$$DisplayClass144_0_UseFormChange)->copyWith(method);
                return Method$$OnSelectedIncenseBurner;
            };

            inline void ctor() {
                external<void>(0x01be1910, this);
            }
        };

        struct BattleBootParam : ILStruct<BattleBootParam> {
            struct Fields {
                int32_t TopMemberIndex;
                bool IsTrainerBattle;
                bool isDouble;
                System::Boolean_array* SasiosaeFlags;
            };
        };

        struct Fields : Dpr::UI::UIWindow::Fields {
            Dpr::UI::BagItemPanel::Object* bagItemPanel; // BagItemPanel_o
            void * pokemonParty; // PokemonParty_o
            void * wazaSelectPanel; // BagWazaSelectPanel_o
            UnityEngine::RectTransform::Object * effectRoot;
            int32_t _animStateIn;
            int32_t _animStateOut;
            int32_t bootType;
            int32_t modeType;
            Pml::PokePara::PokemonParam::Object * onlyViewPokemonParam;
            int32_t onlyViewPartyIndex;
            int32_t onlyViewBattlePokemonId;
            BattleBootParam::Object battleBootParam;
            void * onSelectedKinomiItem; // System_Action_int__o
            void * onCompleteAutoPilot; // System_Action_o
            Dpr::UI::UIMsgWindowController::Object * msgWindowController;
            void * pokemonPartyParam; // PokemonParty_Param_o
            int32_t displayMoney;
            bool isWaitUpdate;
            bool isWaitPokemonParty;
            bool isKinomiContact;
            bool isDontBuryNuts;
            bool isAutoPilot;
            bool _IsOpenFromBattleTeam_k__BackingField;
        };

        inline void Close(void* onClosed, int32_t nextWindowId, bool isPlayCloseSe) {
            external<void>(0x01859870, this, onClosed, nextWindowId, isPlayCloseSe);
        }

        inline void OpenContextMenu(System::Int32_array* contextMenuIDs, System::Action::Object* onSelected, UnityEngine::Vector2::Object pivot, UnityEngine::Vector3::Object pos, System::Action::Object* onClosed, bool isNoDecideSe, bool isNoCancelSe) {
            UnityEngine::Vector2::Fields pivotProxy = { .x = pivot.fields.x, .y = pivot.fields.y };
            UnityEngine::Vector3::Fields posProxy = { .x = pos.fields.x, .y = pos.fields.y, .z = pos.fields.z };
            external<void>(0x0185c1f0, this, contextMenuIDs, onSelected, pivotProxy, posProxy, onClosed, isNoDecideSe, isNoCancelSe);
        }
    };
}
