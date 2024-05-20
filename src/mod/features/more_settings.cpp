#include "exlaunch.hpp"

#include "memory/vector.h"

#include "externals/Dpr/UI/SettingWindow.h"
#include "externals/FlagWork.h"
#include "externals/UnityEngine/UI/HorizontalLayoutGroup.h"

#include "logger/logger.h"
#include "ui/ui.h"

HOOK_DEFINE_TRAMPOLINE(GetSetting) {
    static int32_t Callback(DPData::CONFIG::Object* __this, int32_t id) {
        switch (id) {
            case 14:
                return !FlagWork::GetFlag(FlagWork_Flag::FLAG_DISABLE_EXP_SHARE) ? 0 : 1;
            case 15:
                return !FlagWork::GetFlag(FlagWork_Flag::FLAG_AFFECTION_ENABLED) ? 0 : 1;
            case 16:
                return !FlagWork::GetFlag(FlagWork_Flag::FLAG_DISABLE_LEVEL_CAP) ? 0 : 1;
            default:
                return Orig(__this, id);
                break;
        }
    }
};

HOOK_DEFINE_TRAMPOLINE(SetSetting) {
    static void Callback(DPData::CONFIG::Object* __this, int32_t id, int32_t value) {
        switch (id) {
            case 14:
                FlagWork::SetFlag(FlagWork_Flag::FLAG_DISABLE_EXP_SHARE, value == 0 ? 1 : 0);
                break;
            case 15:
                FlagWork::SetFlag(FlagWork_Flag::FLAG_AFFECTION_ENABLED, value == 0 ? 1 : 0);
                break;
            case 16:
                FlagWork::SetFlag(FlagWork_Flag::FLAG_DISABLE_LEVEL_CAP, value == 0 ? 1 : 0);
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
            case 15:
            case 16:
                // TODO: Rewrite to support comparisons, otherwise reverting settings does not work
                return true;
            default:
                return Orig(__this, id, other);
                break;
        }
    }
};

void AddSetting(Dpr::UI::SettingWindow::_OpOpen_d__11::Object* __this, int configId, int selectedIndex, const char* title, const char* description, nn::vector<const char*> optionNames, int position) {
    auto window = __this->fields.__4__this;

    system_load_typeinfo(0x870a);

    auto contentRoot = window->fields._scrollRect->fields.m_Content;
    auto child = contentRoot->cast<UnityEngine::Transform>()->Find(System::String::Create("BattleAnim"));

    auto origItem = child->GetComponent(UnityEngine::Component::Method$$SettingMenuItem$$GetComponent);

    auto newTransform = UnityEngine::_Object::Instantiate<UnityEngine::Transform>(child);
    newTransform->SetParent(contentRoot->cast<UnityEngine::Transform>(), false);

    Dpr::UI::SettingMenuItem::Object* newItem = newTransform->GetComponent(UnityEngine::Component::Method$$SettingMenuItem$$GetComponent);
    newItem->Setup(configId, selectedIndex, System::String::Create(description), origItem->fields._onValueChanged);
    auto textRoot = newTransform->GetChild(1);
    auto titleChild = textRoot->GetChild(0);

    system_load_typeinfo(0x870a);

    auto titleComponent = titleChild->GetComponent(UnityEngine::Component::Method$$UIText$$GetComponent);
    titleComponent->SetupMessage(
        System::String::Create("ss_option"),
        System::String::Create(title)
    );
    auto options = textRoot->GetChild({ 1, 0 });
    for (uint64_t i=0; i<optionNames.size(); i++) {
        auto option = options->GetChild({ (int32_t)i, 0 })->GetComponent(UnityEngine::Component::Method$$UIText$$GetComponent);
        option->SetupMessage(
            System::String::Create("ss_option"),
            System::String::Create(optionNames[i])
        );
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

        nn::vector<const char*> onOffNames = { "SS_option_007", "SS_option_008" };
        AddSetting(__this, 14, FlagWork::GetFlag(FlagWork_Flag::FLAG_DISABLE_EXP_SHARE) ? 0 : 1, "SS_option_110", "SS_option_113", onOffNames, 1);
        AddSetting(__this, 15, FlagWork::GetFlag(FlagWork_Flag::FLAG_AFFECTION_ENABLED) ? 0 : 1, "SS_option_109", "SS_option_112", onOffNames, 2);
        AddSetting(__this, 16, FlagWork::GetFlag(FlagWork_Flag::FLAG_DISABLE_LEVEL_CAP) ? 0 : 1, "SS_option_111", "SS_option_114", onOffNames, 3);

        return res;
    }
};


void exl_settings_main() {
    AddSettingsEntries::InstallAtOffset(0x01d42830);
    GetSetting::InstallAtOffset(0x02299720);
    SetSetting::InstallAtOffset(0x022997d0);
    SettingIsEqual::InstallAtOffset(0x022998a0);
}
