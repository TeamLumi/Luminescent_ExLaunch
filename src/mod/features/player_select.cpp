#include "exlaunch.hpp"

#include "data/outfits.h"
#include "data/utils.h"

#include "externals/Audio/AudioManager.h"
#include "externals/Dpr/UI/SelectPlayerVisualWindow.h"
#include "externals/Dpr/UI/UIManager.h"
#include "externals/OpeningController.h"
#include "externals/UnityEngine/Mathf.h"

#include "utils/utils.h"

void SelectGridIndex(Dpr::UI::SelectPlayerVisualWindow::Object* window, int32_t hSelection, int32_t vSelection, int32_t hCount)
{
    if (window->SetSelectIndex(vSelection * hCount + hSelection, false))
    {
        Audio::AudioManager::getClass()->initIfNeeded();
        auto audioManager = Audio::AudioManager::get_Instance();
        audioManager->PlaySe(0xb7533038, nullptr);
    }
}

HOOK_DEFINE_REPLACE(SelectPlayerVisualWindow_UpdateSelect) {
    static void Callback(Dpr::UI::SelectPlayerVisualWindow::Object* __this, float deltaTime) {
        system_load_typeinfo(game::TypeInfo::Dpr_UI_SelectPlayerVisualWindow_UpdateSelect);
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

        int32_t totalCount = __this->fields._items->fields._size;
        int32_t hCount = 8; // Amount of columns
        int32_t vCount = (totalCount - 1) / hCount + 1; // Amount of rows
        int32_t lasthCount = totalCount % hCount; // Amount of columns in the last row
        if (totalCount < hCount) hCount = totalCount;
        if (lasthCount == 0) lasthCount = hCount;

        int32_t hSelection = selection % hCount; // The internal list goes left->right THEN up->down
        int32_t vSelection = selection / hCount;

        // Horizontal movement
        if (rightPush)
        {
            hSelection++;
            hSelection = HorizontalRepeat(hSelection, hCount, vSelection, vCount, lasthCount);
            SelectGridIndex(__this, hSelection, vSelection, hCount);
        }
        else if (rightRepeat)
        {
            hSelection = HorizontalClamp(hSelection + 1, hCount, vSelection, vCount, lasthCount);
            SelectGridIndex(__this, hSelection, vSelection, hCount);
        }
        else if (leftPush)
        {
            hSelection--;
            hSelection = HorizontalRepeat(hSelection, hCount, vSelection, vCount, lasthCount);
            SelectGridIndex(__this, hSelection, vSelection, hCount);
        }
        else if (leftRepeat)
        {
            hSelection = HorizontalClamp(hSelection - 1, hCount, vSelection, vCount, lasthCount);
            SelectGridIndex(__this, hSelection, vSelection, hCount);
        }

        // Vertical movement
        if (downPush)
        {
            vSelection++;
            vSelection = VerticalRepeat(hSelection, hCount, vSelection, vCount, lasthCount);
            SelectGridIndex(__this, hSelection, vSelection, hCount);
        }
        else if (downRepeat)
        {
            vSelection = VerticalClamp(hSelection, hCount, vSelection + 1, vCount, lasthCount);
            SelectGridIndex(__this, hSelection, vSelection, hCount);
        }
        else if (upPush)
        {
            vSelection--;
            vSelection = VerticalRepeat(hSelection, hCount, vSelection, vCount, lasthCount);
            SelectGridIndex(__this, hSelection, vSelection, hCount);
        }
        else if (upRepeat)
        {
            vSelection = VerticalClamp(hSelection , hCount, vSelection - 1, vCount, lasthCount);
            SelectGridIndex(__this, hSelection, vSelection, hCount);
        }
    }
};

HOOK_DEFINE_REPLACE(SelectPlayerVisualItem_Awake_Nop) {
    static void Callback(Dpr::UI::SelectPlayerVisualItem::Object* __this) {
        // Do nothing, we don't want to overwrite the values
        // Bad ILCA
    }
};

HOOK_DEFINE_TRAMPOLINE(OpeningController_OpenKeyboardByPlayerName_b__10_Log) {
    static void Callback(OpeningController::Object* __this, bool isSuccess, System::String::Object* resultText) {
        auto sex = __this->fields._selectPlayerVisualItem->fields.sex;
        auto colorID = __this->fields._selectPlayerVisualItem->fields.colorId;
        Logger::log("Selected %s with color ID %d\n", sex ? "Lucas" : "Dawn", colorID);
        Orig(__this, isSuccess, resultText);
    }
};

void exl_player_select_main() {
    SelectPlayerVisualWindow_UpdateSelect::InstallAtOffset(game::Address::Dpr_UI_SelectPlayerVisualWindow_UpdateSelect);
    SelectPlayerVisualItem_Awake_Nop::InstallAtOffset(game::Address::Dpr_UI_SelectPlayerVisualItem_Awake);
    OpeningController_OpenKeyboardByPlayerName_b__10_Log::InstallAtOffset(game::Address::OpeningController_OpenKeyboardByPlayerName_b_10_0);

    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p;
    auto inst = nn::vector<exl::patch::Instruction> {
        { game::Address::PlayerWork_SetPlayerStatus + 0x204, Movz(W21, array_index(OUTFITS, "Platinum Style Masculine")) }, // Change Lucas default outfit in intro to platinum outfit
        { game::Address::PlayerWork_SetPlayerStatus + 0x248, Movz(W21, array_index(OUTFITS, "Platinum Style Feminine")) },  // Change Dawn default outfit in intro to platinum outfit
    };
    p.WriteInst(inst);
}
