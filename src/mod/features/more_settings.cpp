#include "exlaunch.hpp"

#include "memory/vector.h"

#include "externals/Dpr/UI/SettingWindow.h"
#include "externals/FlagWork.h"
#include "externals/UnityEngine/UI/HorizontalLayoutGroup.h"
#include "externals/XMenuTopItem.h"
#include "externals/Dpr/Message/MessageMsgFile.h"
#include "externals/PlayerWork.h"

#include "logger/logger.h"
#include "ui/ui.h"

using namespace UnityEngine::Events;


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

int32_t valueBoundsCheck(int32_t value) {
    if (value > 2 && value < 24)
        return 0;
    return value;
}

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
                checkedValue = valueBoundsCheck(value);
                FlagWork::SetWork(FlagWork_Work::WK_GAME_MODE, checkedValue);
                Logger::log("[SetSetting] WK_GAME_MODE: %d\n", FlagWork::GetWork(FlagWork_Work::WK_GAME_MODE));
                break;
            case 19:
                checkedValue = valueBoundsCheck(value);
                FlagWork::SetWork(FlagWork_Work::WK_RANDOM_TEAMS, checkedValue);
                Logger::log("[SetSetting] WK_RANDOM_TEAMS: %d\n", FlagWork::GetWork(FlagWork_Work::WK_RANDOM_TEAMS));
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
                }
            }
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

void AddSetting(Dpr::UI::SettingWindow::_OpOpen_d__11::Object* __this, int configId, int selectedIndex, const char* title, const char* description, nn::vector<const char*> optionNames, int position, bool isFrame) {
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
        for (uint64_t i=0; i<optionNames.size(); i++) {
            auto option = options->GetChild({ (int32_t)i, 0 })->GetComponent(UnityEngine::Component::Method$$UIText$$GetComponent);
            option->SetupMessage(
                    System::String::Create("ss_option"),
                    System::String::Create(optionNames[i]));
        }
    }


    newTransform->SetSiblingIndex(position+1);
    window->fields._activeItems->insert(newItem, position);
}

/* Captures all current custom setting flag/work states when the SettingsWindow is opened. */
void storeCustomTempConfig() {
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

HOOK_DEFINE_TRAMPOLINE(AddSettingsEntries) {
    static bool Callback(Dpr::UI::SettingWindow::_OpOpen_d__11::Object* __this) {
        auto res = Orig(__this);

        auto window = __this->fields.__4__this;
        if (window->fields._activeItems->fields._size > 13) {
            return res;
        }
        nn::vector<const char*> onOffNames = { "SS_option_007", "SS_option_008" };
        nn::vector<const char*> gameModes = { "SS_option_117", "SS_option_118", "SS_option_119" };
        nn::vector<const char*> teamModes = { "SS_option_122", "SS_option_123", "SS_option_124" };
        AddSetting(__this, 14,
                   FlagWork::GetFlag(FlagWork_Flag::FLAG_DISABLE_EXP_SHARE) ? 0 : 1,
                   "SS_option_110", "SS_option_113", onOffNames, 1, false);
        AddSetting(__this, 15,
                   FlagWork::GetFlag(FlagWork_Flag::FLAG_DISABLE_AFFECTION) ? 0 : 1,
                   "SS_option_109", "SS_option_112", onOffNames, 2, false);
        AddSetting(__this, 16,
                   FlagWork::GetFlag(FlagWork_Flag::FLAG_DISABLE_LEVEL_CAP) ? 0 : 1,
                   "SS_option_111", "SS_option_114", onOffNames, 3, false);
        AddSetting(__this, 17,
                   FlagWork::GetFlag(FlagWork_Flag::FLAG_DISABLE_VISIBLE_SHINY_EGGS) ? 0 : 1,
                   "SS_option_125", "SS_option_126", onOffNames, 4, false);
        AddSetting(__this, 18, FlagWork::GetWork(FlagWork_Work::WK_GAME_MODE),
                   "SS_option_115", "SS_option_116", gameModes, 5, true);
        AddSetting(__this, 19, FlagWork::GetWork(FlagWork_Work::WK_RANDOM_TEAMS),
                   "SS_option_120", "SS_option_121", teamModes, 6, true);

        storeCustomTempConfig();

        return res;
    }
};

void indexBoundsCheck(Dpr::UI::SettingMenuItem::Object* __this) {
    if (__this->fields._selectIndex > 2) {
        __this->fields._selectIndex = 0;
    }

    else if (__this->fields._selectIndex < 0) {
        __this->fields._selectIndex = 2;
    }
}

HOOK_DEFINE_INLINE(SetSelectIndex) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto __this = (Dpr::UI::SettingMenuItem::Object*) ctx->X[19];
        auto selectIndex = __this->fields._selectIndex;
        auto UITextObjList = __this->fields._texts;
        auto UITextObj = UITextObjList->fields._items->m_Items[0];

        nn::vector<const char*> gameModes = { "SS_option_117", "SS_option_118", "SS_option_119" };
        nn::vector<const char*> teamModes = { "SS_option_122", "SS_option_123", "SS_option_124" };
        nn::vector<const char*> types = {
                "SS_option_080", "SS_option_081", "SS_option_082", "SS_option_083",
                "SS_option_084", "SS_option_085", "SS_option_086", "SS_option_087",
                "SS_option_088", "SS_option_089", "SS_option_090", "SS_option_091",
                "SS_option_092", "SS_option_093", "SS_option_094", "SS_option_095",
                "SS_option_096", "SS_option_097", "SS_option_098", "SS_option_099",
                "SS_option_101", "SS_option_102", "SS_option_103", "SS_option_104"
        };


        auto itemType = __this->fields._itemType;

        switch(itemType) {
            case 2: {
                UITextObj->SetupMessage(System::String::Create("ss_option"),
                                        System::String::Create(types[selectIndex]));
                break;
            }

            case 3: {
                UITextObj->SetupMessage(System::String::Create("ss_option"),
                                        System::String::Create(gameModes[selectIndex]));
                break;
            }

            case 4: {
                UITextObj->SetupMessage(System::String::Create("ss_option"),
                                        System::String::Create(teamModes[selectIndex]));
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
            indexBoundsCheck(__this);
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
            indexBoundsCheck(__this);
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
    SetSetting::InstallAtOffset(0x022997d0);
    SettingIsEqual::InstallAtOffset(0x022998a0);

    Setup::InstallAtOffset(0x01d3f240);
    SetSelectIndex::InstallAtOffset(0x01d3fda8);
    SetupContent::InstallAtOffset(0x01d3efa8);
    RevertSettings::InstallAtOffset(0x01d411b0);
    IsEqual::InstallAtOffset(0x02299ac4);
}
