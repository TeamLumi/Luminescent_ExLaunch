#include "exlaunch.hpp"

#include "features/overworld_multiplayer.h"
#include "features/team_up.h"
#include "romdata/data/ColorSet.h"

#include "externals/Audio/AudioManager.h"
#include "externals/DPData/CONFIG.h"
#include "externals/ContextMenuID.h"
#include "externals/Dpr/Box/BoxPokemonWork.h"
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
#include "externals/Dpr/UI/UIZukanRegister.h"
#include "externals/UnityEngine/CanvasGroup.h"
#include "externals/UnityEngine/Component.h"
#include "externals/UnityEngine/Time.h"
#include "externals/UnityEngine/Vector3.h"

#include "externals/Dpr/Battle/Logic/TRAINER_DATA.h"
#include "externals/Dpr/Message/MessageManager.h"
#include "externals/Dpr/MsgWindow/MsgWindowManager.h"
#include "externals/il2cpp-api.h"
#include "helpers/InputHelper.h"
#include "logger/logger.h"
#include "save/save.h"

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
    TeamUpBattleStarting = 18, // (unused — Player B now uses sync-wait + DeferEncountStart)
    TradePreview = 19,         // UIZukanRegister showing partner pokemon preview
    IncomingRequestPending = 20, // Deferred incoming request dialog (wait a few frames)
    TradePreviewPending = 21,    // Deferred trade preview (BoxWindow callback must complete first)
    TradeWarningMsg = 22,          // Showing "need 2+ pokemon" MsgWindow, waiting for A/B dismiss
};

struct InteractContext {
    InteractState state;
    int32_t targetStationIndex;
    float timeoutTimer;
    float emoteMenuDelay;  // brief delay before opening emote sub-menu
    float tradeStartDelay; // brief delay before opening trade UI (after accept)
    float incomingRequestDelay; // brief delay before showing incoming request dialog

    void Reset() {
        state = InteractState::Idle;
        targetStationIndex = -1;
        timeoutTimer = 0.0f;
        emoteMenuDelay = 0.0f;
        tradeStartDelay = 0.0f;
        incomingRequestDelay = 0.0f;
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
// Fake TradeParam buffer — SetOtherPokeParam writes otherOriginalParam (+0x40)
// and otherPokeParam (+0x48) into _tradeParam. A static buffer avoids needing
// the TradeParam TypeInfo which requires an unknown system_load_typeinfo index.
static uint8_t s_fakeTradeParam[128] = {};
static int32_t s_myTradeTrayIndex = -1;  // -1 = party, 0+ = box tray
static bool s_myTradeIsBox = false;       // true if trading from box, false if from party
static bool s_boxWindowPreloading = false;  // true while OpLoadWindows coroutine is running
static float s_tradeWaitTimeout = 0.0f;     // timeout for TradeWaitPartner / TradeWaitFinal / TradePreview
static constexpr float TRADE_WAIT_TIMEOUT = 30.0f; // 30 second timeout
static char s_tradeConfirmMsgBuf[256] = {};  // filled by showTradePreview(), consumed by TradePreview tick

// UIZukanRegister preview state
static Dpr::UI::UIZukanRegister::Object* s_zukanRegister = nullptr;
static bool s_zukanDismissed = false;
static MethodInfo* s_zukanCompleteMI = nullptr;
static bool s_suppressZukanRegistration = false;

// Active ContextMenuWindow — tracked so disconnect handler can close it
static Dpr::UI::ContextMenuWindow::Object* s_activeContextMenu = nullptr;

// MsgWindow helpers — used for trade confirm prompt (same singleton as team_up.cpp)
static void* s_cachedInteractMsgTMP = nullptr;
static bool s_interactMsgShown = false;

static void* getInteractMsgWindowTMP() {
    if (s_cachedInteractMsgTMP != nullptr) return s_cachedInteractMsgTMP;

    auto* mgr = Dpr::MsgWindow::MsgWindowManager::get_Instance();
    if (mgr == nullptr) return nullptr;

    void* msgWindow = mgr->get_MsgWindow();
    if (msgWindow == nullptr) return nullptr;

    // WindowMessage at MsgWindow+0x28
    void* windowMessage = *(void**)((uintptr_t)msgWindow + 0x28);
    if (windowMessage == nullptr) return nullptr;

    // MsgTextContainer at WindowMessage+0x18
    void* msgTextContainer = *(void**)((uintptr_t)windowMessage + 0x18);
    if (msgTextContainer == nullptr) return nullptr;

    // WindowMsgText[] (Il2CppArray) at MsgTextContainer+0x20
    void* textArray = *(void**)((uintptr_t)msgTextContainer + 0x20);
    if (textArray == nullptr) return nullptr;

    uint64_t maxLength = *(uint64_t*)((uintptr_t)textArray + 0x18);
    if (maxLength == 0) return nullptr;

    void* windowMsgText = *(void**)((uintptr_t)textArray + 0x20);
    if (windowMsgText == nullptr) return nullptr;

    void* tmp = *(void**)((uintptr_t)windowMsgText + 0x18);
    if (tmp == nullptr) return nullptr;

    s_cachedInteractMsgTMP = tmp;
    return tmp;
}

static void setInteractMsgWindowActive(bool active) {
    auto* mgr = Dpr::MsgWindow::MsgWindowManager::get_Instance();
    if (mgr == nullptr) return;

    void* msgWindow = mgr->get_MsgWindow();
    if (msgWindow == nullptr) return;

    // Only call MsgWindow::SetActive when SHOWING.  Calling SetActive(false)
    // deactivates the GameObject, removing it from the Canvas render stack.
    // When reactivated later, it re-enters at a lower position — behind any
    // UIManager windows (like UIZukanRegister) that were created in between.
    // For hiding, we rely solely on DoPlayImmediateAnimation(0) which sets
    // CanvasGroup alpha=0 and scale=0, keeping the GameObject in the hierarchy.
    if (active) {
        _ILExternal::external<void>(0x1FAC740, msgWindow, true);
    }

    void* windowAnimator = *(void**)((uintptr_t)msgWindow + 0x38);
    if (windowAnimator == nullptr) return;

    _ILExternal::external<void>(0x1DDC840, windowAnimator, active);
}

static void showInteractMsgWindow(const char* text) {
    void* tmp = getInteractMsgWindowTMP();
    if (tmp == nullptr) {
        Logger::log("[OverworldMP] showInteractMsgWindow: TMP chain failed\n");
        return;
    }

    setInteractMsgWindowActive(true);

    auto* str = System::String::Create(text);
    if (str != nullptr) {
        _ILExternal::external<void>(0x01e94520, tmp, str);
    }
}

static void hideInteractMsgWindow() {
    setInteractMsgWindowActive(false);
    s_cachedInteractMsgTMP = nullptr;
    s_interactMsgShown = false;
}

// Hook ShowRegisterNewMessage: suppress the "data will be added to the Pokedex" message
// during trade preview. We use bootType=1 so State 2 calls this (hooked as no-op) instead
// of ShowInputNickNameMessage, and b__37_0 sees bootType=1 → Close (skip nickname).
HOOK_DEFINE_TRAMPOLINE(UIZukanRegister$$ShowRegisterNewMessage) {
    static void Callback(void* self) {
        if (s_suppressZukanRegistration) {
            Logger::log("[OverworldMP] Suppressed Zukan registration message\n");
            return;
        }
        Orig(self);
    }
};

// Hook ZukanDescriptionPanel::ShowDescription: suppress during trade preview.
// The original OnUpdateDefault A-button path calls ShowDescription, which crashes for
// foreign pokemon (Pokedex resources not loaded). By no-op'ing ShowDescription, A press
// just plays a sound and returns harmlessly. B press goes through the original Close path.
// ShowDescription @ 0x1BB4DB0
HOOK_DEFINE_TRAMPOLINE(ZukanDescriptionPanel$$ShowDescription) {
    static void Callback(void* self, uint64_t param2, uint8_t param3, void* param4) {
        if (s_suppressZukanRegistration) {
            Logger::log("[OverworldMP] Suppressed ShowDescription during trade preview\n");
            return;
        }
        Orig(self, param2, param3, param4);
    }
};

// Hook ZukanDescriptionPanel::Dispose: suppress during trade preview.
// OpClose State 0 calls Dispose which calls ModelViewBase::Clear — this crashes when the
// description panel was set up with a foreign pokemon not in the player's Pokedex.
// Dispose @ 0x1BB3B80
HOOK_DEFINE_TRAMPOLINE(ZukanDescriptionPanel$$Dispose) {
    static void Callback(void* self) {
        if (s_suppressZukanRegistration) {
            Logger::log("[OverworldMP] Suppressed ZukanDescriptionPanel Dispose during trade preview\n");
            return;
        }
        Orig(self);
    }
};

// Hook UIZukanRegister::Close: when suppressed, set our dismissed flag so the tick loop
// knows to proceed. Still calls Orig so OpClose runs its cleanup (with Dispose safely hooked).
// Close @ 0x1A3D220  signature: (self, onPreClose, prevWindowId)
HOOK_DEFINE_TRAMPOLINE(UIZukanRegister$$Close) {
    static void Callback(void* self, void* onPreClose, int32_t prevWindowId) {
        if (s_suppressZukanRegistration) {
            // Block UIZukanRegister's own B-button close during trade preview.
            // The Yes/No handler clears s_suppressZukanRegistration before calling
            // Close explicitly, so that close will go through.
            Logger::log("[OverworldMP] UIZukanRegister Close blocked — waiting for Yes/No\n");
            return;
        }
        Orig(self, onPreClose, prevWindowId);
    }
};

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

// Saved local wazaeff_mode for PvP battles — comm battles sync animation
// settings from the server, overwriting the local player's preference
static int32_t s_pvpSavedWazaeffMode = -1;

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
// Stop player movement and set idle animation
// ---------------------------------------------------------------------------
static void stopPlayerMovement() {
    EntityManager::getClass()->initIfNeeded();
    auto* player = EntityManager::getClass()->static_fields->_activeFieldPlayer_k__BackingField;
    if (player == nullptr) return;

    auto* fpe = (FieldPlayerEntity::Object*)player;

    // Stop cross-input movement and bicycle (same as game's StopData pattern)
    fpe->StopCrossInputAndBicycle();

    // Zero out movement vectors so the entity doesn't keep drifting
    fpe->fields.InputMoveVector = {};
    fpe->cast<FieldObjectEntity>()->fields.moveVector = {};

    // Switch to idle animation (clip 0) to stop walk/run animation
    auto* animPlayer = fpe->cast<FieldCharacterEntity>()->fields._animationPlayer;
    if (animPlayer != nullptr) {
        animPlayer->Play(0, 0.1f); // blend to idle over 0.1s
    }
}

// ---------------------------------------------------------------------------
// Close menu helper
// ---------------------------------------------------------------------------
static void closeInteractionMenu() {
    // Close any active ContextMenuWindow (trade confirm, incoming request, etc.)
    if (s_activeContextMenu != nullptr) {
        s_activeContextMenu->Close(nullptr);
        s_activeContextMenu = nullptr;
    }
    PlayerWork::getClass()->initIfNeeded();
    PlayerWork::getClass()->static_fields->_isPlayerInputActive = true;
    stopPlayerMovement();
    // Reset the network-level interaction state so new requests can be received
    overworldMPResetInteractionState();
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

    s_activeContextMenu = window;
    window->Open(param);
    return true;
}

// ---------------------------------------------------------------------------
// Message file constants and helpers
// ---------------------------------------------------------------------------
static constexpr const char* MP_MESSAGE_FILE = "ss_multiplayer";

// Replace {0}, {1}, ... placeholders in a template string with provided values.
// Result is written to outBuf. Returns outBuf for convenience.
static char* formatMPTemplate(char* outBuf, size_t outSize,
                               const char* tmpl,
                               const char* const* values, int valueCount) {
    char* dst = outBuf;
    char* end = outBuf + outSize - 1;
    const char* src = tmpl;
    while (*src && dst < end) {
        if (src[0] == '{' && src[1] >= '0' && src[1] <= '9' && src[2] == '}') {
            int idx = src[1] - '0';
            if (idx < valueCount && values[idx] != nullptr) {
                size_t len = strlen(values[idx]);
                if (dst + len > end) len = end - dst;
                memcpy(dst, values[idx], len);
                dst += len;
            }
            src += 3; // skip {N}
        } else {
            *dst++ = *src++;
        }
    }
    *dst = '\0';
    return outBuf;
}

// Load a string from the ss_multiplayer message file. Returns nullptr if unavailable.
static System::String::Object* getMPMessage(const char* label) {
    auto* mgr = Dpr::Message::MessageManager::get_Instance();
    if (mgr == nullptr) return nullptr;
    return mgr->GetSimpleMessage(
        System::String::Create(MP_MESSAGE_FILE),
        System::String::Create(label));
}

// Load a string from the message file as a C string, with English fallback.
static const char* getMPMessageCStr(const char* label, const char* fallback) {
    auto* str = getMPMessage(label);
    if (str == nullptr || System::String::IsNullOrEmpty(str)) return fallback;
    // asCString() returns a std::string — store in static buffer for snprintf use
    static char s_msgBuf[256];
    auto cstr = str->asCString();
    strncpy(s_msgBuf, cstr.c_str(), sizeof(s_msgBuf) - 1);
    s_msgBuf[sizeof(s_msgBuf) - 1] = '\0';
    // YAML single-quoted strings store \n as literal backslash+n — convert to real newlines
    for (char* p = s_msgBuf; *p; p++) {
        if (p[0] == '\\' && p[1] == 'n') {
            p[0] = '\n';
            memmove(p + 1, p + 2, strlen(p + 2) + 1);
        }
    }
    return s_msgBuf;
}

// ---------------------------------------------------------------------------
// Open a ContextMenuWindow using message file labels instead of raw text.
// Each item resolves its text from messageFile+messageLabel via the vanilla
// ContextMenuItem::Setup path (Param.text left null).
// ---------------------------------------------------------------------------
static bool openContextMenuFromLabels(
    const char* messageFile,
    const char* labels[], int32_t itemCount, int32_t cancelIndex,
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

    auto* msgFileStr = System::String::Create(messageFile);

    for (int32_t i = 0; i < itemCount; i++) {
        param->fields.itemParams->m_Items[i] = itemParamClass->newInstance();
        auto& fields = param->fields.itemParams->m_Items[i]->fields;
        fields.menuId = (ContextMenuID)i;
        fields.messageFile = msgFileStr;
        fields.messageLabel = System::String::Create(labels[i]);
        fields.messageIndex = -1;  // use label, not index
        fields.text = nullptr;     // null triggers messageFile/label resolution
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

    s_activeContextMenu = window;
    window->Open(param);
    return true;
}

// ---------------------------------------------------------------------------
// Open a ContextMenuWindow with a mix: some items use labels, some use raw text.
// textItems[i] != nullptr → use text directly; textItems[i] == nullptr → use labels[i].
// ---------------------------------------------------------------------------
static bool openContextMenuMixed(
    const char* messageFile,
    const char* labels[], const char* textItems[], int32_t itemCount, int32_t cancelIndex,
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

    auto* msgFileStr = System::String::Create(messageFile);

    for (int32_t i = 0; i < itemCount; i++) {
        param->fields.itemParams->m_Items[i] = itemParamClass->newInstance();
        auto& fields = param->fields.itemParams->m_Items[i]->fields;
        fields.menuId = (ContextMenuID)i;
        if (textItems[i] != nullptr) {
            // Dynamic text — use text field directly
            fields.text = System::String::Create(textItems[i]);
        } else {
            // Label-based — resolve from message file
            fields.messageFile = msgFileStr;
            fields.messageLabel = System::String::Create(labels[i]);
            fields.messageIndex = -1;
            fields.text = nullptr;
        }
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

    s_activeContextMenu = window;
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
    const char* labels[] = { "SS_mp_SingleBattle", "SS_mp_DoubleBattle", "SS_mp_Back" };
    s_interact.state = InteractState::BattleMenuOpen;

    if (!openContextMenuFromLabels(MP_MESSAGE_FILE, labels, 3, 2, &onBattleSubmenuClicked, &s_battleSubmenuMethodInfo)) {
        Logger::log("[OverworldMP] ERROR: Failed to open battle submenu\n");
        s_interact.Reset();
        closeInteractionMenu();
    }
}

// ---------------------------------------------------------------------------

// Returns true if trade was blocked (caller should break out of menu handler).
static bool checkTradeBlocked() {
    auto* party = PlayerWork::get_playerParty();
    if (party != nullptr && party->fields.m_memberCount <= 1) {
        Logger::log("[OverworldMP] Trade blocked — only %d pokemon in party\n",
                    (int)party->fields.m_memberCount);
        const char* msg = getMPMessageCStr("SS_mp_TradeLastPokemon",
                                           "You need at least 2 Pok\xc3\xa9mon to trade!");
        showInteractMsgWindow(msg);
        s_interactMsgShown = true;
        s_interact.state = InteractState::TradeWarningMsg;
        s_interact.incomingRequestDelay = 0.2f;  // grace period so held A doesn't dismiss instantly
        return true;
    }
    return false;
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

        case 1: { // Trade
            if (checkTradeBlocked()) break;
            auto& tuTrade = overworldMPGetTeamUpState();
            if (tuTrade.battlePending) {
                Logger::log("[OverworldMP] Trade blocked: team-up battle pending\n");
                s_interact.Reset();
                closeInteractionMenu();
                break;
            }
            Logger::log("[OverworldMP] Requesting trade with station %d\n", s_interact.targetStationIndex);
            s_pendingRequestType = InteractionType::Trade;
            s_isRequester = true;
            overworldMPSendInteractionRequest(s_interact.targetStationIndex,
                                              InteractionType::Trade, BattleSubtype::Single);
            s_interact.state = InteractState::WaitingResponse;
            s_interact.timeoutTimer = INTERACT_TIMEOUT;
            break;
        }

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

        case 1: { // Trade
            if (checkTradeBlocked()) break;
            auto& tuTrade = overworldMPGetTeamUpState();
            if (tuTrade.battlePending) {
                Logger::log("[OverworldMP] Trade blocked (team menu): team-up battle pending\n");
                s_interact.Reset();
                closeInteractionMenu();
                break;
            }
            Logger::log("[OverworldMP] Requesting trade with partner station %d\n", s_interact.targetStationIndex);
            s_pendingRequestType = InteractionType::Trade;
            s_isRequester = true;
            overworldMPSendInteractionRequest(s_interact.targetStationIndex,
                                              InteractionType::Trade, BattleSubtype::Single);
            s_interact.state = InteractState::WaitingResponse;
            s_interact.timeoutTimer = INTERACT_TIMEOUT;
            break;
        }

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
            if (checkTradeBlocked()) break;
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
    const char* labels[] = { "SS_mp_EmoteLike", "SS_mp_EmoteSurprise", "SS_mp_EmoteNoWay", "SS_mp_EmoteHello", "SS_mp_Cancel" };
    s_interact.state = InteractState::EmoteMenuOpen;

    if (!openContextMenuFromLabels(MP_MESSAGE_FILE, labels, 5, 4, &onEmoteMenuClicked, &s_emoteMenuMethodInfo)) {
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

    // Disable player input and stop any current movement/animation
    PlayerWork::getClass()->initIfNeeded();
    PlayerWork::getClass()->static_fields->_isPlayerInputActive = false;
    stopPlayerMovement();

    auto& tu = overworldMPGetTeamUpState();
    if (tu.isTeamedUp && tu.partnerStation == targetStationIndex) {
        // Teamed up, targeting partner: Disband, Trade, Emote, Cancel
        const char* labels[] = { "SS_mp_Disband", "SS_mp_Trade", "SS_mp_Emote", "SS_mp_Cancel" };
        if (!openContextMenuFromLabels(MP_MESSAGE_FILE, labels, 4, 3, &onTeamMenuClicked, &s_teamMenuMethodInfo)) {
            Logger::log("[OverworldMP] ERROR: Failed to open team menu\n");
            s_interact.Reset();
            closeInteractionMenu();
        }
    } else if (tu.isTeamedUp) {
        // Teamed up, targeting non-partner: Team Battle, Trade, Emote, Cancel
        const char* labels[] = { "SS_mp_TeamBattle", "SS_mp_Trade", "SS_mp_Emote", "SS_mp_Cancel" };
        if (!openContextMenuFromLabels(MP_MESSAGE_FILE, labels, 4, 3, &onTeamBattleMenuClicked, &s_teamBattleMenuMethodInfo)) {
            Logger::log("[OverworldMP] ERROR: Failed to open team battle menu\n");
            s_interact.Reset();
            closeInteractionMenu();
        }
    } else {
        // Not teamed up: Battle, Trade, Team Up, Emote, Cancel
        const char* labels[] = { "SS_mp_Battle", "SS_mp_Trade", "SS_mp_TeamUp", "SS_mp_Emote", "SS_mp_Cancel" };
        if (!openContextMenuFromLabels(MP_MESSAGE_FILE, labels, 5, 4, &onMainMenuClicked, &s_mainMenuMethodInfo)) {
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
    // Hide the MsgWindow prompt regardless of which option was picked
    if (s_interactMsgShown) {
        hideInteractMsgWindow();
    }

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

        // If this is a trade request, check party size before accepting
        if (s_pendingRequestType == InteractionType::Trade) {
            auto* party = PlayerWork::get_playerParty();
            if (party != nullptr && party->fields.m_memberCount <= 1) {
                Logger::log("[OverworldMP] Trade blocked on receiver — only %d pokemon, declining\n",
                            (int)party->fields.m_memberCount);
                overworldMPSendInteractionResponse(s_pendingFromStation, false);
                s_pendingFromStation = -1;
                // Show the warning in a MsgWindow so the player knows why
                const char* msg = getMPMessageCStr("SS_mp_TradeLastPokemon",
                                                   "You need at least 2 Pok\xc3\xa9mon to trade!");
                showInteractMsgWindow(msg);
                s_interactMsgShown = true;
                s_interact.state = InteractState::TradeWarningMsg;
                s_interact.incomingRequestDelay = 0.2f;
                return true;
            }
        }

        Logger::log("[OverworldMP] Accepting request from station %d (type=%d)\n",
                    s_pendingFromStation, (int)s_pendingRequestType);
        overworldMPSendInteractionResponse(s_pendingFromStation, true);
        overworldMPShowRemoteEmote(s_pendingFromStation, EMOTE_ID_LIKES);

        if (s_pendingRequestType == InteractionType::TeamUp) {
            // Team-up needs no UI transition — complete immediately and unlock
            overworldMPTeamUp(s_pendingFromStation);
            s_pendingFromStation = -1;
            s_interact.Reset();
            closeInteractionMenu();
        } else {
            s_interact.state = InteractState::Accepted;
            s_interact.targetStationIndex = s_pendingFromStation;
            s_interact.tradeStartDelay = 0.3f;
            s_pendingFromStation = -1;
        }
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

    // If in any battle (wild, trainer, etc.), decline — can't show UI during battle
    auto* fmReq = FieldManager::getClass()->static_fields->_Instance_k__BackingField;
    if (fmReq != nullptr && fmReq->fields._updateType != 0) {
        Logger::log("[OverworldMP] Declining incoming request — in battle (updateType=%d)\n",
                    fmReq->fields._updateType);
        overworldMPSendInteractionResponse(fromStation, false);
        return;
    }

    // If a team-up battle is pending, decline incoming requests
    auto& tuIncoming = overworldMPGetTeamUpState();
    if (tuIncoming.battlePending) {
        Logger::log("[OverworldMP] Declining incoming request — team-up battle pending\n");
        overworldMPSendInteractionResponse(fromStation, false);
        return;
    }

    s_pendingFromStation = fromStation;
    s_pendingRequestType = type;
    s_pendingBattleSubtype = battleSubtype;
    s_isRequester = false;  // we are the receiver

    // Disable player input and stop movement while dialog is open
    PlayerWork::getClass()->initIfNeeded();
    PlayerWork::getClass()->static_fields->_isPlayerInputActive = false;
    stopPlayerMovement();

    // Defer the actual dialog display by a few frames so the MsgWindow
    // has time to finish any pending layout/animation from the field UI.
    s_interact.state = InteractState::IncomingRequestPending;
    s_interact.incomingRequestDelay = 0.15f;  // ~4-5 frames at 30fps
    Logger::log("[OverworldMP] Incoming request deferred (0.15s delay)\n");
}

// Actually show the incoming request dialog (called from tick after delay)
static void showIncomingRequestDialog() {
    auto& ctx = getOverworldMPContext();
    auto& remote = ctx.remotePlayers[s_pendingFromStation];
    auto type = s_pendingRequestType;
    auto battleSubtype = s_pendingBattleSubtype;

    s_interact.state = InteractState::ReceivedRequest;

    // Build the MsgWindow prompt text — load action name and format from message file
    const char* actionLabel = "SS_mp_Trade";
    const char* actionFallback = "Trade";
    if (type == InteractionType::Battle) {
        if (battleSubtype == BattleSubtype::Double) {
            actionLabel = "SS_mp_DoubleBattle";
            actionFallback = "Double Battle";
        } else {
            actionLabel = "SS_mp_SingleBattle";
            actionFallback = "Single Battle";
        }
    } else if (type == InteractionType::TeamUp) {
        actionLabel = "SS_mp_TeamUp";
        actionFallback = "Team Up";
    }

    // Load translatable action name
    static char actionTextBuf[64];
    auto* actionStr = getMPMessage(actionLabel);
    if (actionStr != nullptr && !System::String::IsNullOrEmpty(actionStr)) {
        auto cstr = actionStr->asCString();
        strncpy(actionTextBuf, cstr.c_str(), sizeof(actionTextBuf) - 1);
        actionTextBuf[sizeof(actionTextBuf) - 1] = '\0';
    } else {
        strncpy(actionTextBuf, actionFallback, sizeof(actionTextBuf) - 1);
        actionTextBuf[sizeof(actionTextBuf) - 1] = '\0';
    }

    // Load translatable default name
    const char* nameBuf = remote.playerNameSet ? remote.playerNameBuf :
        getMPMessageCStr("SS_mp_DefaultName", "Player");

    // MSBT has proper Name tags (generated by ev-as) — GetSimpleMessage strips them,
    // so we use the template directly. Tags: {0}=action, {1}=name
    const char* promptFmt = "{0} with {1}?";

    static char promptBuf[128];
    const char* vals[] = { actionTextBuf, nameBuf };
    formatMPTemplate(promptBuf, sizeof(promptBuf), promptFmt, vals, 2);
    Logger::log("[OverworldMP] Showing incoming request dialog: %s\n", promptBuf);

    // Show the prompt in the MsgWindow dialogue box
    showInteractMsgWindow(promptBuf);
    s_interactMsgShown = true;

    // Show Accept/Decline ContextMenuWindow for the decision
    const char* labels[] = { "SS_mp_Yes", "SS_mp_Decline" };
    if (!openContextMenuFromLabels(MP_MESSAGE_FILE, labels, 2, 1, &onIncomingRequestClicked, &s_incomingRequestMethodInfo)) {
        Logger::log("[OverworldMP] ERROR: Failed to open incoming request dialog\n");
        hideInteractMsgWindow();
        overworldMPSendInteractionResponse(s_pendingFromStation, false);
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
    s_myTradeIsBox = false;
    s_boxWindow = nullptr;
    s_myTradePokeParam = nullptr;
    s_partnerTradePokeParam = nullptr;
    memset(s_myTradePokeData, 0, sizeof(s_myTradePokeData));
    memset(s_partnerTradePokeData, 0, sizeof(s_partnerTradePokeData));
    s_partnerPokeReceived = false;
    s_partnerConfirmReceived = false;
    s_partnerConfirmValue = false;
    s_zukanRegister = nullptr;
    s_zukanDismissed = false;
    s_suppressZukanRegistration = false;
    s_tradeWaitTimeout = 0.0f;
}

// ---------------------------------------------------------------------------
// Trade: show partner pokemon preview, then confirm dialog
// ---------------------------------------------------------------------------
static void showTradeConfirmDialog();
static void showTradePreview();

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

    if (s_partnerTradePokeParam != nullptr && s_myTradePartySlot >= 0) {
        if (s_myTradeIsBox) {
            // Box trade: replace the box slot with partner's pokemon
            Dpr::Box::BoxPokemonWork::UpdatePokemon(s_partnerTradePokeParam,
                                                     s_myTradeTrayIndex, s_myTradePartySlot);
            Logger::log("[OverworldMP] Trade complete — replaced box tray %d pos %d\n",
                        s_myTradeTrayIndex, s_myTradePartySlot);
        } else {
            // Party trade: replace party slot
            auto* party = PlayerWork::get_playerParty();
            if (party != nullptr) {
                party->ReplaceMember(s_myTradePartySlot, s_partnerTradePokeParam);
                Logger::log("[OverworldMP] Trade complete — replaced party slot %d\n", s_myTradePartySlot);
            } else {
                Logger::log("[OverworldMP] ERROR: party is null in onDemoEnd\n");
            }
        }
    } else {
        Logger::log("[OverworldMP] ERROR: cannot swap — partnerPoke=%p, slot=%d\n",
                    s_partnerTradePokeParam, s_myTradePartySlot);
    }

    // Unlock box (matches NPC trade pattern)
    Dpr::UI::BoxWindow::LimitBox(false);

    // Re-spawn MP entities that were despawned before the demo.
    // The field scene is restored at this point, safe to create new entities.
    auto& ctx = getOverworldMPContext();
    for (int i = 0; i < OW_MP_MAX_PLAYERS; i++) {
        auto& remote = ctx.remotePlayers[i];
        if (remote.isActive && !remote.isSpawned &&
            remote.areaID == ctx.myAreaID) {
            overworldMPSpawnEntity(i);
        }
    }

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
    // Load Demo_Trade metadata (0x3760 from DemoSceneManager resolves Demo_Trade TypeInfo)
    system_load_typeinfo(0x3760);

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

    // Despawn all MP entities before demo scene transition.
    // PlayDemo loads/unloads scenes which destroys non-DDOL GameObjects.
    // Without this, entity pointers become dangling and players vanish.
    overworldMPDespawnAllEntities();

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
    Logger::log("[OverworldMP] Executing trade swap: isBox=%d, tray=%d, slot=%d, partner station=%d\n",
                (int)s_myTradeIsBox, s_myTradeTrayIndex, s_myTradePartySlot, s_tradePartnerStation);

    auto* party = PlayerWork::get_playerParty();
    if (party == nullptr) {
        Logger::log("[OverworldMP] ERROR: playerParty is null, cannot execute swap\n");
        resetTradeState();
        s_interact.Reset();
        closeInteractionMenu();
        return;
    }

    // Get our pokemon before the swap (for Demo_Trade display)
    Pml::PokePara::PokemonParam::Object* myPoke = nullptr;
    if (s_myTradeIsBox) {
        myPoke = Dpr::Box::BoxPokemonWork::GetPokemon(s_myTradeTrayIndex, s_myTradePartySlot);
    } else if (s_myTradePartySlot >= 0) {
        myPoke = party->GetMemberPointer(s_myTradePartySlot);
    }
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

    // Close BoxWindow before starting Demo_Trade (it should already be closed
    // by the time we get here, but ensure it's cleared)
    s_boxWindow = nullptr;

    // Try to start the trade cutscene animation
    if (tryStartDemoTrade(myPoke, partnerPoke)) {
        s_interact.state = InteractState::TradeAnimating;
        Logger::log("[OverworldMP] Demo_Trade started — swap will happen when animation ends\n");
        return;
    }

    // Fallback: no animation available — swap immediately
    Logger::log("[OverworldMP] Demo_Trade unavailable — doing immediate swap\n");
    if (s_myTradeIsBox) {
        Dpr::Box::BoxPokemonWork::UpdatePokemon(partnerPoke, s_myTradeTrayIndex, s_myTradePartySlot);
        Logger::log("[OverworldMP] Trade complete — replaced box tray %d pos %d\n",
                    s_myTradeTrayIndex, s_myTradePartySlot);
    } else {
        party->ReplaceMember(s_myTradePartySlot, partnerPoke);
        Logger::log("[OverworldMP] Trade complete — replaced party slot %d\n", s_myTradePartySlot);
    }

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
    // Hide the MsgWindow prompt regardless of which option was picked
    if (s_interactMsgShown) {
        hideInteractMsgWindow();
    }

    // Close UIZukanRegister if it's showing the 3D pokemon model.
    // Clear suppress flag FIRST so the Close hook allows this through.
    if (s_zukanRegister != nullptr) {
        s_suppressZukanRegistration = false;
        s_zukanRegister->Close(nullptr, 0);
        s_zukanRegister = nullptr;
    }

    if (item == nullptr) {
        // B-button cancel on confirm dialog — abort trade
        Logger::log("[OverworldMP] Trade confirm cancelled via B-button — aborting trade\n");
        s_activeContextMenu = nullptr;  // menu auto-closes on return true
        overworldMPSendTradeConfirm(s_tradePartnerStation, false);
        // Don't call SetTradeInfo/ToNextPhase — BoxWindow in selection mode, trade panel uninit
        s_boxWindow = nullptr;
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
        s_activeContextMenu = nullptr;  // menu auto-closes on return true
        overworldMPSendTradeConfirm(s_tradePartnerStation, true);

        // Close BoxWindow before proceeding to Demo_Trade
        if (s_boxWindow != nullptr) {
            s_boxWindow = nullptr;
        }

        // Check if partner already confirmed
        if (s_partnerConfirmReceived && s_partnerConfirmValue) {
            Logger::log("[OverworldMP] Partner already confirmed — executing swap\n");
            overworldMPExecuteSwap();
        } else {
            // Wait for partner's confirmation
            s_interact.state = InteractState::TradeWaitFinal;
            s_tradeWaitTimeout = TRADE_WAIT_TIMEOUT;
            Logger::log("[OverworldMP] Waiting for partner's trade confirmation\n");
        }
    } else {
        // No — abort trade (re-selection requires bidirectional protocol; abort is safe)
        Logger::log("[OverworldMP] Declined partner offer — aborting trade\n");
        s_activeContextMenu = nullptr;  // menu auto-closes on return true
        overworldMPSendTradeConfirm(s_tradePartnerStation, false);
        // Don't call SetTradeInfo/ToNextPhase — BoxWindow was opened in selection mode
        // (openType=1) so _tradeInfoCanvasGroup was never initialized by SetTraderName
        s_boxWindow = nullptr;
        resetTradeState();
        s_interact.Reset();
        closeInteractionMenu();
    }

    return true;
}

// ---------------------------------------------------------------------------
// Trade: UIZukanRegister OnComplete callback (fires when user presses A path)
// ---------------------------------------------------------------------------
static void onZukanComplete(int32_t addMemberResult, MethodInfo* mi) {
    Logger::log("[OverworldMP] UIZukanRegister OnComplete: result=%d\n", addMemberResult);
    s_zukanDismissed = true;
}

// ---------------------------------------------------------------------------
// Trade: show partner pokemon preview (UIZukanRegister)
// ---------------------------------------------------------------------------
static void showTradePreview() {
    s_interact.state = InteractState::TradeConfirm;
    s_zukanDismissed = false;
    s_zukanRegister = nullptr;

    auto* party = PlayerWork::get_playerParty();
    if (!s_partnerPokeReceived || party == nullptr) {
        Logger::log("[OverworldMP] No partner pokemon to preview, going to confirm only\n");
        showTradeConfirmDialog();
        return;
    }

    auto* partnerPoke = deserializePartnerPokemon(party);
    if (partnerPoke == nullptr) {
        Logger::log("[OverworldMP] Failed to deserialize partner pokemon, going to confirm only\n");
        showTradeConfirmDialog();
        return;
    }

    auto* core = (Pml::PokePara::CoreParam*)partnerPoke;
    int32_t partnerMonsNo = core->GetMonsNo();
    int32_t langId = (int32_t)PlayerWork::get_msgLangID();
    uint32_t level = core->GetLevel();
    auto sex = core->GetSex();
    bool isRare = core->IsRare();
    Logger::log("[OverworldMP] Partner pokemon: monsNo=%d lv=%d sex=%d rare=%d\n",
                partnerMonsNo, level, (int)sex, isRare);

    // --- Open UIZukanRegister for 3D model display ---
    system_load_typeinfo(0x43bd);
    SmartPoint::AssetAssistant::SingletonMonoBehaviour::getClass()->initIfNeeded();
    auto* uiManager = Dpr::UI::UIManager::get_Instance();
    if (uiManager == nullptr) {
        Logger::log("[OverworldMP] UIManager null, going to confirm only\n");
        showTradeConfirmDialog();
        return;
    }

    auto* uiZukanReg = uiManager->CreateUIWindow<Dpr::UI::UIZukanRegister>(UIWindowID::ZUKAN_REGISTER);
    if (uiZukanReg == nullptr) {
        Logger::log("[OverworldMP] Failed to create UIZukanRegister, going to confirm only\n");
        showTradeConfirmDialog();
        return;
    }
    s_zukanRegister = uiZukanReg;

    // Create OnComplete callback (Action<AddMemberResult> = Action<int32_t>)
    if (s_zukanCompleteMI == nullptr) {
        s_zukanCompleteMI = (MethodInfo*)nn_malloc(sizeof(MethodInfo));
        memset(s_zukanCompleteMI, 0, sizeof(MethodInfo));
        s_zukanCompleteMI->methodPointer = (Il2CppMethodPointer)&onZukanComplete;
        s_zukanCompleteMI->flags = 0x0010;          // METHOD_ATTRIBUTE_STATIC
        s_zukanCompleteMI->parameters_count = 1;     // int32_t addMemberResult
    }

    auto* completeActionClass = System::Action::getClass(System::Action::UIZukanRegister_AddMemberResult_TypeInfo);
    if (completeActionClass != nullptr) {
        completeActionClass->initIfNeeded();
        if (s_zukanCompleteMI->klass == nullptr) {
            s_zukanCompleteMI->klass = (Il2CppClass*)completeActionClass;
        }
        auto* completeAction = (System::Action::Object*)il2cpp_object_new((Il2CppClass*)completeActionClass);
        _ILExternal::external<void>(0x0263EBA0, completeAction, nullptr, s_zukanCompleteMI);
        uiZukanReg->add_OnComplete(completeAction);
    }

    // Suppress the "data added to Pokedex" message via our ShowRegisterNewMessage hook
    s_suppressZukanRegistration = true;

    // Open with isSkipAddMemberProc=true (bootType=0 from Open)
    uiZukanReg->Open(partnerPoke, true, -1);

    // Override bootType to 1 after Open returns:
    // - State 2 of the OpOpen coroutine sees bootType=1 → calls ShowRegisterNewMessage (hooked as no-op)
    // - b__37_0 callback (after description panel dismiss) sees bootType=1 → calls Close() (skips nickname)
    // Result: A → ShowDescription → A/B → Close, B → Close directly. No registration text, no nickname.
    *reinterpret_cast<int32_t*>(reinterpret_cast<uintptr_t>(uiZukanReg) + 0x98) = 1;

    Logger::log("[OverworldMP] UIZukanRegister opened for partner pokemon preview\n");

    // --- Build the prompt text now, but defer showing it until after UIZukanRegister
    //     finishes its opening animation. The MsgWindow must be activated AFTER the
    //     UIZukanRegister's Canvas is set up, so it renders on top. ---
    auto& ctx = getOverworldMPContext();
    const char* partnerName = getMPMessageCStr("SS_mp_DefaultName", "Player");
    if (s_tradePartnerStation >= 0 && s_tradePartnerStation < OW_MP_MAX_PLAYERS &&
        ctx.remotePlayers[s_tradePartnerStation].playerNameSet) {
        partnerName = ctx.remotePlayers[s_tradePartnerStation].playerNameBuf;
    }

    auto* nameStr = Pml::Personal::PersonalSystem::GetMonsName(partnerMonsNo, langId);
    if (nameStr != nullptr && !System::String::IsNullOrEmpty(nameStr)) {
        auto cname = nameStr->asCString();
        const char* genderStr = "";
        if (sex == Pml::Sex::MALE) genderStr = " \xe2\x99\x82";
        else if (sex == Pml::Sex::FEMALE) genderStr = " \xe2\x99\x80";
        // MSBT has proper Name tags (ev-as). Tags: {0}=name, {1}=level, {2}=gender, {3}=species, {4}=shiny
        const char* promptFmt = "Trade for {0}'s Lv.{1}{2} {3}{4}?";
        char lvBuf[8];
        snprintf(lvBuf, sizeof(lvBuf), "%u", level);
        const char* vals[] = { partnerName, lvBuf, genderStr, cname.c_str(),
                               isRare ? " \xe2\x98\x85" : "" };
        formatMPTemplate(s_tradeConfirmMsgBuf, sizeof(s_tradeConfirmMsgBuf), promptFmt, vals, 5);
    } else {
        const char* vals[] = { partnerName };
        formatMPTemplate(s_tradeConfirmMsgBuf, sizeof(s_tradeConfirmMsgBuf),
                         "Trade for {0}'s pokemon?", vals, 1);
    }

    Logger::log("[OverworldMP] Trade confirm text prepared: %s\n", s_tradeConfirmMsgBuf);

    // Transition to TradePreview — tick handler will show MsgWindow + Yes/No
    // after a short delay, ensuring the UIZukanRegister Canvas is fully set up.
    s_interact.state = InteractState::TradePreview;
    s_interact.incomingRequestDelay = 0.4f;  // ~24 frames — wait for UIZukanRegister open anim
}

// ---------------------------------------------------------------------------
// Trade: show confirm dialog (Yes/No after preview)
// ---------------------------------------------------------------------------
static void showTradeConfirmDialog() {
    s_interact.state = InteractState::TradeConfirm;

    // Build the MsgWindow prompt text with partner name + pokemon details
    auto& ctx = getOverworldMPContext();
    const char* partnerName = getMPMessageCStr("SS_mp_DefaultName", "Player");
    if (s_tradePartnerStation >= 0 && s_tradePartnerStation < OW_MP_MAX_PLAYERS &&
        ctx.remotePlayers[s_tradePartnerStation].playerNameSet) {
        partnerName = ctx.remotePlayers[s_tradePartnerStation].playerNameBuf;
    }

    static char msgBuf[256] = {};
    const char* promptText = msgBuf;
    snprintf(msgBuf, sizeof(msgBuf), "Trade for %s's pokemon?", partnerName);

    if (s_partnerPokeReceived) {
        auto* party = PlayerWork::get_playerParty();
        if (party != nullptr) {
            auto* partnerPoke = deserializePartnerPokemon(party);
            if (partnerPoke != nullptr) {
                auto* core = (Pml::PokePara::CoreParam*)partnerPoke;
                int32_t partnerMonsNo = core->GetMonsNo();
                int32_t langId = (int32_t)PlayerWork::get_msgLangID();
                uint32_t level = core->GetLevel();
                auto sex = core->GetSex();
                bool isRare = core->IsRare();

                auto* nameStr = Pml::Personal::PersonalSystem::GetMonsName(partnerMonsNo, langId);
                if (nameStr != nullptr && !System::String::IsNullOrEmpty(nameStr)) {
                    auto cname = nameStr->asCString();
                    const char* genderStr = "";
                    if (sex == Pml::Sex::MALE) genderStr = " \xe2\x99\x82";
                    else if (sex == Pml::Sex::FEMALE) genderStr = " \xe2\x99\x80";
                    const char* promptFmt = "Trade for {0}'s Lv.{1}{2} {3}{4}?";
                    char lvBuf[8];
                    snprintf(lvBuf, sizeof(lvBuf), "%u", level);
                    const char* vals[] = { partnerName, lvBuf, genderStr, cname.c_str(),
                                           isRare ? " \xe2\x98\x85" : "" };
                    formatMPTemplate(msgBuf, sizeof(msgBuf), promptFmt, vals, 5);
                }
            }
        }
    }

    // Show the question in the MsgWindow dialogue box
    showInteractMsgWindow(promptText);
    s_interactMsgShown = true;
    Logger::log("[OverworldMP] Showing trade confirm MsgWindow: %s\n", promptText);

    // Show Yes/No ContextMenuWindow for the decision
    const char* labels[] = { "SS_mp_Yes", "SS_mp_No" };
    if (!openContextMenuFromLabels(MP_MESSAGE_FILE, labels, 2, 1, &onTradeConfirmClicked, &s_tradeConfirmMethodInfo)) {
        Logger::log("[OverworldMP] ERROR: Failed to open trade confirm Yes/No menu\n");
        hideInteractMsgWindow();
        overworldMPSendTradeConfirm(s_tradePartnerStation, false);
        if (s_boxWindow != nullptr) {
            s_boxWindow = nullptr;
        }
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
    // Do NOT clear s_boxWindow — BoxWindow stays open in trade mode

    if (selected == nullptr || selected->max_length < 1 || selected->m_Items[0] == nullptr) {
        Logger::log("[OverworldMP] BoxWindow selection was empty — cancelling trade\n");
        overworldMPSendTradeConfirm(s_tradePartnerStation, false);
        s_boxWindow = nullptr;
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
        s_boxWindow = nullptr;
        resetTradeState();
        s_interact.Reset();
        closeInteractionMenu();
        return;
    }

    if (trayIndex == -1) {
        // Party trade — don't allow trading the last party member
        auto* party = PlayerWork::get_playerParty();
        if (party != nullptr && party->fields.m_memberCount <= 1) {
            Logger::log("[OverworldMP] Cannot trade — only 1 pokemon in party\n");
            // Don't close BoxWindow — let the player pick again
            return;
        }
        s_myTradeIsBox = false;
    } else {
        // Box trade
        s_myTradeIsBox = true;
    }

    s_myTradePartySlot = indexInTray;
    s_myTradeTrayIndex = trayIndex;

    // Serialize our selected pokemon
    auto* corePoke = (Pml::PokePara::CoreParam*)poke;
    auto* accessor = poke->fields.m_accessor;
    if (accessor != nullptr) {
        memset(s_myTradePokeData, 0, sizeof(s_myTradePokeData));
        // Use Serialize_FullData (raw pointer version) @ 0x24A4470
        _ILExternal::external<void>(0x24A4470, accessor, s_myTradePokeData);
    }

    Logger::log("[OverworldMP] Selected %s tray=%d slot=%d (monsNo=%d) for trade, sending 0xC4\n",
                s_myTradeIsBox ? "box" : "party", trayIndex, indexInTray, corePoke->GetMonsNo());

    // Send our pokemon data to the partner
    overworldMPSendTradePoke(s_tradePartnerStation, s_myTradePartySlot,
                             s_myTradePokeData, POKE_FULL_DATA_SIZE);

    // Always transition to TradeWaitPartner — the tick handler will check
    // s_partnerPokeReceived and call showTradePreview() on the next frame.
    // This ensures both initiating and receiving players use the exact same
    // code path to showTradePreview(), avoiding MsgWindow visibility issues
    // that occur when calling it from inside the BoxWindow callback.
    s_interact.state = InteractState::TradeWaitPartner;
    s_tradeWaitTimeout = TRADE_WAIT_TIMEOUT;
    if (s_partnerPokeReceived) {
        Logger::log("[OverworldMP] Partner already sent pokemon — will show preview next tick\n");
    } else {
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
    // Don't call SetTradeInfo — BoxWindow was opened in selection mode, trade panel uninit
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
        if (key == boxKey) {
            // Also check that the pool entry VALUE is non-null.
            auto* value = *(void**)(data + (uint64_t)i * 0x18 + 0x10);
            if (value == nullptr) return false;
            return true;
        }
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
// Trade: open BoxWindow for pokemon selection (trade mode with TradeParam)
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

    // Initialize IL2CPP metadata for CreateUIWindow<BoxWindow> and related types.
    // 0x4e3f: GMSSceneDataModel::OpenBoxWindow — resolves CreateUIWindow<BoxWindow>
    //         MethodInfo and Action<BoxWindow,SelectedPokemon[]> TypeInfo.
    // 0x2673: BoxWindow::Open(OpenParam, Action, int) — resolves Open method metadata.
    system_load_typeinfo(0x4e3f);
    system_load_typeinfo(0x2673);

    auto* boxWindow = uiMgr->CreateUIWindow<Dpr::UI::BoxWindow>(UIWindowID::BOX);
    if (boxWindow == nullptr) {
        Logger::log("[OverworldMP] Failed to create BoxWindow\n");
        resetTradeState();
        s_interact.Reset();
        closeInteractionMenu();
        return;
    }

    s_boxWindow = boxWindow;

    // Create OpenParam for selection mode
    system_load_typeinfo(0x2672);
    auto* openParam = Dpr::UI::BoxWindow::OpenParam::newInstance();
    openParam->fields.openType = 1;            // External (select pokemon mode)
    openParam->fields.selectCount = 1;
    openParam->fields.isEnableEgg = false;     // don't allow trading eggs
    openParam->fields.isSelectParty = true;    // start on party view
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

    // Create MethodInfo for our cancel callback (Action<BoxWindow> signature)
    if (s_boxCancelMethodInfo == nullptr) {
        s_boxCancelMethodInfo = (MethodInfo*)nn_malloc(sizeof(MethodInfo));
        memset(s_boxCancelMethodInfo, 0, sizeof(MethodInfo));
        s_boxCancelMethodInfo->methodPointer = (Il2CppMethodPointer)&onBoxCancelled;
        s_boxCancelMethodInfo->flags = 0x0010;          // METHOD_ATTRIBUTE_STATIC
        s_boxCancelMethodInfo->parameters_count = 0;     // void() — non-generic Action
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

    // Open BoxWindow in selection mode
    boxWindow->Open(openParam, selectAction, 0);

    // Set up fake TradeParam buffer so SetOtherPokeParam can write to it later.
    // TradeParam TypeInfo can't be resolved, but we only need a valid memory
    // region at _tradeParam for SetOtherPokeParam to store otherOriginalParam
    // (+0x40) and otherPokeParam (+0x48). SetTradeInfo doesn't access _tradeParam.
    memset(s_fakeTradeParam, 0, sizeof(s_fakeTradeParam));
    boxWindow->fields._tradeParam = (void*)s_fakeTradeParam;

    s_interact.state = InteractState::TradeSelectPoke;
    Logger::log("[OverworldMP] BoxWindow opened for trade (selection mode + fake TradeParam)\n");
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
        Logger::log("[OverworldMP] Partner pokemon received while waiting — showing preview\n");
        showTradePreview();
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
        // Close UIZukanRegister if showing partner pokemon preview
        if (s_zukanRegister != nullptr) {
            s_suppressZukanRegistration = false;
            s_zukanRegister->Close(nullptr, 0);
            s_zukanRegister = nullptr;
        }
        // Hide MsgWindow if showing trade confirm text
        if (s_interactMsgShown) {
            hideInteractMsgWindow();
        }
        // Don't call SetTradeInfo/ToNextPhase — BoxWindow in selection mode, trade panel uninit
        s_boxWindow = nullptr;
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

    // Reject if not in a battle exchange state (e.g., after timeout reset)
    if (s_interact.state != InteractState::BattleExchangeParty &&
        s_interact.state != InteractState::BattleSyncWait) {
        Logger::log("[OverworldMP] Ignoring battle party: not in exchange state (%d)\n",
                    (int)s_interact.state);
        return;
    }

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

// (overworldMPSetTeamUpBattleStarting removed — Player B now uses sync-wait + DeferEncountStart)

void overworldMPOnBattleReadyReceived(int32_t fromStation) {
    // Reject if not in a battle sync state
    if (s_interact.state != InteractState::BattleSyncWait) {
        Logger::log("[OverworldMP] Ignoring BATTLE_READY: not in sync state (%d)\n",
                    (int)s_interact.state);
        return;
    }
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

    // Save local animation setting — SetupBattleComm (comm battle) will sync
    // from the server, overwriting the local player's preference
    auto* myConfig = PlayerWork::get_config();
    if (myConfig != nullptr) {
        s_pvpSavedWazaeffMode = myConfig->fields.wazaeff_mode;
        Logger::log("[OverworldMP] Saved wazaeff_mode=%d before PvP\n", s_pvpSavedWazaeffMode);
    }

    // Enable MP battle color flag BEFORE SetupBattleComm so that
    // SetColorID_TrainerParam_StoreCore (fires during StoreCore) doesn't
    // overwrite per-slot colors with the local save's color.
    {
        extern bool g_owmpBattleColorActive;
        g_owmpBattleColorActive = true;
        Logger::log("[OverworldMP] g_owmpBattleColorActive = true (pre-setup)\n");
    }

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

    // --- Override per-slot trainer colors for MP ---
    // Three data stores must be patched so that every code path in the
    // battle view reads the correct per-slot color:
    //   1. TRAINER_DATA.colorID  — read by MainModule::GetClientTrainerColorID
    //   2. MyStatus field @ +0x25 — read by MyStatus::GetColorID (our hook
    //      defers to this field during MP battles)
    //   3. g_owmpBattleSlotColors[] — used by CardModelViewController_LoadModels
    //      for the trainer card model (cursor-based)
    {
        int32_t localColor = getCustomSaveData()->playerColorVariation.playerColorID;
        auto& remote = getOverworldMPContext().remotePlayers[s_battlePartnerStation];
        int32_t remoteColor = remote.colorId; // Pass -1 through for custom colors

        int32_t localSlot    = (commPos == 0) ? 0 : 1;
        int32_t opponentSlot = (commPos == 0) ? 1 : 0;

        // (1) TRAINER_DATA.colorID
        void* trDataRaw = bsp->instance()->fields.tr_data;
        if (trDataRaw != nullptr) {
            uint32_t arrLen = *(uint32_t*)((uintptr_t)trDataRaw + 0x18);
            if (arrLen >= 2) {
                auto* localTD = *(Dpr::Battle::Logic::TRAINER_DATA::Object**)
                    ((uintptr_t)trDataRaw + 0x20 + (uint64_t)localSlot * 8);
                if (localTD != nullptr)
                    localTD->fields.colorID = localColor;

                auto* opponentTD = *(Dpr::Battle::Logic::TRAINER_DATA::Object**)
                    ((uintptr_t)trDataRaw + 0x20 + (uint64_t)opponentSlot * 8);
                if (opponentTD != nullptr)
                    opponentTD->fields.colorID = remoteColor;
            }
        }

        // (2) MyStatus.colorID (byte at object offset 0x25)
        auto* statusArr = bsp->instance()->fields.playerStatus;
        if (statusArr != nullptr && statusArr->max_length >= 2) {
            auto* localMS  = statusArr->m_Items[localSlot];
            auto* remoteMS = statusArr->m_Items[opponentSlot];
            if (localMS != nullptr)
                *(uint8_t*)((uintptr_t)localMS + 0x25) = (uint8_t)localColor;
            if (remoteMS != nullptr)
                *(uint8_t*)((uintptr_t)remoteMS + 0x25) = (uint8_t)remoteColor;
        }

        // (3) Slot color array + cursors for CardModelViewController and StoreCore
        extern int32_t g_owmpBattleSlotColors[];
        extern int32_t g_owmpBattleSlotCursor;
        extern int32_t g_owmpStoreCoreCursor;
        g_owmpBattleSlotColors[0] = (commPos == 0) ? localColor : remoteColor;
        g_owmpBattleSlotColors[1] = (commPos == 0) ? remoteColor : localColor;
        g_owmpBattleSlotColors[2] = 0;
        g_owmpBattleSlotColors[3] = 0;
        g_owmpBattleSlotCursor = 0;
        g_owmpStoreCoreCursor = 0;
        extern int32_t g_owmpSetSkinColorCursor;
        g_owmpSetSkinColorCursor = 0;

        // (4) Custom battle colors for ALL slots with colorId == -1.
        // The CardModelViewController_LoadModels hook uses a cursor that may not
        // match slot order (e.g. commPos=1 loads local model first). By populating
        // both local and remote slots, the override is correct regardless of order.
        extern bool g_owmpBattleSlotHasCustomColors[];
        extern RomData::ColorSet g_owmpBattleSlotCustomColorSets[];
        extern RomData::ColorSet GetCustomColorSet();
        memset(g_owmpBattleSlotHasCustomColors, 0, sizeof(bool) * 4);
        int32_t localSlotIdx  = (commPos == 0) ? 0 : 1;
        int32_t remoteSlotIdx = (commPos == 0) ? 1 : 0;

        // Local player's slot — populate from local save data
        if (localColor == -1) {
            g_owmpBattleSlotHasCustomColors[localSlotIdx] = true;
            g_owmpBattleSlotCustomColorSets[localSlotIdx] = GetCustomColorSet();
        }

        // Remote player's slot — populate from received 0xCD packet data
        if (remoteColor == -1 && remote.hasCustomColors) {
            g_owmpBattleSlotHasCustomColors[remoteSlotIdx] = true;
            auto& cs = g_owmpBattleSlotCustomColorSets[remoteSlotIdx];
            for (int c = 0; c < 6; c++) {
                float* dst = (c == 0) ? &cs.fieldSkinFace.r :
                             (c == 1) ? &cs.fieldSkinMouth.r :
                             (c == 2) ? &cs.fieldEyes.r :
                             (c == 3) ? &cs.fieldEyebrows.r :
                             (c == 4) ? &cs.fieldSkinBody.r : &cs.fieldHair.r;
                dst[0] = remote.customFieldColors[c * 3];
                dst[1] = remote.customFieldColors[c * 3 + 1];
                dst[2] = remote.customFieldColors[c * 3 + 2];
                dst[3] = 1.0f;
            }
            float* battleBase = &cs.battleSkinFace.r;
            for (int c = 0; c < 6; c++) {
                battleBase[c * 4 + 0] = remote.customBattleColors[c * 3];
                battleBase[c * 4 + 1] = remote.customBattleColors[c * 3 + 1];
                battleBase[c * 4 + 2] = remote.customBattleColors[c * 3 + 2];
                battleBase[c * 4 + 3] = 1.0f;
            }
        }

        Logger::log("[OverworldMP] Set trainer colors — local slot%d=%d, opponent slot%d=%d (custom=%d)\n",
                    localSlot, localColor, opponentSlot, remoteColor, (int)remote.hasCustomColors);
    }

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

    // g_owmpBattleColorActive was already set before SetupBattleComm above

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

    // Record which station is our battle partner BEFORE resetBattleState
    // clears s_battlePartnerStation. This persists through the battle scene
    // so onPlayerLeave can detect a mid-battle disconnect.
    overworldMPSetActiveBattlePartner(s_battlePartnerStation);

    // Clean up battle state (battle scene takes over from here)
    resetBattleState();
    s_interact.Reset();
    closeInteractionMenu();
}

// ---------------------------------------------------------------------------
// Request accepted/declined handlers (called from both sides)
// ---------------------------------------------------------------------------

void overworldMPOnRequestAccepted(int32_t partnerStation) {
    Logger::log("[OverworldMP] Request accepted with station %d (type=%d)\n",
                partnerStation, (int)s_pendingRequestType);

    // Show accept emote balloon above the partner's entity
    overworldMPShowRemoteEmote(partnerStation, EMOTE_ID_LIKES);

    if (s_pendingRequestType == InteractionType::TeamUp) {
        // Team-up needs no UI transition — complete immediately and unlock
        overworldMPTeamUp(partnerStation);
        s_pendingFromStation = -1;
        s_interact.Reset();
        closeInteractionMenu();
    } else {
        // Defer trade/battle start to next frames (avoids opening UI from callback context)
        s_pendingFromStation = -1;
        s_interact.state = InteractState::Accepted;
        s_interact.targetStationIndex = partnerStation;
        s_interact.tradeStartDelay = 0.3f;
    }
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

    // Suppress ALL interactions while in any battle (wild, trainer, team-up, PvP).
    // _updateType != 0 means the field is in a transition or battle scene.
    // Without this, incoming packets (requests, trades) could fire UI actions
    // during a regular wild battle, causing black screen or crashes.
    {
        auto* fmCheck = FieldManager::getClass()->static_fields->_Instance_k__BackingField;
        if (fmCheck != nullptr && fmCheck->fields._updateType != 0 && !overworldMPIsInBattleScene()) {
            return; // In a non-MP battle — skip all interaction processing
        }
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

            // Clear MP battle color override
            {
                extern bool g_owmpBattleColorActive;
                g_owmpBattleColorActive = false;
            }

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

            // Re-apply remote player colors — during the battle, OnEnable may
            // have re-fired on field entities using GetCustomColorSet() with no
            // override, which returns local save data instead of the remote's.
            {
                auto& ctx = getOverworldMPContext();
                for (int i = 0; i < OW_MP_MAX_PLAYERS; i++) {
                    if (ctx.remotePlayers[i].isActive && ctx.remotePlayers[i].isSpawned) {
                        ctx.remotePlayers[i].colorRefreshTimer = 0.1f;
                    }
                }
            }

            // Restore local animation setting after PvP battle
            if (s_pvpSavedWazaeffMode >= 0) {
                auto* localConfig = PlayerWork::get_config();
                if (localConfig != nullptr) {
                    Logger::log("[OverworldMP] Restoring wazaeff_mode: %d -> %d\n",
                                localConfig->fields.wazaeff_mode, s_pvpSavedWazaeffMode);
                    localConfig->fields.wazaeff_mode = s_pvpSavedWazaeffMode;
                }
                s_pvpSavedWazaeffMode = -1;
            }
        }
        return;
    }

    // Second-pass party restore — re-apply battle-modified data in case
    // vanilla FinalizeCoroutine overwrote our deferred restore.
    {
        extern float s_postBattleRestoreTimer;
        if (s_postBattleRestoreTimer > 0.0f) {
            float dt = UnityEngine::Time::get_deltaTime();
            s_postBattleRestoreTimer -= dt;
            if (s_postBattleRestoreTimer <= 0.0f) {
                s_postBattleRestoreTimer = 0.0f;
                extern void applyDeferredPartyRestore();
                applyDeferredPartyRestore();
                Logger::log("[TeamUp] Second-pass party restore applied\n");
            }
        }
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

    // Incoming request pending — wait a few frames before showing dialog
    if (s_interact.state == InteractState::IncomingRequestPending) {
        s_interact.incomingRequestDelay -= 0.01666667f;
        if (s_interact.incomingRequestDelay <= 0.0f) {
            Logger::log("[OverworldMP] Incoming request delay elapsed — showing dialog\n");
            showIncomingRequestDialog();
        }
        return;
    }

    // TradePreview — UIZukanRegister is open, waiting for open animation to finish
    // before showing MsgWindow + Yes/No on top of it
    if (s_interact.state == InteractState::TradePreview) {
        s_interact.incomingRequestDelay -= 0.01666667f;
        if (s_interact.incomingRequestDelay <= 0.0f) {
            Logger::log("[OverworldMP] UIZukanRegister open delay elapsed — showing MsgWindow + Yes/No\n");
            showInteractMsgWindow(s_tradeConfirmMsgBuf);
            s_interactMsgShown = true;
            Logger::log("[OverworldMP] Trade confirm MsgWindow: %s\n", s_tradeConfirmMsgBuf);

            const char* labels[] = { "SS_mp_Yes", "SS_mp_No" };
            if (!openContextMenuFromLabels(MP_MESSAGE_FILE, labels, 2, 1, &onTradeConfirmClicked, &s_tradeConfirmMethodInfo)) {
                Logger::log("[OverworldMP] ERROR: Failed to open trade confirm Yes/No menu\n");
                hideInteractMsgWindow();
                overworldMPSendTradeConfirm(s_tradePartnerStation, false);
                resetTradeState();
                s_interact.Reset();
                closeInteractionMenu();
            } else {
                s_interact.state = InteractState::TradeConfirm;
            }
        }
        return;
    }

    // TradeWarningMsg — "need 2+ pokemon" MsgWindow shown, wait for A/B press to dismiss
    if (s_interact.state == InteractState::TradeWarningMsg) {
        // Grace period: ignore input for the first few frames so a held A/B
        // from the previous menu callback doesn't instantly dismiss this.
        if (s_interact.incomingRequestDelay > 0.0f) {
            s_interact.incomingRequestDelay -= 0.01666667f;
            return;
        }
        static bool s_warningPrevAB = false;
        nn::hid::NpadBaseState padState = {};
        nn::hid::NpadStyleSet styleSet = nn::hid::GetNpadStyleSet(0);
        if (styleSet.isBitSet(nn::hid::NpadStyleTag::NpadStyleFullKey)) {
            nn::hid::GetNpadState((nn::hid::NpadFullKeyState*)&padState, 0);
        } else if (styleSet.isBitSet(nn::hid::NpadStyleTag::NpadStyleJoyDual)) {
            nn::hid::GetNpadState((nn::hid::NpadJoyDualState*)&padState, 0);
        } else if (styleSet.isBitSet(nn::hid::NpadStyleTag::NpadStyleJoyLeft)) {
            nn::hid::GetNpadState((nn::hid::NpadJoyLeftState*)&padState, 0);
        } else if (styleSet.isBitSet(nn::hid::NpadStyleTag::NpadStyleJoyRight)) {
            nn::hid::GetNpadState((nn::hid::NpadJoyRightState*)&padState, 0);
        } else {
            nn::hid::GetNpadState((nn::hid::NpadHandheldState*)&padState, 0x20);
        }
        bool abHeld = padState.mButtons.isBitSet(nn::hid::NpadButton::A) ||
                      padState.mButtons.isBitSet(nn::hid::NpadButton::B);
        bool abPressed = abHeld && !s_warningPrevAB;
        s_warningPrevAB = abHeld;
        if (abPressed) {
            s_warningPrevAB = false;
            Logger::log("[OverworldMP] Trade warning dismissed\n");
            hideInteractMsgWindow();
            s_interact.Reset();
            closeInteractionMenu();
        }
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
        s_tradeWaitTimeout -= 0.01666667f;
        if (s_partnerPokeReceived) {
            Logger::log("[OverworldMP] Partner pokemon received — showing preview\n");
            showTradePreview();
        } else if (s_tradeWaitTimeout <= 0.0f) {
            Logger::log("[OverworldMP] Trade wait timed out — partner never sent pokemon\n");
            overworldMPSendTradeConfirm(s_tradePartnerStation, false);
            resetTradeState();
            s_interact.Reset();
            closeInteractionMenu();
        }
        return;
    }

    // Trade: waiting for partner's final confirmation
    if (s_interact.state == InteractState::TradeWaitFinal) {
        s_tradeWaitTimeout -= 0.01666667f;
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
        } else if (s_tradeWaitTimeout <= 0.0f) {
            Logger::log("[OverworldMP] Trade confirm timed out — partner never confirmed\n");
            resetTradeState();
            s_interact.Reset();
            closeInteractionMenu();
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

    // (TeamUpBattleStarting handler removed — Player B now enters battle via sync-wait + DeferEncountStart)

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

    // Check Y-button press via direct pad read (InputHelper depends on debug UI loop).
    // Use edge detection: only trigger on the frame Y transitions from released to pressed.
    {
        static bool s_prevYHeld = false;
        nn::hid::NpadBaseState padState = {};
        nn::hid::NpadStyleSet styleSet = nn::hid::GetNpadStyleSet(0);
        if (styleSet.isBitSet(nn::hid::NpadStyleTag::NpadStyleFullKey)) {
            nn::hid::GetNpadState((nn::hid::NpadFullKeyState*)&padState, 0);
        } else if (styleSet.isBitSet(nn::hid::NpadStyleTag::NpadStyleJoyDual)) {
            nn::hid::GetNpadState((nn::hid::NpadJoyDualState*)&padState, 0);
        } else if (styleSet.isBitSet(nn::hid::NpadStyleTag::NpadStyleJoyLeft)) {
            nn::hid::GetNpadState((nn::hid::NpadJoyLeftState*)&padState, 0);
        } else if (styleSet.isBitSet(nn::hid::NpadStyleTag::NpadStyleJoyRight)) {
            nn::hid::GetNpadState((nn::hid::NpadJoyRightState*)&padState, 0);
        } else {
            nn::hid::GetNpadState((nn::hid::NpadHandheldState*)&padState, 0x20);
        }
        bool yHeld = padState.mButtons.isBitSet(nn::hid::NpadButton::Y);
        bool yPressed = yHeld && !s_prevYHeld;
        s_prevYHeld = yHeld;
        if (!yPressed) {
            return;
        }
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

// ---------------------------------------------------------------------------
// Partner disconnect handler — called from overworldMPOnPlayerLeave when a
// remote station leaves/disconnects. Cancels any in-progress interaction with
// that station and re-enables player input + closes any open UI.
// ---------------------------------------------------------------------------
void overworldMPOnInteractionPartnerLeft(int32_t stationIndex) {
    // Check if this station is involved in our current interaction
    bool isPartner = (s_interact.targetStationIndex == stationIndex) ||
                     (s_tradePartnerStation == stationIndex) ||
                     (s_battlePartnerStation == stationIndex) ||
                     (s_pendingFromStation == stationIndex);

    if (!isPartner || s_interact.state == InteractState::Idle) return;

    Logger::log("[OverworldMP] Interaction partner station %d disconnected (state=%d) — aborting\n",
                stationIndex, (int)s_interact.state);

    // Don't call SetTradeInfo/ToNextPhase — BoxWindow in selection mode, trade panel uninit
    s_boxWindow = nullptr;

    // Close UIZukanRegister if showing partner pokemon preview
    if (s_zukanRegister != nullptr) {
        s_suppressZukanRegistration = false;
        s_zukanRegister->Close(nullptr, 0);
        s_zukanRegister = nullptr;
    }

    // Hide trade confirm MsgWindow if showing
    if (s_interactMsgShown) {
        hideInteractMsgWindow();
    }

    // Reset all sub-states
    resetTradeState();
    resetBattleState();
    s_pendingFromStation = -1;

    // Re-enable player input and reset interaction state machine
    s_interact.Reset();
    closeInteractionMenu();
}

// ---------------------------------------------------------------------------
// Hook installation (called from exl_overworld_multiplayer_main)
// ---------------------------------------------------------------------------
void exl_overworld_mp_interact_hooks() {
    // Hook ShowRegisterNewMessage to suppress "data added to Pokedex" during trade preview
    UIZukanRegister$$ShowRegisterNewMessage::InstallAtOffset(0x1A3D940);
    // Hook ShowDescription to prevent crash on A-button (Pokedex resources not loaded for foreign pokemon)
    ZukanDescriptionPanel$$ShowDescription::InstallAtOffset(0x1BB4DB0);
    // Hook Dispose to prevent crash on B-button Close (OpClose calls Dispose which crashes)
    ZukanDescriptionPanel$$Dispose::InstallAtOffset(0x1BB3B80);
    // Hook Close to set dismissed flag when user presses B during trade preview
    UIZukanRegister$$Close::InstallAtOffset(0x1A3D220);
}
