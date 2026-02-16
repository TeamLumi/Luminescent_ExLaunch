#include "exlaunch.hpp"

#include "features/overworld_multiplayer.h"

#include "externals/ContextMenuID.h"
#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Dpr/UI/ContextMenuWindow.h"
#include "externals/Dpr/UI/ContextMenuItem.h"
#include "externals/Dpr/UI/UIManager.h"
#include "externals/Dpr/UI/UIPofinCase.h"
#include "externals/EntityManager.h"
#include "externals/FieldCanvas.h"
#include "externals/FieldManager.h"
#include "externals/FieldObjectEntity.h"
#include "externals/FieldPlayerEntity.h"
#include "externals/OpcState.h"
#include "externals/PlayerWork.h"
#include "externals/System/String.h"
#include "externals/UnityEngine/Component.h"
#include "externals/UnityEngine/Vector3.h"

#include "externals/il2cpp-api.h"
#include "logger/logger.h"

// ---------------------------------------------------------------------------
// Interaction state machine
// ---------------------------------------------------------------------------

enum class InteractState : int32_t {
    Idle = 0,              // No interaction in progress
    MenuOpen = 1,          // Main context menu displayed (Battle/Trade/Emote/Cancel)
    EmoteMenuPending = 2,  // Main menu closing, emote sub-menu about to open
    EmoteMenuOpen = 3,     // Emote sub-menu displayed
    WaitingResponse = 4,   // Sent request, waiting for remote player's response
    Accepted = 5,          // Remote player accepted — transitioning to activity
    Declined = 6,          // Remote player declined
};

struct InteractContext {
    InteractState state;
    int32_t targetStationIndex;
    float timeoutTimer;
    float emoteMenuDelay;  // brief delay before opening emote sub-menu

    void Reset() {
        state = InteractState::Idle;
        targetStationIndex = -1;
        timeoutTimer = 0.0f;
        emoteMenuDelay = 0.0f;
    }
};

static InteractContext s_interact;
static constexpr float INTERACT_TIMEOUT = 30.0f;

// MethodInfo for our onClicked callbacks — lazily initialized via copyWith
static MethodInfo* s_mainMenuMethodInfo = nullptr;
static MethodInfo* s_emoteMenuMethodInfo = nullptr;

// Active emote balloons pending deletion
static constexpr int32_t MAX_ACTIVE_BALLOONS = 16;
static constexpr float BALLOON_LIFETIME = 2.0f;

struct ActiveBalloon {
    void* balloon;
    float timer;
};
static ActiveBalloon s_activeBalloons[MAX_ACTIVE_BALLOONS] = {};

static void trackBalloon(void* balloon) {
    if (balloon == nullptr) return;
    for (int i = 0; i < MAX_ACTIVE_BALLOONS; i++) {
        if (s_activeBalloons[i].balloon == nullptr) {
            s_activeBalloons[i].balloon = balloon;
            s_activeBalloons[i].timer = BALLOON_LIFETIME;
            return;
        }
    }
    // All slots full — delete the oldest (slot 0) immediately
    FieldCanvas::DeleteBalloon(s_activeBalloons[0].balloon, true);
    s_activeBalloons[0].balloon = balloon;
    s_activeBalloons[0].timer = BALLOON_LIFETIME;
}

// Called each frame to tick down balloon timers and delete expired ones
void overworldMPTickBalloons(float deltaTime) {
    for (int i = 0; i < MAX_ACTIVE_BALLOONS; i++) {
        if (s_activeBalloons[i].balloon == nullptr) continue;
        s_activeBalloons[i].timer -= deltaTime;
        if (s_activeBalloons[i].timer <= 0.0f) {
            FieldCanvas::DeleteBalloon(s_activeBalloons[i].balloon, false);
            s_activeBalloons[i].balloon = nullptr;
        }
    }
}

// Dpr.SubContents.Utils.get_PushA() @ 0x186A3C0
static inline bool getSubContentsUtilsPushA() {
    return _ILExternal::external<bool>(0x186A3C0);
}

// ---------------------------------------------------------------------------
// Close menu helper
// ---------------------------------------------------------------------------
static void closeInteractionMenu() {
    PlayerWork::getClass()->initIfNeeded();
    PlayerWork::getClass()->static_fields->_isPlayerInputActive = true;
}

// ---------------------------------------------------------------------------
// Show emote balloon above the local player
// ---------------------------------------------------------------------------
static void showLocalEmoteBalloon(uint8_t emoteId) {
    EntityManager::getClass()->initIfNeeded();
    auto* player = EntityManager::getClass()->static_fields->_activeFieldPlayer_k__BackingField;
    if (player == nullptr) return;

    auto* transform = player->cast<UnityEngine::Component>()->get_transform();
    if (transform == nullptr) return;

    auto* balloon = FieldCanvas::SetBalloon((int32_t)emoteId, transform);
    trackBalloon(balloon);
}

// ---------------------------------------------------------------------------
// Show emote balloon above a remote player entity
// ---------------------------------------------------------------------------
void overworldMPShowRemoteEmote(int32_t stationIndex, uint8_t emoteId) {
    auto& ctx = getOverworldMPContext();
    if (stationIndex < 0 || stationIndex >= OW_MP_MAX_PLAYERS) return;
    if (ctx.spawnedEntities[stationIndex] == nullptr) return;

    auto* entity = (FieldObjectEntity::Object*)ctx.spawnedEntities[stationIndex];
    auto* transform = entity->cast<UnityEngine::Component>()->get_transform();
    if (transform == nullptr) return;

    auto* balloon = FieldCanvas::SetBalloon((int32_t)emoteId, transform);
    trackBalloon(balloon);
}

// ---------------------------------------------------------------------------
// Shared: open a ContextMenuWindow with given items
// ---------------------------------------------------------------------------
// items: array of {text} pairs. menuId is always XMENU_YES (vanilla safe).
// callback: the C++ function to invoke when an item is selected.
// methodInfoCache: pointer to a static MethodInfo* for caching the copyWith result.
static bool openContextMenu(
    const char* items[], int32_t itemCount, int32_t cancelIndex,
    bool (*callback)(void*, Dpr::UI::ContextMenuItem::Object*, MethodInfo*),
    MethodInfo** methodInfoCache)
{
    system_load_typeinfo(0x9f8f);
    system_load_typeinfo(0x9cb4);

    auto* paramClass = Dpr::UI::ContextMenuWindow::Param::getClass();
    if (paramClass == nullptr) return false;
    paramClass->initIfNeeded();
    auto* param = paramClass->newInstance();

    auto* itemParamArrayClass = Dpr::UI::ContextMenuItem::Param::getArrayClass();
    if (itemParamArrayClass == nullptr) return false;
    itemParamArrayClass->initIfNeeded();
    param->fields.itemParams = itemParamArrayClass->newArray(itemCount);

    auto* itemParamClass = Dpr::UI::ContextMenuItem::Param::getClass();
    if (itemParamClass == nullptr) return false;
    itemParamClass->initIfNeeded();

    // Use sequential vanilla menuIds (0,1,2,...) so GetContextMenuData doesn't throw.
    // Identify selections by reading menuId back from item->_param->menuId in the callback.
    for (int32_t i = 0; i < itemCount; i++) {
        param->fields.itemParams->m_Items[i] = itemParamClass->newInstance();
        param->fields.itemParams->m_Items[i]->fields.menuId = (ContextMenuID)i;
        param->fields.itemParams->m_Items[i]->fields.text = System::String::Create(items[i]);
    }

    param->fields.pivot = { .fields = { .x = 0.5f, .y = 0.5f } };
    param->fields.position = { .fields = { .x = 800.0f, .y = 300.0f, .z = 0.0f } };
    param->fields.minItemWidth = 122.0f;
    param->fields.cancelIndex = cancelIndex;
    param->fields.useCancel = true;
    param->fields.useLoopAndRepeat = false;
    param->fields.isInputEnable = true;

    auto* uiMgr = Dpr::UI::UIManager::get_Instance();
    if (uiMgr == nullptr) return false;

    auto* createMethod = *Dpr::UI::ContextMenuWindow::Method$$CreateUIWindow;
    if (createMethod == nullptr) return false;

    auto* window = uiMgr->CreateUIWindow<Dpr::UI::ContextMenuWindow>(UIWindowID::CONTEXTMENU);
    if (window == nullptr) return false;

    // Create Func<ContextMenuItem, bool> delegate
    if (*methodInfoCache == nullptr) {
        auto* srcMethod = *Dpr::UI::UIPofinCase::DisplayClass35_0::Method$$ShowItemContextMenu_b__1;
        if (srcMethod == nullptr) return false;
        *methodInfoCache = srcMethod->copyWith((Il2CppMethodPointer)callback);
    }

    auto* dispClass = Dpr::UI::UIPofinCase::DisplayClass35_0::getClass();
    dispClass->initIfNeeded();
    auto* disp = dispClass->newInstance();

    auto* funcClass = System::Func::getClass(System::Func::ContextMenuItem_bool__TypeInfo);
    if (funcClass == nullptr) return false;
    funcClass->initIfNeeded();
    window->fields.onClicked = funcClass->newInstance(disp, *methodInfoCache);

    window->Open(param);
    return true;
}

// ---------------------------------------------------------------------------
// Emote sub-menu callback
// ---------------------------------------------------------------------------
static bool onEmoteMenuClicked(void* __this, Dpr::UI::ContextMenuItem::Object* item, MethodInfo* mi) {
    if (item == nullptr) {
        s_interact.Reset();
        closeInteractionMenu();
        return true;
    }

    int32_t index = (int32_t)item->fields._param->fields.menuId;
    Logger::log("[OverworldMP] Emote selection: index=%d\n", index);

    // Map index to emote ID
    uint8_t emoteId = 0;
    switch (index) {
        case 0: emoteId = EMOTE_ID_LIKES;       break; // Like
        case 1: emoteId = EMOTE_ID_EXCLAMATION;  break; // Surprise
        case 2: emoteId = EMOTE_ID_CROSS;        break; // No Way
        case 3: emoteId = EMOTE_ID_TALK;         break; // Hello
        case 4: // Cancel
        default:
            Logger::log("[OverworldMP] Emote cancelled\n");
            s_interact.Reset();
            closeInteractionMenu();
            return true;
    }

    Logger::log("[OverworldMP] Sending emote: id=%d\n", (int)emoteId);
    overworldMPSendEmote(emoteId);
    showLocalEmoteBalloon(emoteId);

    s_interact.Reset();
    closeInteractionMenu();
    return true;
}

// ---------------------------------------------------------------------------
// Main menu callback
// ---------------------------------------------------------------------------
static bool onMainMenuClicked(void* __this, Dpr::UI::ContextMenuItem::Object* item, MethodInfo* mi) {
    if (item == nullptr) {
        s_interact.Reset();
        closeInteractionMenu();
        return true;
    }

    int32_t index = (int32_t)item->fields._param->fields.menuId;
    Logger::log("[OverworldMP] Menu selection: index=%d\n", index);

    switch (index) {
        case 0: // Battle
            Logger::log("[OverworldMP] Battle selected (TODO)\n");
            s_interact.Reset();
            closeInteractionMenu();
            break;

        case 1: // Trade
            Logger::log("[OverworldMP] Trade selected (TODO)\n");
            s_interact.Reset();
            closeInteractionMenu();
            break;

        case 2: // Emote → open sub-menu after main menu closes
            Logger::log("[OverworldMP] Emote selected — opening sub-menu\n");
            s_interact.state = InteractState::EmoteMenuPending;
            s_interact.emoteMenuDelay = 0.3f; // wait for close animation
            // Don't re-enable input — emote menu will take over
            break;

        case 3: // Cancel
        default:
            Logger::log("[OverworldMP] Interaction cancelled\n");
            s_interact.Reset();
            closeInteractionMenu();
            break;
    }

    return true;
}

// ---------------------------------------------------------------------------
// Open the emote sub-menu
// ---------------------------------------------------------------------------
static void overworldMPShowEmoteMenu() {
    const char* items[] = { "Like", "Surprise", "No Way", "Hello", "Cancel" };
    s_interact.state = InteractState::EmoteMenuOpen;

    if (!openContextMenu(items, 5, 4, &onEmoteMenuClicked, &s_emoteMenuMethodInfo)) {
        Logger::log("[OverworldMP] ERROR: Failed to open emote menu\n");
        s_interact.Reset();
        closeInteractionMenu();
    }
}

// ---------------------------------------------------------------------------
// Interaction menu — follows UgFieldManager::OpenCustomMenu pattern
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

    // Disable player input
    PlayerWork::getClass()->initIfNeeded();
    PlayerWork::getClass()->static_fields->_isPlayerInputActive = false;

    const char* items[] = { "Battle", "Trade", "Emote", "Cancel" };
    if (!openContextMenu(items, 4, 3, &onMainMenuClicked, &s_mainMenuMethodInfo)) {
        Logger::log("[OverworldMP] ERROR: Failed to open interaction menu\n");
        s_interact.Reset();
        closeInteractionMenu();
    }
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

void overworldMPCheckInteraction() {
    if (!isOverworldMPActive()) {
        return;
    }

    // Main or emote menu is open — the callback handles transitions
    if (s_interact.state == InteractState::MenuOpen ||
        s_interact.state == InteractState::EmoteMenuOpen) {
        return;
    }

    // Main menu closed, waiting to open emote sub-menu
    if (s_interact.state == InteractState::EmoteMenuPending) {
        s_interact.emoteMenuDelay -= 0.03333334f; // ~1 frame at 30fps
        if (s_interact.emoteMenuDelay <= 0.0f) {
            overworldMPShowEmoteMenu();
        }
        return;
    }

    // Update timeout for waiting state
    if (s_interact.state == InteractState::WaitingResponse) {
        s_interact.timeoutTimer -= 0.03333334f;
        if (s_interact.timeoutTimer <= 0.0f) {
            Logger::log("[OverworldMP] Interaction timed out\n");
            s_interact.Reset();
            closeInteractionMenu();
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
