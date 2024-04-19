#include "exlaunch.hpp"

#include "memory/vector.h"

#include "externals/Dpr/UI/SettingWindow.h"
#include "externals/FlagWork.h"
#include "externals/UnityEngine/UI/HorizontalLayoutGroup.h"
#include "externals/XMenuTopItem.h"
#include "externals/Dpr/Message/MessageMsgFile.h"

#include "logger/logger.h"
#include "ui/ui.h"

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
                return FlagWork::GetWork(FlagWork_Work::WK_GAME_MODE);
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
                FlagWork::SetFlag(FlagWork_Flag::FLAG_DISABLE_AFFECTION, value == 0 ? 1 : 0);
                break;
            case 16:
                FlagWork::SetFlag(FlagWork_Flag::FLAG_DISABLE_LEVEL_CAP, value == 0 ? 1 : 0);
                break;
            case 17:
                FlagWork::SetWork(FlagWork_Work::WK_GAME_MODE, value);
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

void AddSetting(Dpr::UI::SettingWindow::_OpOpen_d__11::Object* __this, int configId, int selectedIndex, const char* title, const char* description, nn::vector<const char*> optionNames, int position, bool isFrame) {
    auto window = __this->fields.__4__this;

    system_load_typeinfo(0x870a);

    auto contentRoot = window->fields._scrollRect->fields.m_Content;
    auto cloneType = (isFrame) ? "WindowChange" : "BattleAnim";
    auto child = contentRoot->cast<UnityEngine::Transform>()->Find(System::String::Create(cloneType));

    auto origItem = child->GetComponent(UnityEngine::Component::Method$$SettingMenuItem$$GetComponent);

    if (isFrame) {
        origItem->fields._itemType = 3;
    }

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




    if (isFrame) {
        auto options = textRoot->GetChild({ 1, 2 });
        for (uint64_t i=0; i<optionNames.size(); i++) {
            auto option = options->GetChild({ (int32_t)i, 0 })->GetComponent(UnityEngine::Component::Method$$UIText$$GetComponent);
            option->SetupMessage(
                    System::String::Create("ss_option"),
                    System::String::Create(optionNames[i])
            );
        }

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

HOOK_DEFINE_TRAMPOLINE(AddSettingsEntries) {
    static bool Callback(Dpr::UI::SettingWindow::_OpOpen_d__11::Object* __this) {
        auto res = Orig(__this);

        auto window = __this->fields.__4__this;
        if (window->fields._activeItems->fields._size > 13) {
            return res;
        }

        nn::vector<const char*> onOffNames = { "SS_option_007", "SS_option_008" };
        nn::vector<const char*> modes = { "SS_option_117", "SS_option_118", "SS_option_119" };
        AddSetting(__this, 14, FlagWork::GetFlag(FlagWork_Flag::FLAG_DISABLE_EXP_SHARE) ? 0 : 1, "SS_option_110", "SS_option_113", onOffNames, 1, false);
        AddSetting(__this, 15, FlagWork::GetFlag(FlagWork_Flag::FLAG_DISABLE_AFFECTION) ? 0 : 1, "SS_option_109", "SS_option_112", onOffNames, 2, false);
        AddSetting(__this, 16, FlagWork::GetFlag(FlagWork_Flag::FLAG_DISABLE_LEVEL_CAP) ? 0 : 1, "SS_option_111", "SS_option_114", onOffNames, 3, false);
        AddSetting(__this, 17, FlagWork::GetWork(FlagWork_Work::WK_GAME_MODE), "SS_option_116", "SS_option_115", modes, 4, true);
        return res;
    }
};

HOOK_DEFINE_TRAMPOLINE(SetSelectIndex) {
    static bool Callback(Dpr::UI::SettingMenuItem::Object* __this, int32_t selectIndex, bool isEqualChecked, bool isCallbacked) {
        nn::vector<const char*> modes = { "SS_option_117", "SS_option_118", "SS_option_119" };
        if (__this->fields._itemType == 2) {
            Logger::log("[SetSelectIndex] selectIndex: %d.\n", selectIndex);
            auto contentRoot = __this->fields._contentRoot;
            auto transform = contentRoot->cast<UnityEngine::Transform>()->Find(System::String::Create("WindowChange"));
            auto textRoot = transform->GetChild(1);
            auto options = textRoot->GetChild({ 1, 2 });
            auto option = options->GetChild({ 1, 0 })->GetComponent(UnityEngine::Component::Method$$UIText$$GetComponent);
            option->SetupMessage(
                    System::String::Create("ss_option"),
                    System::String::Create(modes[selectIndex])
            );
        }

        return Orig(__this, selectIndex, isEqualChecked, isCallbacked);
    }

};

HOOK_DEFINE_TRAMPOLINE(SetupContent) {
    static void Callback(Dpr::UI::SettingMenuItem::Object* __this) {

        auto iVar21 = (__this->fields)._itemType;
        if (iVar21 != 2) {
            Orig(__this);
        }

        else {
            auto selectIndex = __this->fields._selectIndex;
            Logger::log("[SetupContent] selectIndex: %d.\n", selectIndex);
            Orig(__this);
        }

//        auto iVar21 = (__this->fields)._itemType;
//        if (iVar21 != 2) {
//            Orig(__this);
//        }
//        Logger::log("[Setup Content] Begin trampoline hook.\n");
//        ((System::Collections::Generic::List$$XMenuTopItem::Object*) ((__this->fields)._texts))->Clear(
//                System::Collections::Generic::List$$UIText::Method$$UIText$$Clear);
//
//        Logger::log("[Setup Content] Generic List cleared.\n");
//
//        auto iVar4 = ((UnityEngine::Transform::Object*) (__this->fields)._content)->get_childCount();
//        Logger::log("[Setup Content] Child count: %d.\n", iVar4);
//        if (0 < iVar4) {
//            iVar21 = 0;
//            do {
//                auto pUVar5 = ((UnityEngine::Transform::Object*) (__this->fields)._content)->GetChild(1);
//                auto pXVar6 = (XMenuTopItem::Object*) (((UnityEngine::Component::Object*) pUVar5)->GetComponentInChildren(
//                        true, UnityEngine::Component::Method$$UIText$$GetComponentInChildren));
//                ((System::Collections::Generic::List$$XMenuTopItem::Object*) ((__this->fields)._texts))->Add(
//                        pXVar6, System::Collections::Generic::List$$UIText::Method$$UIText$$Add);
//
//                iVar21 += 1;
//
//                iVar4 = ((UnityEngine::Transform::Object*) (__this->fields)._content)->get_childCount();
//            } while (iVar21 < iVar4);
//        }
//
//        Logger::log("[Setup Content] Do/While Loop completed.\n");
//
//        iVar21 = (__this->fields)._selectIndex;
//
//        system_load_typeinfo(0x7c42);
//
//        if ((__this->fields)._selectIndex == iVar21) {
//            return;
//        }
//
//        (__this->fields)._selectIndex = iVar21;
//
//        Logger::log("[Setup Content] End of trampoline hook.\n");
//
    }
};

HOOK_DEFINE_TRAMPOLINE(SetupContent2) {
    static void Callback(Dpr::UI::SettingMenuItem::Object* __this) {
        Logger::log("[SetupContent2] Begin trampoline hook\n");
        auto iVar21 = (__this->fields)._itemType;
        Logger::log("[SetupContent2] itemType: %d.\n", iVar21);
        if (iVar21 != 2) {
            Orig(__this);
        }

        else {
            auto texts = __this->fields._texts;
            auto uiTextObj = texts->fields._items->m_Items[0];
            auto String = (System::String::Object*) uiTextObj->fields._messageId;
            if (String == nullptr) {
                Logger::log("[SetupContent2] String is nullptr\n");
            }
            else {
                Logger::log("[SetupContent2] String: %s\n", String->asCString().c_str());
            }

            Orig(__this);
        }
    }
};

HOOK_DEFINE_INLINE(Bruh) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {

        nn::vector<const char*> modes = { "SS_option_117", "SS_option_118", "SS_option_119" };

        auto text = (Dpr::UI::UIText::Object*) ctx->X[22];
        auto selectIndex = (int32_t) ctx->W[1];
        Logger::log("[Bruh] selectIndex: %d.\n", selectIndex);
        auto mode = System::String::Create(modes.at(selectIndex));
        Logger::log("[Bruh] Mode: %s.\n", mode->asCString().c_str());
        if (selectIndex >= (int32_t) modes.size() && selectIndex != 23) {
            selectIndex = 0;
        }

        else if (selectIndex == 23) {
            selectIndex = 2;
        }
        auto __this = (Dpr::UI::SettingMenuItem::Object*) ctx->X[0];

        Logger::log("[Bruh] SetupMessage\n");
        if (__this->fields._itemType == 2 && (System::String::Object*) ctx->X[8] != nullptr) {
            Logger::log("[Bruh] !nullptr\n");
            text->SetupMessage(System::String::Create("ss_option"), System::String::Create(modes.at(selectIndex)));
        }

        else {
            Logger::log("[Bruh] else\n");
            text->SetupMessage(System::String::Create("ss_option"), (System::String::Object*) ctx->X[8]);
        }
    }
};

HOOK_DEFINE_INLINE(Inject) {
  static void Callback(exl::hook::nx64::InlineCtx* ctx) {
      auto msgFile = (Dpr::Message::MessageMsgFile::Object*) ctx->X[20];
      auto UIText = (Dpr::UI::UIText::Object*) ctx->X[0];
      Logger::log("[Inject] sizeId: %d\n", UIText->fields._sizeId);
      ctx->X[0] = (u64) msgFile->GetLabel(UIText->fields._sizeId);
  }
};



void exl_settings_main() {
    AddSettingsEntries::InstallAtOffset(0x01d42830);
    GetSetting::InstallAtOffset(0x02299720);
    SetSetting::InstallAtOffset(0x022997d0);
    SettingIsEqual::InstallAtOffset(0x022998a0);
    //Bruh::InstallAtOffset(0x01d3fda8);
    //SetSelectIndex::InstallAtOffset(0x01d3f1a0);
    //Inject::InstallAtOffset(0x01dd166c);
    //SetupContent2::InstallAtOffset(0x01d3ef70);
    SetupContent::InstallAtOffset(0x01d3ef70);

//    ThisIsPain::InstallAtOffset(0x01d3fda8);


//    using namespace exl::armv8::inst;
//    using namespace exl::armv8::reg;
//
//    exl::patch::CodePatcher p(0x01da02bc);
//    p.WriteInst(LdrRegisterImmediate());

}
