#include "exlaunch.hpp"

#include "data/setting_labels.h"
#include "data/settings.h"
#include "data/utils.h"

#include "externals/ConfigWork.h"
#include "externals/Dpr/UI/Keyguide.h"
#include "externals/Dpr/UI/SettingWindow.h"
#include "externals/Dpr/UI/UIManager.h"
#include "externals/FlagWork.h"
#include "externals/PlayerWork.h"
#include "externals/SmartPoint/AssetAssistant/Sequencer.h"
#include "save/save.h"

#include "logger/logger.h"

static ExtraSettingsSaveData tempExtraSettings;

/* Captures all current custom setting flag/work states when the SettingsWindow is opened. */
void StoreCustomTempConfig() {
    FlagWork::SetFlag(FlagWork_Flag::FLAG_TEMP_DISABLE_EXP_SHARE,
                      FlagWork::GetFlag(FlagWork_Flag::FLAG_DISABLE_EXP_SHARE));
    FlagWork::SetFlag(FlagWork_Flag::FLAG_TEMP_DISABLE_AFFECTION,
                      FlagWork::GetFlag(FlagWork_Flag::FLAG_DISABLE_AFFECTION));
    FlagWork::SetFlag(FlagWork_Flag::FLAG_TEMP_DISABLE_LEVEL_CAP,
                      FlagWork::GetFlag(FlagWork_Flag::FLAG_DISABLE_LEVEL_CAP));
    FlagWork::SetFlag(FlagWork_Flag::FLAG_TEMP_DISABLE_VISIBLE_SHINY_EGGS,
                      FlagWork::GetFlag(FlagWork_Flag::FLAG_DISABLE_VISIBLE_SHINY_EGGS));
    FlagWork::SetWork(FlagWork_Work::WK_TEMP_GAME_MODE,
                      FlagWork::GetWork(FlagWork_Work::WK_GAME_MODE));
    FlagWork::SetWork(FlagWork_Work::WK_TEMP_RANDOM_TEAMS,
                      FlagWork::GetWork(FlagWork_Work::WK_RANDOM_TEAMS));
}

void OnValueChanged(int32_t configId, int32_t value) {
    ConfigWork::getClass()->initIfNeeded();
    if (ConfigWork::getClass()->static_fields->onValueChanged != nullptr)
        ConfigWork::getClass()->static_fields->onValueChanged->Invoke(configId, value);
}

void SetSetting(DPData::CONFIG::Object* config, ExtraSettingsSaveData* extraSettings, int32_t configId, int32_t value) {
    switch (configId) {
        case array_index(SETTINGS, "EXP Share"):
            extraSettings->expShareEnabled = value != 0;
            break;
        case array_index(SETTINGS, "Affection"):
            extraSettings->affectionEnabled = value != 0;
            break;
        case array_index(SETTINGS, "Level Cap"):
            extraSettings->levelCapEnabled = value != 0;
            break;
        case array_index(SETTINGS, "Visible Shiny Eggs"):
            extraSettings->shinyEggsEnabled = value != 0;
            break;
        case array_index(SETTINGS, "Game Mode"):
            extraSettings->gameMode = (ExtraSettingsSaveData::GameMode)value;
            break;
        case array_index(SETTINGS, "Team Randomization"):
            extraSettings->randomTeamMode = (ExtraSettingsSaveData::RandomTeamMode)value;
            break;
        default:
            config->SetValue(configId, value);
            break;
    }
}

int32_t GetSetting(DPData::CONFIG::Object* config, ExtraSettingsSaveData* extraSettings, int32_t configId) {
    switch (configId) {
        case array_index(SETTINGS, "EXP Share"):
            return extraSettings->expShareEnabled ? 1 : 0;
        case array_index(SETTINGS, "Affection"):
            return extraSettings->affectionEnabled ? 1 : 0;
        case array_index(SETTINGS, "Level Cap"):
            return extraSettings->levelCapEnabled ? 1 : 0;
        case array_index(SETTINGS, "Visible Shiny Eggs"):
            return extraSettings->shinyEggsEnabled ? 1 : 0;
        case array_index(SETTINGS, "Game Mode"):
            return (int32_t)extraSettings->gameMode;
        case array_index(SETTINGS, "Team Randomization"):
            return (int32_t)extraSettings->randomTeamMode;
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
        case array_index(SETTINGS, "Game Mode"):
            return extraSettings->gameMode == otherExtraSettings->gameMode;
        case array_index(SETTINGS, "Team Randomization"):
            return extraSettings->randomTeamMode == otherExtraSettings->randomTeamMode;
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
    memmove(&prevConfig, &config, sizeof(DPData::CONFIG::Object));

    ExtraSettingsSaveData prevExtraSettings {};
    memmove(&prevExtraSettings, &extraSettings, sizeof(ExtraSettingsSaveData));

    DefaultSettings(config, extraSettings);

    CheckAndInvokeChangedValues(config, &prevConfig, extraSettings, &prevExtraSettings);
}

/* Ensures that out of bounds values can't be used from unintended selector behaviour */
int32_t ValueBoundsCheck(int32_t value, int32_t configId) {
    if (configId == 18) {
        if (value > 1 && value < 23) return 0;
        else if (value == 23) return 1;
    }

    else if (configId == 19) {
        if (value > 2 && value < 23) return 0;
        else if (value == 23) return 2;
    }

    return value;
}

/* Prevents out of bounds selectIndexes and restores selector wrap-around while restricting the index range
 * from 0-23 down to the applicable size */
void IndexBoundsCheck(Dpr::UI::SettingMenuItem::Object* __this) {
    int32_t itemType = __this->fields._itemType;
    int32_t selectIndex = __this->fields._selectIndex;

    if (itemType == 3) {
//        if (selectIndex > 1 && selectIndex < 23) {
//            selectIndex = 0;
//        }
//        else if (selectIndex == 23) {
//            selectIndex = 1;
//        }
        selectIndex = 0; // Leaves NatDex Mode & Challenge Mode unreachable for now
    }

    else if (itemType == 4) {
        if (selectIndex > 2 && selectIndex < 23) {
            selectIndex = 0;
        }
        else if (selectIndex == 23) {
            selectIndex = 2;
        }
    }

    __this->fields._selectIndex = selectIndex;
}

template <size_t N>
void AddSetting(Dpr::UI::SettingWindow::_OpOpen_d__11::Object* __this, int configId, int selectedIndex, const char* title, const char* description, const char* const (&optionNames)[N], int position, bool isFrame) {
    auto window = __this->fields.__4__this;

    system_load_typeinfo(0x870a);

    auto contentRoot = window->fields._scrollRect->fields.m_Content;
    auto cloneType = (isFrame) ? "WindowChange" : "BattleAnim";
    auto child = contentRoot->cast<UnityEngine::Transform>()->Find(System::String::Create(cloneType));
    auto origItem = child->GetComponent(UnityEngine::Component::Method$$SettingMenuItem$$GetComponent);
    auto newTransform = UnityEngine::_Object::Instantiate<UnityEngine::Transform>(child);
    newTransform->SetParent(contentRoot->cast<UnityEngine::Transform>(), false);
    Dpr::UI::SettingMenuItem::Object* newItem = newTransform->GetComponent(UnityEngine::Component::Method$$SettingMenuItem$$GetComponent);

    if (isFrame) {
        if (configId == 18)
            newItem->fields._itemType = 3;
        else if (configId == 19)
            newItem->fields._itemType = 4;
    }

    newItem->Setup(configId, selectedIndex, System::String::Create(description), origItem->fields._onValueChanged);
    auto textRoot = newTransform->GetChild(1);
    auto titleChild = textRoot->GetChild(0);

    system_load_typeinfo(0x870a);

    auto titleComponent = titleChild->GetComponent(UnityEngine::Component::Method$$UIText$$GetComponent);
    titleComponent->SetupMessage(
            System::String::Create("ss_option"),
            System::String::Create(title)
    );

    if (isFrame) {
        int32_t currentMode;
        switch (configId) {
            case 18:
                currentMode = FlagWork::GetWork(FlagWork_Work::WK_GAME_MODE);
                break;
            case 19:
                currentMode = FlagWork::GetWork(FlagWork_Work::WK_RANDOM_TEAMS);
                break;
            default:
                currentMode = 0;
                break;
        }

        auto options = textRoot->GetChild({ 1, 2 });
        auto option = options->GetChild({ 1, 0 })->GetComponent(UnityEngine::Component::Method$$UIText$$GetComponent);
        option->SetupMessage(
                System::String::Create("ss_option"),
                System::String::Create(optionNames[currentMode]));

    }

    else {
        auto options = textRoot->GetChild({ 1, 0 });
        for (uint64_t i = 0; i < N; i++) {
            auto option = options->GetChild({ (int32_t)i, 0 })->GetComponent(UnityEngine::Component::Method$$UIText$$GetComponent);
            option->SetupMessage(
                    System::String::Create("ss_option"),
                    System::String::Create(optionNames[i]));
        }
    }


    newTransform->SetSiblingIndex(position+1);
    window->fields._activeItems->insert(newItem, position);
}

HOOK_DEFINE_TRAMPOLINE(AddSettingsEntries) {
    static bool Callback(Dpr::UI::SettingWindow::_OpOpen_d__11::Object* __this) {
        auto res = Orig(__this);

        auto window = __this->fields.__4__this;
        if (window->fields._activeItems->fields._size > 13) {
            return res;
        }

        AddSetting(__this, 14,
                   FlagWork::GetFlag(FlagWork_Flag::FLAG_DISABLE_EXP_SHARE) ? 1 : 0,
                   "SS_option_110", "SS_option_113", ON_OFF_NAMES, 1, false);
        AddSetting(__this, 15,
                   FlagWork::GetFlag(FlagWork_Flag::FLAG_DISABLE_AFFECTION) ? 1 : 0,
                   "SS_option_109", "SS_option_112", ON_OFF_NAMES, 2, false);
        AddSetting(__this, 16,
                   FlagWork::GetFlag(FlagWork_Flag::FLAG_DISABLE_LEVEL_CAP) ? 1 : 0,
                   "SS_option_111", "SS_option_114", ON_OFF_NAMES, 3, false);
        AddSetting(__this, 17,
                   FlagWork::GetFlag(FlagWork_Flag::FLAG_DISABLE_VISIBLE_SHINY_EGGS) ? 1 : 0,
                   "SS_option_125", "SS_option_126", ON_OFF_NAMES, 4, false);
        AddSetting(__this, 18, FlagWork::GetWork(FlagWork_Work::WK_GAME_MODE),
                   "SS_option_115", "SS_option_116", GAME_MODES, 5, true);
        AddSetting(__this, 19, FlagWork::GetWork(FlagWork_Work::WK_RANDOM_TEAMS),
                   "SS_option_120", "SS_option_121", RANDOM_TEAM_MODES, 6, true);

        StoreCustomTempConfig();

        return res;
    }
};





HOOK_DEFINE_INLINE(SetSelectIndex) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto __this = (Dpr::UI::SettingMenuItem::Object*) ctx->X[19];
        auto selectIndex = __this->fields._selectIndex;
        auto UITextObjList = __this->fields._texts;
        auto UITextObj = UITextObjList->fields._items->m_Items[0];

        auto itemType = __this->fields._itemType;

        switch(itemType) {
            case 2: {
                UITextObj->SetupMessage(System::String::Create("ss_option"),
                                        System::String::Create(WINDOW_FRAME_TYPES[selectIndex]));
                break;
            }

            case 3: {
                UITextObj->SetupMessage(System::String::Create("ss_option"),
                                        System::String::Create(GAME_MODES[selectIndex]));
                break;
            }

            case 4: {
                UITextObj->SetupMessage(System::String::Create("ss_option"),
                                        System::String::Create(RANDOM_TEAM_MODES[selectIndex]));
                break;
            }
        }

    }

};

HOOK_DEFINE_INLINE(Setup) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto __this = (Dpr::UI::SettingMenuItem::Object*) ctx->X[19];
        int32_t itemType = __this->fields._itemType;
        if (itemType == 3 || itemType == 4) {
            IndexBoundsCheck(__this);
            ctx->W[8] = 2;
        }

        else {
            ctx->W[8] = itemType;
        }
    }
};

HOOK_DEFINE_INLINE(SetupContent) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto __this = (Dpr::UI::SettingMenuItem::Object*) ctx->X[19];
        int32_t itemType = __this->fields._itemType;
        if (itemType == 3 || itemType == 4) {
            IndexBoundsCheck(__this);
            ctx->W[8] = 2;
        }

        else {
            ctx->W[8] = itemType;
        }
    }
};







// Default to checking the save's config in case we still call this method (happens in a few places)
HOOK_DEFINE_TRAMPOLINE(CONFIG$$GetValue) {
    static int32_t Callback(DPData::CONFIG::Object* config, int32_t configId) {
        return GetSetting(config, &getCustomSaveData()->settings, configId);
    }
};

// Only really used in GameManager's OnetimeInitializeOperation, we do want to keep this
HOOK_DEFINE_REPLACE(ConfigWork$$InvokeChangedValues) {
    static void Callback(DPData::CONFIG::Object* config) {
        for (int i=0; i<SETTING_COUNT; i++) {
            OnValueChanged(i, GetSetting(config, &getCustomSaveData()->settings, i));
        }
    }
};

HOOK_DEFINE_REPLACE(SettingWindow$$InitializeSettings) {
    static void Callback(Dpr::UI::SettingWindow::Object *__this) {
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
        SetSetting(&__this->fields._tempConfig, &tempExtraSettings, menuItem->fields._configId, menuItem->fields._selectIndex);
        OnValueChanged(menuItem->fields._configId, menuItem->fields._selectIndex);
    }
};

HOOK_DEFINE_REPLACE(SettingWindow$$RevertSettings) {
    static void Callback(Dpr::UI::SettingWindow::Object *__this) {
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
        PlayerWork::getClass()->initIfNeeded();
        memmove(PlayerWork::get_config(), &__this->fields._tempConfig, sizeof(DPData::CONFIG::Object));

        tempExtraSettings.Initialize();
        memmove(&getCustomSaveData()->settings, &tempExtraSettings, sizeof(ExtraSettingsSaveData));

        InvokeChangedValues(PlayerWork::get_config(), &getCustomSaveData()->settings);
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

HOOK_DEFINE_REPLACE(SettingWindow_OpOpen$$MoveNext) {
    static bool Callback(Dpr::UI::SettingWindow::_OpOpen_d__11::Object* __this) {
        auto window = __this->fields.__4__this;

        switch (__this->fields.__1__state) {
            case 0: {
                __this->fields.__1__state = -1;
                window->cast<Dpr::UI::UIWindow>()->OnOpen(__this->fields.prevWindowId);

                Dpr::UI::UIManager::getClass()->initIfNeeded();
                auto keyguide = Dpr::UI::UIManager::instance()->GetKeyguide(nullptr, true)->instance();
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

                nn::vector<std::string> descs { // Vanilla
                        "SS_option_005", "SS_option_017", "SS_option_100", "SS_option_009", "SS_option_013",
                        "SS_option_025", "SS_option_074", "SS_option_033", "SS_option_108", "SS_option_035",
                        "SS_option_070", "SS_option_041", "SS_option_043", "SS_option_045",
                };

                descs.emplace_back("SS_option_113"); // EXP Share
                descs.emplace_back("SS_option_112"); // Affection
                descs.emplace_back("SS_option_114"); // Level Cap
                descs.emplace_back("SS_option_126"); // Shiny Eggs
                descs.emplace_back("SS_option_116"); // Game Mode
                descs.emplace_back("SS_option_121"); // Team Random

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
                    settingItem->Setup(i, selectIndex, System::String::Create(descs[i].c_str()), onValueChanged);
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

void exl_settings_main() {
    /*AddSettingsEntries::InstallAtOffset(0x01d42830);
    SetSelectIndex::InstallAtOffset(0x01d3fda8);
    Setup::InstallAtOffset(0x01d3f240);
    SetupContent::InstallAtOffset(0x01d3efa8);*/


    CONFIG$$GetValue::InstallAtOffset(0x02299720);

    ConfigWork$$InvokeChangedValues::InstallAtOffset(0x018ed400);

    SettingWindow$$InitializeSettings::InstallAtOffset(0x01d413b0);
    SettingWindow$$OnMenuItemValueChaged::InstallAtOffset(0x022998a0);
    SettingWindow$$RevertSettings::InstallAtOffset(0x01d411b0);
    SettingWindow$$AcceptSettings::InstallAtOffset(0x01d414d0);

    SettingWindow$$OnUpdate_CONFIG$$IsEqual::InstallAtOffset(0x01d40624);

    SettingWindow_OpOpen$$MoveNext::InstallAtOffset(0x01d42830);
}
