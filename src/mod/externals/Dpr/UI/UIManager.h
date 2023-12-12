#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/UI/UIModelViewController.h"
#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"
#include "externals/System/Action.h"
#include "externals/System/Func.h"
#include "externals/UIWindowID.h"
#include "externals/UnityEngine/Events/UnityAction.h"
#include "externals/UnityEngine/Transform.h"
#include "externals/XLSXContent/UIDatabase.h"

namespace Dpr::UI {
    struct UIWindow;

    struct BoxWindow;
    struct ShopBoutiqueChange;
    struct UIWazaManage;
}

namespace Dpr::UI {
    struct UIManager : ILClass<UIManager, 0x04c5cc28> {
        struct UIInstance : ILClass<UIInstance> {
            struct Fields {
                UIWindow* _uiWindow;
                UIWindowID _windowId;
            };
        };

        struct Fields : SmartPoint::AssetAssistant::SingletonMonoBehaviour::Fields {
            UnityEngine::Transform::Object* _activeRoot;
            Dpr::UI::UIModelViewController::Object* _modelView;
            UnityEngine::Transform::Object* _blurBgRoot;
            XLSXContent::UIDatabase::Object* _mdUis;
            char todo[240];
        };

        struct StaticFields {
            int32_t StickLLeft;
            int32_t StickLRight;
            int32_t StickLUp;
            int32_t StickLDown;
            int32_t ButtonA;
            int32_t ButtonB;
            int32_t ButtonX;
            int32_t ButtonY;
            int32_t ButtonPlusMinus;
            UnityEngine::Events::UnityAction::Object* onWazaFly;
            UnityEngine::Events::UnityAction::Object* onDressChanged;
            System::Func::Object* onFieldWaza;
            System::Func::Object* onUseFieldItem;
            System::Action::Object* onUseItemInBattle;
            System::Action::Object* onUseHidenWaza;
            System::Func::Object* onUseDowsing;
            void* _comparerPokemonIcon; // Dpr_UI_UIManager_ComparerPokemonIcon_o*
            void* _comparerAshiatoIcon; // Dpr_UI_UIManager_ComparerAshiatoIcon_o*
            void* _comparerZukanDisplay; // Dpr_UI_UIManager_ComparerZukanDisplay_o*
            void* _comparerPokemonVoice; // Dpr_UI_UIManager_ComparerPokemonVoice_o*
            void* _comparerZukanCompareHeight; // Dpr_UI_UIManager_ComparerZukanCompareHeight_o*
            void* _langParams; // Dpr_UI_UIManager_LangParam_array*
            void* _pokemonParamMappings; // System_Collections_Generic_List_UIManager_PokemonParamMapping__o*
            int32_t _id_GrayscaleAmount;
            void* _comparerTownMapGuide; // Dpr_UI_UIManager_ComparerTownMapGuide_o*
            void* _comparerPlaceName; // Dpr_UI_UIManager_ComparerPlaceName_o*
        };

        static inline Dpr::UI::UIManager::Object* instance() {
            return SmartPoint::AssetAssistant::SingletonMonoBehaviour::get_Instance(SmartPoint::AssetAssistant::SingletonMonoBehaviour::Method$$UIManager$$get_Instance);
        }

        static inline StaticILMethod<0x04c8ff30, Dpr::UI::BoxWindow> Method$$CreateUIWindow_BoxWindow_ {};
        static inline StaticILMethod<0x04c8ffe8, Dpr::UI::ShopBoutiqueChange> Method$$CreateUIWindow_ShopBoutiqueChange_ {};
        static inline StaticILMethod<0x04c90098, Dpr::UI::UIWazaManage> Method$$CreateUIWindow_UIWazaManage_ {};

        static inline StaticILMethod<0x04c90130, Dpr::UI::UIWindow> Method$$GetCurrentUIWindow_UIWindow_ {};

        template <typename T>
        inline T::Object* CreateUIWindow(UIWindowID windowId, ILMethod<T>& method) {
            return external<typename T::Object*>(0x01cf9f20, this, (int32_t)windowId, *method);
        }

        template <typename T>
        inline T::Object* GetCurrentUIWindow(ILMethod<T>& method) {
            return external<typename T::Object*>(0x01cfa100, this, *method);
        }

        static inline int32_t Repeat(int32_t value, int32_t start, int32_t end) {
            return external<int32_t>(0x017c4990, value, start, end);
        }

        inline void LoadSpritePokemon(int32_t monsNo, uint16_t formNo, uint8_t sex, uint8_t rareType, bool isEgg, UnityEngine::Events::UnityAction::Object* onComplete) {
            external<void>(0x017c3ef0, this, monsNo, formNo, sex, rareType, isEgg, onComplete);
        }
    };
}

static_assert(offsetof(Dpr::UI::UIManager::Fields, _mdUis) == 0x20);
static_assert(sizeof(Dpr::UI::UIManager::Fields) == 280);
