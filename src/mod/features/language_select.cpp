#include "exlaunch.hpp"

#include "externals/Audio/AudioManager.h"
#include "externals/Dpr/UI/SelectLanguageWindow.h"
#include "externals/Dpr/UI/UIManager.h"
#include "externals/UnityEngine/GameObject.h"
#include "externals/UnityEngine/Mathf.h"

#include "romdata/romdata.h"
#include "utils/utils.h"

void SelectGridIndex(Dpr::UI::SelectLanguageWindow::Object* window, int32_t hSelection, int32_t vSelection, int32_t vCount)
{
    if (window->SetSelectIndex(hSelection * vCount + vSelection, false))
    {
        Audio::AudioManager::getClass()->initIfNeeded();
        auto audioManager = Audio::AudioManager::get_Instance();
        audioManager->PlaySe(0xb7533038, nullptr);
    }
}

HOOK_DEFINE_INLINE(LanguageSelect_Add) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto languageWindow = (Dpr::UI::SelectLanguageWindow::Object*)ctx->X[19];
        auto content = (UnityEngine::Transform::Object*)languageWindow->fields._content;

        for (int32_t i=0; i<content->get_childCount(); i++)
        {
            UnityEngine::Transform::Object* child = content->GetChild(i);
            auto component = (UnityEngine::Component::Object*)child;

            Dpr::UI::SelectLanguageItem::Object* item = component->GetComponent(UnityEngine::Component::Method$$SelectLanguageItem$$GetComponent);

            UnityEngine::GameObject::Object* go = component->get_gameObject()->instance();
            bool active = IsLanguageActivated(item->fields._langId);
            go->SetActive(active);

            if (active) languageWindow->fields._items->Add(item);
        }
    }
};

HOOK_DEFINE_REPLACE(SelectLanguageWindow_UpdateSelect) {
    static void Callback(Dpr::UI::SelectLanguageWindow::Object* __this, float deltaTime) {
        system_load_typeinfo(0x79c0);
        using namespace Dpr::UI;
        UIManager::getClass()->initIfNeeded();
        UnityEngine::Mathf::getClass()->initIfNeeded();

        auto uiWindow = (UIWindow::Object*)__this;

        bool leftPush = uiWindow->IsPushButton(UIManager::getClass()->static_fields->StickLLeft, false);
        bool rightPush = uiWindow->IsPushButton(UIManager::getClass()->static_fields->StickLRight, false);
        bool upPush = uiWindow->IsPushButton(UIManager::getClass()->static_fields->StickLUp, false);
        bool downPush = uiWindow->IsPushButton(UIManager::getClass()->static_fields->StickLDown, false);

        bool leftRepeat = uiWindow->IsRepeatButton(UIManager::getClass()->static_fields->StickLLeft, false);
        bool rightRepeat = uiWindow->IsRepeatButton(UIManager::getClass()->static_fields->StickLRight, false);
        bool upRepeat = uiWindow->IsRepeatButton(UIManager::getClass()->static_fields->StickLUp, false);
        bool downRepeat = uiWindow->IsRepeatButton(UIManager::getClass()->static_fields->StickLDown, false);

        // Don't move if there is both a horizontal and vertical movement
        if ((leftPush || rightPush) && (upPush || downPush))
            return;

        int32_t selection = __this->fields._selectIndex;

        // NOTE: This would probably break with more than 2 columns
        // If you want more columns, re-arrange the UI bundle so that it's ordered
        // and change the math to match the one for player selection
        int32_t totalCount = __this->fields._items->fields._size;
        int32_t hCount = 2; // Amount of columns
        int32_t vCount = (totalCount - 1) / hCount + 1; // Amount of rows
        int32_t lasthCount = totalCount % hCount; // Amount of columns in the last row
        if (totalCount < hCount) hCount = totalCount;
        if (lasthCount == 0) lasthCount = hCount;

        int32_t hSelection = selection / vCount; // The internal list goes up->down THEN left->right
        int32_t vSelection = selection % vCount;

        // Horizontal movement
        if (rightPush)
        {
            hSelection++;
            hSelection = HorizontalRepeat(hSelection, hCount, vSelection, vCount, lasthCount);
            SelectGridIndex(__this, hSelection, vSelection, vCount);
        }
        else if (rightRepeat)
        {
            hSelection = HorizontalClamp(hSelection + 1, hCount, vSelection, vCount, lasthCount);
            SelectGridIndex(__this, hSelection, vSelection, vCount);
        }
        else if (leftPush)
        {
            hSelection--;
            hSelection = HorizontalRepeat(hSelection, hCount, vSelection, vCount, lasthCount);
            SelectGridIndex(__this, hSelection, vSelection, vCount);
        }
        else if (leftRepeat)
        {
            hSelection = HorizontalClamp(hSelection - 1, hCount, vSelection, vCount, lasthCount);
            SelectGridIndex(__this, hSelection, vSelection, vCount);
        }

        // Vertical movement
        if (downPush)
        {
            vSelection++;
            vSelection = VerticalRepeat(hSelection, hCount, vSelection, vCount, lasthCount);
            SelectGridIndex(__this, hSelection, vSelection, vCount);
        }
        else if (downRepeat)
        {
            vSelection = VerticalClamp(hSelection, hCount, vSelection + 1, vCount, lasthCount);
            SelectGridIndex(__this, hSelection, vSelection, vCount);
        }
        else if (upPush)
        {
            vSelection--;
            vSelection = VerticalRepeat(hSelection, hCount, vSelection, vCount, lasthCount);
            SelectGridIndex(__this, hSelection, vSelection, vCount);
        }
        else if (upRepeat)
        {
            vSelection = VerticalClamp(hSelection , hCount, vSelection - 1, vCount, lasthCount);
            SelectGridIndex(__this, hSelection, vSelection, vCount);
        }
    }
};

void exl_language_select_main() {
    LanguageSelect_Add::InstallAtOffset(0x01d3ce48);
    SelectLanguageWindow_UpdateSelect::InstallAtOffset(0x01d3ba50);

    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p(0);
    auto inst = nn::vector<exl::patch::Instruction> {
        { 0x01d3ce4c, Branch(0x54) },
    };
    p.WriteInst(inst);
}
