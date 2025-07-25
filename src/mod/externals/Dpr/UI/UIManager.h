#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "externals/Dpr/Message/MessageEnumData.h"
#include "externals/Dpr/UI/UIModelViewController.h"
#include "externals/Dpr/UI/UIWazaManage.h"
#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"
#include "externals/SpriteAtlasID.h"
#include "externals/System/Action.h"
#include "externals/System/Func.h"
#include "externals/UIWindowID.h"
#include "externals/UnityEngine/Events/UnityAction.h"
#include "externals/UnityEngine/Transform.h"
#include "externals/XLSXContent/UIDatabase.h"

namespace Dpr::UI {
    struct Keyguide;
    struct ShopBoutiqueChange;
    struct UIZukanRegister;
}

namespace Dpr::UI {
    struct UIManager : ILClass<UIManager, 0x04c5cc28> {
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

        static inline Dpr::UI::UIManager::Object* get_Instance() {
            return SmartPoint::AssetAssistant::SingletonMonoBehaviour::get_Instance(SmartPoint::AssetAssistant::SingletonMonoBehaviour::Method$$UIManager$$get_Instance);
        }

        static inline StaticILMethod<0x04c90130, Dpr::UI::UIWindow> Method$$GetCurrentUIWindow_UIWindow_ {};

        template <typename T>
        inline T::Object* CreateUIWindow(UIWindowID windowId, ILMethod<T>& method) {
            return external<typename T::Object*>(0x01cf9f20, this, (int32_t)windowId, *method);
        }

        template <typename T>
        inline T::Object* CreateUIWindow(UIWindowID windowId) {
            return external<typename T::Object*>(0x01cf9f20, this, (int32_t)windowId, *T::Method$$CreateUIWindow);
        }

        template <typename T>
        inline T::Object* GetCurrentUIWindow(ILMethod<T>& method) {
            return external<typename T::Object*>(0x01cfa100, this, *method);
        }

        static inline int32_t Repeat(int32_t value, int32_t start, int32_t end) {
            return external<int32_t>(0x017c4990, value, start, end);
        }

        inline void LoadSpritePokemon(int32_t monsNo, uint16_t formNo, Pml::Sex sex, Pml::PokePara::RareType rareType, bool isEgg, UnityEngine::Events::UnityAction::Object* onComplete) {
            external<void>(0x017c3ef0, this, monsNo, formNo, sex, rareType, isEgg, onComplete);
        }

        inline void LoadSpritePokemon(Pml::PokePara::PokemonParam::Object* pokemonParam, UnityEngine::Events::UnityAction::Object* onComplete) {
            external<void>(0x017c3e40, this, pokemonParam, onComplete);
        }

        inline UnityEngine::Sprite::Object* GetSpritePokemonTypeZukan(int32_t typeNo, int32_t langId) {
            return external<UnityEngine::Sprite::Object*>(0x017c2650, this, typeNo, langId);
        }

        inline UnityEngine::Sprite::Object* GetAtlasSprite(SpriteAtlasID spriteAtlasId, System::String::Object* name) {
            return external<UnityEngine::Sprite::Object*>(0x017a9080, this, spriteAtlasId, name);
        }

        inline XLSXContent::UIDatabase::SheetPokemonIcon::Object* GetPokemonIconData(int32_t monsNo, uint16_t formNo, Pml::Sex sex, Pml::PokePara::RareType rareType, bool isEgg) {
            return external<XLSXContent::UIDatabase::SheetPokemonIcon::Object*>(0x017c1250, this, monsNo, formNo, sex, rareType, isEgg);
        }

        inline Dpr::UI::Keyguide* GetKeyguide(UnityEngine::Transform::Object* transform, bool isCreate) {
            return external<Dpr::UI::Keyguide*>(0x017a8ce0, this, transform, isCreate);
        }

        static inline Dpr::Message::MessageEnumData::MsgLangId GetCurrentLangId() {
            return external<Dpr::Message::MessageEnumData::MsgLangId>(0x017be8c0);
        }

        inline UnityEngine::Sprite::Object* GetSpritePokemonSex(Pml::Sex sex) {
            return external<UnityEngine::Sprite::Object*>(0x017c11f0, this, sex);
        }

        static inline Dpr::Battle::Logic::BTL_POKEPARAM::Object* ToBattlePokemonParam(Pml::PokePara::PokemonParam::Object* pokemonParam) {
            return external<Dpr::Battle::Logic::BTL_POKEPARAM::Object*>(0x017c2ea0, pokemonParam);
        }

        inline UnityEngine::Sprite::Object* GetSpritePokemonLanguage(Dpr::Message::MessageEnumData::MsgLangId langId) {
            return external<UnityEngine::Sprite::Object*>(0x017c2720, this, langId);
        }

        inline UnityEngine::Sprite::Object* GetSpriteMonsterBall(uint8_t ballId) {
            return external<UnityEngine::Sprite::Object*>(0x017c2300, this, ballId);
        }

        inline XLSXContent::UIDatabase::SheetBoxOpenParam::Object* GetBoxOpenData(int32_t type) {
            return external<XLSXContent::UIDatabase::SheetBoxOpenParam::Object*>(0x017c2bd0, this, type);
        }
    };
}

static_assert(offsetof(Dpr::UI::UIManager::Fields, _mdUis) == 0x20);
static_assert(sizeof(Dpr::UI::UIManager::Fields) == 280);
