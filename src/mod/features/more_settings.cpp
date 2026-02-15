#include "exlaunch.hpp"

#include "data/game_modes.h"
#include "data/random_team_modes.h"
#include "data/settings.h"
#include "data/utils.h"
#include "data/window_frames.h"

#include "externals/ConfigWork.h"
#include "externals/Dpr/Message/MessageManager.h"
#include "externals/Dpr/Message/MessageWordSetHelper.h"
#include "externals/Dpr/MsgWindow/MsgWindowParam.h"
#include "externals/Dpr/UI/Keyguide.h"
#include "externals/Dpr/UI/SettingWindow.h"
#include "externals/Dpr/UI/UIManager.h"
#include "externals/FlagWork.h"
#include "externals/PlayerWork.h"
#include "externals/SmartPoint/AssetAssistant/Sequencer.h"
#include "externals/UnityEngine/Mathf.h"
#include "externals/UnityEngine/QualitySettings.h"
#include "romdata/romdata.h"
#include "save/save.h"

#include "logger/logger.h"

static ExtraSettingsSaveData tempExtraSettings {};

void ApplyTargetFrameRate(int32_t selectorIndex) {
    // 0=20fps, 1=30fps, 2=60fps, 3=Unlimited
    static constexpr int32_t VSYNC_VALUES[] = { 3, 2, 1, 0 };
    if (selectorIndex >= 0 && selectorIndex <= 3)
        UnityEngine::QualitySettings::set_vSyncCount(VSYNC_VALUES[selectorIndex]);
}

void OnValueChanged(int32_t configId, int32_t value) {
    ConfigWork::getClass()->initIfNeeded();
    if (ConfigWork::getClass()->static_fields->onValueChanged != nullptr)
        ConfigWork::getClass()->static_fields->onValueChanged->Invoke(configId, value);
}

void SetSetting(DPData::CONFIG::Object* config, ExtraSettingsSaveData* extraSettings, int32_t configId, int32_t value) {
    switch (configId) {
        case array_index(SETTINGS, "EXP Share"):
            extraSettings->expShareEnabled = value == 0; // Index 0 is "On" and index 1 is "Off"
            break;
        case array_index(SETTINGS, "Affection"):
            extraSettings->affectionEnabled = value == 0; // Index 0 is "On" and index 1 is "Off"
            break;
        case array_index(SETTINGS, "Level Cap"):
            extraSettings->levelCapEnabled = value == 0; // Index 0 is "On" and index 1 is "Off"
            break;
        case array_index(SETTINGS, "Visible Shiny Eggs"):
            extraSettings->shinyEggsEnabled = value == 0; // Index 0 is "On" and index 1 is "Off"
            break;
        case array_index(SETTINGS, "Trainer Sets"):
            extraSettings->gameMode = (ExtraSettingsSaveData::GameMode)value;
            break;
        case array_index(SETTINGS, "Team Randomization"):
            extraSettings->randomTeamMode = (ExtraSettingsSaveData::RandomTeamMode)value;
            break;
        case array_index(SETTINGS, "Frame Rate"):
            extraSettings->targetFps = value;
            break;
        default:
            config->SetValue(configId, value);
            break;
    }
}

int32_t GetSetting(DPData::CONFIG::Object* config, ExtraSettingsSaveData* extraSettings, int32_t configId) {
    switch (configId) {
        case array_index(SETTINGS, "EXP Share"):
            return extraSettings->expShareEnabled ? 0 : 1; // Index 0 is "On" and index 1 is "Off"
        case array_index(SETTINGS, "Affection"):
            return extraSettings->affectionEnabled ? 0 : 1; // Index 0 is "On" and index 1 is "Off"
        case array_index(SETTINGS, "Level Cap"):
            return extraSettings->levelCapEnabled ? 0 : 1; // Index 0 is "On" and index 1 is "Off"
        case array_index(SETTINGS, "Visible Shiny Eggs"):
            return extraSettings->shinyEggsEnabled ? 0 : 1; // Index 0 is "On" and index 1 is "Off"
        case array_index(SETTINGS, "Trainer Sets"):
            return (int32_t)extraSettings->gameMode;
        case array_index(SETTINGS, "Team Randomization"):
            return (int32_t)extraSettings->randomTeamMode;
        case array_index(SETTINGS, "Frame Rate"):
            return extraSettings->targetFps;
        default:
            return config->GetValue(configId);
    }
}

bool IsEqualValue(DPData::CONFIG::Object* config, DPData::CONFIG::Object* otherConfig, ExtraSettingsSaveData* extraSettings, ExtraSettingsSaveData* otherExtraSettings, int32_t configId) {
    switch (configId) {
        case array_index(SETTINGS, "EXP Share"):
            return extraSettings->expShareEnabled == otherExtraSettings->expShareEnabled;
        case array_index(SETTINGS, "Affection"):
            return extraSettings->affectionEnabled == otherExtraSettings->affectionEnabled;
        case array_index(SETTINGS, "Level Cap"):
            return extraSettings->levelCapEnabled == otherExtraSettings->levelCapEnabled;
        case array_index(SETTINGS, "Visible Shiny Eggs"):
            return extraSettings->shinyEggsEnabled == otherExtraSettings->shinyEggsEnabled;
        case array_index(SETTINGS, "Trainer Sets"):
            return extraSettings->gameMode == otherExtraSettings->gameMode;
        case array_index(SETTINGS, "Team Randomization"):
            return extraSettings->randomTeamMode == otherExtraSettings->randomTeamMode;
        case array_index(SETTINGS, "Frame Rate"):
            return extraSettings->targetFps == otherExtraSettings->targetFps;
        default:
            return config->IsEqualValue(configId, otherConfig);
    }
}

void CheckAndInvokeChangedValues(DPData::CONFIG::Object* config, DPData::CONFIG::Object* prevConfig, ExtraSettingsSaveData* extraSettings, ExtraSettingsSaveData* prevExtraSettings) {
    for (int i=0; i<SETTING_COUNT; i++) {
        if (!IsEqualValue(config, prevConfig, extraSettings, prevExtraSettings, i)) {
            OnValueChanged(i, config->GetValue(i));
        }
    }
}

void InvokeChangedValues(DPData::CONFIG::Object* config, ExtraSettingsSaveData* extraSettings) {
    for (int i=0; i<SETTING_COUNT; i++) {
        OnValueChanged(i, GetSetting(config, extraSettings, i));
    }
}

void DefaultSettings(DPData::CONFIG::Object* config, ExtraSettingsSaveData* extraSettings) {
    config->fields.msg_speed = DPData::MSGSPEED::MSGSPEED_FAST;
    config->fields.window_type = 0;
    config->fields.wazaeff_mode = 0;
    config->fields.battle_rule = 0;
    config->fields.party_and_box = 0;
    config->fields.regist_nickname = true;
    config->fields.gyrosensor = true;
    config->fields.camerashake_of_fossil = true;
    config->fields.autoreport = true;
    config->fields.show_nickname = true;
    config->fields.bgm_volume = 8;
    config->fields.se_volume = 8;
    config->fields.voice_volume = 8;
    config->fields.camera_updown = 0;
    config->fields.camera_leftright = 0;
    config->fields.input_mode = 1;

    extraSettings->Initialize();
}

void ResetSettings(DPData::CONFIG::Object* config, ExtraSettingsSaveData* extraSettings) {
    DPData::CONFIG::Object prevConfig {};
    memmove(&prevConfig, config, sizeof(DPData::CONFIG::Object));

    ExtraSettingsSaveData prevExtraSettings {};
    memmove(&prevExtraSettings, extraSettings, sizeof(ExtraSettingsSaveData));

    DefaultSettings(config, extraSettings);

    CheckAndInvokeChangedValues(config, &prevConfig, extraSettings, &prevExtraSettings);
}

int32_t MaxWindowSelectorValue(int32_t configId) {
    switch (configId) {
        case array_index(SETTINGS, "Change Window"):
        default:
            return WINDOW_FRAME_COUNT - 1;

        case array_index(SETTINGS, "Trainer Sets"):
            return GAME_MODE_COUNT - 1;

        case array_index(SETTINGS, "Team Randomization"):
            return RANDOM_TEAM_MODE_COUNT - 1;

        case array_index(SETTINGS, "Frame Rate"):
            return 3; // 20=0, 30=1, 60=2, Unlimited=3
    }
}


// Default to checking the save's config in case we still call this method (happens in a few places)
HOOK_DEFINE_TRAMPOLINE(CONFIG$$GetValue) {
    static int32_t Callback(DPData::CONFIG::Object* config, int32_t configId) {
        if (configId <= array_index(SETTINGS, "Pokémon Cries")) // Avoids an infinite loop
            return Orig(config, configId);
        else
            return GetSetting(config, &getCustomSaveData()->settings, configId);
    }
};

// Only really used in GameManager's OnetimeInitializeOperation, we do want to keep this
HOOK_DEFINE_REPLACE(ConfigWork$$InvokeChangedValues) {
    static void Callback(DPData::CONFIG::Object* config) {
        system_load_typeinfo(0x2e11);

        ApplyTargetFrameRate(getCustomSaveData()->settings.targetFps);

        for (int i=0; i<SETTING_COUNT; i++) {
            OnValueChanged(i, GetSetting(config, &getCustomSaveData()->settings, i));
        }
    }
};

HOOK_DEFINE_REPLACE(SettingWindow$$InitializeSettings) {
    static void Callback(Dpr::UI::SettingWindow::Object *__this) {
        system_load_typeinfo(0x7c49);

        ResetSettings(&__this->fields._tempConfig, &tempExtraSettings);

        PlayerWork::getClass()->initIfNeeded();
        memmove(PlayerWork::get_config(), &__this->fields._tempConfig, sizeof(DPData::CONFIG::Object));
        memmove(&getCustomSaveData()->settings, &tempExtraSettings, sizeof(ExtraSettingsSaveData));

        for (int i=0; i<__this->fields._activeItems->fields._size; i++) {
            auto activeItem = __this->fields._activeItems->fields._items->m_Items[i];
            activeItem->SetSelectIndex(GetSetting(&__this->fields._tempConfig, &tempExtraSettings, activeItem->fields._configId), true, true);
        }
    }
};

HOOK_DEFINE_REPLACE(SettingWindow$$OnMenuItemValueChaged) {
    static void Callback(Dpr::UI::SettingWindow::Object* __this, Dpr::UI::SettingMenuItem::Object* menuItem) {
        system_load_typeinfo(0x7c4c);

        SetSetting(&__this->fields._tempConfig, &tempExtraSettings, menuItem->fields._configId, menuItem->fields._selectIndex);
        OnValueChanged(menuItem->fields._configId, menuItem->fields._selectIndex);
    }
};

HOOK_DEFINE_REPLACE(SettingWindow$$RevertSettings) {
    static void Callback(Dpr::UI::SettingWindow::Object *__this) {
        system_load_typeinfo(0x7c53);

        for (int i=0; i<SETTING_COUNT; i++) {
            PlayerWork::getClass()->initIfNeeded();
            if (!IsEqualValue(PlayerWork::get_config(), &__this->fields._tempConfig, &getCustomSaveData()->settings, &tempExtraSettings, i)) {
                SetSetting(&__this->fields._tempConfig, &tempExtraSettings, i, GetSetting(PlayerWork::get_config(), &getCustomSaveData()->settings, i));
                OnValueChanged(i, GetSetting(&__this->fields._tempConfig, &tempExtraSettings, i));
            }
        }
    }
};

HOOK_DEFINE_REPLACE(SettingWindow$$AcceptSettings) {
    static void Callback(Dpr::UI::SettingWindow::Object *__this) {
        system_load_typeinfo(0x7c47);

        PlayerWork::getClass()->initIfNeeded();
        memmove(PlayerWork::get_config(), &__this->fields._tempConfig, sizeof(DPData::CONFIG::Object));

        memmove(&getCustomSaveData()->settings, &tempExtraSettings, sizeof(ExtraSettingsSaveData));

        ApplyTargetFrameRate(getCustomSaveData()->settings.targetFps);

        InvokeChangedValues(PlayerWork::get_config(), &getCustomSaveData()->settings);
    }
};

HOOK_DEFINE_REPLACE(SettingWindow$$UpdateSelectValue) {
    static bool Callback(Dpr::UI::SettingWindow::Object* __this, float deltaTime) {
        system_load_typeinfo(0x7c5e);
        system_load_typeinfo(0x7c46);

        auto item = __this->fields._activeItems->fields._items->m_Items[__this->fields._selectIndex];

        int32_t maxValue;
        switch (item->fields._itemType) {
            case Dpr::UI::SettingMenuItem::ItemType::Selector:
                if (item->fields._texts->fields._size == 0)
                    return false;

                maxValue = item->fields._texts->fields._size - 1;
                break;

            case Dpr::UI::SettingMenuItem::ItemType::Gauge:
                maxValue = 10;
                break;

            case Dpr::UI::SettingMenuItem::ItemType::WindowSelector:
            default:
                maxValue = MaxWindowSelectorValue(item->fields._configId);
                break;
        }

        int32_t newValue;
        Dpr::UI::UIManager::getClass()->initIfNeeded();
        UnityEngine::Mathf::getClass()->initIfNeeded();
        if (__this->cast<Dpr::UI::UIWindow>()->IsPushButton(Dpr::UI::UIManager::getClass()->static_fields->StickLRight, false))
        {
            switch (item->fields._itemType) {
                case Dpr::UI::SettingMenuItem::ItemType::Gauge:
                    newValue = UnityEngine::Mathf::Min(item->fields._selectIndex + 1,maxValue);
                    break;

                default:
                    newValue = Dpr::UI::UIManager::Repeat(item->fields._selectIndex + 1, 0, maxValue);
                    break;
            }
        }
        else if (__this->cast<Dpr::UI::UIWindow>()->IsRepeatButton(Dpr::UI::UIManager::getClass()->static_fields->StickLRight, false))
        {
            newValue = UnityEngine::Mathf::Min(item->fields._selectIndex + 1,maxValue);
        }
        else if (__this->cast<Dpr::UI::UIWindow>()->IsPushButton(Dpr::UI::UIManager::getClass()->static_fields->StickLLeft, false))
        {
            switch (item->fields._itemType) {
                case Dpr::UI::SettingMenuItem::ItemType::Gauge:
                    newValue = UnityEngine::Mathf::Max(item->fields._selectIndex - 1,0);
                    break;

                default:
                    newValue = Dpr::UI::UIManager::Repeat(item->fields._selectIndex - 1, 0, maxValue);
                    break;
            }
        }
        else if (__this->cast<Dpr::UI::UIWindow>()->IsRepeatButton(Dpr::UI::UIManager::getClass()->static_fields->StickLLeft, false))
        {
            newValue = UnityEngine::Mathf::Max(item->fields._selectIndex - 1,0);
        }
        else
        { // No valid input
            return false;
        }

        __this->SetSelectValue(newValue);
        return true;
    }
};

HOOK_DEFINE_REPLACE(SettingWindow$$OpenDescriptionMessageWindow) {
    static void Callback(Dpr::UI::SettingWindow::Object* __this) {
        system_load_typeinfo(0x7c50);

        auto param = Dpr::MsgWindow::MsgWindowParam::newInstance();
        param->fields.useMsgFile = Dpr::Message::MessageManager::get_Instance()->GetMsgFile(System::String::Create("ss_option"));
        param->fields.labelName = __this->fields._activeItems->fields._items->m_Items[__this->fields._selectIndex]->fields._descMessagelabel;
        param->fields.inputEnabled = true;
        param->fields.inputCloseEnabled = false;

        switch (__this->fields._activeItems->fields._items->m_Items[__this->fields._selectIndex]->fields._configId) {
            case array_index(SETTINGS, "Level Cap"):
                Dpr::Message::MessageWordSetHelper::SetDigitWord(0, (int32_t)GetLevelCapLevel(GetLevelCapIndex()));
                break;
        }

        __this->virtual_OpenMessageWindow(param);
    }
};

HOOK_DEFINE_INLINE(SettingWindow$$OnUpdate_CONFIG$$IsEqual) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto __this = (DPData::CONFIG::Object*) ctx->X[0]; // PlayerWork's config
        auto t = (DPData::CONFIG::Object*) ctx->X[1]; // SettingWindow's _tempConfig

        for (int i=0; i<SETTING_COUNT; i++) {
            if (!IsEqualValue(__this, t, &getCustomSaveData()->settings, &tempExtraSettings, i)) {
                ctx->W[0] = false;
                return;
            }
        }

        ctx->W[0] = true;
    }
};

HOOK_DEFINE_INLINE(SettingWindow$$OnUpdate_ResetString) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        ctx->X[1] = (uint64_t)System::String::Create("SS_option_Reset");
    }
};

HOOK_DEFINE_INLINE(SettingWindow$$OnUpdate_AcceptString) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        ctx->X[1] = (uint64_t)System::String::Create("SS_option_ConfirmSave");
    }
};

HOOK_DEFINE_INLINE(SettingWindow$$OnUpdate_CancelString) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        ctx->X[1] = (uint64_t)System::String::Create("SS_option_CancelSave");
    }
};

HOOK_DEFINE_INLINE(SettingWindow$$OnUpdate_SavedString) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        ctx->X[1] = (uint64_t)System::String::Create("SS_option_DoneSave");
    }
};

HOOK_DEFINE_REPLACE(SettingWindow_OpOpen$$MoveNext) {
    static bool Callback(Dpr::UI::SettingWindow::_OpOpen_d__11::Object* __this) {
        system_load_typeinfo(0x927b);

        auto window = __this->fields.__4__this;

        switch (__this->fields.__1__state) {
            case 0: {
                __this->fields.__1__state = -1;
                window->cast<Dpr::UI::UIWindow>()->OnOpen(__this->fields.prevWindowId);

                Dpr::UI::UIManager::getClass()->initIfNeeded();
                auto keyguide = Dpr::UI::UIManager::get_Instance()->GetKeyguide(nullptr, true)->instance();
                keyguide->cast<UnityEngine::Component>()->get_transform()->SetParent(
                        window->cast<UnityEngine::Component>()->get_transform(), false);

                auto keyguideParam = Dpr::UI::Keyguide::Param::newInstance();

                auto itemParam = KeyguideItem::Param::newInstance();
                itemParam->fields.keyguideId = KeyguideID::OPTION_DECIDE;
                keyguideParam->fields.itemParams->Add(itemParam);

                itemParam = KeyguideItem::Param::newInstance();
                itemParam->fields.keyguideId = KeyguideID::OPTION_INITIALIZE;
                keyguideParam->fields.itemParams->Add(itemParam);

                itemParam = KeyguideItem::Param::newInstance();
                itemParam->fields.keyguideId = KeyguideID::OPTION_FINISH;
                keyguideParam->fields.itemParams->Add(itemParam);

                keyguide->Open(keyguideParam);

                memmove(&window->fields._tempConfig, PlayerWork::get_config(), sizeof(DPData::CONFIG::Object));
                memmove(&tempExtraSettings, &getCustomSaveData()->settings, sizeof(ExtraSettingsSaveData));

                window->fields._activeItems->Clear();

                auto parentTF = window->fields._scrollRect->fields.m_Content->cast<UnityEngine::Transform>();
                for (int i = 0; i < parentTF->get_childCount(); i++) {
                    auto child = parentTF->GetChild(i);
                    auto settingItem = child->cast<UnityEngine::Component>()->GetComponent(
                            UnityEngine::Component::Method$$SettingMenuItem$$GetComponent);

                    // Hide Kanji Mode if language is not Japanese
                    // Hide Camera Shake if Explorer's Kit is not obtained
                    if ((i == array_index(SETTINGS, "Kanji Mode") && Dpr::UI::UIManager::GetCurrentLangId() != Dpr::Message::MessageEnumData::MsgLangId::JPN) ||
                        (i == array_index(SETTINGS, "Camera Shake") && !FlagWork::GetFlag(FlagWork_Flag::FE_TANKENSETTO_GET))) {
                        settingItem->cast<UnityEngine::Component>()->get_gameObject()->SetActive(false);
                        continue;
                    }

                    settingItem->cast<UnityEngine::Component>()->get_gameObject()->SetActive(true);

                    auto selectIndex = GetSetting(PlayerWork::get_config(), &getCustomSaveData()->settings, i);
                    auto mi = *Dpr::UI::SettingWindow::Method$$OnMenuItemValueChaged;
                    auto onValueChanged = UnityEngine::Events::UnityAction::getClass(UnityEngine::Events::UnityAction::SettingMenuItem_TypeInfo)->newInstance(window, mi);
                    settingItem->Setup(i, selectIndex, System::String::Create(SETTING_DESCRIPTION_LABELS[i]), onValueChanged);

                    // Fix title text for reused vanilla items
                    if (i == array_index(SETTINGS, "Frame Rate")) {
                        auto titleText = settingItem->cast<UnityEngine::Component>()->GetComponentInChildren<Dpr::UI::UIText>(
                                true, UnityEngine::Component::Method$$UIText$$GetComponentInChildren);
                        auto valueText = settingItem->instance()->fields._texts->fields._items->m_Items[0];
                        if (titleText != nullptr && titleText != valueText) {
                            titleText->SetupMessage(nullptr, System::String::Create("SS_option_TargetFPS"));
                        }
                    }

                    window->fields._activeItems->Add(settingItem);
                }

                window->SetSelectIndex(0, true);
                window->fields._cursor->SetActive(true);

                __this->fields.__2__current = (Il2CppObject*)window->virtual_OpPlayOpenWindowAnimation(__this->fields.prevWindowId, nullptr);
                __this->fields.__1__state = 1;
                return true;
            }

            case 1: {
                __this->fields.__1__state = -1;

                SmartPoint::AssetAssistant::Sequencer::getClass()->initIfNeeded();
                auto addUpdate = SmartPoint::AssetAssistant::Sequencer::TickCallback::newInstance(window, *Dpr::UI::SettingWindow::Method$$OnUpdate);
                auto newUpdate = System::Delegate::Combine(SmartPoint::AssetAssistant::Sequencer::getClass()->static_fields->update->cast<System::Delegate>(), addUpdate->cast<System::Delegate>());
                SmartPoint::AssetAssistant::Sequencer::getClass()->static_fields->update = (SmartPoint::AssetAssistant::Sequencer::TickCallback::Object*)newUpdate;

                window->fields._input->fields._inputEnabled = true;
                window->OpenDescriptionMessageWindow();

                return false;
            }

            default:
                return false;
        }
    }
};

HOOK_DEFINE_REPLACE(SettingMenuItem$$SetSelectIndex) {
    static bool Callback(Dpr::UI::SettingMenuItem::Object* __this, int32_t selectIndex, bool isEqualChecked, bool isCallbacked) {
        system_load_typeinfo(0x7c42);

        if (isEqualChecked && __this->fields._selectIndex == selectIndex)
            return false;

        auto dispClass = Dpr::UI::SettingMenuItem::__DisplayClass32_0::newInstance();
        dispClass->fields.selectIndex = selectIndex;

        __this->fields._selectIndex = selectIndex;

        if (isCallbacked)
            __this->fields._onValueChanged->Invoke((Il2CppObject*)__this);

        switch (__this->fields._itemType) {
            case Dpr::UI::SettingMenuItem::ItemType::Selector: {
                for (int i=0; i<__this->fields._texts->fields._size; i++) {
                    auto text = __this->fields._texts->fields._items->m_Items[i];

                    int32_t index = (__this->fields._selectBg->cast<UnityEngine::Behaviour>()->get_enabled() ? 0 : 2) +
                                    (__this->fields._selectIndex == i ? 0 : 1);
                    text->virtual_set_color(__this->fields._selectorParam->fields.textColors->m_Items[index]);
                }
            }
            break;

            case Dpr::UI::SettingMenuItem::ItemType::Gauge: {
                __this->fields._slider->virtual_set_value(selectIndex);

                auto mi = *Dpr::UI::SettingMenuItem::__DisplayClass32_0::Method$$SetSelectIndex_b0;
                auto onSet = UnityEngine::Events::UnityAction::getClass(UnityEngine::Events::UnityAction::void_TypeInfo)->newInstance(dispClass, mi);
                __this->fields._sliderText->SetFormattedText(onSet, nullptr, nullptr);

                int32_t index = __this->fields._selectBg->cast<UnityEngine::Behaviour>()->get_enabled() ? 0 : 1;
                __this->fields._gaugeSelectorParam->fields.gaugeBg->virtual_set_color(__this->fields._gaugeSelectorParam->fields.gaugeBgColors->m_Items[index]);
                __this->fields._gaugeSelectorParam->fields.gauge->virtual_set_color(__this->fields._gaugeSelectorParam->fields.gaugeColors->m_Items[index]);
                __this->fields._gaugeSelectorParam->fields.handle->set_sprite(__this->fields._gaugeSelectorParam->fields.spriteHandles->m_Items[index]);
                __this->fields._sliderText->virtual_set_color(__this->fields._gaugeSelectorParam->fields.textColors->m_Items[index]);
            }
            break;

            case Dpr::UI::SettingMenuItem::ItemType::WindowSelector: {
                nn::vector<std::string> options;
                switch (__this->fields._configId) {
                    case array_index(SETTINGS, "Change Window"):
                    default:
                        __this->fields._texts->fields._items->m_Items[0]->SetupMessage(nullptr, System::String::Create(WINDOW_FRAME_LABELS[__this->fields._selectIndex]));
                        break;

                    case array_index(SETTINGS, "Trainer Sets"):
                        __this->fields._texts->fields._items->m_Items[0]->SetupMessage(nullptr, System::String::Create(GAME_MODE_LABELS[__this->fields._selectIndex]));
                        break;

                    case array_index(SETTINGS, "Team Randomization"):
                        __this->fields._texts->fields._items->m_Items[0]->SetupMessage(nullptr, System::String::Create(RANDOM_TEAM_MODE_LABELS[__this->fields._selectIndex]));
                        break;

                    case array_index(SETTINGS, "Frame Rate"): {
                        static const char* FPS_LABELS[] = { "SS_option_TargetFPS_01", "SS_option_TargetFPS_02", "SS_option_TargetFPS_03", "SS_option_TargetFPS_04" };
                        __this->fields._texts->fields._items->m_Items[0]->SetupMessage(nullptr, System::String::Create(FPS_LABELS[__this->fields._selectIndex]));
                        break;
                    }
                }

                int32_t index = __this->fields._selectBg->cast<UnityEngine::Behaviour>()->get_enabled() ? 0 : 1;
                __this->fields._texts->fields._items->m_Items[0]->virtual_set_color(__this->fields._windowSelectorParam->fields.textColors->m_Items[index]);

                for (uint64_t i=0; i<__this->fields._windowSelectorParam->fields.arrows->max_length; i++)
                    __this->fields._windowSelectorParam->fields.arrows->m_Items[i]->virtual_set_color(__this->fields._windowSelectorParam->fields.arrowColors->m_Items[index]);
            }
            break;
        }

        return true;
    }
};

void exl_settings_main() {
    CONFIG$$GetValue::InstallAtOffset(0x02299720);

    ConfigWork$$InvokeChangedValues::InstallAtOffset(0x018ed400);

    SettingWindow$$InitializeSettings::InstallAtOffset(0x01d413b0);
    SettingWindow$$OnMenuItemValueChaged::InstallAtOffset(0x01d401f0);
    SettingWindow$$RevertSettings::InstallAtOffset(0x01d411b0);
    SettingWindow$$AcceptSettings::InstallAtOffset(0x01d414d0);
    SettingWindow$$UpdateSelectValue::InstallAtOffset(0x01d40ec0);
    SettingWindow$$OpenDescriptionMessageWindow::InstallAtOffset(0x01d41c30);

    SettingWindow$$OnUpdate_CONFIG$$IsEqual::InstallAtOffset(0x01d40624);
    SettingWindow$$OnUpdate_ResetString::InstallAtOffset(0x01d40930);
    SettingWindow$$OnUpdate_AcceptString::InstallAtOffset(0x01d40754);
    SettingWindow$$OnUpdate_CancelString::InstallAtOffset(0x01d40800);
    SettingWindow$$OnUpdate_SavedString::InstallAtOffset(0x01d41ffc);
    SettingWindow$$OnUpdate_SavedString::InstallAtOffset(0x01d4223c);
    SettingWindow$$OnUpdate_SavedString::InstallAtOffset(0x01d4247c);

    SettingWindow_OpOpen$$MoveNext::InstallAtOffset(0x01d42830);

    SettingMenuItem$$SetSelectIndex::InstallAtOffset(0x01d3f1a0);
}
