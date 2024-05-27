#include "exlaunch.hpp"

#include "data/setting_labels.h"
#include "externals/Dpr/UI/SettingWindow.h"
#include "externals/FlagWork.h"
#include "externals/PlayerWork.h"

#include "logger/logger.h"
#include "ui/ui.h"

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
        if (selectIndex > 1 && selectIndex < 23) {
            selectIndex = 0;
        }
        else if (selectIndex == 23) {
            selectIndex = 1; // Leaves Challenge Mode unreachable for now
        }
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
                   FlagWork::GetFlag(FlagWork_Flag::FLAG_DISABLE_EXP_SHARE) ? 0 : 1,
                   "SS_option_110", "SS_option_113", ON_OFF_NAMES, 1, false);
        AddSetting(__this, 15,
                   FlagWork::GetFlag(FlagWork_Flag::FLAG_DISABLE_AFFECTION) ? 0 : 1,
                   "SS_option_109", "SS_option_112", ON_OFF_NAMES, 2, false);
        AddSetting(__this, 16,
                   FlagWork::GetFlag(FlagWork_Flag::FLAG_DISABLE_LEVEL_CAP) ? 0 : 1,
                   "SS_option_111", "SS_option_114", ON_OFF_NAMES, 3, false);
        AddSetting(__this, 17,
                   FlagWork::GetFlag(FlagWork_Flag::FLAG_DISABLE_VISIBLE_SHINY_EGGS) ? 0 : 1,
                   "SS_option_125", "SS_option_126", ON_OFF_NAMES, 4, false);
        AddSetting(__this, 18, FlagWork::GetWork(FlagWork_Work::WK_GAME_MODE),
                   "SS_option_115", "SS_option_116", GAME_MODES, 5, true);
        AddSetting(__this, 19, FlagWork::GetWork(FlagWork_Work::WK_RANDOM_TEAMS),
                   "SS_option_120", "SS_option_121", RANDOM_TEAM_MODES, 6, true);

        StoreCustomTempConfig();

        return res;
    }
};

HOOK_DEFINE_TRAMPOLINE(GetSetting) {
    static int32_t Callback(DPData::CONFIG::Object* __this, int32_t id) {
        switch (id) {
            case 14:
                return !FlagWork::GetFlag(FlagWork_Flag::FLAG_DISABLE_EXP_SHARE) ? 0 : 1;
            case 15:
                return !FlagWork::GetFlag(FlagWork_Flag::FLAG_DISABLE_AFFECTION) ? 0 : 1;
            case 16:
                return !FlagWork::GetFlag(FlagWork_Flag::FLAG_DISABLE_LEVEL_CAP) ? 0 : 1;
            case 17:
                return !FlagWork::GetFlag(FlagWork_Flag::FLAG_DISABLE_VISIBLE_SHINY_EGGS) ? 0 : 1;
            case 18:
                return FlagWork::GetWork(FlagWork_Work::WK_GAME_MODE);
            case 19:
                return FlagWork::GetWork(FlagWork_Work::WK_RANDOM_TEAMS);
            default:
                return Orig(__this, id);
        }
    }
};

/* Changes "Config" max length to 20 (19). This means when Dpr.UI.SettingWindow.OnUpdate() calls DpData.Config.Equal,
 * it passes our extra configIds through DpData.Config.IsEqualValue. Consequently, the B button save settings check
 * is called on our changed custom settings and allows for Dpr.UI.SettingWindow.RevertSettings to be called.
 * */
HOOK_DEFINE_INLINE(IsEqual) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        ctx->W[22] = 20;
    }
};

HOOK_DEFINE_TRAMPOLINE(RevertSettings) {
    static void Callback(Dpr::UI::SettingWindow::Object* __this) {
        Orig(__this);
        bool settingIsEqual;
        auto Config = PlayerWork::get_config();
        auto tempConfig = __this->fields._tempConfig;
        for (int i = 14; i < 20; i++) {
            settingIsEqual = Config->IsEqualValue(i, tempConfig);
            if (!settingIsEqual) {
                switch (i) {
                    case 14: {
                        FlagWork::SetFlag(FlagWork_Flag::FLAG_DISABLE_EXP_SHARE,
                                          FlagWork::GetFlag(FlagWork_Flag::FLAG_TEMP_DISABLE_EXP_SHARE));
                        break;
                    }
                    case 15: {
                        FlagWork::SetFlag(FlagWork_Flag::FLAG_DISABLE_AFFECTION,
                                          FlagWork::GetFlag(FlagWork_Flag::FLAG_TEMP_DISABLE_AFFECTION));
                        break;
                    }
                    case 16: {
                        FlagWork::SetFlag(FlagWork_Flag::FLAG_DISABLE_LEVEL_CAP,
                                          FlagWork::GetFlag(FlagWork_Flag::FLAG_TEMP_DISABLE_LEVEL_CAP));
                        break;
                    }
                    case 17: {
                        FlagWork::SetFlag(FlagWork_Flag::FLAG_DISABLE_VISIBLE_SHINY_EGGS,
                                          FlagWork::GetFlag(FlagWork_Flag::FLAG_TEMP_DISABLE_VISIBLE_SHINY_EGGS));
                        break;
                    }
                    case 18: {
                        FlagWork::SetWork(FlagWork_Work::WK_GAME_MODE,
                                          FlagWork::GetWork(FlagWork_Work::WK_TEMP_GAME_MODE));
                        break;
                    }
                    case 19: {
                        FlagWork::SetWork(FlagWork_Work::WK_RANDOM_TEAMS,
                                          FlagWork::GetWork(FlagWork_Work::WK_TEMP_RANDOM_TEAMS));
                        break;
                    }
                    default: break;
                }
            }
        }
    }
};

HOOK_DEFINE_TRAMPOLINE(SetSetting) {
    static void Callback(DPData::CONFIG::Object* __this, int32_t id, int32_t value) {
        int32_t checkedValue;
        switch (id) {
            case 14:
                FlagWork::SetFlag(FlagWork_Flag::FLAG_DISABLE_EXP_SHARE, value == 0 ? 1 : 0);
                break;
            case 15:
                FlagWork::SetFlag(FlagWork_Flag::FLAG_DISABLE_AFFECTION, value == 0 ? 1 : 0);
                break;
            case 16:
                FlagWork::SetFlag(FlagWork_Flag::FLAG_DISABLE_LEVEL_CAP, value == 0 ? 1 : 0);
                break;
            case 17:
                FlagWork::SetFlag(FlagWork_Flag::FLAG_DISABLE_VISIBLE_SHINY_EGGS, value == 0 ? 1 : 0);
                break;
            case 18:
                checkedValue = ValueBoundsCheck(value, id);
                FlagWork::SetWork(FlagWork_Work::WK_GAME_MODE, checkedValue);
                break;
            case 19:
                checkedValue = ValueBoundsCheck(value, id);
                FlagWork::SetWork(FlagWork_Work::WK_RANDOM_TEAMS, checkedValue);
                break;
            default:
                Orig(__this, id, value);
                break;
        }
    }
};

HOOK_DEFINE_TRAMPOLINE(SettingIsEqual) {
    static bool Callback(DPData::CONFIG::Object* __this, int32_t id, DPData::CONFIG::Object* other) {
        switch (id) {
            case 14:
                return (FlagWork::GetFlag(FlagWork_Flag::FLAG_TEMP_DISABLE_EXP_SHARE) == FlagWork::GetFlag(
                        FlagWork_Flag::FLAG_DISABLE_EXP_SHARE));
            case 15:
                return (FlagWork::GetFlag(FlagWork_Flag::FLAG_TEMP_DISABLE_AFFECTION) == FlagWork::GetFlag(
                        FlagWork_Flag::FLAG_DISABLE_AFFECTION));
            case 16:
                return (FlagWork::GetFlag(FlagWork_Flag::FLAG_TEMP_DISABLE_LEVEL_CAP) == FlagWork::GetFlag(
                        FlagWork_Flag::FLAG_DISABLE_LEVEL_CAP));
            case 17:
                return (FlagWork::GetFlag(FlagWork_Flag::FLAG_TEMP_DISABLE_VISIBLE_SHINY_EGGS) == FlagWork::GetFlag(
                        FlagWork_Flag::FLAG_DISABLE_VISIBLE_SHINY_EGGS));
            case 18:
                return (FlagWork::GetWork(FlagWork_Work::WK_TEMP_GAME_MODE) == FlagWork::GetWork(
                        FlagWork_Work::WK_GAME_MODE));
            case 19:
                return (FlagWork::GetWork(FlagWork_Work::WK_TEMP_RANDOM_TEAMS) == FlagWork::GetWork(
                        FlagWork_Work::WK_RANDOM_TEAMS));
            default:
                return Orig(__this, id, other);
        }
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

void exl_settings_main() {
    AddSettingsEntries::InstallAtOffset(0x01d42830);
    GetSetting::InstallAtOffset(0x02299720);
    IsEqual::InstallAtOffset(0x02299ac4);
    RevertSettings::InstallAtOffset(0x01d411b0);
    SetSelectIndex::InstallAtOffset(0x01d3fda8);
    SetSetting::InstallAtOffset(0x022997d0);
    SettingIsEqual::InstallAtOffset(0x022998a0);
    Setup::InstallAtOffset(0x01d3f240);
    SetupContent::InstallAtOffset(0x01d3efa8);
}
