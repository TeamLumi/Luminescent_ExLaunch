#include "exlaunch.hpp"

#include "features/overworld_multiplayer.h"

#include "externals/ContextMenuID.h"
#include "externals/Dpr/UI/ContextMenuWindow.h"
#include "externals/Dpr/UI/ContextMenuItem.h"
#include "externals/Dpr/UI/UIManager.h"
#include "externals/Dpr/UI/UIPofinCase.h"
#include "externals/EntityManager.h"
#include "externals/FieldManager.h"
#include "externals/FieldPlayerEntity.h"
#include "externals/OpcState.h"
#include "externals/PlayerWork.h"
#include "externals/System/String.h"
#include "externals/UnityEngine/Vector3.h"

#include "logger/logger.h"

// ---------------------------------------------------------------------------
// Interaction state machine
// ---------------------------------------------------------------------------

enum class InteractState : int32_t {
    Idle = 0,           // No interaction in progress
    MenuOpen = 1,       // Context menu displayed (Battle/Trade/Emote/Cancel)
    WaitingResponse = 2,// Sent request, waiting for remote player's response
    Accepted = 3,       // Remote player accepted — transitioning to activity
    Declined = 4,       // Remote player declined
};

struct InteractContext {
    InteractState state;
    int32_t targetStationIndex;
    float timeoutTimer;

    void Reset() {
        state = InteractState::Idle;
        targetStationIndex = -1;
        timeoutTimer = 0.0f;
    }
};

static InteractContext s_interact;
static constexpr float INTERACT_TIMEOUT = 30.0f; // seconds to wait for response

// MethodInfo for our onClicked callback — lazily initialized via copyWith
static MethodInfo* s_onClickedMethodInfo = nullptr;

// Dpr.SubContents.Utils.get_PushA() @ 0x186A3C0 — returns true on A-button press
static inline bool getSubContentsUtilsPushA() {
    return _ILExternal::external<bool>(0x186A3C0);
}

// ---------------------------------------------------------------------------
// Menu click callback
// ---------------------------------------------------------------------------

// Signature must match Func<ContextMenuItem, bool>: (void* __this, ContextMenuItem* item) -> bool
static bool onMPMenuClicked(void* __this, Dpr::UI::ContextMenuItem::Object* item) {
    if (item == nullptr || item->fields._param == nullptr) {
        s_interact.Reset();
        PlayerWork::getClass()->static_fields->_isPlayerInputActive = true;
        return true;
    }

    ContextMenuID id = item->fields._param->fields.menuId;
    Logger::log("[OverworldMP] Menu selection: id=%d\n", (int)id);

    switch (id) {
        case ContextMenuID::OWMP_BATTLE:
        {
            if (s_interact.targetStationIndex < 0) {
                s_interact.Reset();
                break;
            }
            Logger::log("[OverworldMP] Requesting battle with station %d\n", s_interact.targetStationIndex);
            overworldMPSendInteractionRequest(s_interact.targetStationIndex,
                                              InteractionType::Battle, BattleSubtype::Single);
            s_interact.state = InteractState::WaitingResponse;
            s_interact.timeoutTimer = INTERACT_TIMEOUT;
            break;
        }

        case ContextMenuID::OWMP_TRADE:
        {
            if (s_interact.targetStationIndex < 0) {
                s_interact.Reset();
                break;
            }
            Logger::log("[OverworldMP] Requesting trade with station %d\n", s_interact.targetStationIndex);
            overworldMPSendInteractionRequest(s_interact.targetStationIndex,
                                              InteractionType::Trade, BattleSubtype::Single);
            s_interact.state = InteractState::WaitingResponse;
            s_interact.timeoutTimer = INTERACT_TIMEOUT;
            break;
        }

        case ContextMenuID::OWMP_EMOTE:
        {
            Logger::log("[OverworldMP] Sending emote\n");
            overworldMPSendEmote(EMOTE_ID_LIKES);
            s_interact.Reset();
            break;
        }

        case ContextMenuID::OWMP_CANCEL:
        default:
        {
            Logger::log("[OverworldMP] Interaction cancelled\n");
            s_interact.Reset();
            break;
        }
    }

    // Re-enable player input unless we're waiting for a response
    // (WaitingResponse state keeps input disabled until timeout or response)
    if (s_interact.state != InteractState::WaitingResponse) {
        PlayerWork::getClass()->static_fields->_isPlayerInputActive = true;
    }

    return true;
}

// ---------------------------------------------------------------------------
// Interaction menu
// ---------------------------------------------------------------------------

void overworldMPShowInteractionMenu(int32_t targetStationIndex) {
    if (s_interact.state != InteractState::Idle) {
        return;
    }

    auto& ctx = getOverworldMPContext();
    if (targetStationIndex < 0 || targetStationIndex >= OW_MP_MAX_PLAYERS) {
        return;
    }

    auto& remote = ctx.remotePlayers[targetStationIndex];
    if (!remote.isActive) {
        return;
    }

    // Check if the remote player is in a talkable state
    if (remote.onlineState == OpcState::OnlineState::NOW_BATTLE ||
        remote.onlineState == OpcState::OnlineState::NOW_TRADE ||
        remote.onlineState == OpcState::OnlineState::NOW_MENU) {
        Logger::log("[OverworldMP] Target station %d is busy (state=%d)\n",
                    targetStationIndex, (int)remote.onlineState);
        return;
    }

    Logger::log("[OverworldMP] Opening interaction menu for station %d\n", targetStationIndex);

    s_interact.targetStationIndex = targetStationIndex;
    s_interact.state = InteractState::MenuOpen;

    // Build context menu with 4 items: Battle, Trade, Emote, Cancel
    auto* param = Dpr::UI::ContextMenuWindow::Param::getClass()->newInstance();
    param->fields.itemParams = Dpr::UI::ContextMenuItem::Param::newArray(4);

    // Battle
    param->fields.itemParams->m_Items[0] = Dpr::UI::ContextMenuItem::Param::newInstance();
    param->fields.itemParams->m_Items[0]->fields.menuId = ContextMenuID::OWMP_BATTLE;
    param->fields.itemParams->m_Items[0]->fields.text = System::String::Create("Battle");

    // Trade
    param->fields.itemParams->m_Items[1] = Dpr::UI::ContextMenuItem::Param::newInstance();
    param->fields.itemParams->m_Items[1]->fields.menuId = ContextMenuID::OWMP_TRADE;
    param->fields.itemParams->m_Items[1]->fields.text = System::String::Create("Trade");

    // Emote
    param->fields.itemParams->m_Items[2] = Dpr::UI::ContextMenuItem::Param::newInstance();
    param->fields.itemParams->m_Items[2]->fields.menuId = ContextMenuID::OWMP_EMOTE;
    param->fields.itemParams->m_Items[2]->fields.text = System::String::Create("Emote");

    // Cancel
    param->fields.itemParams->m_Items[3] = Dpr::UI::ContextMenuItem::Param::newInstance();
    param->fields.itemParams->m_Items[3]->fields.menuId = ContextMenuID::OWMP_CANCEL;
    param->fields.itemParams->m_Items[3]->fields.text = System::String::Create("Cancel");

    // Position the menu near screen center
    param->fields.pivot = { .fields = { .x = 0.5f, .y = 0.5f } };
    param->fields.position = { .fields = { .x = 640.0f, .y = 360.0f, .z = 0.0f } };
    param->fields.minItemWidth = 160.0f;
    param->fields.cancelIndex = 3;       // B button selects Cancel
    param->fields.useCancel = true;
    param->fields.useLoopAndRepeat = false;

    // Create the context menu window
    auto* window = Dpr::UI::UIManager::get_Instance()->CreateUIWindow<Dpr::UI::ContextMenuWindow>(UIWindowID::CONTEXTMENU);
    if (window == nullptr) {
        Logger::log("[OverworldMP] ERROR: Failed to create ContextMenuWindow\n");
        s_interact.Reset();
        return;
    }

    // Create MethodInfo for our callback by copying from the poffin case's method
    // (same signature: Func<ContextMenuItem, bool>)
    if (s_onClickedMethodInfo == nullptr) {
        s_onClickedMethodInfo = (*Dpr::UI::UIPofinCase::DisplayClass35_0::Method$$ShowItemContextMenu_b__1)
            ->copyWith((Il2CppMethodPointer)&onMPMenuClicked);
    }

    // Set the onClicked delegate
    window->fields.onClicked = System::Func::getClass(System::Func::ContextMenuItem_bool__TypeInfo)
        ->newInstance(nullptr, s_onClickedMethodInfo);

    // Open the menu
    window->Open(param);

    // Disable player input while menu is open
    PlayerWork::getClass()->initIfNeeded();
    PlayerWork::getClass()->static_fields->_isPlayerInputActive = false;

    Logger::log("[OverworldMP] Context menu opened\n");
}

// ---------------------------------------------------------------------------
// Incoming request handling
// ---------------------------------------------------------------------------

void overworldMPOnBattleRequest(int32_t fromStationIndex) {
    auto& ctx = getOverworldMPContext();
    if (fromStationIndex < 0 || fromStationIndex >= OW_MP_MAX_PLAYERS) {
        return;
    }

    auto& remote = ctx.remotePlayers[fromStationIndex];
    if (!remote.isActive) {
        return;
    }

    Logger::log("[OverworldMP] Battle request from station %d\n", fromStationIndex);

    // TODO: Show confirmation dialog
    //   "PlayerName wants to battle! Accept?"
    //   Yes → send accept, enter LocalBattleManager
    //   No → send decline
}

void overworldMPOnTradeRequest(int32_t fromStationIndex) {
    auto& ctx = getOverworldMPContext();
    if (fromStationIndex < 0 || fromStationIndex >= OW_MP_MAX_PLAYERS) {
        return;
    }

    auto& remote = ctx.remotePlayers[fromStationIndex];
    if (!remote.isActive) {
        return;
    }

    Logger::log("[OverworldMP] Trade request from station %d\n", fromStationIndex);

    // TODO: Show confirmation dialog
    //   "PlayerName wants to trade! Accept?"
    //   Yes → send accept, enter LocalKoukan
    //   No → send decline
}

// ---------------------------------------------------------------------------
// Proximity check integration
// ---------------------------------------------------------------------------

// Called each frame from overworldMPUpdate to check A-button near remote player
void overworldMPCheckInteraction() {
    if (!isOverworldMPActive()) {
        return;
    }

    // If menu is open, check if it closed (interaction state goes back to Idle
    // when onClicked fires via the callback above, or if the window is dismissed)
    if (s_interact.state == InteractState::MenuOpen) {
        // The callback handles transitions — nothing else to do here
        return;
    }

    // Update timeout for waiting state
    if (s_interact.state == InteractState::WaitingResponse) {
        s_interact.timeoutTimer -= 0.03333334f;
        if (s_interact.timeoutTimer <= 0.0f) {
            Logger::log("[OverworldMP] Interaction timed out\n");
            s_interact.Reset();
            // Re-enable player input
            PlayerWork::getClass()->initIfNeeded();
            PlayerWork::getClass()->static_fields->_isPlayerInputActive = true;
        }
        return;
    }

    // Only check for new interactions when idle
    if (s_interact.state != InteractState::Idle) {
        return;
    }

    // Don't check input if player input is disabled (cutscene, dialog, etc.)
    PlayerWork::getClass()->initIfNeeded();
    if (!PlayerWork::get_isPlayerInputActive()) {
        return;
    }

    // Check A-button press
    if (!getSubContentsUtilsPushA()) {
        return;
    }

    // Get local player position
    EntityManager::getClass()->initIfNeeded();
    auto* player = EntityManager::getClass()->static_fields->_activeFieldPlayer_k__BackingField;
    if (player == nullptr) {
        return;
    }

    auto pos = player->cast<BaseEntity>()->fields.worldPosition;

    // Check if any remote player is within contact radius
    int32_t nearestStation = overworldMPFindNearestPlayer(pos);
    if (nearestStation < 0) {
        return;
    }

    // Found a nearby player — open the interaction menu
    overworldMPShowInteractionMenu(nearestStation);
}
