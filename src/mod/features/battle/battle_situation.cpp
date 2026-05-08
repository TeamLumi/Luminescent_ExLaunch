#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/BattleStr.h"
#include "externals/Dpr/Battle/Logic/BtlGround.h"
#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "externals/Dpr/Battle/View/BattleViewCore.h"
#include "externals/Dpr/Battle/View/BtlvInput.h"
#include "externals/Dpr/Battle/View/Systems/BattleViewUISystem.h"
#include "externals/Dpr/Battle/View/UI/BUISituation.h"
#include "externals/Dpr/Battle/View/UI/BUISituationDetail.h"
#include "externals/Dpr/Message/MessageWordSetHelper.h"
#include "externals/Dpr/NetworkUtils/NetworkManager.h"
#include "externals/Dpr/UI/PokemonIcon.h"
#include "externals/Dpr/UI/TypePanel.h"
#include "externals/Dpr/UI/UIManager.h"
#include "externals/ExtensionMethods.h"
#include "externals/Pml/Personal/PersonalSystem.h"
#include "externals/System/Linq/Enumerable.h"
#include "externals/System/Math.h"
#include "externals/UnityEngine/Sprite.h"

#include "features/field_handlers/field_handlers.h"
#include "features/side_handlers/side_handlers.h"

#include "data/field_effects.h"
#include "data/side_effects.h"
#include "data/types.h"
#include "data/utils.h"
#include "data/weathers.h"

#include "logger/logger.h"

Dpr::Battle::View::UI::BUISituation::Object* getSituationUI(Dpr::Battle::View::Systems::BattleViewUISystem::Object* __this) {
    auto situationTF = __this->cast<UnityEngine::Component>()->get_transform()->Find(System::String::Create("BUISituation"));
    auto situationGO = situationTF->cast<UnityEngine::Component>()->get_gameObject();
    auto situationCmpList = situationGO->GetAllComponents();
    return (Dpr::Battle::View::UI::BUISituation::Object*)situationCmpList->m_Items[2];
}

Dpr::Battle::View::UI::BUISituationDetail::Object* getSituationDetailUI(Dpr::Battle::View::Systems::BattleViewUISystem::Object* __this) {
    auto situationDetailTF = __this->cast<UnityEngine::Component>()->get_transform()->Find(System::String::Create("BUISituationDetail"));
    auto situationDetailGO = situationDetailTF->cast<UnityEngine::Component>()->get_gameObject();
    auto situationDetailCmpList = situationDetailGO->GetAllComponents();
    return (Dpr::Battle::View::UI::BUISituationDetail::Object*)situationDetailCmpList->m_Items[4];
}

Dpr::UI::PokemonSick::Object* getSituationDetailPokemonSickUI(Dpr::Battle::View::Systems::BattleViewUISystem::Object* __this) {
    auto battleCondTF = __this->cast<UnityEngine::Component>()->get_transform()->Find(System::String::Create("BUISituationDetail"))
            ->Find(System::String::Create("MonsInfo"))
            ->Find(System::String::Create("BattleCond"));
    auto battleCondGO = battleCondTF->cast<UnityEngine::Component>()->get_gameObject();
    auto battleCondCmpList = battleCondGO->GetAllComponents();
    return (Dpr::UI::PokemonSick::Object*)battleCondCmpList->m_Items[4];
}

Dpr::UI::TypePanel::Object* getTypePanelFromStatusPanelUI(Dpr::Battle::View::Systems::BattleViewUISystem::Object* __this, const char* child) {
    auto typePanelTF = __this->cast<UnityEngine::Component>()->get_transform()->Find(System::String::Create("BUISituationDetail"))
            ->Find(System::String::Create("MonsInfo"))
            ->Find(System::String::Create("Status"))
            ->Find(System::String::Create(child));
    auto typePanelGO = typePanelTF->cast<UnityEngine::Component>()->get_gameObject();
    auto typePanelCmpList = typePanelGO->GetAllComponents();
    return (Dpr::UI::TypePanel::Object*)typePanelCmpList->m_Items[3];
}

Dpr::UI::TypePanel::Object* getSituationDetailType1UI(Dpr::Battle::View::Systems::BattleViewUISystem::Object* __this) {
    return getTypePanelFromStatusPanelUI(__this, "TypePanel1");
}

Dpr::UI::TypePanel::Object* getSituationDetailType2UI(Dpr::Battle::View::Systems::BattleViewUISystem::Object* __this) {
    return getTypePanelFromStatusPanelUI(__this, "TypePanel2");
}

Dpr::UI::TypePanel::Object* getSituationDetailTypeTeraUI(Dpr::Battle::View::Systems::BattleViewUISystem::Object* __this) {
    return getTypePanelFromStatusPanelUI(__this, "TeraType");
}

void LoadSpriteToImage(UnityEngine::UI::Image::Object* __this, UnityEngine::Sprite::Object* sprite) {
    Logger::log("LoadSpriteToImage!\n");
    system_load_typeinfo(0x6d82);

    __this->set_sprite(sprite);
    auto go = __this->cast<UnityEngine::Component>()->get_gameObject();
    go->SetActive(UnityEngine::_Object::op_Inequality(sprite->cast<UnityEngine::_Object>(), nullptr));
}

void GetUiDisplay_Turn_FieldEffect(Dpr::Battle::View::Systems::BattleViewSystem::Object* __this, uint32_t* numerator, uint32_t* denominator, int32_t effect) {
    *numerator = 0;
    *denominator = 0;

    auto fieldStatus = __this->virtual_GetFieldStatus();
    if (fieldStatus->CheckStatus(effect))
    {
        auto clientID = __this->virtual_GetClientID();
        auto causePokeID = fieldStatus->GetDependPokeID(effect);
        auto wholeTurn = fieldStatus->GetWholeTurn(effect);
        auto passedTurn = fieldStatus->GetPassedTurn(effect);
        if (wholeTurn < 0xFF)
            __this->virtual_GetUiDisplay_Turn(numerator, denominator, clientID, causePokeID, wholeTurn, 0, wholeTurn - passedTurn, passedTurn);
    }
}

void GetUiDisplay_Turn_SideEffect_All(Dpr::Battle::View::Systems::BattleViewSystem::Object* __this, uint32_t* numerator, uint32_t* denominator, int32_t side, int32_t sideEffect) {
    *numerator = 0;
    *denominator = 0;

    auto sideStatus = __this->virtual_GetSideEffectStatus(side, sideEffect);
    if (sideStatus->IsEffective())
    {
        auto clientID = __this->virtual_GetClientID();
        auto causePokeID = sideStatus->GetCausePokeID();
        auto maxTurn = sideStatus->GetMaxTurnCount();
        auto turnUpCount = sideStatus->GetTurnUpCount();
        auto remainingCount = sideStatus->GetRemainingTurn();
        auto currentCount = sideStatus->GetCurrentTurnCount();
        __this->virtual_GetUiDisplay_Turn(numerator, denominator, clientID, causePokeID, maxTurn, turnUpCount, remainingCount, currentCount);
    }
}

void GenerateNameAndDescStrings(Dpr::Battle::View::UI::BUISituationDetail::Object* __this, Dpr::Battle::Logic::BattleStr::Object* btlStr, Dpr::Message::MessageMsgFile::Object* msgFile, System::String::Object* effectIDStr, System::String::Object** nameStr, System::String::Object** descStr) {
    auto nameLabel = System::String::Concat(System::String::Create("BTR_STATE_"), effectIDStr, System::String::Create("_01"));
    Dpr::Message::MessageWordSetHelper::SetStringWord(0, btlStr->GetFormatUIText(nameLabel, msgFile));
    *nameStr = btlStr->GetFormatUIText(System::String::Create("msg_ui_btl_joutai_00"), nullptr);

    auto descLabel = System::String::Concat(System::String::Create("BTR_STATE_"), effectIDStr, System::String::Create("_02"));
    *descStr = btlStr->GetFormatUIText(descLabel, msgFile);
}

void AddElementToSituationDetail(Dpr::Battle::View::UI::BUISituationDetail::Object* __this, Dpr::Battle::Logic::BattleStr::Object* btlStr, System::String::Object* nameStr, System::String::Object* descStr, uint32_t remaining, uint32_t total, System::String::Object* slashStr) {
    auto tuple = System::ValueTuple3$$String$$String$$String::Object {
        .fields = {
            .Item1 = nameStr,
            .Item2 = nullptr,
            .Item3 = descStr,
        }
    };

    if (remaining != 0 && total != 0)
    {
        Dpr::Message::MessageWordSetHelper::SetDigitWord(0, remaining);
        auto remainingStr = btlStr->GetFormatUIText(System::String::Create("msg_ui_btl_joutai_01"), nullptr);

        Dpr::Message::MessageWordSetHelper::SetDigitWord(0, total);
        auto totalStr = btlStr->GetFormatUIText(System::String::Create("msg_ui_btl_joutai_03"), nullptr);

        tuple.fields.Item2 = System::String::Concat(remainingStr, slashStr, totalStr);
    }
    else
    {
        tuple.fields.Item2 = System::String::Create("");
    }

    __this->fields._statusTexts->Add(&tuple);

    auto descButton = UnityEngine::_Object::Instantiate(__this->fields._itemButtonPrefab,
                                                        ((UnityEngine::Transform*)__this->fields._scrollRect->fields.m_Content),
                                                        UnityEngine::_Object::Method$$BUISituationDescriptionButton$$Instantiate_2)->instance();
    __this->fields._itemButtons->Add(descButton);

    auto index = __this->fields._itemButtons->fields._size - 1;
    descButton->fields._index = index;
    descButton->cast<Dpr::Battle::View::UI::BUIButtonBase>()->set_Text(tuple.fields.Item1);
    descButton->fields._contentsText->virtual_set_text(tuple.fields.Item2);
}


HOOK_DEFINE_REPLACE(BUISituation_Initialize) {
    static void Callback(Dpr::Battle::View::UI::BUISituation::Object* __this) {
        Logger::log("BUISituation_Initialize\n");

        system_load_typeinfo(0x1f4e);
        system_load_typeinfo(0x239d);

        Dpr::Battle::View::BattleViewCore::getClass()->initIfNeeded();
        Dpr::Battle::Logic::BattleStr::getClass()->initIfNeeded();

        __this->fields._IsValid_k__BackingField = false;

        auto battleViewCore = Dpr::Battle::View::BattleViewCore::get_Instance();
        auto battleViewSystem = battleViewCore->fields._ViewSystem_k__BackingField;

        //battleViewCore->fields._UISystem_k__BackingField->fields._cursor->SetActive(true);

        // Setup each button
        for (uint64_t i=0; i<__this->fields._situationButtons->max_length; i++)
        {
            Logger::log("[BUISituation_Initialize] btn %d\n", i);
            Dpr::Battle::View::UI::BUISituationButton::Object* __this_01 = __this->fields._situationButtons->m_Items[i];
            auto __this_00 = Dpr::Battle::Logic::BTL_POKEPARAM::newInstance(nullptr);
            Logger::log("[BUISituation_Initialize] btn %08X\n", __this_01);
            Logger::log("[BUISituation_Initialize] param %08X\n", __this_00);

            if (!battleViewSystem->virtual_GetBattlePokeParam_forUI(__this_00, __this_01->fields._index))
            {
                ExtensionMethods::SetActive(__this_01->cast<UnityEngine::Component>(), false);
                continue;
            }

            ExtensionMethods::SetActive(__this_01->cast<UnityEngine::Component>(), true);

            uint8_t clientId = battleViewSystem->virtual_GetBtlvPosToClientId(__this_01->fields._index);

            battleViewSystem->virtual_GetBattleParty(clientId);

            System::String::Object* trainerName = System::String::Create("");
            if (battleViewSystem->virtual_IsClientTrainerExist(clientId))
            {
                auto btlStr = Dpr::Battle::Logic::BattleStr::getClass()->static_fields->s_Instance;
                trainerName = btlStr->GetFormatUITrainerName(System::String::Create("msg_ui_btl_playername"), clientId, 0);
            }

            Logger::log("[BUISituation_Initialize] trainer %08X %s\n", trainerName, trainerName->asCString().c_str());
            __this_01->Initialize(__this_00, trainerName);

            __this->fields._monsParams->set_Item(__this_01->fields._index, __this_00);
            __this->fields._trainerNames->set_Item(__this_01->fields._index, trainerName);
            __this->fields._icons->set_Item(__this_01->fields._index, __this_01->fields._pokeIcon);
        }

        __this->fields._CurrentIndex_k__BackingField = 1;
        auto uiCanvasBase = __this->cast<Dpr::Battle::View::UI::BattleViewUICanvasBase>();
        uiCanvasBase->SelectButton(__this->fields._situationButtons, 1, false, Dpr::Battle::View::UI::BattleViewUICanvasBase::Method$$BUISituationButton$$SelectButton);

        Logger::log("Base Initialize done!\n");
    }
};

HOOK_DEFINE_REPLACE(BUISituation_OnShow) {
    static void Callback(Dpr::Battle::View::UI::BUISituation::Object* __this) {
        Logger::log("BUISituation_OnShow\n");

        system_load_typeinfo(0x1f50);

        Dpr::Battle::View::BattleViewCore::getClass()->initIfNeeded();

        __this->fields._IsFocus_k__BackingField = true;
        __this->fields._IsShow_k__BackingField = true;
        __this->fields._animationState_k__BackingField = 2;

        auto uiCanvasBase = __this->cast<Dpr::Battle::View::UI::BattleViewUICanvasBase>();
        uiCanvasBase->SelectButton(__this->fields._situationButtons, __this->fields._CurrentIndex_k__BackingField, false, Dpr::Battle::View::UI::BattleViewUICanvasBase::Method$$BUISituationButton$$SelectButton);

        auto battleViewCore = Dpr::Battle::View::BattleViewCore::get_Instance();

        battleViewCore->fields._UISystem_k__BackingField->fields._cursor->SetActive(true);
    }
};

HOOK_DEFINE_REPLACE(BUISituation_OnSubmit) {
    static void Callback(Dpr::Battle::View::UI::BUISituation::Object* __this) {
        Logger::log("BUISituation_OnSubmit\n");

        system_load_typeinfo(0x1f51);

        Dpr::Battle::View::BattleViewCore::getClass()->initIfNeeded();

        auto battleViewUISystem = Dpr::Battle::View::BattleViewCore::get_Instance()->fields._UISystem_k__BackingField;

        __this->fields._IsValid_k__BackingField = true;
        __this->fields._IsFocus_k__BackingField = false;

        auto mons = __this->fields._monsParams->get_Values<System::Collections::Generic::IEnumerable$$BTL_POKEPARAM>();
        auto monsList = System::Linq::Enumerable::ToList<System::Collections::Generic::IEnumerable$$BTL_POKEPARAM, System::Collections::Generic::List$$BTL_POKEPARAM, Dpr::Battle::Logic::BTL_POKEPARAM>(mons->instance());

        auto trainerNames = __this->fields._trainerNames->get_Values<System::Collections::Generic::IEnumerable$$String>();
        auto trainerNamesList = System::Linq::Enumerable::ToList<System::Collections::Generic::IEnumerable$$String, System::Collections::Generic::List$$String, System::String>(trainerNames->instance());

        auto icons = __this->fields._icons->get_Values<System::Collections::Generic::IEnumerable$$UnityEngine_UI_Image>();
        auto iconsList = System::Linq::Enumerable::ToList<System::Collections::Generic::IEnumerable$$UnityEngine_UI_Image, System::Collections::Generic::List$$UnityEngine_UI_Image, UnityEngine::UI::Image>(icons->instance());

        Dpr::Battle::Logic::BTL_POKEPARAM::Object* currentMon = __this->fields._monsParams->get_Item(__this->fields._CurrentIndex_k__BackingField)->instance();
        int32_t index = monsList->IndexOf(currentMon);

        __this->cast<Dpr::Battle::View::UI::BattleViewUICanvasBase>()->Hide(false, nullptr);
        auto situationDetailUI = getSituationDetailUI(battleViewUISystem);
        situationDetailUI->Initialize(monsList->instance(), trainerNamesList->instance(), iconsList->instance(), index);
        situationDetailUI->cast<Dpr::Battle::View::UI::BattleViewUICanvasBase>()->Show(nullptr);

        battleViewUISystem->PlaySe(0x132562f0);
    }
};

HOOK_DEFINE_TRAMPOLINE(BUISituation_OnCancel) {
    static void Callback(Dpr::Battle::View::UI::BUISituationButton::Object* __this) {
        Logger::log("BUISituation_OnCancel with %08X\n", __this);
        Orig(__this);
        Logger::log("BUISituation_OnCancel Orig done\n");

        Dpr::Battle::View::BattleViewCore::getClass()->initIfNeeded();

        auto battleViewUISystem = Dpr::Battle::View::BattleViewCore::get_Instance()->fields._UISystem_k__BackingField;
        battleViewUISystem->fields._cursor->SetActive(false);
    }
};

HOOK_DEFINE_TRAMPOLINE(BUISituationButton_Initialize) {
    static void Callback(Dpr::Battle::View::UI::BUISituationButton::Object* __this, Dpr::Battle::Logic::BTL_POKEPARAM::Object* btlParam, System::String::Object* trainerName) {
        Logger::log("BUISituationButton_Initialize with %08X\n", __this);
        Logger::log("BUISituationButton_Initialize with %08X\n", btlParam);
        Logger::log("BUISituationButton_Initialize with %08X, %s\n", trainerName, trainerName->asCString().c_str());
        Orig(__this, btlParam, trainerName);
        Logger::log("BUISituationButton_Initialize Orig done\n");

        system_load_typeinfo(0x1f63);
        system_load_typeinfo(0x239d);
        system_load_typeinfo(0x6d80);

        auto coreParam = btlParam->GetSrcData()->cast<Pml::PokePara::CoreParam>();
        MethodInfo* mi = Dpr::UI::PokemonIcon::getMethod$$BUISituationLoadIcon((Il2CppMethodPointer) &LoadSpriteToImage);
        auto onLoad = UnityEngine::Events::UnityAction::getClass(UnityEngine::Events::UnityAction::Sprite_TypeInfo)->newInstance(__this->fields._pokeIcon, mi);
        auto uiManager = Dpr::UI::UIManager::get_Instance();

        uiManager->LoadSpritePokemon(coreParam->GetMonsNo(), coreParam->GetFormNo(), coreParam->GetSex(), coreParam->GetRareType(), coreParam->IsEgg(Pml::PokePara::EggCheckType::BOTH_EGG), onLoad);
        Logger::log("BUISituationButton_Initialize END\n");
    }
};

HOOK_DEFINE_INLINE(BattleViewUISystem_OnUpdate) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto viewUISys = (Dpr::Battle::View::Systems::BattleViewUISystem::Object*)ctx->X[19];

        // Get from float register s0
        float deltaTime;
        __asm(
                R"(
                mov %0.16B, v8.16B
            )"
                : "=w"(deltaTime)
                );

        // Add BUISituation OnUpdate
        auto situationUI = getSituationUI(viewUISys);
        situationUI->virtual_OnUpdate(deltaTime);

        // Add BUISituationDetail OnUpdate
        auto situationDetailUI = getSituationDetailUI(viewUISys);
        situationDetailUI->virtual_OnUpdate(deltaTime);

        // Overwrite
        ctx->X[8] = (uint64_t)viewUISys->fields._autoPilot;
    }
};

HOOK_DEFINE_INLINE(BUISituationDetail_ctor_fieldIDs) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        Logger::log("BUISituationDetail_ctor_fieldIDs\n");

        auto fieldIds = (System::Collections::Generic::Dictionary$$EffectType$$String::Object*)ctx->X[0];
        fieldIds->ctor();

        fieldIds->Add(array_index(FIELD_EFFECTS, "Trick Room"), System::String::Create("TrickRoom"));
        fieldIds->Add(array_index(FIELD_EFFECTS, "Gravity"), System::String::Create("Gravity"));
        fieldIds->Add(array_index(FIELD_EFFECTS, "Imprison"), System::String::Create("Imprison"));
        fieldIds->Add(array_index(FIELD_EFFECTS, "Wonder Room"), System::String::Create("WonderRoom"));
        fieldIds->Add(array_index(FIELD_EFFECTS, "Magic Room"), System::String::Create("MagicRoom"));
        fieldIds->Add(array_index(FIELD_EFFECTS, "Ion Deluge"), System::String::Create("IonDeluge"));
        fieldIds->Add(array_index(FIELD_EFFECTS, "Fairy Lock"), System::String::Create("FairyLock"));
        fieldIds->Add(array_index(FIELD_EFFECTS, "Neutralizing Gas"), System::String::Create("NeutralizingGas"));

        AddSituationDetailFieldEffectLabels(fieldIds);

        ctx->X[21] = (uint64_t)fieldIds;
    }
};

HOOK_DEFINE_INLINE(BUISituationDetail_ctor_weatherIDs) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        Logger::log("BUISituationDetail_ctor_weatherIDs\n");

        auto weatherIDs = (System::Collections::Generic::Dictionary$$BtlWeather$$String::Object*)ctx->X[0];
        weatherIDs->ctor();

        weatherIDs->Add(array_index(BATTLE_WEATHERS, "Harsh Sunlight"), System::String::Create("HarshSunlight"));
        weatherIDs->Add(array_index(BATTLE_WEATHERS, "Rain"), System::String::Create("Rain"));
        weatherIDs->Add(array_index(BATTLE_WEATHERS, "Hail"), System::String::Create("Hail"));
        weatherIDs->Add(array_index(BATTLE_WEATHERS, "Sandstorm"), System::String::Create("Sandstorm"));
        weatherIDs->Add(array_index(BATTLE_WEATHERS, "Heavy Rain"), System::String::Create("HeavyRain"));
        weatherIDs->Add(array_index(BATTLE_WEATHERS, "Extremely Harsh Sunlight"), System::String::Create("ExtremelyHarshSunlight"));
        weatherIDs->Add(array_index(BATTLE_WEATHERS, "Strong Winds"), System::String::Create("StrongWinds"));

        AddSituationDetailWeatherFieldEffectLabels(weatherIDs);

        ctx->X[21] = (uint64_t)weatherIDs;
    }
};

HOOK_DEFINE_INLINE(BUISituationDetail_ctor_groundIDs) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        Logger::log("BUISituationDetail_ctor_groundIDs\n");

        auto groundIDs = (System::Collections::Generic::Dictionary$$BtlGround$$String::Object*)ctx->X[0];
        groundIDs->ctor();

        groundIDs->Add((uint8_t)Dpr::Battle::Logic::BtlGround::BTL_GROUND_GRASS, System::String::Create("GrassyTerrain"));
        groundIDs->Add((uint8_t)Dpr::Battle::Logic::BtlGround::BTL_GROUND_MIST, System::String::Create("MistyTerrain"));
        groundIDs->Add((uint8_t)Dpr::Battle::Logic::BtlGround::BTL_GROUND_ELEKI, System::String::Create("ElectricTerrain"));
        groundIDs->Add((uint8_t)Dpr::Battle::Logic::BtlGround::BTL_GROUND_PHYCHO, System::String::Create("PsychicTerrain"));

        ctx->X[21] = (uint64_t)groundIDs;
    }
};

HOOK_DEFINE_INLINE(BUISituationDetail_ctor_sideIDs) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        Logger::log("BUISituationDetail_ctor_sideIDs\n");

        auto sideIDs = (System::Collections::Generic::Dictionary$$BtlSideEffect$$String::Object*)ctx->X[0];
        sideIDs->ctor();

        sideIDs->Add(array_index(SIDE_EFFECTS, "Reflect"), System::String::Create("Reflect"));
        sideIDs->Add(array_index(SIDE_EFFECTS, "Light Screen"), System::String::Create("LightScreen"));
        sideIDs->Add(array_index(SIDE_EFFECTS, "Safeguard"), System::String::Create("Safeguard"));
        sideIDs->Add(array_index(SIDE_EFFECTS, "Mist"), System::String::Create("Mist"));
        sideIDs->Add(array_index(SIDE_EFFECTS, "Tailwind"), System::String::Create("Tailwind"));
        sideIDs->Add(array_index(SIDE_EFFECTS, "Lucky Chant"), System::String::Create("LuckyChant"));
        sideIDs->Add(array_index(SIDE_EFFECTS, "Spikes"), System::String::Create("Spikes"));
        sideIDs->Add(array_index(SIDE_EFFECTS, "Toxic Spikes"), System::String::Create("ToxicSpikes"));
        sideIDs->Add(array_index(SIDE_EFFECTS, "Stealth Rock"), System::String::Create("StealthRock"));
        sideIDs->Add(array_index(SIDE_EFFECTS, "Wide Guard"), System::String::Create("WideGuard"));
        sideIDs->Add(array_index(SIDE_EFFECTS, "Quick Guard"), System::String::Create("QuickGuard"));
        sideIDs->Add(array_index(SIDE_EFFECTS, "Rainbow"), System::String::Create("Rainbow"));
        sideIDs->Add(array_index(SIDE_EFFECTS, "Sea of Fire"), System::String::Create("SeaOfFire"));
        sideIDs->Add(array_index(SIDE_EFFECTS, "Swamp"), System::String::Create("Swamp"));
        sideIDs->Add(array_index(SIDE_EFFECTS, "Sticky Web"), System::String::Create("StickyWeb"));
        sideIDs->Add(array_index(SIDE_EFFECTS, "Mat Block"), System::String::Create("MatBlock"));
        sideIDs->Add(array_index(SIDE_EFFECTS, "Crafty Shield"), System::String::Create("CraftyShield"));
        sideIDs->Add(array_index(SIDE_EFFECTS, "Aurora Veil"), System::String::Create("AuroraVeil"));
        sideIDs->Add(array_index(SIDE_EFFECTS, "Spotlight"), System::String::Create("Spotlight"));
        sideIDs->Add(array_index(SIDE_EFFECTS, "G-Max Steelsurge"), System::String::Create("GSteelsurge"));
        sideIDs->Add(array_index(SIDE_EFFECTS, "G-Max Wildfire"), System::String::Create("GWildfire"));
        sideIDs->Add(array_index(SIDE_EFFECTS, "G-Max Volcalith"), System::String::Create("GVolcalith"));

        AddSituationDetailSideEffectLabels(sideIDs);

        ctx->X[21] = (uint64_t)sideIDs;
    }
};

HOOK_DEFINE_REPLACE(BUISituationDetail_Initialize) {
    static void Callback(Dpr::Battle::View::UI::BUISituationDetail::Object* __this) {
        Logger::log("BUISituationDetail_Initialize\n");

        system_load_typeinfo(0x1f42);
        system_load_typeinfo(0x239d);

        Dpr::Battle::View::BattleViewCore::getClass()->initIfNeeded();
        Dpr::Message::MessageWordSetHelper::getClass()->initIfNeeded();
        Dpr::Battle::Logic::BattleStr::getClass()->initIfNeeded();
        Pml::Personal::PersonalSystem::getClass()->initIfNeeded();
        System::Math::getClass()->initIfNeeded();

        auto battleViewCore = Dpr::Battle::View::BattleViewCore::get_Instance();
        auto battleViewSystem = battleViewCore->fields._ViewSystem_k__BackingField;
        auto battleViewSystemUI = battleViewCore->fields._UISystem_k__BackingField;

        __this->fields._CurrentIndex_k__BackingField = 0;

        auto btlPokeParam = __this->fields._pokeList->fields._items->m_Items[__this->fields._pokeIndex];
        auto coreParam = btlPokeParam->GetSrcData()->cast<Pml::PokePara::CoreParam>();

        bool isPlayerOwned = true;
        if ((4 < __this->fields._pokeIndex) || ((1 << (ulong)(__this->fields._pokeIndex & 0x1f) & 0x15U) == 0)) {
            isPlayerOwned = __this->fields._pokeIndex == 6;
        }

        // Pokémon Sprite
        __this->fields._pokeImage->set_sprite(__this->fields._icons->fields._items->m_Items[__this->fields._pokeIndex]->fields.m_Sprite);

        // Level Text
        auto lvlText = Dpr::Battle::Logic::BattleStr::getClass()->static_fields->s_Instance->GetFormatUIText(System::String::Create("msg_ui_raidbtl_lv_00"), nullptr);
        __this->fields._lvText->virtual_set_text(lvlText);
        Dpr::Message::MessageWordSetHelper::SetDigitWord(0, coreParam->GetLevel());

        // Level Value
        auto lvlValue = Dpr::Battle::Logic::BattleStr::getClass()->static_fields->s_Instance->GetFormatUIText(System::String::Create("msg_ui_raidbtl_lv_01"), nullptr);
        __this->fields._lvValue->virtual_set_text(lvlValue);

        // Sex
        battleViewSystem->SetSexIcon(coreParam->GetSex(), __this->fields._genderImage);

        // Pokémon Name
        auto pokeName = Dpr::Battle::Logic::BattleStr::getClass()->static_fields->s_Instance->GetFormatUIPokeName(System::String::Create("msg_ui_btl_pokename"), btlPokeParam->GetID(), false, false);
        __this->fields._nameText->virtual_set_text(pokeName);

        // EXP Slider
        if (isPlayerOwned) {
            int32_t monsno = coreParam->GetMonsNo();
            uint16_t formno = coreParam->GetFormNo();

            Pml::Personal::PersonalSystem::LoadGrowTable(monsno, formno);

            uint8_t nextLevel = System::Math::Min(coreParam->GetLevel() + 1, 100);
            uint32_t currentLevelMin = Pml::Personal::PersonalSystem::GetMinExp(coreParam->GetLevel());
            uint32_t nextLevelMin = Pml::Personal::PersonalSystem::GetMinExp(nextLevel);

            auto sliderTf = __this->fields._expSlider->cast<UnityEngine::Component>()->get_transform()->instance();
            ExtensionMethods::SetActive(sliderTf->GetParent()->cast<UnityEngine::Component>(), true);
            __this->fields._expSlider->set_minValue(0.0f);
            __this->fields._expSlider->set_maxValue((float)(nextLevelMin - currentLevelMin));
            __this->fields._expSlider->set_value((float)(coreParam->GetExp() - currentLevelMin));
        } else {
            auto sliderTf = __this->fields._expSlider->cast<UnityEngine::Component>()->get_transform()->instance();
            ExtensionMethods::SetActive(sliderTf->GetParent()->cast<UnityEngine::Component>(), false);
        }

        // HP Text
        int32_t currentHP = btlPokeParam->GetValue(Dpr::Battle::Logic::BTL_POKEPARAM::ValueID::BPP_HP);
        int32_t maxHP = btlPokeParam->GetValue(Dpr::Battle::Logic::BTL_POKEPARAM::ValueID::BPP_MAX_HP);
        Dpr::Message::MessageWordSetHelper::SetDigitWord(0, currentHP);
        Dpr::Message::MessageWordSetHelper::SetDigitWord(1, maxHP);
        auto hpText = Dpr::Battle::Logic::BattleStr::getClass()->static_fields->s_Instance->GetFormatUIText(System::String::Create("msg_ui_raidbtl_hp_00"), nullptr);
        __this->fields._hpText->virtual_set_text(hpText);

        // Status Condition
        auto pokemonSick = getSituationDetailPokemonSickUI(battleViewSystemUI);
        pokemonSick->Setup(btlPokeParam);

        // Types
        uint8_t typing[2] = { TYPE_COUNT, TYPE_COUNT };
        battleViewSystem->virtual_GetUiDisplay_PokeType(&typing[0], &typing[1], &btlPokeParam);

        auto type1Panel = getSituationDetailType1UI(battleViewSystemUI);
        auto type2Panel = getSituationDetailType2UI(battleViewSystemUI);
        //auto teraTypePanel = getSituationDetailTypeTeraUI(battleViewSystemUI);

        if (typing[0] != TYPE_COUNT) type1Panel->Set(typing[0]);
        else type1Panel->Deactive();

        if (typing[1] != TYPE_COUNT && typing[0] != typing[1]) type2Panel->Set(typing[1]);
        else type2Panel->Deactive();

        // TODO: Add tera type eventually
        //if (typing[2] != TYPE_COUNT) teraTypePanel->Set(typing[2]);
        //else teraTypePanel->Deactive();

        // Stat changes
        for (uint64_t i=0; i<__this->fields._params->max_length && i<__this->fields._paramIDs->max_length; i++) {
            auto tuple = __this->fields._paramIDs->m_Items[i];
            auto paramText = Dpr::Battle::Logic::BattleStr::getClass()->static_fields->s_Instance->GetFormatUIText(tuple.fields.Item1, nullptr);
            int32_t paramValue = btlPokeParam->GetValue((Dpr::Battle::Logic::BTL_POKEPARAM::ValueID)tuple.fields.Item2);
            __this->fields._params->m_Items[i]->Initialize(paramText, paramValue);
        }

        // Trainer name
        if (__this->fields._trainerNames != nullptr) {
            auto trainerName = __this->fields._trainerNames->fields._items->m_Items[__this->fields._pokeIndex];
            if (System::String::IsNullOrEmpty(trainerName)) {
                __this->fields._trainerPanel->SetActive(false);
            }
            else {
                __this->fields._trainerPanel->SetActive(true);
                __this->fields._trainerName->virtual_set_text(trainerName);
            }
        } else {
            __this->fields._trainerPanel->SetActive(false);
        }

        __this->fields._itemButtons->RemoveRange(2, __this->fields._itemButtons->fields._size - 2);
        __this->fields._statusTexts->Clear();

        __this->SetPokeStatus(btlPokeParam, 0);
        __this->SetPokeStatus(btlPokeParam, 1);
        uint64_t fieldStatusCount = __this->SetFieldStatus();

        // Scroll List
        if (fieldStatusCount > __this->fields._blankItems->max_length) {
            auto itemBtnTf = __this->fields._itemButtons->fields._items->m_Items[2]->cast<Dpr::Battle::View::UI::BUIButtonBase>()->get_rectTransform();
            auto itemBtnSizeDelta = itemBtnTf->get_sizeDelta();
            float spacing = __this->fields._verticalLayout->fields.m_ChildScaleWidth;
            int32_t btnCount = __this->fields._itemButtons->fields._size;
            auto contentSizeDelta = __this->fields._scrollRect->fields.m_Content->get_sizeDelta();
            auto padding = __this->fields._verticalLayout->fields.m_Padding;

            UnityEngine::Vector2::Object newContentSizeDelta = (UnityEngine::Vector2::Object) {
                    .fields = {
                            .x = contentSizeDelta.fields.x,
                            .y = (itemBtnSizeDelta.fields.y + spacing) * (float)(btnCount - 2) + (float)(padding->get_top() + padding->get_bottom()),
                    }
            };
            __this->fields._scrollRect->fields.m_Content->set_sizeDelta(newContentSizeDelta);
        } else {
            __this->fields._scrollRect->cast<UnityEngine::Behaviour>()->set_enabled(false);
        }

        for (uint64_t i=0; i<__this->fields._blankItems->max_length; i++)
            __this->fields._blankItems->m_Items[i]->cast<UnityEngine::Behaviour>()->set_enabled(i >= fieldStatusCount);

        __this->SelectButton(false);
        auto cursorSizeDelta = __this->fields._itemButtonPrefab->cast<Dpr::Battle::View::UI::BUIButtonBase>()->get_rectTransform()->get_sizeDelta();
        __this->fields._cursorBase->set_sizeDelta(cursorSizeDelta);
    }
};

HOOK_DEFINE_REPLACE(BUISituationDetail_OnCancel) {
    static void Callback(Dpr::Battle::View::UI::BUISituationDetail::Object* __this) {
        Logger::log("BUISituationDetail_OnCancel\n");

        system_load_typeinfo(0x1f43);

        __this->fields._IsFocus_k__BackingField = false;

        __this->cast<Dpr::Battle::View::UI::BattleViewUICanvasBase>()->PlayTransitionAnimation(false);

        Dpr::Battle::View::BattleViewCore::getClass()->initIfNeeded();
        auto battleViewCore = Dpr::Battle::View::BattleViewCore::get_Instance();
        auto battleViewUISystem = battleViewCore->fields._UISystem_k__BackingField;

        battleViewUISystem->fields._cursor->SetActive(false);

        if (UnityEngine::_Object::op_Equality(battleViewUISystem->cast<UnityEngine::_Object>(), nullptr))
            return;

        __this->cast<Dpr::Battle::View::UI::BattleViewUICanvasBase>()->Hide(false, nullptr);
        battleViewUISystem->fields._actionList->cast<Dpr::Battle::View::UI::BattleViewUICanvasBase>()->Show(nullptr);
        battleViewUISystem->SwitchShowDecoImage(true);

        Logger::log("[BUISituationDetail$$OnCancel] Show waza list\n");
        battleViewUISystem->fields._wazaList->cast<Dpr::Battle::View::UI::BattleViewUICanvasBase>()->Show(nullptr);

        battleViewUISystem->PlaySe(0xa4eb827e);
    }
};

HOOK_DEFINE_TRAMPOLINE(BUISituation_OnUpdate) {
    static void Callback(Dpr::Battle::View::UI::BUISituation::Object *__this, float deltaTime) {
        Logger::log("BUISituation_OnUpdate\n");
        Orig(__this, deltaTime);
    }
};

HOOK_DEFINE_REPLACE(BUISituationDetail_OnUpdate) {
    static void Callback(Dpr::Battle::View::UI::BUISituationDetail::Object* __this, float deltaTime) {
        system_load_typeinfo(0x1f45);

        if (!__this->fields._IsFocus_k__BackingField)
            return;

        if (Dpr::NetworkUtils::NetworkManager::IsShowApplicationErrorDialog())
            return;

        if (Dpr::Battle::View::BtlvInput::GetPush(Dpr::UI::UIManager::getClass()->static_fields->StickLDown, true) ||
            Dpr::Battle::View::BtlvInput::GetRepeat(Dpr::UI::UIManager::getClass()->static_fields->StickLDown, true)) {
            __this->virtual_PreparaNext(true);
        }
        else if (Dpr::Battle::View::BtlvInput::GetPush(Dpr::UI::UIManager::getClass()->static_fields->StickLUp, true) ||
            Dpr::Battle::View::BtlvInput::GetRepeat(Dpr::UI::UIManager::getClass()->static_fields->StickLUp, true)) {
            __this->virtual_PreparaNext(false);
        }
        else if (Dpr::Battle::View::BtlvInput::GetPush(Dpr::UI::UIManager::getClass()->static_fields->StickLRight, true)) {
            __this->OnChangePoke(true);
        }
        else if (Dpr::Battle::View::BtlvInput::GetPush(Dpr::UI::UIManager::getClass()->static_fields->StickLLeft, true)) {
            __this->OnChangePoke(false);
        }

        if (Dpr::Battle::View::BtlvInput::GetPush(Dpr::UI::UIManager::getClass()->static_fields->ButtonB, true)) {
            __this->OnCancel();
        }
    }
};

HOOK_DEFINE_REPLACE(BUISituationDetail_SetFieldStatus) {
    static int32_t Callback(Dpr::Battle::View::UI::BUISituationDetail::Object* __this) {
        system_load_typeinfo(0x1f40);
        system_load_typeinfo(0x1f49);
        system_load_typeinfo(0x239d);

        Dpr::Battle::View::BattleViewCore::getClass()->initIfNeeded();
        Dpr::Message::MessageManager::getClass()->initIfNeeded();
        Dpr::Battle::Logic::BattleStr::getClass()->initIfNeeded();
        Dpr::Message::MessageWordSetHelper::getClass()->initIfNeeded();

        auto battleViewCore = Dpr::Battle::View::BattleViewCore::get_Instance();
        auto battleViewSystem = battleViewCore->fields._ViewSystem_k__BackingField;
        auto msgManager = Dpr::Message::MessageManager::get_Instance();
        auto btlStr = Dpr::Battle::Logic::BattleStr::getClass()->static_fields->s_Instance;

        auto btlPokeParam = __this->fields._pokeList->fields._items->m_Items[__this->fields._pokeIndex];
        auto coreParam = btlPokeParam->GetSrcData()->cast<Pml::PokePara::CoreParam>();

        auto btlStateMsgFile = msgManager->GetMsgFile(System::String::Create("ss_btl_state"));
        auto slashStr = btlStr->GetFormatUIText(System::String::Create("msg_ui_btl_joutai_02"), nullptr);

        int32_t totalCount = 0;

        // Field Effects
        for (int32_t i=0; i<FIELD_EFFECT_COUNT; i++)
        {
            System::String::Object* nameStr = nullptr;
            System::String::Object* descStr = nullptr;
            uint32_t remaining;
            uint32_t total;

            switch (i)
            {
                case array_index(FIELD_EFFECTS, "Weather"):
                {
                    auto fieldStatus = battleViewSystem->virtual_GetFieldStatus();
                    auto weather = fieldStatus->GetWeather();
                    if (weather != array_index(BATTLE_WEATHERS, "-BATTLE WEATHER ZERO-") && __this->fields._weatherIDs->ContainsKey(weather))
                    {
                        GenerateNameAndDescStrings(__this, btlStr, btlStateMsgFile, __this->fields._weatherIDs->get_Item(weather)->instance(), &nameStr, &descStr);
                        battleViewSystem->virtual_GetUiDisplay_Turn_Weather(&remaining, &total);
                    }
                }
                break;

                case array_index(FIELD_EFFECTS, "Terrain"):
                {
                    auto fieldStatus = battleViewSystem->virtual_GetFieldStatus();
                    auto terrain = fieldStatus->GetGround();

                    if (terrain != Dpr::Battle::Logic::BtlGround::BTL_GROUND_NONE && __this->fields._groundIDs->ContainsKey((uint8_t)terrain))
                    {
                        GenerateNameAndDescStrings(__this, btlStr, btlStateMsgFile, __this->fields._groundIDs->get_Item((uint8_t)terrain)->instance(), &nameStr, &descStr);
                        battleViewSystem->virtual_GetUiDisplay_Turn_Ground(&remaining, &total);
                    }
                }
                break;

                default:
                {
                    auto fieldStatus = battleViewSystem->virtual_GetFieldStatus();

                    if (fieldStatus->CheckStatus(i) && __this->fields._fieldIDs->ContainsKey(i))
                    {
                        GenerateNameAndDescStrings(__this, btlStr, btlStateMsgFile, __this->fields._fieldIDs->get_Item(i)->instance(), &nameStr, &descStr);
                        GetUiDisplay_Turn_FieldEffect(battleViewSystem, &remaining, &total, i);
                    }
                }
                break;
            }

            if (nameStr != nullptr)
            {
                AddElementToSituationDetail(__this, btlStr, nameStr, descStr, remaining, total, slashStr);
                totalCount++;
            }
        }

        // Side Effects
        // TODO: Currently Dictionary MethodInfos don't work :(
        /*for (int32_t i=0; i<SIDE_EFFECT_COUNT; i++)
        {
            System::String::Object* nameStr = nullptr;
            System::String::Object* descStr = nullptr;
            uint32_t remaining;
            uint32_t total;

            auto mainModule = battleViewSystem->virtual_GetMainModule()->instance();
            auto side = mainModule->PokeIDtoSide(btlPokeParam->GetID());
            auto sideStatus = battleViewSystem->virtual_GetSideEffectStatus(side, i);

            if (sideStatus->IsEffective() && __this->fields._sideIDs->ContainsKey(i))
            {
                GenerateNameAndDescStrings(__this, btlStr, btlStateMsgFile, __this->fields._sideIDs->get_Item(i)->instance(), &nameStr, &descStr);

                switch (i)
                {
                    case array_index(SIDE_EFFECTS, "Reflect"):
                        battleViewSystem->virtual_GetUiDisplay_Turn_Reflector(&remaining, &total, side);
                        break;

                    case array_index(SIDE_EFFECTS, "Light Screen"):
                        battleViewSystem->virtual_GetUiDisplay_Turn_Hikarinokabe(&remaining, &total, side);
                        break;

                    case array_index(SIDE_EFFECTS, "Aurora Veil"):
                        battleViewSystem->virtual_GetUiDisplay_Turn_AuroraVeil(&remaining, &total, side);
                        break;

                    default:
                        GetUiDisplay_Turn_SideEffect_All(battleViewSystem, &remaining, &total, side, i);
                        break;
                }
            }

            if (nameStr != nullptr)
            {
                AddElementToSituationDetail(__this, btlStr, nameStr, descStr, remaining, total, slashStr);
                totalCount++;
            }
        }*/

        return totalCount;
    }
};


void exl_battle_situation_main() {
    // Add more to OnUpdate
    BattleViewUISystem_OnUpdate::InstallAtOffset(0x01e79c04);

    // Rewrite methods
    BUISituation_Initialize::InstallAtOffset(0x01d22cb0);
    BUISituation_OnShow::InstallAtOffset(0x01d23a40);
    BUISituation_OnSubmit::InstallAtOffset(0x01d23850);
    BUISituation_OnCancel::InstallAtOffset(0x01d23980);

    //BUISituation_OnUpdate::InstallAtOffset(0x01d23280);

    BUISituationButton_Initialize::InstallAtOffset(0x01d22fb0);

    BUISituationDetail_ctor_fieldIDs::InstallAtOffset(0x01d26780);
    BUISituationDetail_ctor_weatherIDs::InstallAtOffset(0x01d26868);
    BUISituationDetail_ctor_groundIDs::InstallAtOffset(0x01d26930);
    BUISituationDetail_ctor_sideIDs::InstallAtOffset(0x01d26988);

    BUISituationDetail_Initialize::InstallAtOffset(0x01d23f70);
    BUISituationDetail_OnCancel::InstallAtOffset(0x01d26080);
    BUISituationDetail_OnUpdate::InstallAtOffset(0x01d25d00);
    BUISituationDetail_SetFieldStatus::InstallAtOffset(0x01d24f70);

    // Assembly Patches
    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p(0);
    auto inst = nn::vector<exl::patch::Instruction> {
        { 0x01d26784, Branch(0xb4) }, // BUISituationDetail_ctor_fieldIDs skip vanilla assignments
        { 0x01d2686c, Branch(0x94) }, // BUISituationDetail_ctor_weatherIDs skip vanilla assignments
        { 0x01d26934, Branch(0x24) }, // BUISituationDetail_ctor_groundIDs skip vanilla assignments
        { 0x01d2698c, Branch(0x190) }, // BUISituationDetail_ctor_sideIDs skip vanilla assignments
    };
    p.WriteInst(inst);
}