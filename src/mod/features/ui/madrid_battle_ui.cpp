#include "exlaunch.hpp"

#include "data/features.h"
#include "data/gimmicks.h"
#include "data/utils.h"

#include "externals/Audio/AudioManager.h"
#include "externals/Dpr/Battle/Logic/BattleStr.h"
#include "externals/Dpr/Battle/View/BattleViewCore.h"
#include "externals/Dpr/Battle/View/BTLV_WAZA_INFO.h"
#include "externals/Dpr/Battle/View/BtlvInput.h"
#include "externals/Dpr/Battle/View/UI/BattleAffinityInfo.h"
#include "externals/Dpr/Battle/View/UI/BUIActionList.h"
#include "externals/Dpr/Battle/View/UI/BUISituation.h"
#include "externals/Dpr/Battle/View/UI/BUIWazaDescription.h"
#include "externals/Dpr/Battle/View/UI/BUIWazaList.h"
#include "externals/Dpr/Message/MessageWordSetHelper.h"
#include "externals/Dpr/NetworkUtils/NetworkManager.h"
#include "externals/Dpr/UI/CharacterModelView.h"
#include "externals/Dpr/UI/UIManager.h"
#include "externals/FlagWork.h"
#include "externals/GameController.h"
#include "externals/Pml/WazaData/WazaDataSystem.h"
#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"
#include "externals/System/Nullable.h"
#include "externals/UnityEngine/Debug.h"
#include "externals/UnityEngine/UI/LayoutRebuilder.h"

#include "features/activated_features.h"
#include "features/battle/battle.h"

#include "logger/logger.h"

const uint32_t AK_EVENTS_UI_COMMON_BEEP = 0x707237c4;
const uint32_t AK_EVENTS_UI_COMMON_CANCEL = 0xa4eb827e;
const uint32_t AK_EVENTS_UI_COMMON_MENU_OPEN = 0x132562f0;
const uint32_t AK_EVENTS_UI_COMMON_SELECT = 0xb7533038;
const uint32_t AK_EVENTS_UI_COMMON_SLIDE = 0x19377fd7;

const UnityEngine::Color::Fields SELECTED_MOVE_BACKGROUND_COLOR = { .r = 1, .g = 1, .b = 1, .a = 1 };
const UnityEngine::Color::Fields NOT_SELECTED_MOVE_BACKGROUND_COLOR = { .r = 0.4, .g = 0.4, .b = 0.4, .a = 1 };
const UnityEngine::Color::Fields SELECTED_MOVE_TEXT_COLOR = { .r = 0, .g = 0, .b = 0, .a = 1 };
const UnityEngine::Color::Fields NOT_SELECTED_MOVE_TEXT_COLOR = { .r = 1, .g = 1, .b = 1, .a = 1 };

static int32_t selectedGimmick = array_index(GIMMICKS, "None");

UnityEngine::Transform::Object* GetWazaButtonBackgroundRoot(Dpr::Battle::View::UI::BUIWazaButton::Object* btn) {
    return ((UnityEngine::Component::Object*)btn)->get_transform()
            ->Find(System::String::Create("background"));
}

UnityEngine::Transform::Object* GetWazaButtonArrowRoot(Dpr::Battle::View::UI::BUIWazaButton::Object* btn) {
    return GetWazaButtonBackgroundRoot(btn)->Find(System::String::Create("DirectionArrow"));
}

void SubmitActionButton(Dpr::Battle::View::UI::BUIActionList::Object* actionList, int32_t index) {
    if (actionList->fields._IsFocus_k__BackingField && !actionList->fields.isButtonAction) {
        actionList->fields.isButtonAction = true;
        actionList->fields._CurrentIndex_k__BackingField = index;
        auto currentButton = actionList->fields._actionButtons->fields._items->m_Items[index];
        actionList->fields._IsValid_k__BackingField = currentButton->Submit();
    }
}

void SelectWazaButton(Dpr::Battle::View::UI::BUIWazaList::Object* wazaList, int32_t index, bool playSe) {
    if (!wazaList->fields._IsValid_k__BackingField) {
        playSe = playSe && wazaList->fields._CurrentIndex_k__BackingField != index;
        wazaList->fields._CurrentIndex_k__BackingField = index;

        for (uint64_t i=0; i<wazaList->fields._wazaButtons->max_length; i++) {
            auto btn = wazaList->fields._wazaButtons->m_Items[i];
            btn->fields._isSelected = ((int32_t)i) == index;
            btn->fields._state = btn->fields._isSelected ? 1 : 0;

            if (btn->fields._isSelected && btn->fields._onSelected != nullptr)
                btn->fields._onSelected->Invoke();

            ((UnityEngine::Component::Object*)GetWazaButtonArrowRoot(btn))->get_gameObject()->SetActive(btn->fields._isSelected);
            GetWazaButtonBackgroundRoot(btn)->GetComponent(UnityEngine::Component::Method$$Image$$GetComponent)
                ->virtual_set_color(btn->fields._isSelected ? SELECTED_MOVE_BACKGROUND_COLOR : NOT_SELECTED_MOVE_BACKGROUND_COLOR);

            btn->fields._text->virtual_set_color(btn->fields._isSelected ? SELECTED_MOVE_TEXT_COLOR : NOT_SELECTED_MOVE_TEXT_COLOR);
        }

        Dpr::Battle::View::BattleViewCore::getClass()->initIfNeeded();
        auto battleViewCore = Dpr::Battle::View::BattleViewCore::get_Instance();
        if (playSe)
            battleViewCore->fields._UISystem_k__BackingField->PlaySe(AK_EVENTS_UI_COMMON_SELECT);
    }
}

UnityEngine::Transform::Object* GetGimmickRoot(Dpr::Battle::View::UI::BUIWazaList::Object* wazaList) {
    return ((UnityEngine::Component::Object*)wazaList)->get_transform()
            ->Find(System::String::Create("GimmickButton"))
            ->Find(System::String::Create("Gimmicks"));
}

UnityEngine::GameObject::Object* GetMegaIcon(Dpr::Battle::View::UI::BUIWazaList::Object* wazaList) {
    return ((UnityEngine::Component::Object*)GetGimmickRoot(wazaList)->Find(System::String::Create("MegaIcon")))->get_gameObject()->instance();
}

UnityEngine::GameObject::Object* GetZMoveIcon(Dpr::Battle::View::UI::BUIWazaList::Object* wazaList) {
    return ((UnityEngine::Component::Object*)GetGimmickRoot(wazaList)->Find(System::String::Create("ZMoveIcon")))->get_gameObject()->instance();
}

UnityEngine::GameObject::Object* GetUBurstIcon(Dpr::Battle::View::UI::BUIWazaList::Object* wazaList) {
    return ((UnityEngine::Component::Object*)GetGimmickRoot(wazaList)->Find(System::String::Create("UBurstIcon")))->get_gameObject()->instance();
}

UnityEngine::GameObject::Object* GetDMaxIcon(Dpr::Battle::View::UI::BUIWazaList::Object* wazaList) {
    return ((UnityEngine::Component::Object*)GetGimmickRoot(wazaList)->Find(System::String::Create("DMaxIcon")))->get_gameObject()->instance();
}

UnityEngine::GameObject::Object* GetTeraIcon(Dpr::Battle::View::UI::BUIWazaList::Object* wazaList) {
    return ((UnityEngine::Component::Object*)GetGimmickRoot(wazaList)->Find(System::String::Create("TeraIcon")))->get_gameObject()->instance();
}

UnityEngine::GameObject::Object* GetGimmickIconFromIndex(Dpr::Battle::View::UI::BUIWazaList::Object* wazaList, int32_t index) {
    switch (index)
    {
        case array_index(GIMMICKS, "Mega Evolution"):
            return GetMegaIcon(wazaList);

        case array_index(GIMMICKS, "Z-Move"):
            return GetZMoveIcon(wazaList);

        case array_index(GIMMICKS, "Ultra Burst"):
            return GetUBurstIcon(wazaList);

        case array_index(GIMMICKS, "Dynamax"):
            return GetDMaxIcon(wazaList);

        case array_index(GIMMICKS, "Terastallization"):
            return GetTeraIcon(wazaList);

        default:
            return nullptr;
    }
}

UnityEngine::Transform::Object* GetAffinitySpritesRoot(Dpr::Battle::View::UI::BUIWazaList::Object* wazaList) {
    return ((UnityEngine::Component::Object*)wazaList)->get_transform()
            ->Find(System::String::Create("WazaButtons"))
            ->Find(System::String::Create("AffinityIcons"));
}

UnityEngine::Sprite::Object* GetNoneAffinitySprite(Dpr::Battle::View::UI::BUIWazaList::Object* wazaList) {
    return ((UnityEngine::Component::Object*)GetAffinitySpritesRoot(wazaList)->Find(System::String::Create("None")))
                ->GetComponent(UnityEngine::Component::Method$$Image$$GetComponent)->get_sprite();
}

UnityEngine::Sprite::Object* GetImmuneAffinitySprite(Dpr::Battle::View::UI::BUIWazaList::Object* wazaList) {
    return ((UnityEngine::Component::Object*)GetAffinitySpritesRoot(wazaList)->Find(System::String::Create("Immune")))
                ->GetComponent(UnityEngine::Component::Method$$Image$$GetComponent)->get_sprite();
}

UnityEngine::Sprite::Object* GetNotVeryAffinitySprite(Dpr::Battle::View::UI::BUIWazaList::Object* wazaList) {
    return ((UnityEngine::Component::Object*)GetAffinitySpritesRoot(wazaList)->Find(System::String::Create("NotVery")))
                ->GetComponent(UnityEngine::Component::Method$$Image$$GetComponent)->get_sprite();
}

UnityEngine::Sprite::Object* GetRegularAffinitySprite(Dpr::Battle::View::UI::BUIWazaList::Object* wazaList) {
    return ((UnityEngine::Component::Object*)GetAffinitySpritesRoot(wazaList)->Find(System::String::Create("Regular")))
                ->GetComponent(UnityEngine::Component::Method$$Image$$GetComponent)->get_sprite();
}

UnityEngine::Sprite::Object* GetSuperAffinitySprite(Dpr::Battle::View::UI::BUIWazaList::Object* wazaList) {
    return ((UnityEngine::Component::Object*)GetAffinitySpritesRoot(wazaList)->Find(System::String::Create("Super")))
                ->GetComponent(UnityEngine::Component::Method$$Image$$GetComponent)->get_sprite();
}

UnityEngine::Transform::Object* GetDamageTypeSpritesRoot(Dpr::Battle::View::UI::BUIWazaDescription::Object* wazaDesc) {
    return ((UnityEngine::Component::Object*)wazaDesc)->get_transform()
            ->Find(System::String::Create("CategoryIcons"));
}

UnityEngine::Sprite::Object* GetPhysicalDamageSprite(Dpr::Battle::View::UI::BUIWazaDescription::Object* wazaDesc) {
    return ((UnityEngine::Component::Object*)GetDamageTypeSpritesRoot(wazaDesc)->Find(System::String::Create("Physical")))
            ->GetComponent(UnityEngine::Component::Method$$Image$$GetComponent)->get_sprite();
}

UnityEngine::Sprite::Object* GetSpecialDamageSprite(Dpr::Battle::View::UI::BUIWazaDescription::Object* wazaDesc) {
    return ((UnityEngine::Component::Object*)GetDamageTypeSpritesRoot(wazaDesc)->Find(System::String::Create("Special")))
            ->GetComponent(UnityEngine::Component::Method$$Image$$GetComponent)->get_sprite();
}

UnityEngine::Sprite::Object* GetStatusDamageSprite(Dpr::Battle::View::UI::BUIWazaDescription::Object* wazaDesc) {
    return ((UnityEngine::Component::Object*)GetDamageTypeSpritesRoot(wazaDesc)->Find(System::String::Create("Status")))
            ->GetComponent(UnityEngine::Component::Method$$Image$$GetComponent)->get_sprite();
}

UnityEngine::Sprite::Object* GetDamageSpriteFromType(Dpr::Battle::View::UI::BUIWazaDescription::Object* wazaDesc, Pml::WazaData::WazaDamageType type) {
    switch (type)
    {
        case Pml::WazaData::WazaDamageType::NONE:
            return GetStatusDamageSprite(wazaDesc);

        case Pml::WazaData::WazaDamageType::PHYSIC:
            return GetPhysicalDamageSprite(wazaDesc);

        case Pml::WazaData::WazaDamageType::SPECIAL:
            return GetSpecialDamageSprite(wazaDesc);

        default:
            return nullptr;
    }
}

Dpr::Battle::View::UI::BUISituation::Object* GetSituationObject(Dpr::Battle::View::Systems::BattleViewUISystem::Object* __this) {
    auto situationTF = ((UnityEngine::Component::Object*)__this)->get_transform()->Find(System::String::Create("BUISituation"));
    auto situationGO = ((UnityEngine::Component::Object*)situationTF)->get_gameObject();
    auto situationCmpList = (UnityEngine::Component::Array*)situationGO->GetAllComponents();
    return (Dpr::Battle::View::UI::BUISituation::Object*)situationCmpList->m_Items[2];
}

void SetMoveDescDamageType(Dpr::Battle::View::UI::BUIWazaDescription::Object* wazaDesc, int32_t wazaNo) {
    Logger::log("[SetMoveDescDamageType] we're in\n");
    auto type = Pml::WazaData::WazaDataSystem::GetDamageType(wazaNo);
    auto sprite = GetDamageSpriteFromType(wazaDesc, type);

    if (sprite != nullptr) {
        ((UnityEngine::Component::Object*)wazaDesc->fields._wazaCategory)->get_gameObject()->SetActive(true);
        wazaDesc->fields._wazaCategory->fields.iconImage->set_sprite(sprite);
    }
    else {
        wazaDesc->fields._wazaCategory->Deactive();
    }
}

int32_t GetNextGimmick(Dpr::Battle::View::UI::BUIWazaList::Object* wazaList, int32_t previousGimmick) {
    for (int32_t i=previousGimmick+1; i<GIMMICK_COUNT; i++)
    {
        auto gimmick = GetGimmickIconFromIndex(wazaList, i);
        if (gimmick != nullptr && gimmick->get_activeSelf())
            return i;
    }

    return array_index(GIMMICKS, "None");
}

void SetGimmickIconActive(Dpr::Battle::View::UI::BUIWazaList::Object* wazaList, int32_t index, bool state) {
    auto gimmickIcon = GetGimmickIconFromIndex(wazaList, index);
    if (gimmickIcon != nullptr)
        gimmickIcon->SetActive(state);
}

void SetGimmickIconsFromFlags(Dpr::Battle::View::UI::BUIWazaList::Object* wazaList) {
    for (int32_t i=0; i<GIMMICK_COUNT; i++)
        SetGimmickIconActive(wazaList, i, false);

    SetGimmickIconActive(wazaList, array_index(GIMMICKS, "Mega Evolution"), !FlagWork::GetFlag(FlagWork_Flag::FLAG_MEGA_EVOLUTION_UNAVAILABLE));
    SetGimmickIconActive(wazaList, array_index(GIMMICKS, "Ultra Burst"), true);
    // TODO: Add flags to other gimmicks
}

void SetGimmickIconBackgroundActive(Dpr::Battle::View::UI::BUIWazaList::Object* wazaList, int32_t index, bool state) {
    auto gimmickIcon = GetGimmickIconFromIndex(wazaList, index);
    if (gimmickIcon != nullptr)
        ((UnityEngine::Component::Object*)gimmickIcon->get_transform()->Find(System::String::Create("Active")))->get_gameObject()->SetActive(state);
}

HOOK_DEFINE_REPLACE(BUIActionList$$OnUpdate) {
    static void Callback(Dpr::Battle::View::UI::BUIActionList::Object* __this, float deltatime) {
        system_load_typeinfo(0x1f11);
        system_load_typeinfo(0x1f10);
        system_load_typeinfo(0x1f0d);

        if (!__this->fields._IsFocus_k__BackingField)
            return;

        if (__this->fields.isButtonAction)
            return;

        Dpr::NetworkUtils::NetworkManager::getClass()->initIfNeeded();
        if (Dpr::NetworkUtils::NetworkManager::IsShowApplicationErrorDialog())
            return;

        Dpr::Battle::View::BattleViewCore::getClass()->initIfNeeded();
        auto battleViewCore = Dpr::Battle::View::BattleViewCore::get_Instance();
        auto wazaList = battleViewCore->fields._UISystem_k__BackingField->fields._wazaList;

        Dpr::UI::UIManager::getClass()->initIfNeeded();

        if (Dpr::Battle::View::BtlvInput::GetPush(Dpr::UI::UIManager::getClass()->static_fields->ButtonX, true))
            wazaList->OnSubmitWazaDescription();

        if (Dpr::Battle::View::BtlvInput::GetPush(GameController::ButtonMask::R, true))
            __this->OnSubmitPokeBall();

        if (Dpr::Battle::View::BtlvInput::GetPush(Dpr::UI::UIManager::getClass()->static_fields->ButtonB, true)) {
            if (__this->fields._IsFocus_k__BackingField && !__this->fields.isButtonAction && __this->fields._IsReturnable_k__BackingField) {
                __this->fields.isButtonAction = true;
                __this->fields._IsValid_k__BackingField = true;
                __this->fields._Result_k__BackingField = Dpr::Battle::Logic::BtlAction::BTL_ACTION_ESCAPE;
                battleViewCore->fields._UISystem_k__BackingField->PlaySe(AK_EVENTS_UI_COMMON_CANCEL);
            }
        }

        if (Dpr::Battle::View::BtlvInput::GetPush(GameController::ButtonMask::Up, true)) {
            Logger::log("[BUIActionList$$OnUpdate] Battle Situation\n");
            if (__this->fields._IsFocus_k__BackingField && !__this->fields.isButtonAction) {
                Dpr::Battle::View::BattleViewCore::getClass()->initIfNeeded();
                Dpr::Battle::View::Systems::BattleViewUISystem::Object *battleViewUISystem = Dpr::Battle::View::BattleViewCore::get_Instance()->fields._UISystem_k__BackingField;

                if (IsActivatedBattleFeature(array_index(BATTLE_FEATURES, "Battle Situation"))) {
                    __this->fields.isButtonAction = true;
                    auto situationUI = GetSituationObject(battleViewUISystem);

                    __this->fields._IsValid_k__BackingField = false;
                    battleViewUISystem->SwitchShowDecoImage(false);
                    ((Dpr::Battle::View::UI::BattleViewUICanvasBase::Object *) __this)->Hide(false, nullptr);
                    ((Dpr::Battle::View::UI::BattleViewUICanvasBase::Object *) wazaList)->Hide(false, nullptr);

                    situationUI->Initialize();
                    ((Dpr::Battle::View::UI::BattleViewUICanvasBase::Object *) situationUI)->Show(nullptr);

                    battleViewUISystem->PlaySe(AK_EVENTS_UI_COMMON_MENU_OPEN);
                }
                else {
                    battleViewUISystem->PlaySe(AK_EVENTS_UI_COMMON_BEEP);
                }
            }
        }
        else if (Dpr::Battle::View::BtlvInput::GetPush(GameController::ButtonMask::Left, true)) {
            Logger::log("[BUIActionList$$OnUpdate] Pokémon\n");
            SubmitActionButton(__this, 1);
        }
        else if (Dpr::Battle::View::BtlvInput::GetPush(GameController::ButtonMask::Right, true)) {
            Logger::log("[BUIActionList$$OnUpdate] Bag\n");
            SubmitActionButton(__this, 2);
        }
        else if (Dpr::Battle::View::BtlvInput::GetPush(GameController::ButtonMask::Down, true)) {
            Logger::log("[BUIActionList$$OnUpdate] Run\n");
            SubmitActionButton(__this, 3);
        }

        if (Dpr::Battle::View::BtlvInput::GetPush(Dpr::UI::UIManager::getClass()->static_fields->ButtonY, true)) {
            Logger::log("[BUIActionList$$OnUpdate] Change gimmick\n");
            for (int32_t i=0; i<GIMMICK_COUNT; i++)
                SetGimmickIconBackgroundActive(wazaList, i, false);

            int32_t nextGimmick = GetNextGimmick(wazaList, selectedGimmick);

            if (nextGimmick != selectedGimmick)
                battleViewCore->fields._UISystem_k__BackingField->PlaySe(AK_EVENTS_UI_COMMON_SELECT);

            selectedGimmick = nextGimmick;
            SetGimmickIconBackgroundActive(wazaList, selectedGimmick, true);

            Logger::log("[BUIActionList$$OnUpdate] New gimmick is %s\n", GIMMICKS[selectedGimmick]);
        }

        if (Dpr::Battle::View::BtlvInput::GetPush(Dpr::UI::UIManager::getClass()->static_fields->ButtonA, true)) {
            Logger::log("[BUIActionList$$OnUpdate] Submit waza\n");
            if (wazaList->fields._CurrentIndex_k__BackingField >= 0 && wazaList->fields._CurrentIndex_k__BackingField < wazaList->fields._wazaCount)
            {
                wazaList->OnSubmit();
                if (wazaList->fields._IsValid_k__BackingField) {
                    Logger::log("[BUIActionList$$OnUpdate] Fight\n");
                    SubmitActionButton(__this, 0);
                    return;
                }
            }
            else
            {
                battleViewCore->fields._UISystem_k__BackingField->PlaySe(AK_EVENTS_UI_COMMON_BEEP);
            }
        }

        if (Dpr::Battle::View::BtlvInput::GetPush(
                GameController::ButtonMask::StickLUp | GameController::ButtonMask::StickRUp, true)) {
            if (wazaList->fields._wazaCount > 0)
                SelectWazaButton(wazaList, 0, true);
        }
        else if (Dpr::Battle::View::BtlvInput::GetPush(
                GameController::ButtonMask::StickLRight | GameController::ButtonMask::StickRRight, true)) {
            if (wazaList->fields._wazaCount > 1)
                SelectWazaButton(wazaList, 1, true);
        }
        else if (Dpr::Battle::View::BtlvInput::GetPush(
                GameController::ButtonMask::StickLDown | GameController::ButtonMask::StickRDown, true)) {
            if (wazaList->fields._wazaCount > 2)
                SelectWazaButton(wazaList, 2, true);
        }
        else if (Dpr::Battle::View::BtlvInput::GetPush(
                GameController::ButtonMask::StickLLeft | GameController::ButtonMask::StickRLeft, true)) {
            if (wazaList->fields._wazaCount > 3)
                SelectWazaButton(wazaList, 3, true);
        }
    }
};

HOOK_DEFINE_TRAMPOLINE(BUIActionList$$OnSubmitPokeBall) {
    static void Callback(Dpr::Battle::View::UI::BUIActionList::Object* __this) {
        Logger::log("[BUIActionList$$OnSubmitPokeBall] we're in\n");

        if (__this->fields._IsFocus_k__BackingField && __this->fields._isBallEnable && !__this->fields.isButtonAction)
        {
            Orig(__this);

            Logger::log("[BUIActionList$$OnSubmitPokeBall] custom stuff\n");
            Dpr::Battle::View::BattleViewCore::getClass()->initIfNeeded();
            auto battleViewCore = Dpr::Battle::View::BattleViewCore::get_Instance();
            ((Dpr::Battle::View::UI::BattleViewUICanvasBase::Object*)battleViewCore->fields._UISystem_k__BackingField->fields._wazaList)->Hide(false, nullptr);
        }
    }
};


HOOK_DEFINE_TRAMPOLINE(BUIPokeBallList$$OnCancel) {
    static void Callback(Dpr::Battle::View::UI::BUIPokeBallList::Object* __this) {
        Logger::log("[BUIPokeBallList$$OnCancel] we're in\n");

        if (__this->fields.isAction) {
            return;
        }

        Orig(__this);

        Logger::log("[BUIPokeBallList$$OnCancel] custom stuff\n");
        Dpr::Battle::View::BattleViewCore::getClass()->initIfNeeded();
        auto battleViewCore = Dpr::Battle::View::BattleViewCore::get_Instance();
        ((Dpr::Battle::View::UI::BattleViewUICanvasBase::Object*)battleViewCore->fields._UISystem_k__BackingField->fields._wazaList)->Show(nullptr);
    }
};


HOOK_DEFINE_REPLACE(BUIWazaList$$OnUpdate) {
    static void Callback(Dpr::Battle::View::UI::BUIWazaList::Object* __this, float deltatime) {
        // Do nothing, the OnUpdate is done in BUIActionList$$OnUpdate
    }
};

HOOK_DEFINE_REPLACE(BUIWazaList$$OnShow) {
    static void Callback(Dpr::Battle::View::UI::BUIWazaList::Object* __this) {
        system_load_typeinfo(0x1f80);
        Dpr::Battle::View::BattleViewCore::getClass()->initIfNeeded();

        __this->fields._IsFocus_k__BackingField = false;
        __this->fields._IsShow_k__BackingField = true;
        __this->fields._animationState_k__BackingField = 2;

        auto battleViewCore = Dpr::Battle::View::BattleViewCore::get_Instance();
        battleViewCore->fields._UISystem_k__BackingField->fields._cursor->SetActive(false);
    }
};

HOOK_DEFINE_TRAMPOLINE(BUIWazaList$$Initialize) {
    static void Callback(Dpr::Battle::View::UI::BUIWazaList::Object* __this,
                         Dpr::Battle::Logic::BTL_POKEPARAM::Object* bpp, uint8_t pokeIndex,
                         Dpr::Battle::Logic::BTL_ACTION::PARAM_OBJ::Object* dest) {
        Logger::log("[BUIWazaList$$Initialize] we're in\n");
        Orig(__this, bpp, pokeIndex, dest);

        selectedGimmick = array_index(GIMMICKS, "None");
        SetGimmickIconsFromFlags(__this);
        for (int32_t i=0; i<GIMMICK_COUNT; i++)
            SetGimmickIconBackgroundActive(__this, i, false);

        UnityEngine::UI::LayoutRebuilder::ForceRebuildLayoutImmediate((UnityEngine::RectTransform::Object*)GetGimmickRoot(__this));

        if (__this->fields._CurrentIndex_k__BackingField < 0 || __this->fields._CurrentIndex_k__BackingField >= __this->fields._wazaCount)
            SelectWazaButton(__this, 0, false);
        else
            SelectWazaButton(__this, __this->fields._CurrentIndex_k__BackingField, false);
    }
};


HOOK_DEFINE_REPLACE(BUIWazaButton$$Initialize) {
    static void Callback(Dpr::Battle::View::UI::BUIWazaButton::Object* __this, int32_t index,
                         System::Nullable<Dpr::Battle::View::BTLV_WAZA_INFO>::Object* info,
                         UnityEngine::Sprite::Object* typeSprite, UnityEngine::Sprite::Object* effBgSprite,
                         UnityEngine::Sprite::Object* effSprite, System::String::Object* affinityString,
                         UnityEngine::Color::Array* ppColors) {
        system_load_typeinfo(0x1f77);
        system_load_typeinfo(0x239d);

        __this->fields._index = index;
        if (!info->fields.has_value)
            return;

        __this->fields._Info_k__BackingField = *info;
        __this->fields._backgroundImage->set_sprite(typeSprite);
        __this->fields._effectiveBG->set_sprite(effBgSprite);

        auto fullInfo = info->fields.value;

        Dpr::Message::MessageWordSetHelper::getClass()->initIfNeeded();
        Dpr::Message::MessageWordSetHelper::SetStringWord(0, fullInfo.fields.WazaName);
        Dpr::Battle::Logic::BattleStr::getClass()->initIfNeeded();
        auto wazaStr = Dpr::Battle::Logic::BattleStr::getClass()->static_fields->s_Instance->GetFormatUIText(System::String::Create("msg_ui_raidbtl_fr_pokewaza"), nullptr);
        ((Dpr::Battle::View::UI::BUIButtonBase::Object*)__this)->set_Text(wazaStr);

        Dpr::Message::MessageWordSetHelper::SetDigitWord(0, fullInfo.fields.MaxPP);
        auto maxPPStr = Dpr::Battle::Logic::BattleStr::getClass()->static_fields->s_Instance->GetFormatUIText(System::String::Create("msg_ui_btl_pp_02"), nullptr);
        ((TMPro::TMP_Text::Object*)__this->fields._maxPpText)->set_text(maxPPStr);

        Dpr::Message::MessageWordSetHelper::SetDigitWord(0, fullInfo.fields.PP);
        auto ppStr = Dpr::Battle::Logic::BattleStr::getClass()->static_fields->s_Instance->GetFormatUIText(System::String::Create("msg_ui_btl_pp_01"), nullptr);
        ((TMPro::TMP_Text::Object*)__this->fields._ppText)->set_text(ppStr);

        int32_t colorIndex;
        if (ppColors == nullptr || ppColors->max_length < 4)
            colorIndex = 0;
        else if (fullInfo.fields.PP == 0)
            colorIndex = 3;
        else if (fullInfo.fields.PP > (fullInfo.fields.MaxPP / 2))
            colorIndex = 0;
        else if (fullInfo.fields.PP > (fullInfo.fields.MaxPP / 4))
            colorIndex = 1;
        else
            colorIndex = 2;

        ((TMPro::TMP_Text::Object*)__this->fields._maxPpText)->set_color(ppColors->m_Items[colorIndex]);
        ((TMPro::TMP_Text::Object*)__this->fields._ppText)->set_color(ppColors->m_Items[colorIndex]);

        Dpr::Battle::View::BattleViewCore::getClass()->initIfNeeded();
        Dpr::Battle::View::UI::BattleAffinityInfo::getClass()->initIfNeeded();
        auto uiSystem = Dpr::Battle::View::BattleViewCore::get_Instance()->fields._UISystem_k__BackingField;
        auto bpp = uiSystem->fields._wazaList->fields._btlPokeParam;
        int32_t wazano = bpp->WAZA_GetID(index);
        auto targets = Dpr::Battle::View::UI::BattleAffinityInfo::GetBattleTargets();

        __this->fields._effectiveText->cast<UnityEngine::Component>()->get_gameObject()->SetActive(false);

        Pml::Battle::TypeAffinity::AboutAffinityID affinity;
        bool canHit = Dpr::Battle::View::UI::BattleAffinityInfo::CheckWazaAffinity(bpp, wazano, targets, &affinity);
        if (!canHit) {
            __this->fields._effectiveObj->SetActive(false);
        }
        else {
            __this->fields._effectiveObj->SetActive(true);
            switch (affinity)
            {
                case Pml::Battle::TypeAffinity::AboutAffinityID::NONE:
                    __this->fields._effectiveImage->set_sprite(GetImmuneAffinitySprite(uiSystem->fields._wazaList));
                    break;

                case Pml::Battle::TypeAffinity::AboutAffinityID::NORMAL:
                    __this->fields._effectiveImage->set_sprite(GetRegularAffinitySprite(uiSystem->fields._wazaList));
                    break;

                case Pml::Battle::TypeAffinity::AboutAffinityID::ADVANTAGE:
                    __this->fields._effectiveImage->set_sprite(GetSuperAffinitySprite(uiSystem->fields._wazaList));
                    break;

                case Pml::Battle::TypeAffinity::AboutAffinityID::DISADVANTAGE:
                    __this->fields._effectiveImage->set_sprite(GetNotVeryAffinitySprite(uiSystem->fields._wazaList));
                    break;
            }
        }
    }
};


HOOK_DEFINE_TRAMPOLINE(BUIWazaDescription$$Initialize) {
    static void Callback(Dpr::Battle::View::UI::BUIWazaDescription::Object* __this, Dpr::Battle::View::BTLV_WAZA_INFO::Object* info, float posY) {
        Logger::log("[BUIWazaDescription$$Initialize] we're in\n");
        Orig(__this, info, posY);

        Logger::log("[BUIWazaDescription$$Initialize] custom stuff\n");
        SetMoveDescDamageType(__this, info->fields.WazaNo);
    }
};

HOOK_DEFINE_REPLACE(BUIWazaDescription$$SetWazaDamageType) {
    static void Callback(Dpr::Battle::View::UI::BUIWazaDescription::Object* __this, int32_t wazaNo) {
        Logger::log("[BUIWazaDescription$$SetWazaDamageType] we're in\n");
        SetMoveDescDamageType(__this, wazaNo);
    }
};


HOOK_DEFINE_REPLACE(BattleViewUISystem_SwitchActionListCoroutine$$MoveNext) {
    static bool Callback(Dpr::Battle::View::Systems::BattleViewUISystem::SwitchActionListCoroutine_d__198::Object* __this) {
        system_load_typeinfo(0x1f16);

        switch (__this->fields.__1__state)
        {
            case 0:
            case 1:
            {
                auto uiSystem = __this->fields.__4__this;
                if (uiSystem->IsOpenMessage() ||
                    uiSystem->IsStatusWindowTarasit() ||
                    uiSystem->fields._actionList->fields._IsTransition_k__BackingField)
                {
                    __this->fields.__2__current = nullptr;
                    __this->fields.__1__state = 1;
                    return true;
                }

                if (!__this->fields.isShow)
                {
                    if (uiSystem->fields._actionList->fields._IsShow_k__BackingField) {
                        ((Dpr::Battle::View::UI::BattleViewUICanvasBase::Object*)uiSystem->fields._actionList)->Hide(false, nullptr);
                        ((Dpr::Battle::View::UI::BattleViewUICanvasBase::Object*)uiSystem->fields._wazaList)->Hide(false, nullptr);
                    }
                }
                else if (!uiSystem->fields._actionList->fields._IsShow_k__BackingField)
                {
                    ((Dpr::Battle::View::UI::BattleViewUICanvasBase::Object*)uiSystem->fields._actionList)->Show(nullptr);
                    ((Dpr::Battle::View::UI::BattleViewUICanvasBase::Object*)uiSystem->fields._wazaList)->Show(nullptr);
                    uiSystem->SwitchShowDecoImage(true);
                    uiSystem->fields._pokeBallList->SetInvalid();
                    uiSystem->PlaySe(AK_EVENTS_UI_COMMON_SLIDE);
                }

                uiSystem->SetCurrentPokemonStatus();

                if (__this->fields.withStatusWindow)
                {
                    uiSystem->Apply(0);
                    uiSystem->SwitchShowStatusWindowsCore(__this->fields.isShow, false);
                }

                if (__this->fields.withDecoraton)
                    uiSystem->SwitchShowDecoImage(__this->fields.isShow);
            }
            break;

            default:
                return false;
        }

        return false;
    }
};


HOOK_DEFINE_REPLACE(BattleViewUISystem$$CMD_UI_SelectAction_Start) {
    static void Callback(Dpr::Battle::View::Systems::BattleViewUISystem::Object* __this,
                         Dpr::Battle::Logic::BattleViewBase::SelectActionParam::Object** param,
                         Dpr::Battle::Logic::BTL_ACTION::PARAM_OBJ::Object* dest) {
        Logger::log("[BattleViewUISystem$$CMD_UI_SelectAction_Start] we're in\n");
        system_load_typeinfo(0x2476);
        system_load_typeinfo(0x24a1);
        UnityEngine::Debug::getClass()->initIfNeeded();

        UnityEngine::Debug::Log(System::String::Create("[BattleUI]SelectAction"));
        __this->fields._actionList->Initialize(param);
        __this->fields._actionParam = *param;
        __this->fields._currentPoke = (*param)->fields.pActPoke;

        if ((*param)->fields.pokeIndex == 0)
            __this->fields._swapWaitPokemon = nullptr;

        __this->fields._wazaList->Initialize((*param)->fields.pActPoke, (*param)->fields.pokeIndex, dest);

        __this->SwitchShowActionList(true, !__this->IsOpenedStatus(), !__this->fields._wazaList->fields._IsShow_k__BackingField);
    }
};

HOOK_DEFINE_REPLACE(BattleViewUISystem$$CMD_UI_SelectAction_Wait) {
    static Dpr::Battle::Logic::BtlAction Callback(Dpr::Battle::View::Systems::BattleViewUISystem::Object* __this) {
        system_load_typeinfo(0x24a1);

        if (!__this->fields._actionList->fields._IsValid_k__BackingField && !__this->fields._wazaList->fields._IsValid_k__BackingField)
            return Dpr::Battle::Logic::BtlAction::BTL_ACTION_NULL;

        Logger::log("[BattleViewUISystem$$CMD_UI_SelectAction_Wait] At the switch with result = %d\n", __this->fields._actionList->fields._Result_k__BackingField);
        switch (__this->fields._actionList->fields._Result_k__BackingField)
        {
            case Dpr::Battle::Logic::BtlAction::BTL_ACTION_FIGHT:
                __this->fields._wazaList->SetInvalid();
                __this->SwitchShowActionList(false, false, true);
                return __this->fields._actionList->fields._Result_k__BackingField;

            case Dpr::Battle::Logic::BtlAction::BTL_ACTION_ESCAPE:
                __this->SwitchShowActionList(false, !__this->fields._actionList->fields._IsReturnable_k__BackingField, true);
                return __this->fields._actionList->fields._Result_k__BackingField;

            default:
                __this->SwitchShowActionList(false, true, true);
                return __this->fields._actionList->fields._Result_k__BackingField;
        }
    }
};

HOOK_DEFINE_REPLACE(BattleViewUISystem$$CMD_UI_SelectAction_ForceQuit) {
    static void Callback(Dpr::Battle::View::Systems::BattleViewUISystem::Object* __this) {
        Logger::log("[BattleViewUISystem$$CMD_UI_SelectAction_ForceQuit] we're in\n");
        __this->fields._actionList->virtual_ForceHide();
    }
};


HOOK_DEFINE_REPLACE(BattleViewUISystem$$CMD_UI_SelectWaza_Start) {
    static void Callback(Dpr::Battle::View::Systems::BattleViewUISystem::Object* __this,
                         Dpr::Battle::Logic::BTL_POKEPARAM::Object* bpp, uint8_t pokeIndex,
                         Dpr::Battle::Logic::BTL_ACTION::PARAM_OBJ::Object* dest) {
        // Do nothing, we want to skip to combat
        Logger::log("[BattleViewUISystem$$CMD_UI_SelectWaza_Start] we're in\n");
    }
};

HOOK_DEFINE_REPLACE(BattleViewUISystem$$CMD_UI_SelectWaza_Wait) {
    static bool Callback(Dpr::Battle::View::Systems::BattleViewUISystem::Object* __this) {
        Logger::log("[BattleViewUISystem$$CMD_UI_SelectWaza_Wait] we're in\n");
        // If we're coming back from targeting, we cancel back to SelectAction
        if (__this->fields._targetSelect->fields._Result_k__BackingField == Dpr::Battle::Logic::BtlPokePos::POS_NULL &&
            __this->fields._targetSelect->fields._IsValid_k__BackingField) {
            Logger::log("[BattleViewUISystem$$CMD_UI_SelectWaza_Wait] Returning from target select\n");
            __this->fields._targetSelect->fields._IsValid_k__BackingField = false;
            __this->fields._wazaList->OnCancel();
        }

        // TODO: Situation Detail cancel
        return true;
    }
};

HOOK_DEFINE_REPLACE(BattleViewUISystem$$CMD_UI_SelectWaza_End) {
    static bool Callback(Dpr::Battle::View::Systems::BattleViewUISystem::Object* __this) {
        // Hide the UI in case it's still up (shouldn't be)
        Logger::log("[BattleViewUISystem$$CMD_UI_SelectWaza_End] we're in\n");

        if (__this->fields._wazaList->fields._IsShow_k__BackingField)
            ((Dpr::Battle::View::UI::BattleViewUICanvasBase::Object*)__this->fields._wazaList)->Hide(false, nullptr);

        if (__this->fields._actionList->fields._IsShow_k__BackingField)
            ((Dpr::Battle::View::UI::BattleViewUICanvasBase::Object*)__this->fields._actionList)->Hide(false, nullptr);

        return true;
    }
};

HOOK_DEFINE_REPLACE(BattleViewUISystem$$CMD_UI_SelectWaza_ForceQuit) {
    static void Callback(Dpr::Battle::View::Systems::BattleViewUISystem::Object* __this) {
        Logger::log("[BattleViewUISystem$$CMD_UI_SelectWaza_ForceQuit] we're in\n");
        ((Dpr::Battle::View::UI::BattleViewUICanvasBase::Object*)__this->fields._wazaList)->Hide(false, nullptr);
        ((Dpr::Battle::View::UI::BattleViewUICanvasBase::Object*)__this->fields._actionList)->Hide(false, nullptr);
    }
};


void exl_madrid_ui_main() {
    BUIActionList$$OnUpdate::InstallAtOffset(0x01e8bdb0);
    BUIActionList$$OnSubmitPokeBall::InstallAtOffset(0x01e8c1b0);

    BUIPokeBallList$$OnCancel::InstallAtOffset(0x01d221b0);

    BUIWazaList$$OnUpdate::InstallAtOffset(0x01d2c490);
    BUIWazaList$$OnShow::InstallAtOffset(0x01d2cb70);
    BUIWazaList$$Initialize::InstallAtOffset(0x01d2b800);

    BUIWazaButton$$Initialize::InstallAtOffset(0x01d2a8f0);

    BUIWazaDescription$$Initialize::InstallAtOffset(0x01d2aea0);
    BUIWazaDescription$$SetWazaDamageType::InstallAtOffset(0x01d2b3c0);

    BattleViewUISystem_SwitchActionListCoroutine$$MoveNext::InstallAtOffset(0x01e7efe0);

    BattleViewUISystem$$CMD_UI_SelectAction_Start::InstallAtOffset(0x01e75c40);
    BattleViewUISystem$$CMD_UI_SelectAction_Wait::InstallAtOffset(0x01e764a0);
    BattleViewUISystem$$CMD_UI_SelectAction_ForceQuit::InstallAtOffset(0x01e76590);

    BattleViewUISystem$$CMD_UI_SelectWaza_Start::InstallAtOffset(0x01e765a0);
    BattleViewUISystem$$CMD_UI_SelectWaza_Wait::InstallAtOffset(0x01e76ac0);
    BattleViewUISystem$$CMD_UI_SelectWaza_End::InstallAtOffset(0x01e76b40);
    BattleViewUISystem$$CMD_UI_SelectWaza_ForceQuit::InstallAtOffset(0x01e76b70);
}