#include "exlaunch.hpp"

#include "features/overworld_multiplayer.h"
#include "features/team_up.h"

#include "externals/Audio/AudioManager.h"
#include "externals/ContextMenuID.h"
#include "externals/DPData/MYSTATUS.h"
#include "externals/Dpr/Demo/Demo_Trade.h"
#include "externals/Dpr/EncountTools.h"
#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Dpr/NetworkUtils/NetworkManager.h"
#include "externals/Dpr/UI/BoxWindow.h"
#include "externals/Dpr/UI/ContextMenuWindow.h"
#include "externals/Dpr/UI/ContextMenuItem.h"
#include "externals/Dpr/UI/UIManager.h"
#include "externals/Dpr/UI/UIPofinCase.h"
#include "externals/EntityManager.h"
#include "externals/GameManager.h"
#include "externals/FieldCanvas.h"
#include "externals/FieldManager.h"
#include "externals/FieldObjectEntity.h"
#include "externals/FieldPlayerEntity.h"
#include "externals/OpcState.h"
#include "externals/PlayerWork.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/Pml/PokePara/CoreParam.h"
#include "externals/Pml/PokeParty.h"
#include "externals/Pml/Personal/PersonalSystem.h"
#include "externals/SmartPoint/AssetAssistant/Sequencer.h"
#include "externals/System/String.h"
#include "externals/UIWindowID.h"
#include "externals/MYSTATUS_COMM.h"
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
    ReceivedRequest = 5,   // Received request, showing accept/decline dialog
    Accepted = 6,          // Remote player accepted — transitioning to activity
    Declined = 7,          // Remote player declined
    TradeSelectPoke = 8,   // Party selection menu displayed
    TradeWaitPartner = 9,  // Sent our pick, waiting for partner's 0xC4
    TradeConfirm = 10,     // Both picks received, showing confirm dialog
    TradeWaitFinal = 11,   // We confirmed, waiting for partner's 0xC5
    TradeAnimating = 12,   // Demo_Trade animation playing
    BattleExchangeParty = 13, // Sending/receiving battle party data (0xC6)
    BattleSyncWait = 14,      // Sent BATTLE_READY, waiting for partner's BATTLE_READY
    BattleStarting = 15,      // Both sides ready, entering battle scene
    BattleMenuOpen = 16,      // Battle type submenu displayed (Single/Double/Back)
    TradeLoadingBox = 17,     // Waiting for BoxWindow prefab to finish preloading
    TeamUpBattleStarting = 18, // Partner received TEAMUP_BATTLE, entering battle
};

struct InteractContext {
    InteractState state;
    int32_t targetStationIndex;
    float timeoutTimer;
    float emoteMenuDelay;  // brief delay before opening emote sub-menu
    float tradeStartDelay; // brief delay before opening trade UI (after accept)

    void Reset() {
        state = InteractState::Idle;
        targetStationIndex = -1;
        timeoutTimer = 0.0f;
        emoteMenuDelay = 0.0f;
        tradeStartDelay = 0.0f;
    }
};

static InteractContext s_interact;
static constexpr float INTERACT_TIMEOUT = 30.0f;

// MethodInfo for our onClicked callbacks — lazily initialized via copyWith
static MethodInfo* s_mainMenuMethodInfo = nullptr;
static MethodInfo* s_emoteMenuMethodInfo = nullptr;
static MethodInfo* s_incomingRequestMethodInfo = nullptr;

// Pending incoming request state (stored when 0xC2 arrives, used by the dialog callback)
static int32_t s_pendingFromStation = -1;
static InteractionType s_pendingRequestType = InteractionType::Trade;

// Trade state
static constexpr int32_t POKE_CORE_DATA_SIZE = 328;   // m_pCoreData byte array length
static constexpr int32_t POKE_CALC_DATA_SIZE = 16;    // m_pCalcData byte array length
static constexpr int32_t POKE_FULL_DATA_SIZE = POKE_CORE_DATA_SIZE + POKE_CALC_DATA_SIZE; // 344 bytes
static int32_t s_tradePartnerStation = -1;
static int32_t s_myTradePartySlot = -1;
static uint8_t s_myTradePokeData[POKE_FULL_DATA_SIZE] = {};
static uint8_t s_partnerTradePokeData[POKE_FULL_DATA_SIZE] = {};
static bool s_partnerPokeReceived = false;
static bool s_partnerConfirmReceived = false;
static bool s_partnerConfirmValue = false;
static MethodInfo* s_tradeConfirmMethodInfo = nullptr;

// BoxWindow trade selection
static Dpr::UI::BoxWindow::Object* s_boxWindow = nullptr;
static MethodInfo* s_boxSelectMethodInfo = nullptr;
static MethodInfo* s_boxCancelMethodInfo = nullptr;
static int32_t s_myTradeTrayIndex = -1;  // -1 = party, 0+ = box tray (box deferred to future PR)
static bool s_boxWindowPreloading = false;  // true while OpLoadWindows coroutine is running

// Demo_Trade animation state
static Pml::PokePara::PokemonParam::Object* s_myTradePokeParam = nullptr;       // our poke (for animation display)
static Pml::PokePara::PokemonParam::Object* s_partnerTradePokeParam = nullptr;  // deserialized partner poke
static MethodInfo* s_demoEndMethodInfo = nullptr;

// Battle state
static MethodInfo* s_battleSubmenuMethodInfo = nullptr;
static BattleSubtype s_pendingBattleSubtype = BattleSubtype::Single;
static int32_t s_battlePartnerStation = -1;
static BattleSubtype s_battleSubtype = BattleSubtype::Single;
static bool s_myBattlePartySent = false;
static bool s_partnerBattlePartyReceived = false;
static bool s_isRequester = false;  // true if we initiated the request
static float s_battleExchangeTimeout = 0.0f;
static bool s_myBattleReadySent = false;      // we sent BATTLE_READY
static bool s_partnerBattleReady = false;     // partner sent BATTLE_READY
static float s_battleSyncTimeout = 0.0f;

// Partner battle data (received via 0xC6)
static constexpr int32_t MAX_BATTLE_PARTY_DATA = 6 * POKE_FULL_DATA_SIZE + 64; // 6 pokes * 344 + overhead
static uint8_t s_partnerBattlePartyBuf[MAX_BATTLE_PARTY_DATA] = {};
static int32_t s_partnerBattlePartyBufSize = 0;
static uint8_t s_partnerBattleMemberCount = 0;
static BattleSubtype s_partnerBattleSubtype = BattleSubtype::Single;

// Forward declarations
static bool isBoxWindowInObjectPool();
static void preloadBoxWindow();
static void openTradeBoxWindow(int32_t partnerStation);

static void resetBattleState() {
    s_battlePartnerStation = -1;
    s_battleSubtype = BattleSubtype::Single;
    s_myBattlePartySent = false;
    s_partnerBattlePartyReceived = false;
    s_isRequester = false;
    s_battleExchangeTimeout = 0.0f;
    s_myBattleReadySent = false;
    s_partnerBattleReady = false;
    s_battleSyncTimeout = 0.0f;
    s_partnerBattleMemberCount = 0;
    s_partnerBattlePartyBufSize = 0;
    s_partnerBattleSubtype = BattleSubtype::Single;
    memset(s_partnerBattlePartyBuf, 0, sizeof(s_partnerBattlePartyBuf));
}

// Pre-battle party snapshot — full Serialize_FullData bytes per Pokemon.
// Saved before RecoverAll, restored after battle ends.
// Prevents free healing by battling friends while still allowing a fair PvP fight.
static constexpr int32_t POKE_SNAPSHOT_SIZE = POKE_FULL_DATA_SIZE; // 344 bytes
static uint8_t s_partySnapshot[6][POKE_SNAPSHOT_SIZE] = {};
static int32_t s_partySnapshotCount = 0;
static bool s_partySnapshotValid = false;

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
// Battle submenu callback
// ---------------------------------------------------------------------------
static bool onBattleSubmenuClicked(void* __this, Dpr::UI::ContextMenuItem::Object* item, MethodInfo* mi) {
    if (item == nullptr) {
        s_interact.Reset();
        closeInteractionMenu();
        return true;
    }

    int32_t index = (int32_t)item->fields._param->fields.menuId;
    Logger::log("[OverworldMP] Battle submenu selection: index=%d\n", index);

    switch (index) {
        case 0: // Single Battle
            s_pendingBattleSubtype = BattleSubtype::Single;
            s_pendingRequestType = InteractionType::Battle;
            s_isRequester = true;
            Logger::log("[OverworldMP] Requesting single battle with station %d\n", s_interact.targetStationIndex);
            overworldMPSendInteractionRequest(s_interact.targetStationIndex,
                                              InteractionType::Battle, BattleSubtype::Single);
            s_interact.state = InteractState::WaitingResponse;
            s_interact.timeoutTimer = INTERACT_TIMEOUT;
            break;

        case 1: // Double Battle
            s_pendingBattleSubtype = BattleSubtype::Double;
            s_pendingRequestType = InteractionType::Battle;
            s_isRequester = true;
            Logger::log("[OverworldMP] Requesting double battle with station %d\n", s_interact.targetStationIndex);
            overworldMPSendInteractionRequest(s_interact.targetStationIndex,
                                              InteractionType::Battle, BattleSubtype::Double);
            s_interact.state = InteractState::WaitingResponse;
            s_interact.timeoutTimer = INTERACT_TIMEOUT;
            break;

        case 2: // Back
        default:
            Logger::log("[OverworldMP] Battle submenu — back to main menu\n");
            s_interact.Reset();
            closeInteractionMenu();
            break;
    }

    return true;
}

// ---------------------------------------------------------------------------
// Open the battle type submenu
// ---------------------------------------------------------------------------
static void overworldMPShowBattleSubmenu() {
    const char* items[] = { "Single Battle", "Double Battle", "Back" };
    s_interact.state = InteractState::BattleMenuOpen;

    if (!openContextMenu(items, 3, 2, &onBattleSubmenuClicked, &s_battleSubmenuMethodInfo)) {
        Logger::log("[OverworldMP] ERROR: Failed to open battle submenu\n");
        s_interact.Reset();
        closeInteractionMenu();
    }
}

// ---------------------------------------------------------------------------
// Main menu callback (not teamed up: Battle, Trade, Team Up, Emote, Cancel)
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
        case 0: // Battle → open submenu to pick Single/Double
            Logger::log("[OverworldMP] Battle selected — opening submenu\n");
            s_interact.state = InteractState::BattleMenuOpen;
            s_interact.emoteMenuDelay = 0.3f;
            break;

        case 1: // Trade
            Logger::log("[OverworldMP] Requesting trade with station %d\n", s_interact.targetStationIndex);
            s_pendingRequestType = InteractionType::Trade;
            s_isRequester = true;
            overworldMPSendInteractionRequest(s_interact.targetStationIndex,
                                              InteractionType::Trade, BattleSubtype::Single);
            s_interact.state = InteractState::WaitingResponse;
            s_interact.timeoutTimer = INTERACT_TIMEOUT;
            break;

        case 2: // Team Up
            Logger::log("[OverworldMP] Requesting team-up with station %d\n", s_interact.targetStationIndex);
            s_pendingRequestType = InteractionType::TeamUp;
            s_isRequester = true;
            overworldMPSendInteractionRequest(s_interact.targetStationIndex,
                                              InteractionType::TeamUp, BattleSubtype::Single);
            s_interact.state = InteractState::WaitingResponse;
            s_interact.timeoutTimer = INTERACT_TIMEOUT;
            break;

        case 3: // Emote → open sub-menu after main menu closes
            Logger::log("[OverworldMP] Emote selected — opening sub-menu\n");
            s_interact.state = InteractState::EmoteMenuPending;
            s_interact.emoteMenuDelay = 0.3f;
            break;

        case 4: // Cancel
        default:
            Logger::log("[OverworldMP] Interaction cancelled\n");
            s_interact.Reset();
            closeInteractionMenu();
            break;
    }

    return true;
}

// ---------------------------------------------------------------------------
// Teamed-up menu callback (targeting partner: Disband, Trade, Emote, Cancel)
// ---------------------------------------------------------------------------
static MethodInfo* s_teamMenuMethodInfo = nullptr;

static bool onTeamMenuClicked(void* __this, Dpr::UI::ContextMenuItem::Object* item, MethodInfo* mi) {
    if (item == nullptr) {
        s_interact.Reset();
        closeInteractionMenu();
        return true;
    }

    int32_t index = (int32_t)item->fields._param->fields.menuId;
    Logger::log("[OverworldMP] Team menu selection: index=%d\n", index);

    switch (index) {
        case 0: // Disband
            Logger::log("[OverworldMP] Disbanding team-up\n");
            overworldMPTeamUpDisband();
            s_interact.Reset();
            closeInteractionMenu();
            break;

        case 1: // Trade
            Logger::log("[OverworldMP] Requesting trade with partner station %d\n", s_interact.targetStationIndex);
            s_pendingRequestType = InteractionType::Trade;
            s_isRequester = true;
            overworldMPSendInteractionRequest(s_interact.targetStationIndex,
                                              InteractionType::Trade, BattleSubtype::Single);
            s_interact.state = InteractState::WaitingResponse;
            s_interact.timeoutTimer = INTERACT_TIMEOUT;
            break;

        case 2: // Emote
            Logger::log("[OverworldMP] Emote selected — opening sub-menu\n");
            s_interact.state = InteractState::EmoteMenuPending;
            s_interact.emoteMenuDelay = 0.3f;
            break;

        case 3: // Cancel
        default:
            Logger::log("[OverworldMP] Team menu cancelled\n");
            s_interact.Reset();
            closeInteractionMenu();
            break;
    }

    return true;
}

// ---------------------------------------------------------------------------
// Teamed-up menu callback (targeting non-partner: Team Battle, Trade, Emote, Cancel)
// ---------------------------------------------------------------------------
static MethodInfo* s_teamBattleMenuMethodInfo = nullptr;

static bool onTeamBattleMenuClicked(void* __this, Dpr::UI::ContextMenuItem::Object* item, MethodInfo* mi) {
    if (item == nullptr) {
        s_interact.Reset();
        closeInteractionMenu();
        return true;
    }

    int32_t index = (int32_t)item->fields._param->fields.menuId;
    Logger::log("[OverworldMP] Team battle menu selection: index=%d\n", index);

    switch (index) {
        case 0: // Team Battle (PvP — future)
            Logger::log("[OverworldMP] Team Battle not yet implemented\n");
            s_interact.Reset();
            closeInteractionMenu();
            break;

        case 1: // Trade
            Logger::log("[OverworldMP] Requesting trade with station %d\n", s_interact.targetStationIndex);
            s_pendingRequestType = InteractionType::Trade;
            s_isRequester = true;
            overworldMPSendInteractionRequest(s_interact.targetStationIndex,
                                              InteractionType::Trade, BattleSubtype::Single);
            s_interact.state = InteractState::WaitingResponse;
            s_interact.timeoutTimer = INTERACT_TIMEOUT;
            break;

        case 2: // Emote
            Logger::log("[OverworldMP] Emote selected — opening sub-menu\n");
            s_interact.state = InteractState::EmoteMenuPending;
            s_interact.emoteMenuDelay = 0.3f;
            break;

        case 3: // Cancel
        default:
            Logger::log("[OverworldMP] Team battle menu cancelled\n");
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

    // Start preloading BoxWindow early (async) — by the time the player
    // navigates menus and accepts a trade, the prefab should be loaded.
    preloadBoxWindow();

    // Disable player input
    PlayerWork::getClass()->initIfNeeded();
    PlayerWork::getClass()->static_fields->_isPlayerInputActive = false;

    auto& tu = overworldMPGetTeamUpState();
    if (tu.isTeamedUp && tu.partnerStation == targetStationIndex) {
        // Teamed up, targeting partner: Disband, Trade, Emote, Cancel
        const char* items[] = { "Disband", "Trade", "Emote", "Cancel" };
        if (!openContextMenu(items, 4, 3, &onTeamMenuClicked, &s_teamMenuMethodInfo)) {
            Logger::log("[OverworldMP] ERROR: Failed to open team menu\n");
            s_interact.Reset();
            closeInteractionMenu();
        }
    } else if (tu.isTeamedUp) {
        // Teamed up, targeting non-partner: Team Battle, Trade, Emote, Cancel
        const char* items[] = { "Team Battle", "Trade", "Emote", "Cancel" };
        if (!openContextMenu(items, 4, 3, &onTeamBattleMenuClicked, &s_teamBattleMenuMethodInfo)) {
            Logger::log("[OverworldMP] ERROR: Failed to open team battle menu\n");
            s_interact.Reset();
            closeInteractionMenu();
        }
    } else {
        // Not teamed up: Battle, Trade, Team Up, Emote, Cancel
        const char* items[] = { "Battle", "Trade", "Team Up", "Emote", "Cancel" };
        if (!openContextMenu(items, 5, 4, &onMainMenuClicked, &s_mainMenuMethodInfo)) {
            Logger::log("[OverworldMP] ERROR: Failed to open interaction menu\n");
            s_interact.Reset();
            closeInteractionMenu();
        }
    }
}

// ---------------------------------------------------------------------------
// Incoming request dialog callback
// ---------------------------------------------------------------------------
static bool onIncomingRequestClicked(void* __this, Dpr::UI::ContextMenuItem::Object* item, MethodInfo* mi) {
    if (item == nullptr) {
        // Dialog dismissed (e.g. B-button cancel) — treat as decline
        Logger::log("[OverworldMP] Incoming request dialog dismissed — declining\n");
        overworldMPSendInteractionResponse(s_pendingFromStation, false);
        s_pendingFromStation = -1;
        s_interact.Reset();
        closeInteractionMenu();
        return true;
    }

    int32_t index = (int32_t)item->fields._param->fields.menuId;
    Logger::log("[OverworldMP] Incoming request response: index=%d\n", index);

    if (index == 0) {
        // Accept — defer trade/battle start to avoid opening UI inside callback
        Logger::log("[OverworldMP] Accepting request from station %d (type=%d)\n",
                    s_pendingFromStation, (int)s_pendingRequestType);
        overworldMPSendInteractionResponse(s_pendingFromStation, true);
        overworldMPShowRemoteEmote(s_pendingFromStation, EMOTE_ID_LIKES);
        s_interact.state = InteractState::Accepted;
        s_interact.targetStationIndex = s_pendingFromStation;
        s_interact.tradeStartDelay = 0.3f;
        s_pendingFromStation = -1;
    } else {
        // Decline (index 1 or B-button)
        Logger::log("[OverworldMP] Declining request from station %d\n", s_pendingFromStation);
        overworldMPSendInteractionResponse(s_pendingFromStation, false);
        s_pendingFromStation = -1;
        s_interact.Reset();
        closeInteractionMenu();
    }

    return true;
}

// ---------------------------------------------------------------------------
// Show incoming request dialog (called from 0xC2 receive handler)
// ---------------------------------------------------------------------------
void overworldMPShowIncomingRequestDialog(int32_t fromStation, InteractionType type, BattleSubtype battleSubtype) {
    auto& ctx = getOverworldMPContext();
    if (fromStation < 0 || fromStation >= OW_MP_MAX_PLAYERS) return;

    auto& remote = ctx.remotePlayers[fromStation];
    if (!remote.isActive) return;

    // If the local player is already in a menu/interaction, ignore the request
    if (s_interact.state != InteractState::Idle) {
        Logger::log("[OverworldMP] Ignoring incoming request — local interact state=%d\n",
                    (int)s_interact.state);
        return;
    }

    s_pendingFromStation = fromStation;
    s_pendingRequestType = type;
    s_pendingBattleSubtype = battleSubtype;
    s_isRequester = false;  // we are the receiver
    s_interact.state = InteractState::ReceivedRequest;

    // Disable player input while dialog is open
    PlayerWork::getClass()->initIfNeeded();
    PlayerWork::getClass()->static_fields->_isPlayerInputActive = false;

    // Build descriptive button text
    const char* actionText = "Trade";
    if (type == InteractionType::Battle) {
        actionText = (battleSubtype == BattleSubtype::Double) ? "Double Battle" : "Single Battle";
    } else if (type == InteractionType::TeamUp) {
        actionText = "Team Up";
    }
    const char* nameBuf = remote.playerNameSet ? remote.playerNameBuf : "Player";

    static char acceptBuf[128];
    snprintf(acceptBuf, sizeof(acceptBuf), "%s with %s?", actionText, nameBuf);
    Logger::log("[OverworldMP] Showing incoming request dialog: %s\n", acceptBuf);

    const char* items[] = { acceptBuf, "Decline" };
    if (!openContextMenu(items, 2, 1, &onIncomingRequestClicked, &s_incomingRequestMethodInfo)) {
        Logger::log("[OverworldMP] ERROR: Failed to open incoming request dialog\n");
        overworldMPSendInteractionResponse(fromStation, false);
        s_pendingFromStation = -1;
        s_interact.Reset();
        closeInteractionMenu();
    }
}

// ---------------------------------------------------------------------------
// Trade: reset helper
// ---------------------------------------------------------------------------
static void resetTradeState() {
    s_tradePartnerStation = -1;
    s_myTradePartySlot = -1;
    s_myTradeTrayIndex = -1;
    s_boxWindow = nullptr;
    s_myTradePokeParam = nullptr;
    s_partnerTradePokeParam = nullptr;
    memset(s_myTradePokeData, 0, sizeof(s_myTradePokeData));
    memset(s_partnerTradePokeData, 0, sizeof(s_partnerTradePokeData));
    s_partnerPokeReceived = false;
    s_partnerConfirmReceived = false;
    s_partnerConfirmValue = false;
}

// ---------------------------------------------------------------------------
// Trade: show confirm dialog (both picks received)
// ---------------------------------------------------------------------------
static void showTradeConfirmDialog();

// ---------------------------------------------------------------------------
// Trade: deserialize partner pokemon from received byte data
// ---------------------------------------------------------------------------
static Pml::PokePara::PokemonParam::Object* deserializePartnerPokemon(Pml::PokeParty::Object* party) {
    auto* existingPoke = party->GetMemberPointer(0);
    if (existingPoke == nullptr) {
        Logger::log("[OverworldMP] ERROR: no party member to use as template\n");
        return nullptr;
    }

    auto* ppClass = Pml::PokePara::PokemonParam::getClass();
    auto* newPoke = (Pml::PokePara::PokemonParam::Object*)il2cpp_object_new((Il2CppClass*)ppClass);
    newPoke->ctor(existingPoke);

    auto* accessor = newPoke->fields.m_accessor;
    if (accessor != nullptr) {
        // Use Deserialize_FullData (raw pointer version) @ 0x24A4550
        // Properly handles decode/re-encode cycle for PokePara data
        _ILExternal::external<void>(0x24A4550, accessor, s_partnerTradePokeData);
        Logger::log("[OverworldMP] Deserialized partner pokemon into new PokemonParam\n");
    } else {
        Logger::log("[OverworldMP] WARNING: newPoke accessor is null\n");
    }
    return newPoke;
}

// ---------------------------------------------------------------------------
// Trade: Demo_Trade OnEndDemo callback — fires when animation finishes
// Signature: void(MethodInfo*) — static, no target
// ---------------------------------------------------------------------------
static void onDemoEnd(MethodInfo* mi) {
    Logger::log("[OverworldMP] Demo_Trade animation ended — executing swap\n");

    // Set FieldCanvas flags (matches NPC trade pattern)
    FieldCanvas::getClass()->initIfNeeded();
    auto* statics = FieldCanvas::getClass()->static_fields;
    statics->isNPCTradeEnd = true;
    statics->isNPCTrading = false;

    // Do the actual party replacement
    auto* party = PlayerWork::get_playerParty();
    if (party != nullptr && s_partnerTradePokeParam != nullptr && s_myTradePartySlot >= 0) {
        party->ReplaceMember(s_myTradePartySlot, s_partnerTradePokeParam);
        Logger::log("[OverworldMP] Trade complete — replaced party slot %d\n", s_myTradePartySlot);
    } else {
        Logger::log("[OverworldMP] ERROR: cannot swap — party=%p, partnerPoke=%p, slot=%d\n",
                    party, s_partnerTradePokeParam, s_myTradePartySlot);
    }

    // Unlock box (matches NPC trade pattern)
    Dpr::UI::BoxWindow::LimitBox(false);

    // Show emotes as additional feedback
    overworldMPShowRemoteEmote(s_tradePartnerStation, EMOTE_ID_LIKES);
    showLocalEmoteBalloon(EMOTE_ID_LIKES);

    resetTradeState();
    s_interact.Reset();
    closeInteractionMenu();
}

// ---------------------------------------------------------------------------
// Trade: try to start Demo_Trade animation, returns false if it can't
// ---------------------------------------------------------------------------
static bool tryStartDemoTrade(Pml::PokePara::PokemonParam::Object* myPoke,
                              Pml::PokePara::PokemonParam::Object* partnerPoke) {
    // Create Demo_Trade instance
    auto* demoClass = Dpr::Demo::Demo_Trade::getClass();
    if (demoClass == nullptr) {
        Logger::log("[OverworldMP] Demo_Trade class not available\n");
        return false;
    }
    demoClass->initIfNeeded();
    auto* demo = (Dpr::Demo::Demo_Trade::Object*)il2cpp_object_new((Il2CppClass*)demoClass);
    if (demo == nullptr) {
        Logger::log("[OverworldMP] Failed to create Demo_Trade instance\n");
        return false;
    }
    demo->ctor();

    // Set pokemon params (matches NPC trade flow: offsets 0xC8 and 0xD0)
    demo->fields.MyPokeParam = myPoke;
    demo->fields.FriendPokeParam = partnerPoke;

    // Get trainer IDs and partner name for SetParam
    auto* playerStatus = PlayerWork::get_playerStatus();
    int32_t myTID = (playerStatus != nullptr) ? (int32_t)playerStatus->fields.id : 0;

    int32_t partnerTID = myTID + 1; // Ensure different from myTID for "other trainer" message path
    System::String::Object* partnerName = System::String::Create("Trainer");
    auto& ctx = getOverworldMPContext();
    if (s_tradePartnerStation >= 0 && s_tradePartnerStation < OW_MP_MAX_PLAYERS) {
        auto& remote = ctx.remotePlayers[s_tradePartnerStation];
        if (remote.playerNameSet) {
            partnerName = System::String::Create(remote.playerNameBuf);
        }
    }

    int32_t langId = (int32_t)PlayerWork::get_msgLangID();
    demo->SetParam(myTID, partnerTID, partnerName, false, langId);

    // Lock box UI (matches NPC trade pattern)
    Dpr::UI::BoxWindow::LimitBox(true);

    // Create OnEndDemo callback via nn_malloc MethodInfo pattern
    if (s_demoEndMethodInfo == nullptr) {
        s_demoEndMethodInfo = (MethodInfo*)nn_malloc(sizeof(MethodInfo));
        memset(s_demoEndMethodInfo, 0, sizeof(MethodInfo));
        s_demoEndMethodInfo->methodPointer = (Il2CppMethodPointer)&onDemoEnd;
        s_demoEndMethodInfo->flags = 0x0010;          // METHOD_ATTRIBUTE_STATIC
        s_demoEndMethodInfo->parameters_count = 0;     // void()
    }

    auto* actionClass = System::Action::getClass(System::Action::void_TypeInfo);
    if (actionClass == nullptr) {
        Logger::log("[OverworldMP] Failed to get Action class for OnEndDemo\n");
        Dpr::UI::BoxWindow::LimitBox(false);
        return false;
    }
    actionClass->initIfNeeded();
    if (s_demoEndMethodInfo->klass == nullptr) {
        s_demoEndMethodInfo->klass = (Il2CppClass*)actionClass;
    }
    auto* endAction = (System::Action::Object*)il2cpp_object_new((Il2CppClass*)actionClass);
    _ILExternal::external<void>(0x023feb30, endAction, nullptr, s_demoEndMethodInfo);

    // Set OnEndDemo on DemoBase (offset 0x30 absolute, inherited field)
    demo->fields.OnEndDemo = endAction;

    // Set FieldCanvas NPC trading flag
    FieldCanvas::getClass()->initIfNeeded();
    FieldCanvas::getClass()->static_fields->isNPCTrading = true;

    // Play the trade cutscene
    Logger::log("[OverworldMP] Starting Demo_Trade animation\n");
    FieldCanvas::PlayDemo((Dpr::Demo::DemoBase::Object*)demo, true);

    return true;
}

// ---------------------------------------------------------------------------
// Trade: execute the swap (with Demo_Trade animation, or immediate fallback)
// ---------------------------------------------------------------------------
static void overworldMPExecuteSwap() {
    Logger::log("[OverworldMP] Executing trade swap: my slot=%d, partner station=%d\n",
                s_myTradePartySlot, s_tradePartnerStation);

    auto* party = PlayerWork::get_playerParty();
    if (party == nullptr) {
        Logger::log("[OverworldMP] ERROR: playerParty is null, cannot execute swap\n");
        resetTradeState();
        s_interact.Reset();
        closeInteractionMenu();
        return;
    }

    // Get our pokemon before the swap (for Demo_Trade display)
    auto* myPoke = (s_myTradePartySlot >= 0) ? party->GetMemberPointer(s_myTradePartySlot) : nullptr;
    if (myPoke == nullptr) {
        Logger::log("[OverworldMP] ERROR: our trade pokemon is null\n");
        resetTradeState();
        s_interact.Reset();
        closeInteractionMenu();
        return;
    }

    // Deserialize partner's pokemon from received byte data
    auto* partnerPoke = deserializePartnerPokemon(party);
    if (partnerPoke == nullptr) {
        Logger::log("[OverworldMP] ERROR: failed to deserialize partner pokemon\n");
        resetTradeState();
        s_interact.Reset();
        closeInteractionMenu();
        return;
    }

    // Store for use in the OnEndDemo callback
    s_myTradePokeParam = myPoke;
    s_partnerTradePokeParam = partnerPoke;

    // Try to start the trade cutscene animation
    if (tryStartDemoTrade(myPoke, partnerPoke)) {
        s_interact.state = InteractState::TradeAnimating;
        Logger::log("[OverworldMP] Demo_Trade started — swap will happen when animation ends\n");
        return;
    }

    // Fallback: no animation available — swap immediately
    Logger::log("[OverworldMP] Demo_Trade unavailable — doing immediate swap\n");
    party->ReplaceMember(s_myTradePartySlot, partnerPoke);
    Logger::log("[OverworldMP] Trade complete — replaced party slot %d\n", s_myTradePartySlot);

    overworldMPShowRemoteEmote(s_tradePartnerStation, EMOTE_ID_LIKES);
    showLocalEmoteBalloon(EMOTE_ID_LIKES);

    resetTradeState();
    s_interact.Reset();
    closeInteractionMenu();
}

// ---------------------------------------------------------------------------
// Trade: confirm dialog callback
// ---------------------------------------------------------------------------
static bool onTradeConfirmClicked(void* __this, Dpr::UI::ContextMenuItem::Object* item, MethodInfo* mi) {
    if (item == nullptr) {
        // B-button cancel — treat as decline
        Logger::log("[OverworldMP] Trade confirm cancelled via B-button\n");
        overworldMPSendTradeConfirm(s_tradePartnerStation, false);
        resetTradeState();
        s_interact.Reset();
        closeInteractionMenu();
        return true;
    }

    int32_t index = (int32_t)item->fields._param->fields.menuId;
    Logger::log("[OverworldMP] Trade confirm selection: index=%d\n", index);

    if (index == 0) {
        // Yes — confirm trade
        Logger::log("[OverworldMP] Confirming trade with station %d\n", s_tradePartnerStation);
        overworldMPSendTradeConfirm(s_tradePartnerStation, true);

        // Check if partner already confirmed
        if (s_partnerConfirmReceived && s_partnerConfirmValue) {
            Logger::log("[OverworldMP] Partner already confirmed — executing swap\n");
            overworldMPExecuteSwap();
        } else {
            // Wait for partner's confirmation
            s_interact.state = InteractState::TradeWaitFinal;
            Logger::log("[OverworldMP] Waiting for partner's trade confirmation\n");
        }
    } else {
        // No — cancel trade
        Logger::log("[OverworldMP] Declining trade with station %d\n", s_tradePartnerStation);
        overworldMPSendTradeConfirm(s_tradePartnerStation, false);
        resetTradeState();
        s_interact.Reset();
        closeInteractionMenu();
    }

    return true;
}

// ---------------------------------------------------------------------------
// Trade: show confirm dialog
// ---------------------------------------------------------------------------
static void showTradeConfirmDialog() {
    s_interact.state = InteractState::TradeConfirm;

    // Get names for display
    auto* party = PlayerWork::get_playerParty();
    const char* myPokeName = "???";
    const char* theirPokeName = "???";
    char myNameBuf[64];
    char theirNameBuf[64];

    if (party != nullptr && s_myTradePartySlot >= 0) {
        auto* myPoke = party->GetMemberPointer(s_myTradePartySlot);
        if (myPoke != nullptr) {
            int32_t monsNo = ((Pml::PokePara::CoreParam*)myPoke)->GetMonsNo();
            auto* nameStr = Pml::Personal::PersonalSystem::GetMonsName(monsNo, 2); // 2 = English
            if (nameStr != nullptr) {
                int len = nameStr->fields.m_stringLength;
                if (len > 60) len = 60;
                for (int i = 0; i < len; i++) {
                    uint16_t c = ((uint16_t*)&nameStr->fields.m_firstChar)[i];
                    myNameBuf[i] = (c < 128) ? (char)c : '?';
                }
                myNameBuf[len] = '\0';
                myPokeName = myNameBuf;
            }
        }
    }

    // Read partner pokemon name from their serialized data
    // The partner's accessor data is raw bytes — create a temp PokemonParam to read monsNo
    if (s_partnerPokeReceived) {
        // Quick approach: read monsNo from partner's coreData offset
        // CoreDataBlockA starts at byte 0 of core data. MonsNo is at a known offset.
        // Instead, create a temp param and use GetMonsNo for accuracy.
        auto* existingPoke = party != nullptr ? party->GetMemberPointer(0) : nullptr;
        if (existingPoke != nullptr) {
            auto* ppClass = Pml::PokePara::PokemonParam::getClass();
            auto* tempPoke = (Pml::PokePara::PokemonParam::Object*)il2cpp_object_new((Il2CppClass*)ppClass);
            tempPoke->ctor(existingPoke);
            auto* accessor = tempPoke->fields.m_accessor;
            if (accessor != nullptr) {
                // Use Deserialize_FullData (raw pointer version) @ 0x24A4550
                _ILExternal::external<void>(0x24A4550, accessor, s_partnerTradePokeData);
            }
            int32_t partnerMonsNo = ((Pml::PokePara::CoreParam*)tempPoke)->GetMonsNo();
            auto* partnerNameStr = Pml::Personal::PersonalSystem::GetMonsName(partnerMonsNo, 2); // 2 = English
            if (partnerNameStr != nullptr) {
                int len = partnerNameStr->fields.m_stringLength;
                if (len > 60) len = 60;
                for (int i = 0; i < len; i++) {
                    uint16_t c = ((uint16_t*)&partnerNameStr->fields.m_firstChar)[i];
                    theirNameBuf[i] = (c < 128) ? (char)c : '?';
                }
                theirNameBuf[len] = '\0';
                theirPokeName = theirNameBuf;
            }
            Logger::log("[OverworldMP] Partner pokemon: monsNo=%d name=%s\n", partnerMonsNo, theirPokeName);
        }
    }

    char confirmBuf[128];
    snprintf(confirmBuf, sizeof(confirmBuf), "Trade %s for %s?", myPokeName, theirPokeName);
    Logger::log("[OverworldMP] Showing trade confirm: %s\n", confirmBuf);

    const char* items[] = { "Yes", "No" };
    if (!openContextMenu(items, 2, 1, &onTradeConfirmClicked, &s_tradeConfirmMethodInfo)) {
        Logger::log("[OverworldMP] ERROR: Failed to open trade confirm dialog\n");
        overworldMPSendTradeConfirm(s_tradePartnerStation, false);
        resetTradeState();
        s_interact.Reset();
        closeInteractionMenu();
    }
}

// ---------------------------------------------------------------------------
// Trade: BoxWindow selection callback (called when user picks a pokemon)
// Signature matches Action<BoxWindow, SelectedPokemon[]> invoke for static target:
//   method_ptr(BoxWindow*, SelectedPokemon[]*, MethodInfo*)
// ---------------------------------------------------------------------------
static void onBoxSelected(Dpr::UI::BoxWindow::Object* window,
                          Dpr::UI::BoxWindow::SelectedPokemon::Array* selected,
                          MethodInfo* mi) {
    Logger::log("[OverworldMP] BoxWindow selection callback fired\n");
    s_boxWindow = nullptr; // BoxWindow closes itself after selection

    if (selected == nullptr || selected->max_length < 1 || selected->m_Items[0] == nullptr) {
        Logger::log("[OverworldMP] BoxWindow selection was empty — cancelling trade\n");
        overworldMPSendTradeConfirm(s_tradePartnerStation, false);
        resetTradeState();
        s_interact.Reset();
        closeInteractionMenu();
        return;
    }

    auto* sel = selected->m_Items[0];
    auto* poke = sel->fields.Param;
    int32_t trayIndex = sel->fields.TrayIndex;
    int32_t indexInTray = sel->fields.IndexInTray;

    Logger::log("[OverworldMP] BoxWindow selected: trayIndex=%d, indexInTray=%d\n",
                trayIndex, indexInTray);

    if (poke == nullptr) {
        Logger::log("[OverworldMP] ERROR: selected pokemon Param is null\n");
        overworldMPSendTradeConfirm(s_tradePartnerStation, false);
        resetTradeState();
        s_interact.Reset();
        closeInteractionMenu();
        return;
    }

    // Only support party trades for now (box trades deferred to future PR)
    if (trayIndex != -1) {
        Logger::log("[OverworldMP] Box pokemon selected (tray=%d) — not supported yet, cancelling\n", trayIndex);
        overworldMPSendTradeConfirm(s_tradePartnerStation, false);
        resetTradeState();
        s_interact.Reset();
        closeInteractionMenu();
        return;
    }

    // Don't allow trading the last party member
    auto* party = PlayerWork::get_playerParty();
    if (party != nullptr && party->fields.m_memberCount <= 1) {
        Logger::log("[OverworldMP] Cannot trade — only 1 pokemon in party\n");
        overworldMPSendTradeConfirm(s_tradePartnerStation, false);
        resetTradeState();
        s_interact.Reset();
        closeInteractionMenu();
        return;
    }

    s_myTradePartySlot = indexInTray;
    s_myTradeTrayIndex = trayIndex;

    // Serialize our selected pokemon
    auto* corePoke = (Pml::PokePara::CoreParam*)poke;
    auto* accessor = poke->fields.m_accessor;
    if (accessor != nullptr) {
        memset(s_myTradePokeData, 0, sizeof(s_myTradePokeData));
        // Use Serialize_FullData (raw pointer version) @ 0x24A4470
        // Ensures data is in encoded state before copying
        _ILExternal::external<void>(0x24A4470, accessor, s_myTradePokeData);
    }

    Logger::log("[OverworldMP] Selected party slot %d (monsNo=%d) for trade, sending 0xC4\n",
                indexInTray, corePoke->GetMonsNo());

    // Send our pokemon data to the partner
    overworldMPSendTradePoke(s_tradePartnerStation, s_myTradePartySlot,
                             s_myTradePokeData, POKE_FULL_DATA_SIZE);

    // Transition to waiting for partner's pick
    if (s_partnerPokeReceived) {
        Logger::log("[OverworldMP] Partner already sent pokemon — showing confirm\n");
        showTradeConfirmDialog();
    } else {
        s_interact.state = InteractState::TradeWaitPartner;
        Logger::log("[OverworldMP] Waiting for partner's pokemon selection\n");
    }
}

// ---------------------------------------------------------------------------
// Trade: BoxWindow cancel callback (called when user backs out without selecting)
// Signature matches Action (non-generic) invoke for static target:
//   method_ptr(MethodInfo*)
// ---------------------------------------------------------------------------
static void onBoxCancelled(MethodInfo* mi) {
    Logger::log("[OverworldMP] BoxWindow trade selection cancelled\n");
    s_boxWindow = nullptr;
    overworldMPSendTradeConfirm(s_tradePartnerStation, false);
    resetTradeState();
    s_interact.Reset();
    closeInteractionMenu();
}

// ---------------------------------------------------------------------------
// BoxWindow preloading — loads the BOX prefab into UIManager's ObjectPool
// ---------------------------------------------------------------------------

static bool isBoxWindowInObjectPool() {
    auto* uiMgr = Dpr::UI::UIManager::get_Instance();
    if (uiMgr == nullptr) return false;

    // UIManager._objectPool at +0xB0
    auto* objectPool = *(uint8_t**)(((uint8_t*)uiMgr) + 0xB0);
    if (objectPool == nullptr) return false;

    // ObjectPool<UIWindowID, UIInstance>._dict at +0x10
    auto* dict = *(uint8_t**)(objectPool + 0x10);
    if (dict == nullptr) return false;

    // Read Dictionary internals directly (avoids ContainsKey MethodInfo crash).
    // Dictionary<UIWindowID, ObjectPool.Param> field layout:
    //   +0x10: int[] buckets
    //   +0x18: Entry[] entries
    //   +0x20: int count
    int32_t count = *(int32_t*)(dict + 0x20);
    Logger::log("[OverworldMP] isBoxPool: dict=%p count=%d\n", dict, count);
    if (count <= 0) return false;

    auto* entries = *(uint8_t**)(dict + 0x18);
    if (entries == nullptr) return false;

    // IL2CPP Array: Il2CppObject(16) + bounds(8) + max_length(8) = data at +0x20
    // Entry<UIWindowID, Param>: hashCode(4) + next(4) + key(4) + pad(4) + value(8) = 0x18
    uint8_t* data = entries + 0x20;
    const int32_t boxKey = (int32_t)UIWindowID::BOX;

    for (int32_t i = 0; i < count; i++) {
        int32_t hashCode = *(int32_t*)(data + (uint64_t)i * 0x18);
        if (hashCode < 0) continue;  // freed slot
        int32_t key = *(int32_t*)(data + (uint64_t)i * 0x18 + 0x08);
        Logger::log("[OverworldMP] isBoxPool: entry[%d] hash=%d key=%d\n", i, hashCode, key);
        if (key == boxKey) return true;
    }
    return false;
}

static void preloadBoxWindow() {
    if (s_boxWindowPreloading) return;
    if (isBoxWindowInObjectPool()) return;

    auto* uiMgr = Dpr::UI::UIManager::get_Instance();
    if (uiMgr == nullptr) return;

    // Create List<UIWindowID> with one entry: BOX (16)
    Il2CppClass* listClass = *reinterpret_cast<Il2CppClass**>(
        exl::util::modules::GetTargetOffset(0x04C5E5C8));
    if (listClass == nullptr) {
        Logger::log("[OverworldMP] List<UIWindowID> TypeInfo not resolved\n");
        return;
    }
    auto* list = il2cpp_object_new(listClass);
    if (list == nullptr) return;

    // List<UIWindowID>.ctor() @ 0x2A72C00
    _ILExternal::external<void>(0x2A72C00, list, (void*)0);
    // List<UIWindowID>.Add(BOX=16) @ 0x2A738C0
    _ILExternal::external<void>(0x2A738C0, list, (int32_t)UIWindowID::BOX, (void*)0);

    // UIManager::OpLoadWindows(list, isVariant=false) @ 0x17B0380
    auto* coroutine = _ILExternal::external<Il2CppObject*>(0x17B0380, uiMgr, list, false, (void*)0);
    if (coroutine == nullptr) {
        Logger::log("[OverworldMP] OpLoadWindows returned null\n");
        return;
    }

    SmartPoint::AssetAssistant::Sequencer::getClass()->initIfNeeded();
    SmartPoint::AssetAssistant::Sequencer::Start(coroutine);

    s_boxWindowPreloading = true;
    Logger::log("[OverworldMP] BoxWindow preload coroutine started\n");
}

// ---------------------------------------------------------------------------
// Trade: open BoxWindow for pokemon selection
// ---------------------------------------------------------------------------
static void openTradeBoxWindow(int32_t partnerStation);

// ---------------------------------------------------------------------------
// Trade: start trade (called after handshake accept)
// ---------------------------------------------------------------------------
void overworldMPStartTrade(int32_t partnerStation) {
    Logger::log("[OverworldMP] Starting trade with station %d\n", partnerStation);

    resetTradeState();
    s_tradePartnerStation = partnerStation;

    auto* party = PlayerWork::get_playerParty();
    if (party == nullptr) {
        Logger::log("[OverworldMP] ERROR: playerParty is null, cannot start trade\n");
        s_interact.Reset();
        closeInteractionMenu();
        return;
    }

    int32_t memberCount = party->fields.m_memberCount;
    if (memberCount <= 0) {
        Logger::log("[OverworldMP] ERROR: party is empty\n");
        s_interact.Reset();
        closeInteractionMenu();
        return;
    }

    openTradeBoxWindow(partnerStation);
}

// ---------------------------------------------------------------------------
// Trade: open BoxWindow for pokemon selection
// ---------------------------------------------------------------------------
static void openTradeBoxWindow(int32_t partnerStation) {
    Logger::log("[OverworldMP] openTradeBoxWindow: enter\n");
    auto* uiMgr = Dpr::UI::UIManager::get_Instance();
    if (uiMgr == nullptr) {
        Logger::log("[OverworldMP] UIManager is null, cannot open BoxWindow\n");
        resetTradeState();
        s_interact.Reset();
        closeInteractionMenu();
        return;
    }

    // If BoxWindow prefab isn't loaded yet, enter loading state and poll.
    // preloadBoxWindow() was already called when the interaction menu opened;
    // this handles the case where loading is still in progress.
    bool inPool = isBoxWindowInObjectPool();
    Logger::log("[OverworldMP] openTradeBoxWindow: inPool=%d\n", (int)inPool);
    if (!inPool) {
        // Ensure preload is started (in case it wasn't triggered earlier)
        preloadBoxWindow();
        s_interact.state = InteractState::TradeLoadingBox;
        s_interact.timeoutTimer = 10.0f;  // 10s timeout for asset loading
        Logger::log("[OverworldMP] BoxWindow not yet loaded — waiting for preload\n");
        return;
    }

    s_boxWindowPreloading = false;  // loading complete

    Logger::log("[OverworldMP] openTradeBoxWindow: calling CreateUIWindow\n");
    auto* boxWindow = uiMgr->CreateUIWindow<Dpr::UI::BoxWindow>(UIWindowID::BOX);
    if (boxWindow == nullptr) {
        Logger::log("[OverworldMP] Failed to create BoxWindow\n");
        resetTradeState();
        s_interact.Reset();
        closeInteractionMenu();
        return;
    }

    s_boxWindow = boxWindow;

    // Create OpenParam for single pokemon selection
    system_load_typeinfo(0x2672);
    auto* openParam = Dpr::UI::BoxWindow::OpenParam::newInstance();
    openParam->fields.openType = 1;          // SelectPokemon mode
    openParam->fields.selectCount = 1;
    openParam->fields.isEnableEgg = false;   // don't allow trading eggs
    openParam->fields.isSelectParty = true;  // start on party view
    openParam->fields.isEnableParty = true;

    // Create MethodInfo for our selection callback (nn_malloc pattern)
    if (s_boxSelectMethodInfo == nullptr) {
        s_boxSelectMethodInfo = (MethodInfo*)nn_malloc(sizeof(MethodInfo));
        memset(s_boxSelectMethodInfo, 0, sizeof(MethodInfo));
        s_boxSelectMethodInfo->methodPointer = (Il2CppMethodPointer)&onBoxSelected;
        s_boxSelectMethodInfo->flags = 0x0010;          // METHOD_ATTRIBUTE_STATIC
        s_boxSelectMethodInfo->parameters_count = 2;     // void(BoxWindow, SelectedPokemon[])
    }

    // Create Action<BoxWindow, SelectedPokemon[]> delegate
    auto* selectActionClass = System::Action::getClass(System::Action::BoxWindow_SelectedPokemon_TypeInfo);
    if (selectActionClass == nullptr) {
        Logger::log("[OverworldMP] Failed to get Action<BoxWindow,SelectedPokemon[]> class\n");
        s_boxWindow = nullptr;
        resetTradeState();
        s_interact.Reset();
        closeInteractionMenu();
        return;
    }
    selectActionClass->initIfNeeded();
    if (s_boxSelectMethodInfo->klass == nullptr) {
        s_boxSelectMethodInfo->klass = (Il2CppClass*)selectActionClass;
    }
    auto* selectAction = (System::Action::Object*)il2cpp_object_new((Il2CppClass*)selectActionClass);
    _ILExternal::external<void>(0x0263EBA0, selectAction, nullptr, s_boxSelectMethodInfo);

    // Create MethodInfo for our cancel callback
    if (s_boxCancelMethodInfo == nullptr) {
        s_boxCancelMethodInfo = (MethodInfo*)nn_malloc(sizeof(MethodInfo));
        memset(s_boxCancelMethodInfo, 0, sizeof(MethodInfo));
        s_boxCancelMethodInfo->methodPointer = (Il2CppMethodPointer)&onBoxCancelled;
        s_boxCancelMethodInfo->flags = 0x0010;          // METHOD_ATTRIBUTE_STATIC
        s_boxCancelMethodInfo->parameters_count = 0;     // void()
    }

    // Create Action (non-generic) delegate for cancel callback
    auto* cancelActionClass = System::Action::getClass(System::Action::void_TypeInfo);
    if (cancelActionClass == nullptr) {
        Logger::log("[OverworldMP] Failed to get Action class for cancel\n");
        s_boxWindow = nullptr;
        resetTradeState();
        s_interact.Reset();
        closeInteractionMenu();
        return;
    }
    cancelActionClass->initIfNeeded();
    if (s_boxCancelMethodInfo->klass == nullptr) {
        s_boxCancelMethodInfo->klass = (Il2CppClass*)cancelActionClass;
    }
    auto* cancelAction = (System::Action::Object*)il2cpp_object_new((Il2CppClass*)cancelActionClass);
    _ILExternal::external<void>(0x023feb30, cancelAction, nullptr, s_boxCancelMethodInfo);

    // Set cancel callback on BoxWindow directly
    boxWindow->fields._onCancelSelect = cancelAction;

    // Open BoxWindow with our selection callback
    boxWindow->Open(openParam, selectAction, 0);

    s_interact.state = InteractState::TradeSelectPoke;
    Logger::log("[OverworldMP] BoxWindow opened for trade pokemon selection\n");
}

// ---------------------------------------------------------------------------
// Trade: receive notifications (called from overworld_multiplayer.cpp handlers)
// ---------------------------------------------------------------------------
void overworldMPOnTradePokeReceived(int32_t fromStation, int32_t partySlot, uint8_t* data, int32_t size) {
    Logger::log("[OverworldMP] Trade pokemon received from station %d, slot=%d, size=%d\n",
                fromStation, partySlot, size);

    if (s_tradePartnerStation != fromStation && s_tradePartnerStation != -1) {
        Logger::log("[OverworldMP] Ignoring trade poke from unexpected station %d (expected %d)\n",
                    fromStation, s_tradePartnerStation);
        return;
    }

    // Store partner's pokemon data
    int32_t copySize = (size <= POKE_FULL_DATA_SIZE) ? size : POKE_FULL_DATA_SIZE;
    memcpy(s_partnerTradePokeData, data, copySize);
    s_partnerPokeReceived = true;

    // If we're waiting for this, transition to confirm
    if (s_interact.state == InteractState::TradeWaitPartner) {
        Logger::log("[OverworldMP] Partner pokemon received while waiting — showing confirm\n");
        showTradeConfirmDialog();
    }
}

void overworldMPOnTradeConfirmReceived(int32_t fromStation, bool confirmed) {
    Logger::log("[OverworldMP] Trade confirm received from station %d: confirmed=%d\n",
                fromStation, (int)confirmed);

    if (s_tradePartnerStation != fromStation && s_tradePartnerStation != -1) {
        Logger::log("[OverworldMP] Ignoring trade confirm from unexpected station %d\n", fromStation);
        return;
    }

    s_partnerConfirmReceived = true;
    s_partnerConfirmValue = confirmed;

    if (!confirmed) {
        // Partner cancelled — abort trade
        Logger::log("[OverworldMP] Partner cancelled trade\n");
        overworldMPShowRemoteEmote(fromStation, EMOTE_ID_CROSS);
        resetTradeState();
        s_interact.Reset();
        closeInteractionMenu();
        return;
    }

    // If we're waiting for their confirmation, execute the swap
    if (s_interact.state == InteractState::TradeWaitFinal) {
        Logger::log("[OverworldMP] Partner confirmed — executing swap\n");
        overworldMPExecuteSwap();
    }
}

// ---------------------------------------------------------------------------
// Battle: start battle flow (called after handshake accept on both sides)
// ---------------------------------------------------------------------------
void overworldMPStartBattle(int32_t partnerStation, BattleSubtype subtype) {
    Logger::log("[OverworldMP] Starting battle with station %d, subtype=%d, isRequester=%d\n",
                partnerStation, (int)subtype, (int)s_isRequester);

    // Don't call resetBattleState() — the partner's party (0xC6) may have
    // already arrived during the accept delay. Resetting would wipe the flag.
    s_battlePartnerStation = partnerStation;
    s_battleSubtype = subtype;
    s_battleExchangeTimeout = 10.0f; // 10 second timeout

    // Send our party data to the partner
    overworldMPSendBattleParty(partnerStation, subtype);
    s_myBattlePartySent = true;

    s_interact.state = InteractState::BattleExchangeParty;
    Logger::log("[OverworldMP] Battle party sent, waiting for partner's party (partnerReceived=%d)\n",
                (int)s_partnerBattlePartyReceived);
}

// ---------------------------------------------------------------------------
// Battle: partner party data received (called from 0xC6 handler)
// ---------------------------------------------------------------------------
void overworldMPOnBattlePartyReceived(int32_t fromStation, uint8_t* data, int32_t size) {
    Logger::log("[OverworldMP] Battle party received from station %d, size=%d\n",
                fromStation, size);

    if (s_battlePartnerStation != fromStation && s_battlePartnerStation != -1) {
        Logger::log("[OverworldMP] Ignoring battle party from unexpected station %d (expected %d)\n",
                    fromStation, s_battlePartnerStation);
        return;
    }

    // If we haven't started the battle flow yet (partner sent first), store the station
    if (s_battlePartnerStation == -1) {
        s_battlePartnerStation = fromStation;
    }

    // Copy raw data for later deserialization
    int32_t copySize = (size <= MAX_BATTLE_PARTY_DATA) ? size : MAX_BATTLE_PARTY_DATA;
    memcpy(s_partnerBattlePartyBuf, data, copySize);
    s_partnerBattlePartyBufSize = copySize;
    s_partnerBattlePartyReceived = true;

    // Parse member count from first byte
    if (size >= 1) {
        s_partnerBattleMemberCount = data[0];
        Logger::log("[OverworldMP] Partner battle party: %d members\n", (int)s_partnerBattleMemberCount);
    }

    // If we're already in the exchange state and have sent ours, the tick handler
    // will pick up partnerBattlePartyReceived and proceed to BattleSyncWait.
}

// Called from team_up.cpp when TEAMUP_BATTLE is received and partner should enter battle
void overworldMPSetTeamUpBattleStarting() {
    s_interact.state = InteractState::TeamUpBattleStarting;
    Logger::log("[OverworldMP] TeamUpBattleStarting state set\n");
}

void overworldMPOnBattleReadyReceived(int32_t fromStation) {
    if (s_battlePartnerStation != fromStation && s_battlePartnerStation != -1) {
        Logger::log("[OverworldMP] Ignoring BATTLE_READY from unexpected station %d (expected %d)\n",
                    fromStation, s_battlePartnerStation);
        return;
    }
    Logger::log("[OverworldMP] Partner BATTLE_READY received (station %d)\n", fromStation);
    s_partnerBattleReady = true;
}

// ---------------------------------------------------------------------------
// Battle: deserialize partner party from 0xC6 data into a PokeParty
// ---------------------------------------------------------------------------
static Pml::PokeParty::Object* deserializeBattleParty(uint8_t* buf, int32_t size) {
    if (size < 1) return nullptr;

    uint8_t memberCount = buf[0];
    if (memberCount == 0 || memberCount > 6) return nullptr;

    int32_t offset = 1; // skip member count byte

    // Get local party to get the PokeParty class
    auto* localParty = PlayerWork::get_playerParty();
    if (localParty == nullptr) {
        Logger::log("[OverworldMP] ERROR: no local party available\n");
        return nullptr;
    }

    // Create a new PokeParty via ctor — creates 6 empty PokemonParam slots
    auto* partyKlass = (Il2CppClass*)localParty->klass;
    if (partyKlass == nullptr) {
        Logger::log("[OverworldMP] ERROR: PokeParty class is null\n");
        return nullptr;
    }
    auto* party = (Pml::PokeParty::Object*)il2cpp_object_new(partyKlass);
    _ILExternal::external<void>(0x2055D10, party); // PokeParty::ctor()

    // Deserialize directly into the party's existing slots — matching how
    // the base game's CopyFrom works (slot-to-slot, no AddMember/GetMonsNo check).
    for (int i = 0; i < memberCount; i++) {
        if (offset + POKE_FULL_DATA_SIZE > size) {
            Logger::log("[OverworldMP] ERROR: battle party data truncated at member %d\n", i);
            break;
        }

        auto* slotPoke = party->GetMemberPointer(i);
        if (slotPoke == nullptr) {
            Logger::log("[OverworldMP] ERROR: party slot %d is null\n", i);
            offset += POKE_FULL_DATA_SIZE;
            continue;
        }

        auto* accessor = slotPoke->fields.m_accessor;
        if (accessor != nullptr) {
            // Deserialize_FullData (raw pointer version) @ 0x24A4550
            _ILExternal::external<void>(0x24A4550, accessor, &buf[offset]);
        }

        offset += POKE_FULL_DATA_SIZE;
    }

    // Validate deserialized Pokemon data — count only valid members.
    // Invalid members (corrupt data, MonsNo=0, bad seikaku) are excluded
    // to prevent exceptions in the battle init pipeline (Setup, CalcTool, etc.).
    int validCount = 0;
    for (int i = 0; i < memberCount; i++) {
        auto* valPoke = party->GetMemberPointer(i);
        if (valPoke == nullptr) {
            Logger::log("[OverworldMP] Validate[%d]: null pointer — stopping\n", i);
            break;
        }
        auto* coreParam = (Pml::PokePara::CoreParam*)valPoke;
        int32_t monsNo = coreParam->GetMonsNo();
        int32_t seikaku = coreParam->GetSeikaku();
        uint32_t level = coreParam->GetLevel();
        Logger::log("[OverworldMP] Validate[%d]: monsNo=%d seikaku=%d level=%u\n",
                    i, monsNo, seikaku, level);

        if (monsNo <= 0 || monsNo > 905) {
            Logger::log("[OverworldMP] REJECT[%d]: invalid monsNo=%d — excluded from party\n", monsNo, i);
            break;  // Stop at first invalid member (party must be contiguous)
        }
        if (seikaku < 0 || seikaku >= 25) {
            Logger::log("[OverworldMP] REJECT[%d]: invalid seikaku=%d — excluded from party\n", seikaku, i);
            break;
        }
        if (level == 0 || level > 100) {
            Logger::log("[OverworldMP] REJECT[%d]: invalid level=%u — excluded from party\n", level, i);
            break;
        }
        validCount++;
    }

    if (validCount == 0) {
        Logger::log("[OverworldMP] ERROR: no valid Pokemon in deserialized party!\n");
        return nullptr;
    }

    if (validCount < memberCount) {
        Logger::log("[OverworldMP] WARNING: reduced party from %d to %d valid members\n",
                    (int)memberCount, validCount);
    }

    // Set memberCount to only include validated members
    party->fields.m_memberCount = validCount;

    Logger::log("[OverworldMP] Deserialized battle party: %d valid members (of %d received)\n",
                validCount, (int)memberCount);
    return party;
}

// ---------------------------------------------------------------------------
// Battle: deserialize MYSTATUS_COMM from 0xC6 data
// ---------------------------------------------------------------------------
static MYSTATUS_COMM::Object deserializeBattleStatus(uint8_t* buf, int32_t size, int32_t& offset) {
    MYSTATUS_COMM::Object status = {};

    if (offset + 4 > size) return status;
    memcpy(&status.fields.id, &buf[offset], 4);
    offset += 4;

    // Name: [nameLen:1][chars:nameLen*2]
    if (offset + 1 > size) return status;
    uint8_t nameLen = buf[offset++];
    if (nameLen > 0 && nameLen <= 12 && offset + nameLen * 2 <= size) {
        status.fields.name = System::String::fromUnicodeBytes(&buf[offset], nameLen * 2);
        offset += nameLen * 2;
    } else {
        status.fields.name = System::String::Create("Trainer");
    }

    // Remaining fields
    if (offset + 6 > size) return status;
    status.fields.sex = buf[offset++] != 0;
    status.fields.lang = buf[offset++];
    status.fields.fashion = buf[offset++];
    status.fields.body_type = buf[offset++];
    status.fields.hat = buf[offset++];
    status.fields.shoes = buf[offset++];

    return status;
}

// ---------------------------------------------------------------------------
// Battle: setup and start the battle scene
// ---------------------------------------------------------------------------
void overworldMPSetupAndStartBattle() {
    Logger::log("[OverworldMP] Setting up battle scene...\n");

    // Deserialize partner party
    auto* partnerParty = deserializeBattleParty(s_partnerBattlePartyBuf, s_partnerBattlePartyBufSize);
    if (partnerParty == nullptr) {
        Logger::log("[OverworldMP] ERROR: failed to deserialize partner battle party\n");
        resetBattleState();
        s_interact.Reset();
        closeInteractionMenu();
        return;
    }

    // Heal partner pokemon — party data was sent before local healing,
    // so the received data may contain damage from previous battles.
    for (uint32_t i = 0; i < partnerParty->fields.m_memberCount && i < 6; i++) {
        auto* poke = partnerParty->GetMemberPointer(i);
        if (poke != nullptr) {
            ((Pml::PokePara::CoreParam*)poke)->RecoverAll();
        }
    }
    Logger::log("[OverworldMP] Partner party healed (%d members)\n",
                (int)partnerParty->fields.m_memberCount);

    // Get local party
    auto* myParty = PlayerWork::get_playerParty();
    if (myParty == nullptr) {
        Logger::log("[OverworldMP] ERROR: local party is null\n");
        resetBattleState();
        s_interact.Reset();
        closeInteractionMenu();
        return;
    }

    // Snapshot local party state BEFORE healing — restored after battle ends
    // so the player can't get free healing by battling friends.
    s_partySnapshotCount = myParty->fields.m_memberCount;
    if (s_partySnapshotCount > 6) s_partySnapshotCount = 6;
    for (int i = 0; i < s_partySnapshotCount; i++) {
        auto* poke = myParty->GetMemberPointer(i);
        memset(s_partySnapshot[i], 0, POKE_SNAPSHOT_SIZE);
        if (poke != nullptr && poke->fields.m_accessor != nullptr) {
            // Serialize_FullData (raw pointer) @ 0x24A4470
            _ILExternal::external<void>(0x24A4470, poke->fields.m_accessor, s_partySnapshot[i]);
        }
    }
    s_partySnapshotValid = true;
    Logger::log("[OverworldMP] Saved party snapshot (%d members)\n", s_partySnapshotCount);

    // Heal all local Pokemon for a fair PvP fight
    for (int i = 0; i < s_partySnapshotCount; i++) {
        auto* poke = myParty->GetMemberPointer(i);
        if (poke != nullptr) {
            ((Pml::PokePara::CoreParam*)poke)->RecoverAll();
        }
    }
    Logger::log("[OverworldMP] Local party healed for battle\n");

    // Setup MYSTATUS_COMM for local player
    MYSTATUS_COMM::Object myStatus = {};
    myStatus.SetupFromPlayerWork();

    // Deserialize partner MYSTATUS_COMM from the buffer (after pokemon data)
    int32_t statusOffset = 1 + s_partnerBattleMemberCount * POKE_FULL_DATA_SIZE;
    MYSTATUS_COMM::Object partnerStatus = deserializeBattleStatus(
        s_partnerBattlePartyBuf, s_partnerBattlePartyBufSize, statusOffset);

    // Get battle setup param
    auto* bsp = PlayerWork::get_battleSetupParam();
    if (bsp == nullptr) {
        Logger::log("[OverworldMP] ERROR: battleSetupParam is null\n");
        resetBattleState();
        s_interact.Reset();
        closeInteractionMenu();
        return;
    }

    // Determine commPos: requester=0, accepter=1
    uint8_t commPos = s_isRequester ? 0 : 1;

    // CommRule: 0=single, 1=double
    int32_t commRule = (int32_t)s_battleSubtype;

    // Station indices — IlcaNetSession::ThisStationIndex() @ 0x23BC000
    int32_t myStation = _ILExternal::external<int32_t>(0x23BC000);
    int32_t partnerStation = s_battlePartnerStation;

    Logger::log("[OverworldMP] Battle setup: commPos=%d commRule=%d myStation=%d partnerStation=%d\n",
                (int)commPos, commRule, myStation, partnerStation);
    Logger::log("[OverworldMP] My party: %d members, Partner party: %d members\n",
                (int)myParty->fields.m_memberCount, (int)partnerParty->fields.m_memberCount);

    // Create a Regulation.Data on the stack for no-restriction casual battle
    // Struct layout: [select_num_lo:1][select_num_hi:1][level_range:1][_bitsA:1]
    // level_range=2 (NONE), _bitsA=0x07 (enable_legend + enable_same_poke + enable_same_item)
    uint8_t regulation[4] = { 1, 6, 2, 0x07 };

    // setupPlayerInfo reads capsule->max_length at offset 0x18. Passing nullptr
    // crashes when the station matches the local player. An empty IL2CPP array
    // header (max_length=0 at +0x18) makes the length check fail safely.
    static uint8_t s_emptyCapsuleArray[32] = {};
    void* emptyCapsule = (void*)s_emptyCapsuleArray;

    // Call SetupBattleComm — player at commPos 0 goes first in arg list
    // networkType=0 → commMode=1 (BTL_COMM_WIRELESS/LAN) for local P2P session
    // networkType=1 would give commMode=2 (BTL_COMM_WIFI/Internet) — WRONG for LAN
    // Pass nullptr for bgm/winBgm — we override the effect data below.
    if (commPos == 0) {
        Dpr::EncountTools::SetupBattleComm(bsp, 0, commRule, commPos, regulation,
            myStation, myParty, &myStatus, emptyCapsule,
            partnerStation, partnerParty, &partnerStatus, emptyCapsule,
            -1, nullptr, nullptr, nullptr,
            -1, nullptr, nullptr, nullptr,
            nullptr, nullptr, 0, 0);
    } else {
        Dpr::EncountTools::SetupBattleComm(bsp, 0, commRule, commPos, regulation,
            partnerStation, partnerParty, &partnerStatus, emptyCapsule,
            myStation, myParty, &myStatus, emptyCapsule,
            -1, nullptr, nullptr, nullptr,
            -1, nullptr, nullptr, nullptr,
            nullptr, nullptr, 0, 0);
    }

    Logger::log("[OverworldMP] SetupBattleComm called — starting battle scene\n");

    // --- Override arena with current zone's battle background ---
    // SetupBattleComm hardcodes arena 0x2b (Union Room). Replace it with
    // the arena ID from the current zone's MapInfo so the background matches
    // the overworld location where the battle was initiated.
    {
        GameManager::getClass()->initIfNeeded();
        auto zoneID = PlayerWork::get_zoneID();
        auto* mapInfo = GameManager::get_mapInfo();
        if (mapInfo != nullptr) {
            auto* zoneData = mapInfo->get_Item(zoneID);
            if (zoneData != nullptr && zoneData->fields.BattleBg != nullptr &&
                zoneData->fields.BattleBg->max_length > 0) {
                int32_t arenaID = zoneData->fields.BattleBg->m_Items[0]; // land arena
                void* fieldSit = bsp->instance()->fields.fieldSituation;
                if (fieldSit != nullptr) {
                    void* bgComp = *(void**)((uintptr_t)fieldSit + 0x10);
                    if (bgComp != nullptr) {
                        // BgComponentData::SetUpBgComponentData @ 0x188A6C0
                        _ILExternal::external<void>(0x188A6C0, bgComp, arenaID);
                        Logger::log("[OverworldMP] Arena override: zone %d -> arenaID %d\n",
                                    zoneID, arenaID);
                    }
                }
            }
        }
    }

    // --- Random battle theme via BattleSetupEffectData table ---
    // Re-call SetUpBattleEffectComponentData with a random effect index so that:
    //   - BGM starts/stops through the normal battle audio pipeline
    //   - Encounter transition animation matches the selected theme
    //   - Both players pick the same theme (deterministic seed from station XOR)
    // Only for single battles — double/multi use COMM_DOUBLE which properly
    // formats the "Double battle with [name]" text.
    if (commRule == 0) {
        // BattleSetupEffectId indices from BattleDataTable:
        //   15 = TR_NORMAL  (regular trainer)   → BA002 / BA102
        //   37 = TR_GYMLEADER1 (gym leader)     → BA003 / BA103
        //   25 = TR_RIVAL1_1 (rival Barry)      → BA004 / BA102
        //   23 = TR_CHAMP1 (champion Shirona)   → BA008 / BA105
        //   53 = TR_T (Cyrus)                   → Play_bgm_or_vs_vs07 / Play_bgm_or_wn_win06
        static const int32_t effectPool[] = { 15, 37, 25, 23, 53 };
        static constexpr int POOL_SIZE = sizeof(effectPool) / sizeof(effectPool[0]);

        // Deterministic seed: XOR of both station indices — both players compute
        // the same value regardless of who is requester/accepter.
        uint32_t seed = (uint32_t)(myStation ^ partnerStation);
        seed *= 2654435761u; // Knuth multiplicative hash
        int32_t pick = (int32_t)(seed % POOL_SIZE);

        void* btlEff = bsp->instance()->fields.btlEffComponent;
        if (btlEff != nullptr) {
            // SetUpBattleEffectComponentData @ 0x0187B7E0
            _ILExternal::external<void>(0x0187B7E0, btlEff, effectPool[pick], -1, 0, 0);
            Logger::log("[OverworldMP] Battle effect override: effectIdx=%d (pick %d/%d)\n",
                        effectPool[pick], pick, POOL_SIZE);
        }
    } else {
        Logger::log("[OverworldMP] Double battle — keeping COMM_DOUBLE effect\n");
    }

    // Log PIA session state — type-2 transports need Gaming state (4 or 5)
    {
        int32_t sessState = Dpr::NetworkUtils::NetworkManager::get_SessionState();
        bool isConnect = Dpr::NetworkUtils::NetworkManager::get_IsConnect();
        int32_t myStationCheck = _ILExternal::external<int32_t>(0x23BC000); // ThisStationIndex
        Logger::log("[OverworldMP] Session state=%d isConnect=%d thisStation=%d\n",
                    sessState, (int)isConnect, myStationCheck);
    }

    // Diagnostic: check if per-station transports exist for battle networking.
    // Battle system uses transport type 2 (per-station). IlcaNetSession.static_fields:
    //   +0x28 = type-0 transport (broadcast)
    //   +0x30 = type-1 array (per-station)
    //   +0x38 = type-2 array (per-station) — used by battle
    {
        auto* insTypeInfo = *(Il2CppClass**)exl::util::modules::GetTargetOffset(0x04C59E40);
        if (insTypeInfo != nullptr) {
            auto* sf = *(void**)((uintptr_t)insTypeInfo + 0xb8);
            if (sf != nullptr) {
                void* t0 = *(void**)((uintptr_t)sf + 0x28);
                void* t1arr = *(void**)((uintptr_t)sf + 0x30);
                void* t2arr = *(void**)((uintptr_t)sf + 0x38);
                Logger::log("[OverworldMP] Transport check: t0=%p t1arr=%p t2arr=%p\n", t0, t1arr, t2arr);
                if (t2arr != nullptr) {
                    uint32_t t2len = *(uint32_t*)((uintptr_t)t2arr + 0x18);
                    Logger::log("[OverworldMP] Type-2 array len=%d\n", (int)t2len);
                    for (int i = 0; i < 4 && i < (int)t2len; i++) {
                        void* entry = *(void**)((uintptr_t)t2arr + 0x20 + i * 8);
                        Logger::log("[OverworldMP] Type-2 transport[%d]=%p\n", i, entry);
                    }
                } else {
                    Logger::log("[OverworldMP] WARNING: type-2 transport array is NULL!\n");
                }
            }
        }
    }

    // Battle transition: bypass the encounter effect entirely.
    // EncountStart(0) would play the wild grass encounter animation, and
    // EncountStart(1) crashes because it expects real trainer IDs.
    // The Union Room doesn't use EncountStart at all — it goes through
    // BattleMatchingManager.  We take a similar approach: use EncountStart
    // only for its setup side-effects (stop player, hide poketch, set
    // _updateType=Encount), then immediately skip the encounter effect
    // state machine and trigger the battle scene with a simple fade-out.
    auto* fm = FieldManager::getClass()->static_fields->_Instance_k__BackingField;

    // Clear emote balloons before the battle transition starts — prevents
    // the accept emote (heart) from rendering on top of the transition.
    for (int i = 0; i < MAX_ACTIVE_BALLOONS; i++) {
        if (s_activeBalloons[i].balloon != nullptr) {
            FieldCanvas::DeleteBalloon(s_activeBalloons[i].balloon, true);
            s_activeBalloons[i].balloon = nullptr;
        }
    }

    // Suppress overworld packet reading so the battle system's ExCallback
    // gets clean PacketReaders (both share the same object per packet).
    overworldMPSetInBattleScene(true);

    // Start the battle scene via FieldManager
    if (fm != nullptr) {
        // 1. EncountStart does essential setup: SetSaveObj, PlayIdle,
        //    StopBicycleDecelerate, _updateType=2, HidePoketch.
        fm->EncountStart(0, 0, 0);

        // 2. Immediately skip the encounter effect state machine.
        //    _encountUpdateType=4 (End) prevents EncountUpdate from
        //    loading the wild grass effect or any encounter sequence.
        fm->fields._encountUpdateType = 4;  // EncountUpdateType.End

        // 3. PrepareOperationStatic — normally called in EncountUpdate
        //    state 0, we call it ourselves since we skipped that state.
        //    BattleConnector::PrepareOperationStatic @ 0x1D69360
        _ILExternal::external<void>(0x1D69360);

        // 4. Audio transition — replicate what EncountUpdate state 1 does:
        //    stop field BGM and start battle intro BGM.
        {
            // AudioManager::get_Instance() -> PostEvent(stopFieldBGM)
            auto* amInstance = Audio::AudioManager::get_Instance();
            if (amInstance != nullptr) {
                // Stop field BGM: event hash 0x35f93cfe, callbackFlags=0x100009
                // AudioManager::PostEvent(this, eventId, callbackFlags, isThroughSameEvent) @ 0x021EB100
                _ILExternal::external<uint32_t>(0x021EB100, amInstance,
                    (uint32_t)0x35f93cfe, (uint32_t)0x100009, (bool)false);
                Logger::log("[OverworldMP] Stopped field BGM\n");

                // Get battle BGM event name from BattleEffectComponentData
                void* btlEff = bsp->instance()->fields.btlEffComponent;
                if (btlEff != nullptr) {
                    // BattleEffectComponentData::get_battleBgm() @ 0x0187B510
                    auto* bgmStr = (System::String::Object*)
                        _ILExternal::external<void*>(0x0187B510, btlEff);
                    if (bgmStr != nullptr && bgmStr->fields.m_stringLength > 0) {
                        // AkSoundEngine::GetIDFromString(string) @ 0x02465FA0
                        uint32_t bgmId = _ILExternal::external<uint32_t>(0x02465FA0, bgmStr);
                        // Play battle BGM
                        _ILExternal::external<uint32_t>(0x021EB100, amInstance,
                            bgmId, (uint32_t)0x100009, (bool)false);
                        Logger::log("[OverworldMP] Started battle BGM (id=0x%08x)\n", bgmId);
                    } else {
                        Logger::log("[OverworldMP] No battleBgm string in effect data\n");
                    }
                }
            }
        }

        // 5. Fade to black — Fader::set_fadeMode(0=Color) + FadeOut(0.5s)
        //    This replaces the wild encounter animation with a clean fade.
        //    Fader::set_fadeMode @ 0x209D490, Fader::FadeOut(float) @ 0x209E5E0
        _ILExternal::external<void>(0x209D490, 0);     // fadeMode = Color
        _ILExternal::external<void>(0x209E5E0, 0.5f);  // FadeOut(0.5s)

        // 6. Tell the Sequencer it's time to load the battle scene.
        //    PlayerWork::set_isEncount(true) @ 0x2CEF7C0
        _ILExternal::external<void>(0x2CEF7C0, true);

        Logger::log("[OverworldMP] Battle transition: fade-to-black (bypassed encounter effect)\n");
    } else {
        Logger::log("[OverworldMP] ERROR: FieldManager instance is null\n");
    }

    // Clean up battle state (battle scene takes over from here)
    resetBattleState();
    s_interact.Reset();
    closeInteractionMenu();
}

// ---------------------------------------------------------------------------
// Request accepted/declined handlers (called from both sides)
// ---------------------------------------------------------------------------

void overworldMPOnRequestAccepted(int32_t partnerStation) {
    Logger::log("[OverworldMP] Trade/battle accepted with station %d — deferring start\n", partnerStation);

    // Show accept emote balloon above the partner's entity
    overworldMPShowRemoteEmote(partnerStation, EMOTE_ID_LIKES);

    // Defer trade start to next frames (avoids opening UI from callback context)
    s_pendingFromStation = -1;
    s_interact.state = InteractState::Accepted;
    s_interact.targetStationIndex = partnerStation;
    s_interact.tradeStartDelay = 0.3f;
}

void overworldMPOnRequestDeclined(int32_t partnerStation) {
    Logger::log("[OverworldMP] Request declined/timed out (partner station %d)\n", partnerStation);

    // Show decline emote balloon above the partner's entity
    overworldMPShowRemoteEmote(partnerStation, EMOTE_ID_CROSS);

    s_pendingFromStation = -1;
    s_interact.Reset();
    closeInteractionMenu();
}

// ---------------------------------------------------------------------------
// Proximity check integration
// ---------------------------------------------------------------------------
// Restore local party to pre-battle state after PvP battle
// ---------------------------------------------------------------------------
void overworldMPRestorePartyAfterBattle() {
    if (!s_partySnapshotValid) return;

    auto* party = PlayerWork::get_playerParty();
    if (party == nullptr) {
        Logger::log("[OverworldMP] WARNING: cannot restore party — playerParty is null\n");
        s_partySnapshotValid = false;
        return;
    }

    for (int i = 0; i < s_partySnapshotCount && i < party->fields.m_memberCount; i++) {
        auto* poke = party->GetMemberPointer(i);
        if (poke != nullptr && poke->fields.m_accessor != nullptr) {
            // Deserialize_FullData (raw pointer) @ 0x24A4550
            _ILExternal::external<void>(0x24A4550, poke->fields.m_accessor, s_partySnapshot[i]);
        }
    }

    Logger::log("[OverworldMP] Restored party to pre-battle state (%d members)\n", s_partySnapshotCount);
    s_partySnapshotValid = false;
    s_partySnapshotCount = 0;
}

// ---------------------------------------------------------------------------

void overworldMPCheckInteraction() {
    if (!isOverworldMPActive()) {
        return;
    }

    // While the battle scene flag is set, skip all overworld interaction
    // processing.  Only clear the flag once FieldManager._updateType returns
    // to 0 (normal overworld) — meaning the battle is finished and we're
    // back in the field.  EncountStart sets _updateType to 2; it stays
    // non-zero through the transition animation and the entire battle scene.
    if (overworldMPIsInBattleScene()) {
        auto* fm = FieldManager::getClass()->static_fields->_Instance_k__BackingField;
        if (fm != nullptr && fm->fields._updateType == 0) {
            overworldMPSetInBattleScene(false);

            // Stop battle BGM and restart field BGM (matching base game sequence)
            auto* amInstance = Audio::AudioManager::get_Instance();
            if (amInstance != nullptr) {
                // AudioManager::StopBgm() @ 0x21EB770 — posts Wwise event 0x3ffbcd36
                // and clears the BGM slot (offset 0x5c)
                _ILExternal::external<void>(0x021EB770, amInstance);
                // AudioManager::SetBgmEvent(eventId, isThroughSameEvent) @ 0x21EB840
                // posts 0x642ccdc3 to restore the field BGM
                _ILExternal::external<void>(0x021EB840, amInstance,
                    (uint32_t)0x642ccdc3, (uint32_t)0, (uint32_t)0);
            }
            Logger::log("[OverworldMP] Battle ended — stopped battle BGM, restored field BGM\n");

            // Clear team-up battle state (CreateLocalClient hook flag, etc.)
            overworldMPClearTeamUpBattleState();

            // Team-up post-battle: copy battle party back (EXP/HP/PP),
            // Player B rewards/whiteout
            overworldMPHandleTeamUpPostBattle();

            // Restore local party to pre-battle HP/PP/status (PvP only —
            // no-op for team-up since no snapshot is taken)
            overworldMPRestorePartyAfterBattle();
        }
        return;
    }

    // Main menu, emote menu, incoming request dialog, trade menus, animation, or battle — callbacks handle transitions
    if (s_interact.state == InteractState::MenuOpen ||
        s_interact.state == InteractState::EmoteMenuOpen ||
        s_interact.state == InteractState::ReceivedRequest ||
        s_interact.state == InteractState::TradeSelectPoke ||
        s_interact.state == InteractState::TradeConfirm ||
        s_interact.state == InteractState::TradeAnimating) {
        return;
    }

    // Accepted — waiting for context menu close animation before starting activity
    if (s_interact.state == InteractState::Accepted) {
        if (s_interact.tradeStartDelay > 0.0f) {
            s_interact.tradeStartDelay -= 0.01666667f;
            if (s_interact.tradeStartDelay <= 0.0f) {
                if (s_pendingRequestType == InteractionType::TeamUp) {
                    Logger::log("[OverworldMP] Accept delay elapsed — completing team-up handshake\n");
                    overworldMPTeamUp(s_interact.targetStationIndex);
                    s_interact.Reset();
                    closeInteractionMenu();
                } else if (s_pendingRequestType == InteractionType::Battle) {
                    Logger::log("[OverworldMP] Accept delay elapsed — launching battle (subtype=%d)\n",
                                (int)s_pendingBattleSubtype);
                    overworldMPStartBattle(s_interact.targetStationIndex, s_pendingBattleSubtype);
                } else {
                    Logger::log("[OverworldMP] Accept delay elapsed — launching trade\n");
                    overworldMPStartTrade(s_interact.targetStationIndex);
                }
            }
        }
        return;
    }

    // Trade: waiting for BoxWindow prefab to finish preloading
    if (s_interact.state == InteractState::TradeLoadingBox) {
        s_interact.timeoutTimer -= 0.01666667f;
        if (isBoxWindowInObjectPool()) {
            Logger::log("[OverworldMP] BoxWindow preload complete — opening\n");
            openTradeBoxWindow(s_tradePartnerStation);
        } else if (s_interact.timeoutTimer <= 0.0f) {
            Logger::log("[OverworldMP] BoxWindow preload timed out\n");
            s_boxWindowPreloading = false;
            resetTradeState();
            s_interact.Reset();
            closeInteractionMenu();
        }
        return;
    }

    // Trade: waiting for partner's pokemon selection
    if (s_interact.state == InteractState::TradeWaitPartner) {
        if (s_partnerPokeReceived) {
            Logger::log("[OverworldMP] Partner pokemon received — showing confirm dialog\n");
            showTradeConfirmDialog();
        }
        return;
    }

    // Trade: waiting for partner's final confirmation
    if (s_interact.state == InteractState::TradeWaitFinal) {
        if (s_partnerConfirmReceived) {
            if (s_partnerConfirmValue) {
                Logger::log("[OverworldMP] Partner confirmed trade — executing swap\n");
                overworldMPExecuteSwap();
            } else {
                Logger::log("[OverworldMP] Partner declined trade\n");
                overworldMPShowRemoteEmote(s_tradePartnerStation, EMOTE_ID_CROSS);
                resetTradeState();
                s_interact.Reset();
                closeInteractionMenu();
            }
        }
        return;
    }

    // Main menu closed, waiting to open emote sub-menu
    if (s_interact.state == InteractState::EmoteMenuPending) {
        s_interact.emoteMenuDelay -= 0.01666667f; // ~1 frame at 30fps
        if (s_interact.emoteMenuDelay <= 0.0f) {
            overworldMPShowEmoteMenu();
        }
        return;
    }

    // Main menu closed, waiting to open battle submenu
    if (s_interact.state == InteractState::BattleMenuOpen) {
        if (s_interact.emoteMenuDelay > 0.0f) {
            // Delay phase: waiting for main menu close animation
            s_interact.emoteMenuDelay -= 0.01666667f;
            if (s_interact.emoteMenuDelay <= 0.0f) {
                overworldMPShowBattleSubmenu();
            }
        }
        // After submenu opens: callbacks handle everything, just return
        return;
    }

    // Battle: exchanging party data with partner
    if (s_interact.state == InteractState::BattleExchangeParty) {
        s_battleExchangeTimeout -= 0.01666667f;
        if (s_battleExchangeTimeout <= 0.0f) {
            Logger::log("[OverworldMP] Battle party exchange timed out\n");
            resetBattleState();
            s_interact.Reset();
            closeInteractionMenu();
            return;
        }
        // Both sides sent and received → send BATTLE_READY and wait for partner
        if (s_myBattlePartySent && s_partnerBattlePartyReceived) {
            Logger::log("[OverworldMP] Both battle parties exchanged — sending BATTLE_READY\n");
            // Send BATTLE_READY packet to partner
            overworldMPSendBattleReady(s_battlePartnerStation);
            s_myBattleReadySent = true;
            s_battleSyncTimeout = 10.0f; // 10 second timeout
            s_interact.state = InteractState::BattleSyncWait;
        }
        return;
    }

    // Battle: waiting for both sides to be ready (sync handshake)
    if (s_interact.state == InteractState::BattleSyncWait) {
        s_battleSyncTimeout -= 0.01666667f;
        if (s_battleSyncTimeout <= 0.0f) {
            Logger::log("[OverworldMP] Battle sync timed out — partner never sent READY\n");
            resetBattleState();
            s_interact.Reset();
            closeInteractionMenu();
            return;
        }
        // If partner already sent BATTLE_READY (maybe received during party exchange)
        if (s_partnerBattleReady) {
            Logger::log("[OverworldMP] Both sides READY — entering battle scene\n");
            s_interact.state = InteractState::BattleStarting;
            // Falls through to BattleStarting on next tick
        }
        return;
    }

    // Battle: setting up and entering battle scene
    if (s_interact.state == InteractState::BattleStarting) {
        extern void overworldMPSetupAndStartBattle();
        overworldMPSetupAndStartBattle();
        return;
    }

    // Team-up battle: partner received TEAMUP_BATTLE, entering battle scene
    if (s_interact.state == InteractState::TeamUpBattleStarting) {
        overworldMPSetupTeamUpBattle();
        s_interact.Reset();
        return;
    }

    // Update timeout for waiting state
    if (s_interact.state == InteractState::WaitingResponse) {
        s_interact.timeoutTimer -= 0.01666667f;
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
