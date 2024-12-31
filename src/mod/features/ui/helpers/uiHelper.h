#pragma once

#include "externals/UIWindowID.h"
#include "externals/FlagWork.h"
#include "externals/FlagWork_Enums.h"
#include "externals/UnityEngine/_Object.h"
#include "externals/Dpr/UI/BoxWindow.h"
#include "externals/SmartPoint/AssetAssistant/Sequencer.h"
#include "externals/Audio/AudioManager.h"
#include "externals/UnityEngine/Mathf.h"
#include "externals/Dpr/Message/MessageManager.h"
#include "externals/Dpr/UI/ContextMenuItem.h"
#include "externals/ContextMenuID.h"
#include "externals/Dpr/UI/ContextMenuWindow.h"
#include "externals/Dpr/UI/SelectLanguageWindow.h"
#include "externals/System/Func.h"
#include "externals/Dpr/Message/MessageWordSetHelper.h"
#include "data/utils.h"
#include "data/types.h"
#include "save/save.h"
#include "externals/Dpr/UI/PokemonStatusWindow.h"
#include "externals/Dpr/BattleMatching/BattleMatchingWork.h"
#include "externals/UnityEngine/Sprite.h"
#include "externals/Dpr/UI/PokemonIcon.h"
#include "externals/Dpr/UI/Keyguide.h"
#include "externals/Pml/Sex.h"
#include "features/frontier/frontier_ptr.h"
#include "externals/GameManager.h"
#include "features/ui/frontier/frontier_ui.h"
#include "externals/Dpr/UI/UIManager.h"

static constexpr uint32_t AK_EVENTS_UI_COMMON_SELECT = 0xb7533038;
static constexpr uint32_t AK_EVENTS_UI_COMMON_UNK = 0x5d95f820;
static constexpr uint32_t AK_EVENTS_UI_COMMON_DONE = 0x4491b890;
static constexpr uint32_t AK_EVENTS_UI_COMMON_BEEP = 0x707237C4;

namespace uiHelper {
    UIWindowID GetWindowID();

    void AddCustomContextMenuItems(System::Collections::Generic::List$$ContextMenuItem_Param::Object* contextMenuItemParams,
                                   const nn::vector<ContextMenuID>& customMenuIDs);

    void RetrievePokemonSprite(Pml::PokePara::PokemonParam::Object* param, Dpr::UI::PokemonIcon::Object* monsIcon);

    void RetrievePokemonItem(Pml::PokePara::PokemonParam::Object* param, Dpr::UI::PokemonIcon::Object* monsIcon);

    bool SetSelectIndex(Dpr::UI::BoxWindow::Object* __this, int32_t index, bool isInitialized = false);

    void UpdateSelect(Dpr::UI::BoxWindow::Object* __this);

    void OnUpdate(Dpr::UI::BoxWindow::Object* __this, float deltaTime);

    void OpenCustomContextMenu(Dpr::UI::BoxWindow::Object* window);
}
