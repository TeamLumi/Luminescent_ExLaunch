#include "exlaunch.hpp"

#include "features/overworld_multiplayer.h"

#include "externals/EntityManager.h"
#include "externals/FieldManager.h"
#include "externals/FieldPlayerEntity.h"
#include "externals/OpcState.h"
#include "externals/PlayerWork.h"
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

    // TODO: Open a UI menu with options:
    //   - Battle  (sends RECRUITMENT_BATTLE state + NetBattleRequest)
    //   - Trade   (sends RECRUITMENT_TRADE state + NetTradeRequest)
    //   - Emote   (opens emote submenu, sends NetEmotionData)
    //   - Cancel  (returns to idle)
    //
    // Implementation approach:
    //   1. Use Dpr::UI::UIManager to open a selection window
    //   2. Populate with 4 options
    //   3. On selection, call the appropriate handler below
    //
    // For now, this is a stub. The menu system integration requires
    // hooking into the EvScript or UI window system to show a choice box.
}

// ---------------------------------------------------------------------------
// Menu selection handlers
// ---------------------------------------------------------------------------

static void onSelectBattle() {
    if (s_interact.targetStationIndex < 0) {
        return;
    }

    Logger::log("[OverworldMP] Requesting battle with station %d\n", s_interact.targetStationIndex);

    // TODO: Send battle request to target
    //   1. Set our online state to RECRUITMENT_BATTLE
    //   2. Send NetBattleRequest reliable packet to targetStationIndex
    //   3. Show "Waiting for response..." message
    //   4. Transition to WaitingResponse state

    s_interact.state = InteractState::WaitingResponse;
    s_interact.timeoutTimer = INTERACT_TIMEOUT;
}

static void onSelectTrade() {
    if (s_interact.targetStationIndex < 0) {
        return;
    }

    Logger::log("[OverworldMP] Requesting trade with station %d\n", s_interact.targetStationIndex);

    // TODO: Send trade request to target
    //   1. Set our online state to RECRUITMENT_TRADE
    //   2. Send NetTradeRequest reliable packet to targetStationIndex
    //   3. Show "Waiting for response..." message
    //   4. Transition to WaitingResponse state

    s_interact.state = InteractState::WaitingResponse;
    s_interact.timeoutTimer = INTERACT_TIMEOUT;
}

static void onSelectEmote() {
    Logger::log("[OverworldMP] Opening emote menu\n");

    // TODO: Open emote submenu
    //   1. Show emote selection (reuse existing emoticon UI)
    //   2. On selection, send NetEmotionData reliable to all same-zone players
    //   3. Display emoticon bubble above local player's head
    //   4. Return to idle state

    s_interact.Reset();
}

static void onSelectCancel() {
    Logger::log("[OverworldMP] Interaction cancelled\n");
    s_interact.Reset();
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
    //
    // Reuse underground's battle invitation flow:
    //   1. OpcState.RECRUITMENT_BATTLE on requestor
    //   2. Show accept/decline prompt
    //   3. On accept: both enter link battle via LocalBattleManager
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
    //
    // Reuse underground's trade invitation flow:
    //   1. OpcState.RECRUITMENT_TRADE on requestor
    //   2. Show accept/decline prompt
    //   3. On accept: both enter local trade via LocalKoukan
}

// ---------------------------------------------------------------------------
// Proximity check integration
// ---------------------------------------------------------------------------

// This function is intended to be called from the FieldManager.Update hook
// to check if the player is near a remote player and pressing A
void overworldMPCheckInteraction() {
    auto& ctx = getOverworldMPContext();
    if (!isOverworldMPActive()) {
        return;
    }

    // Only check when in idle state
    if (s_interact.state != InteractState::Idle) {
        // Update timeout for waiting state
        if (s_interact.state == InteractState::WaitingResponse) {
            s_interact.timeoutTimer -= 0.03333334f;
            if (s_interact.timeoutTimer <= 0.0f) {
                Logger::log("[OverworldMP] Interaction timed out\n");
                s_interact.Reset();
            }
        }
        return;
    }

    // Get local player position
    EntityManager::getClass()->initIfNeeded();
    auto player = EntityManager::getClass()->static_fields->_activeFieldPlayer_k__BackingField;
    if (player == nullptr) {
        return;
    }

    auto pos = player->cast<BaseEntity>()->fields.worldPosition;

    // Check if any remote player is within contact radius
    int32_t nearestStation = overworldMPFindNearestPlayer(pos);
    if (nearestStation < 0) {
        return;
    }

    // TODO: Check if A button was just pressed
    // Using UIManager input detection or direct controller polling
    //
    // if (isAButtonPressed()) {
    //     overworldMPShowInteractionMenu(nearestStation);
    // }
}
