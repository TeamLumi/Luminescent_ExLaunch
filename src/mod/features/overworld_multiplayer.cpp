#include "exlaunch.hpp"

#include <cmath>

#include "features/overworld_multiplayer.h"

#include "data/settings.h"
#include "data/utils.h"

#include "externals/AnimationPlayer.h"
#include "externals/BaseEntity.h"
#include "externals/ColorVariation.h"
#include "externals/EntityManager.h"
#include "externals/FieldCharacterEntity.h"
#include "externals/FieldManager.h"
#include "externals/FieldObjectEntity.h"
#include "externals/FieldPlayerEntity.h"
#include "externals/FieldPokemonEntity.h"
#include "externals/GameData/DataManager.h"
#include "externals/DPData/MYSTATUS.h"
#include "externals/PlayerWork.h"
#include "externals/XLSXContent/CharacterDressData.h"
#include "externals/Dpr/Field/Walking/FieldWalkingManager.h"

#include "externals/Dpr/NetworkUtils/NetworkManager.h"
#include "externals/Dpr/SubContents/Utils.h"
#include "externals/SmartPoint/AssetAssistant/Sequencer.h"
#include "externals/System/Action.h"
#include "externals/System/Delegate.h"
#include "externals/System/RuntimeTypeHandle.h"
#include "externals/System/String.h"
#include "externals/System/Type.h"
#include "externals/UnityEngine/_Object.h"
#include "externals/UnityEngine/GameObject.h"
#include "externals/UnityEngine/Time.h"
#include "externals/UnityEngine/Transform.h"

#include "save/save.h"
#include "logger/logger.h"

// ---------------------------------------------------------------------------
// IL2CPP vtable dispatch helpers
// ---------------------------------------------------------------------------
// In IL2CPP, virtual method calls go through the vtable stored in the
// Il2CppClass pointed to by the object's first field. Each vtable entry is
// 0x10 bytes: a function pointer at +0 and a MethodInfo*/RGCTX at +8.
// Ghidra pattern: (**(code**)(*obj + offset))(obj, args, *(*obj + offset + 8))

static inline void il2cpp_vcall_void(void* obj, uint32_t off) {
    uintptr_t k = *(uintptr_t*)obj;
    (*(void(**)(void*, void*))(k + off))(obj, *(void**)(k + off + 8));
}

static inline int32_t il2cpp_vcall_write_byte(void* obj, uint32_t off, uint8_t val) {
    uintptr_t k = *(uintptr_t*)obj;
    return (*(int32_t(**)(void*, uint8_t, void*))(k + off))(obj, val, *(void**)(k + off + 8));
}

static inline int32_t il2cpp_vcall_write_s32(void* obj, uint32_t off, int32_t val) {
    uintptr_t k = *(uintptr_t*)obj;
    return (*(int32_t(**)(void*, int32_t, void*))(k + off))(obj, val, *(void**)(k + off + 8));
}

static inline int32_t il2cpp_vcall_write_fp32(void* obj, uint32_t off, float val) {
    uintptr_t k = *(uintptr_t*)obj;
    return (*(int32_t(**)(void*, float, void*))(k + off))(obj, val, *(void**)(k + off + 8));
}

static inline int32_t il2cpp_vcall_read_out(void* obj, uint32_t off, void* out) {
    uintptr_t k = *(uintptr_t*)obj;
    return (*(int32_t(**)(void*, void*, void*))(k + off))(obj, out, *(void**)(k + off + 8));
}

static inline int32_t il2cpp_vcall_int(void* obj, uint32_t off) {
    uintptr_t k = *(uintptr_t*)obj;
    return (*(int32_t(**)(void*, void*))(k + off))(obj, *(void**)(k + off + 8));
}

// ---------------------------------------------------------------------------
// PacketWriter/PacketReader vtable offsets (verified against Ghidra output
// of ANetData<PosZoneData> and NetDataParser.Parse)
// ---------------------------------------------------------------------------
// PacketWriter (IlcaNetPacket base + PacketWriter virtuals)
static constexpr uint32_t PW_RESET      = 0x1b0;  // IlcaNetPacket.Reset()
static constexpr uint32_t PW_WRITE_BYTE = 0x270;  // PacketWriter.WriteByte(byte)
static constexpr uint32_t PW_WRITE_S32  = 0x2D0;  // PacketWriter.WriteS32(int)
static constexpr uint32_t PW_WRITE_FP32 = 0x320;  // PacketWriter.WriteFP32(float)

// PacketReader (IlcaNetPacket base + PacketReader virtuals)
static constexpr uint32_t PR_FROM_STATION  = 0x260;  // PacketReader.FromStationIndex()
static constexpr uint32_t PR_READ_BYTE_OUT = 0x2B0;  // PacketReader.ReadByteOut(out byte)
static constexpr uint32_t PR_READ_S32_OUT  = 0x370;  // PacketReader.ReadS32Out(out int)
static constexpr uint32_t PR_READ_FP32_OUT = 0x410;  // PacketReader.ReadFP32Out(out float)

// PokePara full data size (core 328 + calc 16 = 344 bytes)
static constexpr int32_t POKE_FULL_DATA_SIZE = 344;

// Custom packet DataID for overworld multiplayer position (0xC0, avoids existing 2-67 range)
// New DataIDs (0xC1-0xC3) are defined in the header alongside position (0xC0).
static constexpr uint8_t OWMP_DATA_ID = 0xC0;

// ---------------------------------------------------------------------------
// IlcaNetComponent / NexAssets.Common TypeInfo addresses (from Ghidra)
// ---------------------------------------------------------------------------
// IlcaNetComponent inherits from NexAssets.Common (MonoBehaviour). Its Update()
// calls Common.Update() which ticks the NEX/PIA framework every frame via
// NexPlugin.Common.Dispatch(). Without this per-frame tick, PIA internals
// never process and the first PiaPlugin::Dispatch crashes.
//
// Common.Update() has an early-return guard: if (s_updateAlive == false) return;
// This static bool must be true for NEX processing to happen.

// Il2CppType* for IlcaNetComponent — used with RuntimeTypeHandle to get System.Type
static constexpr long ILCANETCOMP_TYPE_VAR = 0x04c543d8;

// NexAssets.Common Il2CppClass* — holds s_updateAlive in static_fields+0xC0
static constexpr long NEXASSETS_COMMON_TYPEINFO = 0x04c604b8;

// ---------------------------------------------------------------------------
// Global state
// ---------------------------------------------------------------------------

// The receive callback uses a stub delegate (il2cpp_object_new, no .ctor) stored in
// NM static_fields. The actual packet handling is done by hooking
// ReceivePacketCallback.Invoke (0x1BC5620) — the hook intercepts before delegate
// dispatch, calling our handler directly. Asset-load callbacks still use
// System::Action with direct field assignment.

static OverworldMPContext s_mpContext;

// Forward declaration — defined after global state section
static Dpr::NetworkUtils::NetworkManager::Object* getNMInstance();

// Forward declarations for follow pokemon spawn/despawn
static void overworldMPSpawnFollowPokemon(int32_t stationIndex);
static void overworldMPDespawnFollowPokemon(int32_t stationIndex);

// Spawn queue state — declared early for use in overworldMPUpdate().
// The actual queue array and helper functions are defined later in the entity spawn section.
static bool    s_spawnInFlight = false;
static bool    s_pokeSpawnInFlight = false;

// ---------------------------------------------------------------------------
// NM GameObject deactivation for zone change crash prevention
// ---------------------------------------------------------------------------
// Three rounds of testing proved that blocking individual PIA tick paths
// (IlcaNetComponent.Update, NM.OnUpdate, NM.OnLateUpdate) does NOT prevent
// the crash. The crash is in native Unity code (0x372014) during FM.Update
// Orig() — something on the NM's DontDestroyOnLoad GameObject causes a null
// deref ~1s after zone transition.
//
// Nuclear option: deactivate the entire NM GameObject during grace period.
// GameObject.SetActive(false) stops ALL Unity processing on all components:
// - MonoBehaviour Update/LateUpdate/FixedUpdate (native dispatch)
// - Coroutine processing (Unity halts coroutines on inactive GOs)
// - Rendering callbacks (OnRenderObject, OnWillRenderObject, etc.)
// - Any other lifecycle callbacks
//
// The Sequencer subscriptions (NM.OnUpdate/OnLateUpdate) are still blocked
// by our trampoline hooks as a belt-and-suspenders measure.
static void* s_ilcaNetComponent = nullptr;
static UnityEngine::GameObject::Object* s_nmGameObject = nullptr;

// Diagnostic counter for IlcaNetComponent.Update hook — tracks ticks after re-enable
static int32_t s_ilcaNetUpdateLogCount = 0;

static void setNMGameObjectActive(bool active) {
    if (s_nmGameObject != nullptr) {
        bool wasPreviouslyActive = s_nmGameObject->get_activeSelf();
        s_nmGameObject->SetActive(active);
        Logger::log("[OverworldMP] NM GameObject %s (was %s, ptr=%p)\n",
                    active ? "ACTIVATED" : "DEACTIVATED",
                    wasPreviouslyActive ? "active" : "inactive",
                    s_nmGameObject);
    } else {
        // Fallback: try to get GO from NM instance
        auto* nm = getNMInstance();
        if (nm != nullptr) {
            // NM inherits MonoBehaviour → Component, use Component.get_gameObject
            auto* go = _ILExternal::external<UnityEngine::GameObject::Object*>(0x026a8170, nm);
            if (go != nullptr) {
                s_nmGameObject = go;
                go->SetActive(active);
                Logger::log("[OverworldMP] NM GameObject %s (resolved from NM instance, ptr=%p)\n",
                            active ? "ACTIVATED" : "DEACTIVATED", go);
                return;
            }
        }
        Logger::log("[OverworldMP] WARNING: setNMGameObjectActive(%d) but no GO found!\n",
                    (int)active);
    }
}

// Legacy helper — kept for compatibility but no longer the primary mechanism
static void setIlcaNetComponentEnabled(bool enabled) {
    if (s_ilcaNetComponent != nullptr) {
        _ILExternal::external<void>(0x26A3450, s_ilcaNetComponent, enabled);
        if (enabled) {
            s_ilcaNetUpdateLogCount = 1;
        }
    }
}

// ---------------------------------------------------------------------------
// Interaction state
// ---------------------------------------------------------------------------
static InteractionState s_interactionState = InteractionState::None;
static int32_t s_interactionTarget = -1;  // station index of interaction partner
static float s_interactionTimeoutTime = 0.0f;  // countdown for request timeout (seconds)
static InteractionType s_pendingInteractionType = InteractionType::Trade;  // type of pending request (sent or received)

// NM.Start() is called manually after AddComponent since Unity's non-generic
// AddComponent(Type) doesn't trigger Start(). The NM.Start hook logs all calls.

OverworldMPContext& getOverworldMPContext() {
    return s_mpContext;
}

bool isOverworldMPActive() {
    return s_mpContext.state == OverworldMPState::Connected ||
           s_mpContext.state == OverworldMPState::Searching;
}

// Battle scene flag — when true, the ReceivePacketCallback.Invoke hook
// skips reading from the PacketReader so the battle system's ExCallback
// gets it intact (both callbacks share the same PacketReader object).
static bool s_inBattleScene = false;

// Battle party accumulation state — for chunked 0xC6 protocol.
// The PIA PacketWriter has a ~340-byte user data limit, but a full party
// is 6×344 + MYSTATUS + overhead ≈ 2100 bytes. So we send one Pokemon
// per packet and accumulate on the receive side.
static uint8_t s_accumBuf[6 * 344 + 64] = {};
static int32_t s_accumBufSize = 0;
static uint8_t s_accumMemberCount = 0;
static uint8_t s_accumReceivedCount = 0;
static int32_t s_accumFromStation = -1;

// Cached Net::Client pointer — set from UpdateInitialize when mainModule+0x118
// becomes non-null. Used as fallback in get_Instance hook when the native
// singleton chain (BattleProc.Instance.mainModule.m_iPtrNetClient) returns null.
// This fixes determine_server: the battle sends ServerVersion packets fine
// (sendToServerVersionCoreAll reads netClient directly from mainModule+0x118),
// but received packets are dropped because OnReceivePacketExStatic calls
// get_Instance() which traverses the singleton chain and gets null.
static void* s_cachedBattleNetClient = nullptr;

void overworldMPSetInBattleScene(bool inBattle) {
    s_inBattleScene = inBattle;
    if (!inBattle) {
        s_cachedBattleNetClient = nullptr;  // Clear cache when leaving battle
    }
    Logger::log("[OverworldMP] Battle scene flag: %d\n", (int)inBattle);
}

bool overworldMPIsInBattleScene() {
    return s_inBattleScene;
}

// ---------------------------------------------------------------------------
// NM init-wait helpers
// ---------------------------------------------------------------------------
// After ensureNetworkManagerSingleton(), NM.Start() runs on the next frame
// and creates the IE_Start coroutine. We poll for IE_Start completion by
// checking SC.callObjPtr (set during SC.Initialize inside the coroutine).

static float s_initWaitTime = 0.0f;

// NM.OnUpdate readiness flag — set true on first invocation of NM.OnUpdate,
// proving IE_Start completed and Sequencer subscriptions are active.
static bool s_nmOnUpdateReady = false;

// Guard against duplicate NM.Start() calls (manual + Unity lifecycle)
static void* s_nmStartedInstance = nullptr;

// PIA state machine monitoring — only logs on state CHANGE to avoid spam
static uint32_t s_lastLoggedPiaState = 0xFFFFFFFF;
static int32_t s_searchingFrameCount = 0;

// Session error grace — tolerate transient PIA error states (e.g., brief lag spikes
// or individual player disconnects). When one player disconnects, PIA briefly reports
// GamingError to remaining players before processing the leave event and recovering.
// We use a long grace period to ride through this, and only stop if the session state
// stays bad AND we have no active peers left (truly dead session).
static constexpr float OW_MP_ERROR_GRACE_SEC = 3.0f;  // 3 seconds
static float s_errorGraceTime = 0.0f;

// Zone change grace period — after a zone transition, game systems (Sequencer,
// EntityManager, asset loader) need time to stabilize. During grace time,
// all MP processing (position sends, spawning) is deferred.
static float s_zoneChangeGraceTime = 0.0f;

// Position sync accumulator (time-based, replaces frame counter modulo)
static float s_posSyncAccumulator = 0.0f;

// Diagnostic logging accumulator (log every ~5 seconds when connected)
static float s_diagnosticAccumulator = 0.0f;

// (s_pendingCleanupEntities removed — entities are no longer DDOL,
// Unity destroys them during scene transitions)

static Dpr::NetworkUtils::NetworkManager::Object* getNMInstance() {
    auto* nmKlass = (Il2CppClass*)Dpr::NetworkUtils::NetworkManager::getClass();
    auto* parentKlass = nmKlass->_1.parent;
    if (parentKlass != nullptr && parentKlass->static_fields != nullptr)
        return (Dpr::NetworkUtils::NetworkManager::Object*)(*(void**)parentKlass->static_fields);
    return nullptr;
}

// ---------------------------------------------------------------------------
// Receive callback
// ---------------------------------------------------------------------------
// Called by ReceivePacketCallback::Invoke for every packet received on the
// Station transport. Invoke checks MethodInfo.parameters_count (offset 0x4A):
// when parameters_count == 1, it dispatches as 2 args:
//   method_ptr(PacketReader*, MethodInfo*)
// (No m_target arg — confirmed by Ghidra Invoke @ 0x1BC5620, LAB_5674 path)

static int32_t s_recvCallbackCount = 0;
static bool s_loggedFirstSend = false;

static void onOverworldMPReceivePacket(void* pr, void* /*method*/) {
    s_recvCallbackCount++;
    if (s_recvCallbackCount <= 5) {
        Logger::log("[OverworldMP] Recv callback #%d: pr=%p active=%d\n",
                    s_recvCallbackCount, pr, (int)isOverworldMPActive());
    }
    if (!isOverworldMPActive() || pr == nullptr) return;

    // Read DataID (first byte of every packet)
    uint8_t dataId = 0;
    il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &dataId);
    if (s_recvCallbackCount <= 10) {
        Logger::log("[OverworldMP] Recv callback #%d: dataId=0x%02X\n",
                    s_recvCallbackCount, (int)dataId);
    }

    // Dispatch based on DataID
    switch (dataId) {

    // -----------------------------------------------------------------------
    // 0xC0: Position/avatar broadcast
    // -----------------------------------------------------------------------
    case OWMP_DATA_ID_POSITION: {
        int32_t fromStation = il2cpp_vcall_int(pr, PR_FROM_STATION);
        if (fromStation < 0 || fromStation >= OW_MP_MAX_PLAYERS) return;

        // Read payload: posX, posY, posZ, rotY, areaID, avatarId, colorId
        float posX = 0, posY = 0, posZ = 0, rotY = 0;
        int32_t areaID = 0;
        int32_t avatarId = 0;
        int32_t colorId = 0;
        il2cpp_vcall_read_out(pr, PR_READ_FP32_OUT, &posX);
        il2cpp_vcall_read_out(pr, PR_READ_FP32_OUT, &posY);
        il2cpp_vcall_read_out(pr, PR_READ_FP32_OUT, &posZ);
        il2cpp_vcall_read_out(pr, PR_READ_FP32_OUT, &rotY);
        il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &areaID);
        il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &avatarId);
        il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &colorId);

        auto& remote = s_mpContext.remotePlayers[fromStation];
        if (!remote.isActive) return;

        // Detect movement and speed from position delta between packets.
        float dx = posX - remote.prevPosX;
        float dz = posZ - remote.prevPosZ;
        float distSq = dx * dx + dz * dz;
        remote.isMoving = distSq > 0.0001f;
        remote.isRunning = distSq > 0.12f;
        remote.prevPosX = posX;
        remote.prevPosZ = posZ;

        // Update remote player data
        remote.position.fields.x = posX;
        remote.position.fields.y = posY;
        remote.position.fields.z = posZ;
        remote.rotationY = rotY;
        remote.avatarId = avatarId;
        remote.colorId = colorId;

        // Read follow pokemon data
        uint8_t hasFollowPoke = 0;
        il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &hasFollowPoke);

        if (hasFollowPoke) {
            int32_t oldMonsNo = remote.followMonsNo;
            il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &remote.followMonsNo);
            il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &remote.followFormNo);
            il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &remote.followSex);
            uint8_t rare = 0;
            il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &rare);
            remote.followIsRare = (rare != 0);
            remote.followPokeActive = true;

            // If pokemon species changed, despawn old one so a new one spawns
            if (oldMonsNo != 0 && oldMonsNo != remote.followMonsNo && remote.followPokeSpawned) {
                overworldMPDespawnFollowPokemon(fromStation);
            }
        } else {
            if (remote.followPokeActive && remote.followPokeSpawned) {
                overworldMPDespawnFollowPokemon(fromStation);
            }
            remote.followPokeActive = false;
            remote.followMonsNo = 0;
        }

        // Read player name (UTF-16 chars)
        uint8_t nameLen = 0;
        il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &nameLen);
        if (nameLen > 0 && nameLen <= 12) {
            uint8_t nameBytes[28]; // max 12 chars * 2 bytes + padding
            for (int i = 0; i < nameLen * 2; i++) {
                il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &nameBytes[i]);
            }
            // Only allocate the managed string once to avoid GC churn at 20Hz
            if (remote.playerName == nullptr) {
                remote.playerName = System::String::fromUnicodeBytes(nameBytes, nameLen * 2);
                Logger::log("[OverworldMP] Remote %d name: len=%d\n", fromStation, (int)nameLen);
            }
        }

        int32_t oldArea = remote.areaID;
        remote.areaID = areaID;

        // Handle area change: spawn/despawn based on whether remote is in our area.
        // Skip during zone change grace period — game systems are unstable.
        if (oldArea != areaID && s_zoneChangeGraceTime <= 0.0f) {
            Logger::log("[OverworldMP] Remote %d area change: %d -> %d\n",
                        fromStation, oldArea, areaID);

            if (remote.isSpawned && areaID != s_mpContext.myAreaID) {
                overworldMPDespawnEntity(fromStation);
            } else if (!remote.isSpawned && areaID == s_mpContext.myAreaID) {
                overworldMPSpawnEntity(fromStation);
            }
        }
        break;
    }

    // -----------------------------------------------------------------------
    // 0xC1: Emote broadcast
    // -----------------------------------------------------------------------
    case OWMP_DATA_ID_EMOTE: {
        int32_t fromStation = il2cpp_vcall_int(pr, PR_FROM_STATION);
        if (fromStation < 0 || fromStation >= OW_MP_MAX_PLAYERS) return;

        uint8_t emoteId = 0;
        il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &emoteId);

        Logger::log("[OverworldMP] Received emote from station %d: emoteId=%d\n",
                    fromStation, (int)emoteId);

        overworldMPShowRemoteEmote(fromStation, emoteId);
        break;
    }

    // -----------------------------------------------------------------------
    // 0xC2: Interaction request (targeted)
    // -----------------------------------------------------------------------
    case OWMP_DATA_ID_INTERACT_REQ: {
        int32_t fromStation = il2cpp_vcall_int(pr, PR_FROM_STATION);
        if (fromStation < 0 || fromStation >= OW_MP_MAX_PLAYERS) return;

        int32_t targetStation = 0;
        uint8_t interactType = 0;
        uint8_t subtype = 0;
        il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &targetStation);
        il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &interactType);
        il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &subtype);

        Logger::log("[OverworldMP] Received interaction request from station %d: target=%d type=%d subtype=%d\n",
                    fromStation, targetStation, (int)interactType, (int)subtype);

        // Only process if we're the target and not already in an interaction
        // targetStation is the sender's local station index for us — but since
        // packets are broadcast, we check if it matches our station.
        // For now, accept any request directed at us (TODO: proper station matching).
        if (s_interactionState == InteractionState::None) {
            s_interactionState = InteractionState::ReceivedRequest;
            s_interactionTarget = fromStation;
            s_pendingInteractionType = (InteractionType)interactType;
            Logger::log("[OverworldMP] Interaction request received — showing dialog (from station %d, type=%d, subtype=%d)\n",
                        fromStation, (int)interactType, (int)subtype);
            overworldMPShowIncomingRequestDialog(fromStation, (InteractionType)interactType, (BattleSubtype)subtype);
        } else {
            Logger::log("[OverworldMP] Ignoring interaction request — already in state %d\n",
                        (int)s_interactionState);
        }
        break;
    }

    // -----------------------------------------------------------------------
    // 0xC3: Interaction response (targeted)
    // -----------------------------------------------------------------------
    case OWMP_DATA_ID_INTERACT_RESP: {
        int32_t fromStation = il2cpp_vcall_int(pr, PR_FROM_STATION);
        if (fromStation < 0 || fromStation >= OW_MP_MAX_PLAYERS) return;

        int32_t targetStation = 0;
        uint8_t accepted = 0;
        il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &targetStation);
        il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &accepted);

        Logger::log("[OverworldMP] Received interaction response from station %d: target=%d accepted=%d\n",
                    fromStation, targetStation, (int)accepted);

        // Only process if we're waiting for a response from this station
        if (s_interactionState == InteractionState::WaitingResponse &&
            s_interactionTarget == fromStation) {
            // Reset network-level interaction state BEFORE calling handler —
            // prevents timeout from firing after accept/decline is processed
            s_interactionState = InteractionState::None;
            s_interactionTarget = -1;
            s_interactionTimeoutTime = 0.0f;

            if (accepted) {
                Logger::log("[OverworldMP] Interaction ACCEPTED by station %d\n", fromStation);
                overworldMPOnRequestAccepted(fromStation);
            } else {
                Logger::log("[OverworldMP] Interaction REJECTED by station %d\n", fromStation);
                overworldMPOnRequestDeclined(fromStation);
            }
        } else {
            Logger::log("[OverworldMP] Ignoring interaction response — state=%d target=%d\n",
                        (int)s_interactionState, s_interactionTarget);
        }
        break;
    }

    // -----------------------------------------------------------------------
    // 0xC4: Trade pokemon data (targeted)
    // -----------------------------------------------------------------------
    case OWMP_DATA_ID_TRADE_POKE: {
        int32_t fromStation = il2cpp_vcall_int(pr, PR_FROM_STATION);
        if (fromStation < 0 || fromStation >= OW_MP_MAX_PLAYERS) return;

        int32_t targetStation = 0;
        int32_t partySlot = 0;
        il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &targetStation);

        // Read partySlot as a single byte
        uint8_t slotByte = 0;
        il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &slotByte);
        partySlot = (int32_t)slotByte;

        // Read 344 bytes of pokemon data (86 x int32 = 344 bytes)
        uint8_t pokeData[344];
        for (int i = 0; i < 86; i++) {
            int32_t val = 0;
            il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &val);
            memcpy(&pokeData[i * 4], &val, 4);
        }

        Logger::log("[OverworldMP] Received trade pokemon from station %d: slot=%d\n",
                    fromStation, partySlot);

        overworldMPOnTradePokeReceived(fromStation, partySlot, pokeData, 344);
        break;
    }

    // -----------------------------------------------------------------------
    // 0xC5: Trade confirmation (targeted)
    // -----------------------------------------------------------------------
    case OWMP_DATA_ID_TRADE_CONFIRM: {
        int32_t fromStation = il2cpp_vcall_int(pr, PR_FROM_STATION);
        if (fromStation < 0 || fromStation >= OW_MP_MAX_PLAYERS) return;

        int32_t targetStation = 0;
        uint8_t confirmed = 0;
        il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &targetStation);
        il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &confirmed);

        Logger::log("[OverworldMP] Received trade confirm from station %d: confirmed=%d\n",
                    fromStation, (int)confirmed);

        overworldMPOnTradeConfirmReceived(fromStation, confirmed != 0);
        break;
    }

    // -----------------------------------------------------------------------
    // 0xC6: Battle party data (targeted, chunked protocol)
    // The full party (2100+ bytes) exceeds PIA's PacketWriter limit (~340 bytes).
    // Data arrives as: 1 HEADER packet + N POKE packets (one per Pokemon).
    // We accumulate into s_accumBuf, then call overworldMPOnBattlePartyReceived
    // once all Pokemon have arrived.
    // -----------------------------------------------------------------------
    case OWMP_DATA_ID_BATTLE_PARTY: {
        int32_t fromStation = il2cpp_vcall_int(pr, PR_FROM_STATION);
        if (fromStation < 0 || fromStation >= OW_MP_MAX_PLAYERS) return;

        int32_t targetStation = 0;
        il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &targetStation);

        uint8_t subType = 0;
        il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &subType);

        switch (subType) {
        // ---- HEADER: memberCount + subtype + MYSTATUS_COMM ----
        case BATTLE_PARTY_SUB_HEADER: {
            uint8_t memberCount = 0;
            il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &memberCount);
            uint8_t btlSubtype = 0;
            il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &btlSubtype);

            Logger::log("[OverworldMP] Battle HEADER from station %d: members=%d subtype=%d\n",
                        fromStation, (int)memberCount, (int)btlSubtype);

            // Initialize accumulation
            s_accumFromStation = fromStation;
            s_accumMemberCount = (memberCount > 6) ? 6 : memberCount;
            s_accumReceivedCount = 0;
            memset(s_accumBuf, 0, sizeof(s_accumBuf));
            s_accumBuf[0] = s_accumMemberCount;

            // Read MYSTATUS_COMM into buffer at the correct offset
            // (after memberCount byte + all Pokemon data slots)
            int32_t mystOffset = 1 + s_accumMemberCount * POKE_FULL_DATA_SIZE;

            // id (4 bytes as int32)
            int32_t statusId = 0;
            il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &statusId);
            if (mystOffset + 4 <= (int32_t)sizeof(s_accumBuf)) {
                memcpy(&s_accumBuf[mystOffset], &statusId, 4);
                mystOffset += 4;
            }

            // nameLen (1 byte)
            uint8_t nameLen = 0;
            il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &nameLen);
            if (mystOffset < (int32_t)sizeof(s_accumBuf)) s_accumBuf[mystOffset++] = nameLen;

            // name chars (nameLen * 2 bytes)
            for (int nc = 0; nc < nameLen * 2; nc++) {
                uint8_t ch = 0;
                il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &ch);
                if (mystOffset < (int32_t)sizeof(s_accumBuf)) s_accumBuf[mystOffset++] = ch;
            }

            // sex, lang, fashion, body_type, hat, shoes (6 bytes)
            for (int fi = 0; fi < 6; fi++) {
                uint8_t val = 0;
                il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &val);
                if (mystOffset < (int32_t)sizeof(s_accumBuf)) s_accumBuf[mystOffset++] = val;
            }

            s_accumBufSize = mystOffset;

            Logger::log("[OverworldMP] Battle HEADER stored: %d bytes (MYSTATUS at offset %d)\n",
                        s_accumBufSize, 1 + s_accumMemberCount * POKE_FULL_DATA_SIZE);

            // If memberCount is 0, signal completion immediately
            if (s_accumMemberCount == 0) {
                overworldMPOnBattlePartyReceived(fromStation, s_accumBuf, s_accumBufSize);
            }
            break;
        }

        // ---- POKE: single Pokemon data (pokeIndex + 86 × int32) ----
        case BATTLE_PARTY_SUB_POKE: {
            uint8_t pokeIndex = 0;
            il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &pokeIndex);

            if (pokeIndex >= 6) {
                Logger::log("[OverworldMP] Invalid poke index %d — ignoring\n", (int)pokeIndex);
                break;
            }

            // Read 86 int32s into accumulation buffer at the correct offset
            int32_t bufOffset = 1 + pokeIndex * POKE_FULL_DATA_SIZE;
            for (int j = 0; j < 86; j++) {
                int32_t val = 0;
                il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &val);
                if (bufOffset + 4 <= (int32_t)sizeof(s_accumBuf)) {
                    memcpy(&s_accumBuf[bufOffset], &val, 4);
                    bufOffset += 4;
                }
            }

            s_accumReceivedCount++;

            // Log personalRnd for corruption detection
            {
                int32_t pokeStart = 1 + pokeIndex * POKE_FULL_DATA_SIZE;
                uint32_t rnd = 0;
                memcpy(&rnd, &s_accumBuf[pokeStart], 4);
                Logger::log("[OverworldMP] Battle POKE[%d] received: personalRnd=0x%08x (%d/%d)\n",
                            (int)pokeIndex, rnd,
                            (int)s_accumReceivedCount, (int)s_accumMemberCount);
            }

            // Check if all Pokemon have arrived
            if (s_accumReceivedCount >= s_accumMemberCount && s_accumMemberCount > 0) {
                Logger::log("[OverworldMP] All %d battle Pokemon received — notifying state machine\n",
                            (int)s_accumMemberCount);
                overworldMPOnBattlePartyReceived(s_accumFromStation, s_accumBuf, s_accumBufSize);
            }
            break;
        }

        default:
            Logger::log("[OverworldMP] Unknown 0xC6 sub-type: %d\n", (int)subType);
            break;
        }
        break;
    }

    // -----------------------------------------------------------------------
    // 0xC7: Battle ready sync (targeted)
    // -----------------------------------------------------------------------
    case OWMP_DATA_ID_BATTLE_READY: {
        int32_t fromStation = il2cpp_vcall_int(pr, PR_FROM_STATION);
        if (fromStation < 0 || fromStation >= OW_MP_MAX_PLAYERS) return;

        int32_t targetStation = 0;
        il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &targetStation);

        int32_t myStation = _ILExternal::external<int32_t>(0x23BC000); // ThisStationIndex
        if (targetStation != myStation) return; // not for us

        Logger::log("[OverworldMP] Received BATTLE_READY from station %d\n", fromStation);
        overworldMPOnBattleReadyReceived(fromStation);
        break;
    }

    default:
        // Not our packet — ignore silently
        break;
    }
}

// Register/unregister the receive callback on NetworkManager's static fields.
// NetworkManager.onReceivePacket is a ReceivePacketCallback delegate stored in
// static_fields[0]. When set, CheckReceivePacketImpl invokes it for each packet
// received on the Station transport.
//
// Strategy: allocate a GC-safe stub delegate via il2cpp_object_new (non-null,
// valid klass for GC scanning) but DON'T call .ctor. The delegate's internal
// fields (invoke_impl, method_ptr) are unused because our trampoline hook on
// ReceivePacketCallback.Invoke (0x1BC5620) intercepts the call before any
// delegate dispatch happens. This bypasses the invoke_impl thunk selection
// that failed in 3 previous attempts.

static void registerReceiveCallback() {
    // Store a non-null managed object in static_fields->onReceivePacket so
    // CheckReceivePacketImpl enters the callback branch (param_4 != 0) and
    // calls ReceivePacketCallback.Invoke(). Our trampoline hook on Invoke
    // (0x1BC5620) intercepts BEFORE the delegate body runs, so the object
    // doesn't need to be a real ReceivePacketCallback — just non-null and
    // GC-safe (valid klass pointer on the managed heap).
    //
    // We use the NM instance itself: it's always available at this point,
    // lives on the managed heap, and has a valid klass. This avoids the
    // ReceivePacketCallback_TypeInfo not being loaded yet (0x04C5E0F0 was
    // null when registerReceiveCallback runs after DoStartSession).
    auto* nm = getNMInstance();
    if (nm == nullptr) {
        Logger::log("[OverworldMP] ERROR: Cannot register callback — NM instance is null\n");
        return;
    }

    auto* klass = Dpr::NetworkUtils::NetworkManager::getClass();
    klass->static_fields->onReceivePacket = nm;

    Logger::log("[OverworldMP] Receive callback registered (NM stub=%p)\n", nm);
}

static void unregisterReceiveCallback() {
    auto* klass = Dpr::NetworkUtils::NetworkManager::getClass();
    klass->static_fields->onReceivePacket = nullptr;  // static_fields[0]
    Logger::log("[OverworldMP] Receive callback unregistered\n");
}

// ---------------------------------------------------------------------------
// NetworkManager singleton creation
// ---------------------------------------------------------------------------
// NetworkManager is a SingletonMonoBehaviour<NetworkManager> that normally
// only exists in underground/union room scenes. To use networking in the
// overworld, we create the singleton ourselves by adding a NetworkManager
// component to a new DontDestroyOnLoad GameObject. The AddComponent call
// triggers Unity's MonoBehaviour lifecycle:
//   1. IL2CPP .ctor @ 0x1DEB8D0 (creates SessionConnector, PacketWriter, etc.)
//   2. SingletonMonoBehaviour.Awake @ 0x02323170 (sets _Instance on parent class)
// After this, all static methods (StartSessionRandomJoin, etc.) can use the singleton.
// NOTE: _Instance lives on SingletonMonoBehaviour<NM>'s static_fields, NOT on
// NetworkManager's own static_fields (which only contain delegate callbacks).

static bool ensureNetworkManagerSingleton() {
    auto* nmKlass = Dpr::NetworkUtils::NetworkManager::getClass();
    nmKlass->initIfNeeded();

    // Already exists — capture GO pointer and return
    auto* existing = getNMInstance();
    if (existing != nullptr) {
        Logger::log("[OverworldMP] NetworkManager singleton already exists at %p\n", existing);

        // Capture the GO pointer if we don't have it yet
        if (s_nmGameObject == nullptr) {
            auto* go = _ILExternal::external<UnityEngine::GameObject::Object*>(0x026a8170, existing);
            s_nmGameObject = go;
            Logger::log("[OverworldMP] NM GameObject captured: %p\n", go);
        }

        // Diagnostic: check Common.s_updateAlive on the existing NM
        auto* commonKlass = *(Il2CppClass**)exl::util::modules::GetTargetOffset(NEXASSETS_COMMON_TYPEINFO);
        if (commonKlass != nullptr) {
            auto* staticFields = *(void**)((uintptr_t)commonKlass + 0xb8);
            if (staticFields != nullptr) {
                bool alive = *(bool*)((uintptr_t)staticFields + 0xC0);
                Logger::log("[OverworldMP] Common.s_updateAlive = %d\n", (int)alive);
            }
        }
        return true;
    }

    Logger::log("[OverworldMP] Creating NetworkManager singleton...\n");

    // 1. Create a new GameObject
    auto* goKlass = UnityEngine::GameObject::getClass();
    goKlass->initIfNeeded();
    auto* go = (UnityEngine::GameObject::Object*)il2cpp_object_new((Il2CppClass*)goKlass);
    go->ctor(System::String::Create("NetworkManager"));
    s_nmGameObject = go;  // Store for deactivation during zone transitions
    Logger::log("[OverworldMP] GameObject created at %p (stored for zone change deactivation)\n", go);

    // 2. Mark it as persistent across scene loads
    UnityEngine::_Object::DontDestroyOnLoad(go->cast<UnityEngine::_Object>());

    // 3. Get System.Type for NetworkManager from its Il2CppClass
    System::RuntimeTypeHandle::Object handle {};
    handle.fields.value = &((Il2CppClass*)nmKlass)->_1.byval_arg;
    auto* nmType = System::Type::GetTypeFromHandle(handle);
    Logger::log("[OverworldMP] NM Type object: %p\n", nmType);

    // 4. Call non-generic AddComponent(Type) — triggers .ctor.
    //    Unity will queue Start() to be called on the next frame.
    //    We let Start() run normally — it creates the IE_Start coroutine which
    //    initializes everything (SC delegates, Sequencer registration, etc.).
    //    ShowMessageWindow.Setup is hooked to handle NM+0x98 being null.
    auto* component = go->AddComponentByType(nmType);
    Logger::log("[OverworldMP] AddComponentByType returned: %p\n", component);

    if (component == nullptr) {
        Logger::log("[OverworldMP] ERROR: AddComponent returned null\n");
        return false;
    }

    // 5. Set the singleton manually — SingletonMonoBehaviour<T>.Awake() doesn't
    //    fire reliably for runtime-created components via AddComponent(Type).
    //    The .ctor has already run (SessionConnector, PacketWriter, etc. created).
    //
    //    _Instance lives on the PARENT class (SingletonMonoBehaviour<NM>), NOT on
    //    NetworkManager's own static_fields (which only hold delegate callbacks).
    auto* parentKlass = ((Il2CppClass*)nmKlass)->_1.parent;
    if (parentKlass != nullptr && parentKlass->static_fields != nullptr) {
        *(void**)parentKlass->static_fields = component;
        Logger::log("[OverworldMP] Singleton set on parent SBMB<NM> (%p)\n",
                    parentKlass->static_fields);
    } else {
        Logger::log("[OverworldMP] WARNING: parent class static_fields is null\n");
    }

    auto* nm = (Dpr::NetworkUtils::NetworkManager::Object*)component;
    Logger::log("[OverworldMP] _sessionConnector: %p\n", nm->fields._sessionConnector);

    // 6. Manually call NM.Start() — Unity's non-generic AddComponent(Type)
    //    doesn't reliably trigger Start(). NM.Start creates the IE_Start
    //    coroutine and registers it with the Sequencer. IE_Start waits for
    //    Fader/DataManager/MessageManager (all ready in the overworld), then
    //    does the full init: SC.Initialize, Sequencer registration, etc.
    //    ShowMessageWindow.Setup is null-guarded by our hook (NM+0x98 is null).
    //
    //    Reset readiness flags before Start — the NM.OnUpdate hook will set
    //    s_nmOnUpdateReady=true on first invocation, proving IE_Start completed.
    //    s_nmStartedInstance is set so the Start hook blocks the duplicate
    //    Unity lifecycle call for the same instance.
    s_nmOnUpdateReady = false;
    s_nmStartedInstance = nullptr;
    Logger::log("[OverworldMP] Manually calling NM.Start()...\n");
    Dpr::NetworkUtils::NetworkManager::external<void>(0x1DE62F0, nm);
    Logger::log("[OverworldMP] NM.Start() returned, IE_Start coroutine queued\n");

    // -----------------------------------------------------------------
    // Phase 2: Ensure IlcaNetComponent is on the same GameObject
    // -----------------------------------------------------------------
    // The base game's NetworkManager prefab has [RequireComponent(typeof(IlcaNetComponent))].
    // IlcaNetComponent.Update() → Common.Update() → NexPlugin.Common.Dispatch(), which is
    // the per-frame NEX/PIA tick. Without it, PIA state machine never processes and the
    // first PiaPlugin::Dispatch from IlcaNetSession.UpdateDispatchCore crashes.
    //
    // RequireComponent *should* auto-add IlcaNetComponent when we AddComponent(NM), but
    // verify and add manually if needed. Use the Il2CppType* from the _var metadata entry.
    auto* ilcaNetTypePtr = *(Il2CppType**)exl::util::modules::GetTargetOffset(ILCANETCOMP_TYPE_VAR);
    Logger::log("[OverworldMP] IlcaNetComponent Il2CppType*: %p\n", ilcaNetTypePtr);

    if (ilcaNetTypePtr != nullptr) {
        System::RuntimeTypeHandle::Object ilcaHandle {};
        ilcaHandle.fields.value = ilcaNetTypePtr;
        auto* ilcaType = System::Type::GetTypeFromHandle(ilcaHandle);

        // GetComponent(Type) @ 0x026a8240 — check if already added by RequireComponent
        auto* existingComp = _ILExternal::external<void*>(0x026a8240, go, ilcaType);
        if (existingComp == nullptr) {
            Logger::log("[OverworldMP] IlcaNetComponent not found on GO, adding manually...\n");
            auto* ilcaComp = go->AddComponentByType(ilcaType);
            s_ilcaNetComponent = ilcaComp;
            Logger::log("[OverworldMP] IlcaNetComponent added: %p (stored for disable/enable)\n", ilcaComp);
        } else {
            s_ilcaNetComponent = existingComp;
            Logger::log("[OverworldMP] IlcaNetComponent already exists on GO: %p (stored for disable/enable)\n", existingComp);
        }
    } else {
        Logger::log("[OverworldMP] WARNING: IlcaNetComponent Il2CppType* is null (metadata not initialized)\n");
    }

    // -----------------------------------------------------------------
    // Phase 3: Ensure Common.s_updateAlive is true
    // -----------------------------------------------------------------
    // Common.Update() early-returns if s_updateAlive == false. This static bool lives at
    // NexAssets.Common static_fields + 0xC0 (confirmed via Ghidra decompilation).
    // It's normally set by the game's networking init path. Since we're creating NM
    // ourselves, we need to ensure it's set so Common.Update() actually ticks NEX/PIA.
    auto* commonKlass = *(Il2CppClass**)exl::util::modules::GetTargetOffset(NEXASSETS_COMMON_TYPEINFO);
    if (commonKlass != nullptr) {
        // Initialize the class if its static constructor hasn't run
        if ((commonKlass->_2.bitflags2 >> 1 & 1) && (commonKlass->_2.cctor_finished == 0)) {
            il2cpp_runtime_class_init(commonKlass);
        }
        // static_fields pointer is at Il2CppClass offset 0xb8 (confirmed by Ghidra)
        auto* staticFields = *(void**)((uintptr_t)commonKlass + 0xb8);
        if (staticFields != nullptr) {
            bool* updateAlive = (bool*)((uintptr_t)staticFields + 0xC0);
            Logger::log("[OverworldMP] Common.s_updateAlive = %d (before)\n", (int)*updateAlive);
            if (!*updateAlive) {
                *updateAlive = true;
                Logger::log("[OverworldMP] Set Common.s_updateAlive = true\n");
            }
        } else {
            Logger::log("[OverworldMP] WARNING: Common static_fields is null\n");
        }
    } else {
        Logger::log("[OverworldMP] WARNING: NexAssets.Common Il2CppClass* is null\n");
    }

    Logger::log("[OverworldMP] NetworkManager singleton created at %p, waiting for IE_Start...\n", component);
    return true;
}

// ---------------------------------------------------------------------------
// Session management
// ---------------------------------------------------------------------------

void overworldMPStart() {
    if (s_mpContext.state != OverworldMPState::Disabled) {
        Logger::log("[OverworldMP] Already started, state=%d\n", (int)s_mpContext.state);
        return;
    }

    s_mpContext.Initialize();
    s_recvCallbackCount = 0;
    s_loggedFirstSend = false;

    // Get the current area from FieldManager
    FieldManager::getClass()->initIfNeeded();
    auto fm = FieldManager::getClass()->static_fields->_Instance_k__BackingField;
    if (fm != nullptr) {
        s_mpContext.myAreaID = fm->get_areaID();
    }

    Logger::log("[OverworldMP] Starting session, area=%d\n", s_mpContext.myAreaID);

    // Ensure NetworkManager singleton exists. If creating a new one, NM.Start()
    // will run on the next frame via Unity's deferred lifecycle, launching the
    // IE_Start coroutine which handles all initialization (SC delegates, Sequencer
    // registration, IlcaNet init, etc.). We wait for IE_Start to complete before
    // calling SC.StartSession.
    if (!ensureNetworkManagerSingleton()) {
        Logger::log("[OverworldMP] Cannot create NetworkManager — aborting\n");
        s_mpContext.state = OverworldMPState::Disabled;
        return;
    }

    // Wait for IE_Start to complete before starting the session.
    // IE_Start sets SC.callObjPtr to NM when done; we poll for this.
    s_initWaitTime = 0.0f;
    s_lastLoggedPiaState = 0xFFFFFFFF;  // Reset state change tracker
    s_mpContext.state = OverworldMPState::Initializing;
    Logger::log("[OverworldMP] Waiting for IE_Start coroutine to complete...\n");
}

void overworldMPStop() {
    if (s_mpContext.state == OverworldMPState::Disabled) {
        return;
    }

    Logger::log("[OverworldMP] Stopping session\n");

    s_mpContext.state = OverworldMPState::Disconnecting;

    // Clear grace period if stopping during one
    if (s_zoneChangeGraceTime > 0.0f) {
        Logger::log("[OverworldMP] Stop during grace period (%.2fs remaining)\n",
                    s_zoneChangeGraceTime);
        s_zoneChangeGraceTime = 0.0f;
    }

    // Reset interaction state
    s_interactionState = InteractionState::None;
    s_interactionTarget = -1;
    s_interactionTimeoutTime = 0.0f;
    s_pendingInteractionType = InteractionType::Trade;

    // Unregister receive callback before tearing down the session
    unregisterReceiveCallback();

    // Despawn all remote player entities
    overworldMPDespawnAllEntities();

    // Leave and finish the network session via NetworkManager (only if singleton exists)
    auto* nmInstance = getNMInstance();
    if (nmInstance != nullptr) {
        Dpr::NetworkUtils::NetworkManager::LeaveSession();
        Dpr::NetworkUtils::SessionConnector::FinishSession();
    }

    // Reset readiness flags so a future session start works correctly
    s_nmOnUpdateReady = false;
    s_nmStartedInstance = nullptr;

    s_mpContext.state = OverworldMPState::Disabled;
    Logger::log("[OverworldMP] Session stopped\n");
}

// Forward declarations for spawn queue (defined after entity spawn section)
static void spawnQueueClear();
static void spawnQueueProcessNext();
static bool spawnQueueEmpty();

// ---------------------------------------------------------------------------
// Per-frame update
// ---------------------------------------------------------------------------

void overworldMPUpdate(float deltaTime) {
    // === Initializing: wait for IE_Start coroutine to complete ===
    // NM.Start() creates the IE_Start coroutine which runs via the Sequencer.
    // IE_Start waits for Fader/DataManager/MessageManager, then does the full
    // init sequence. When done, SC.callObjPtr (SC+0x58) is set to NM.
    // We poll for this before starting the network session.
    if (s_mpContext.state == OverworldMPState::Initializing) {
        s_initWaitTime += deltaTime;

        auto* nm = getNMInstance();
        if (nm == nullptr) {
            Logger::log("[OverworldMP] NM singleton disappeared during init\n");
            s_mpContext.state = OverworldMPState::Disabled;
            return;
        }

        // Check if IE_Start has completed: s_nmOnUpdateReady is set true by our
        // NM.OnUpdate hook on its first invocation, which only happens after
        // IE_Start finishes and registers NM.OnUpdate with the Sequencer.
        // The old check (sc->fields.callObjPtr != nullptr) was broken because
        // callObjPtr is set during NM's .ctor, not by IE_Start.
        if (s_nmOnUpdateReady) {
            auto* sc = (Dpr::NetworkUtils::SessionConnector::Object*)nm->fields._sessionConnector;
            Logger::log("[OverworldMP] IE_Start completed after %.2fs (NM.OnUpdate fired)\n",
                        s_initWaitTime);

            // Use the same high-level API the underground uses: populate
            // NetworkParam and call DoStartSession. This lets the Sequencer-
            // driven PIA state machine handle PlatformInitialize naturally.
            // Match the underground's working NP values exactly (netType=0).
            void* np = nm->fields._networkParam;
            _ILExternal::external<void>(0x1BC62B0, np);  // NetworkParam.Reset
            *(int32_t*)((uintptr_t)np + 0x10) = 0;       // networkType = 0 (matches underground)
            *(int32_t*)((uintptr_t)np + 0x18) = 0;       // gamingStartMode = 0
            *(int32_t*)((uintptr_t)np + 0x1c) = 1;       // lastEventLeave = 1 (matches underground)
            *(int16_t*)((uintptr_t)np + 0x20) = 12576;   // matchingType = 12576 (matches underground)
            *(int16_t*)((uintptr_t)np + 0x22) = (int16_t)OW_MP_MAX_PLAYERS;

            // DoStartSession @ 0x1DE69B0 — NM instance method
            // PIA state machine (SettingSet → dispatch → NexPiaInitialize →
            // PlatformInitialize) runs naturally via the Sequencer on subsequent
            // frames — no synchronous PIA hacks needed.
            _ILExternal::external<void>(0x1DE69B0, nm, (void*)nullptr);
            Logger::log("[OverworldMP] DoStartSession returned, bRunningSession=%d\n",
                        (int)sc->fields.bRunningSession);

            if (!sc->fields.bRunningSession) {
                Logger::log("[OverworldMP] DoStartSession failed\n");
                s_mpContext.state = OverworldMPState::Error;
                return;
            }

            registerReceiveCallback();

            // Suppress NM's session event/error/finish delegates on SC.
            // We handle all session events ourselves via the SC hook.
            // Without this, PIA error transitions (e.g., after a player
            // disconnects) fire NM's error handler → ErrorApplet.
            // SC Fields: +0x20=onSessionEvent, +0x28=onSessionError, +0x30=onFinishSession
            if (nm->fields._sessionConnector != nullptr) {
                auto* sc = (void*)nm->fields._sessionConnector;
                *(void**)((uintptr_t)sc + 0x20) = nullptr; // onSessionEvent
                *(void**)((uintptr_t)sc + 0x28) = nullptr; // onSessionError
                *(void**)((uintptr_t)sc + 0x30) = nullptr; // onFinishSession
                Logger::log("[OverworldMP] Suppressed SC session event/error/finish delegates\n");
            }

            s_searchingFrameCount = 0;
            s_mpContext.state = OverworldMPState::Searching;
#if ENABLE_SESSION_UPDATE_HOOK
            s_loggedFirstPostSession = false;
#endif
            Logger::log("[OverworldMP] Session started (netType=0, matchType=12576, max %d players)\n",
                        OW_MP_MAX_PLAYERS);
        } else if (s_initWaitTime > 20.0f) {  // 20 second timeout
            Logger::log("[OverworldMP] IE_Start timeout after %.1fs\n", s_initWaitTime);
            s_mpContext.state = OverworldMPState::Error;
        }
        return;
    }

    if (!isOverworldMPActive()) {
        return;
    }

    // Zone change grace period — skip all MP processing while game systems
    // stabilize after a zone transition. IlcaNetComponent is disabled during
    // this period to prevent PIA from ticking with stale scene references.
    // When the grace period expires, re-enable IlcaNetComponent and resume.
    if (s_zoneChangeGraceTime > 0.0f) {
        float prevTime = s_zoneChangeGraceTime;
        s_zoneChangeGraceTime -= deltaTime;

        // Log every ~0.25s during grace to track countdown + session health
        if ((int)(prevTime * 4) != (int)(s_zoneChangeGraceTime * 4) || s_zoneChangeGraceTime <= 0.0f) {
            int32_t sessionState = Dpr::NetworkUtils::NetworkManager::get_SessionState();
            Logger::log("[OverworldMP] Grace countdown: %.2fs left, sessionState=%d, area=%d\n",
                        s_zoneChangeGraceTime > 0.0f ? s_zoneChangeGraceTime : 0.0f,
                        sessionState, s_mpContext.myAreaID);
        }

        if (s_zoneChangeGraceTime <= 0.0f) {
            s_zoneChangeGraceTime = 0.0f;
            Logger::log("[OverworldMP] Grace period expired\n");

            // Check session health immediately after reactivation
            int32_t sessionState = Dpr::NetworkUtils::NetworkManager::get_SessionState();
            Logger::log("[OverworldMP] Post-grace session state: %d (4/5=gaming OK, 7=error, 9=crash)\n",
                        sessionState);

            if (sessionState == 7 || sessionState == 9) {
                Logger::log("[OverworldMP] WARNING: Session in error state after grace period!\n");
                // Don't stop immediately — let the health check in the main update handle it.
                // This gives PIA one frame to recover if the state is transient.
            }

            Logger::log("[OverworldMP] Resuming MP in area %d\n", s_mpContext.myAreaID);

            // No DDOL cleanup needed — entities were destroyed by Unity
            // during the scene transition. References already nulled.

            // Notify peers of our new area
            overworldMPSendAreaChange(s_mpContext.myAreaID);
            // Spawn any remotes already in our area
            for (int i = 0; i < OW_MP_MAX_PLAYERS; i++) {
                auto& remote = s_mpContext.remotePlayers[i];
                if (remote.isActive && !remote.isSpawned &&
                    remote.areaID == s_mpContext.myAreaID) {
                    overworldMPSpawnEntity(i);
                }
            }
        }
        return;
    }

    // NM.OnUpdate and NM.OnLateUpdate are ticked by the Sequencer
    // (registered during IE_Start via Sequencer.SubscribeUpdate).

    s_mpContext.frameCounter++;

    // Check for session state transitions.
    if (s_mpContext.state == OverworldMPState::Searching) {
        s_searchingFrameCount++;
        // SessionState (from SC.nowSessionState / INL1.IlcaNetSessionState):
        //   0=None, 1=SettingSet, 2=MatchingWait, 3=MatchingComplete,
        //   4=Gaming(Internet, gamingStartMode!=0),
        //   5=Gaming(LAN/local, gamingStartMode==0),
        //   7=GamingError, 9=Crash
        int32_t sessionState = Dpr::NetworkUtils::NetworkManager::get_SessionState();
        // Log first 5 frames, then every 60 frames (~2s), to track state progression
        if (s_searchingFrameCount <= 5 || s_searchingFrameCount % 60 == 0) {
            Logger::log("[NetDiag] Searching frame %d — sessionState=%d, recvCallbacks=%d\n",
                        s_searchingFrameCount, sessionState, s_recvCallbackCount);
        }
        if (sessionState == 4 || sessionState == 5) { // SS_Gaming — session active
            s_mpContext.state = OverworldMPState::Connected;
            Logger::log("[OverworldMP] Session connected (state=%d)\n", sessionState);
        } else if (sessionState == 7 || sessionState == 9) { // GamingError or Crash
            Logger::log("[OverworldMP] Session error (state=%d)\n", sessionState);
            s_mpContext.state = OverworldMPState::Error;
        }
    }

    // Session health check — detect error/crash states and handle gracefully.
    // State 9 (Crash) = irrecoverable: PIA is dead, no more events will fire.
    //   Any remaining "active" peers are ghosts — force-clear and restart.
    // State 7 (GamingError) = potentially recoverable: PIA may still fire
    //   leave events and return to Gaming state after processing disconnects.
    //   Give it time via the grace counter.
    if (s_mpContext.state == OverworldMPState::Connected) {
        int32_t sessionState = Dpr::NetworkUtils::NetworkManager::get_SessionState();

        if (sessionState == 9) {
            // Crash — irrecoverable. PIA is completely dead.
            // Force-clear all ghost peers and restart immediately.
            float prevGrace = s_errorGraceTime;
            s_errorGraceTime += deltaTime;
            if (prevGrace == 0.0f) {
                Logger::log("[OverworldMP] Session CRASH detected (state=9), will restart in 0.5s\n");
            }
            // Small grace (0.5s) to let any final callbacks drain
            if (s_errorGraceTime >= 0.5f) {
                Logger::log("[OverworldMP] Session crashed, clearing %d ghost peers and restarting...\n",
                            (int)[&]{ int n=0; for(int i=0;i<OW_MP_MAX_PLAYERS;i++) if(s_mpContext.remotePlayers[i].isActive) n++; return n; }());
                s_errorGraceTime = 0.0f;
                overworldMPStop();
                overworldMPStart();
                return;
            }
        } else if (sessionState == 7) {
            // GamingError — may recover after processing leave events.
            float prevGrace = s_errorGraceTime;
            s_errorGraceTime += deltaTime;
            if (prevGrace == 0.0f || (int)(prevGrace) != (int)(s_errorGraceTime)) {
                Logger::log("[OverworldMP] Session error state=%d (grace %.2fs/%.1fs)\n",
                            sessionState, s_errorGraceTime, OW_MP_ERROR_GRACE_SEC);
            }
            if (s_errorGraceTime >= OW_MP_ERROR_GRACE_SEC) {
                // Waited long enough — session didn't recover. Restart.
                Logger::log("[OverworldMP] Session error persisted for %.2fs, restarting...\n",
                            s_errorGraceTime);
                s_errorGraceTime = 0.0f;
                overworldMPStop();
                overworldMPStart();
                return;
            }
        } else {
            // Session is healthy — reset grace counter
            if (s_errorGraceTime > 0.0f) {
                Logger::log("[OverworldMP] Session recovered after %.2fs error (state=%d)\n",
                            s_errorGraceTime, sessionState);
                s_errorGraceTime = 0.0f;
            }
        }
    }

    // Periodic Connected state diagnostics (~every 5 seconds)
    if (s_mpContext.state == OverworldMPState::Connected) {
        s_diagnosticAccumulator += deltaTime;
        if (s_diagnosticAccumulator >= 5.0f) {
            s_diagnosticAccumulator = 0.0f;
            int activeCount = 0;
            for (int i = 0; i < OW_MP_MAX_PLAYERS; i++)
                if (s_mpContext.remotePlayers[i].isActive) activeCount++;
            Logger::log("[OverworldMP] Connected: frame=%d recvCB=%d activePlayers=%d\n",
                        s_mpContext.frameCounter, s_recvCallbackCount, activeCount);
        }
    }

    // Send position at sync rate (~20 Hz)
    if (s_mpContext.state == OverworldMPState::Connected) {
        s_posSyncAccumulator += deltaTime;
        if (s_posSyncAccumulator >= OW_MP_POS_SYNC_INTERVAL_SEC) {
            s_posSyncAccumulator = 0.0f;
            overworldMPSendPosition();
        }
    }

    // Check for A-button interaction with nearby remote players
    if (s_mpContext.state == OverworldMPState::Connected) {
        overworldMPCheckInteraction();
    }

    // Tick emote balloon timers
    overworldMPTickBalloons(deltaTime);

    // Interaction timeout — if waiting for a response too long, cancel
    if (s_interactionState == InteractionState::WaitingResponse) {
        s_interactionTimeoutTime -= deltaTime;
        if (s_interactionTimeoutTime <= 0.0f) {
            int32_t target = s_interactionTarget;
            Logger::log("[OverworldMP] Interaction request timed out (target station %d)\n", target);
            // Reset network-level state BEFORE calling handler —
            // prevents this block from firing again on subsequent frames
            s_interactionState = InteractionState::None;
            s_interactionTarget = -1;
            s_interactionTimeoutTime = 0.0f;
            overworldMPOnRequestDeclined(target);
        }
    }

    // Update remote player entity positions (interpolation)
    for (int i = 0; i < OW_MP_MAX_PLAYERS; i++) {
        auto& remote = s_mpContext.remotePlayers[i];
        if (!remote.isActive || !remote.isSpawned) {
            continue;
        }

        auto* entity = (FieldObjectEntity::Object*)s_mpContext.spawnedEntities[i];
        if (entity == nullptr) {
            continue;
        }

        // Interpolate position toward target
        auto currentPos = entity->cast<BaseEntity>()->fields.worldPosition;
        float dx = remote.position.fields.x - currentPos.fields.x;
        float dy = remote.position.fields.y - currentPos.fields.y;
        float dz = remote.position.fields.z - currentPos.fields.z;

        float lerpRate = 8.0f * deltaTime; // Smooth interpolation
        if (lerpRate > 1.0f) lerpRate = 1.0f;

        UnityEngine::Vector3::Object newPos;
        newPos.fields.x = currentPos.fields.x + dx * lerpRate;
        newPos.fields.y = currentPos.fields.y + dy * lerpRate;
        newPos.fields.z = currentPos.fields.z + dz * lerpRate;

        entity->cast<BaseEntity>()->SetPositionDirect(newPos);

        // Update rotation
        float currentYaw = entity->cast<BaseEntity>()->fields.yawAngle;
        float yawDiff = remote.rotationY - currentYaw;
        // Normalize angle difference to [-180, 180]
        while (yawDiff > 180.0f) yawDiff -= 360.0f;
        while (yawDiff < -180.0f) yawDiff += 360.0f;
        entity->cast<BaseEntity>()->SetYawAngleDirect(currentYaw + yawDiff * lerpRate);

        // Drive idle/walk/run animation based on movement state
        // Clip indices: 0=Idle, 1=Walk, 2=Run
        auto* fce = (FieldCharacterEntity::Object*)entity;
        if (fce != nullptr && fce->fields._animationPlayer != nullptr) {
            int32_t currentClip = fce->fields._animationPlayer->get_currentIndex();
            if (remote.isMoving) {
                int32_t targetClip = remote.isRunning ? 2 : 1;  // Run or Walk
                if (currentClip != targetClip) {
                    fce->fields._animationPlayer->Play(targetClip);
                }
            } else if (!remote.isMoving && currentClip != 0) {
                fce->fields._animationPlayer->Play(0);  // Idle
            }
        }

        // Update follow pokemon position and animation
        auto* pokeEntity = (FieldObjectEntity::Object*)remote.followPokeEntity;
        if (pokeEntity != nullptr && remote.followPokeSpawned) {
            // Spawn scale-in animation (0→1 over 0.3s)
            if (remote.followPokeSpawnTimer > 0.0f) {
                remote.followPokeSpawnTimer -= deltaTime;
                if (remote.followPokeSpawnTimer < 0.0f) remote.followPokeSpawnTimer = 0.0f;
                float t = 1.0f - (remote.followPokeSpawnTimer / 0.3f);
                float s = remote.followPokeScale * t;
                UnityEngine::Vector3::Object scaleVec = {};
                scaleVec.fields.x = s;
                scaleVec.fields.y = s;
                scaleVec.fields.z = s;
                auto* pokeTransform = pokeEntity->cast<UnityEngine::Component>()->get_gameObject()->get_transform();
                pokeTransform->set_localScale(scaleVec);
            }

            // Trail-based following: the pokemon's target is the player's previous
            // position, not a yaw-computed offset. The target updates when the player
            // moves far enough from it, creating a natural trailing effect like the
            // base game. The pokemon stays still when the player just turns in place.
            float playerX = remote.position.fields.x;
            float playerZ = remote.position.fields.z;
            float playerY = remote.position.fields.y;

            // Update trail target when player moves > 1.5 units from current target
            float trailDx = playerX - remote.followPokeTargetX;
            float trailDz = playerZ - remote.followPokeTargetZ;
            float trailDistSq = trailDx * trailDx + trailDz * trailDz;
            if (trailDistSq > 2.25f) {  // 1.5^2
                // Place target 1.5 units behind player along movement direction
                float trailDist = sqrtf(trailDistSq);
                float normX = trailDx / trailDist;
                float normZ = trailDz / trailDist;
                remote.followPokeTargetX = playerX - normX * 1.5f;
                remote.followPokeTargetZ = playerZ - normZ * 1.5f;
                remote.followPokeTargetY = playerY;
            }

            float targetX = remote.followPokeTargetX;
            float targetZ = remote.followPokeTargetZ;
            float targetY = remote.followPokeTargetY;

            auto pokeCurPos = pokeEntity->cast<BaseEntity>()->fields.worldPosition;

            // Distance from pokemon to its trail target
            float toTargetDx = targetX - pokeCurPos.fields.x;
            float toTargetDz = targetZ - pokeCurPos.fields.z;
            float toTargetDist = sqrtf(toTargetDx * toTargetDx + toTargetDz * toTargetDz);

            // Distance-based speed matching the base game's WalkingCharacterController:
            //   < 0.5 units from target: idle (stop moving)
            //   0.5 - 2.5 units: walk speed
            //   > 2.5 units: run speed (pokemon fell behind)
            // Thresholds derived from Ghidra WalkData: nearTolerance=0.01+0.5, farDistance~2.5
            float pokeLerp;
            int32_t targetPokeClip;
            if (toTargetDist < 0.5f) {
                // Close enough — idle, no movement
                pokeLerp = 0.0f;
                targetPokeClip = 0;  // Idle
            } else if (toTargetDist < 2.5f) {
                // Normal following distance — walk
                pokeLerp = 3.0f * deltaTime;
                targetPokeClip = 1;  // Walk
            } else {
                // Fell behind — run to catch up
                pokeLerp = 7.0f * deltaTime;
                targetPokeClip = 2;  // Run
            }
            if (pokeLerp > 1.0f) pokeLerp = 1.0f;

            UnityEngine::Vector3::Object pokeNewPos;
            pokeNewPos.fields.x = pokeCurPos.fields.x + toTargetDx * pokeLerp;
            pokeNewPos.fields.y = pokeCurPos.fields.y + (targetY - pokeCurPos.fields.y) * pokeLerp;
            pokeNewPos.fields.z = pokeCurPos.fields.z + toTargetDz * pokeLerp;
            pokeEntity->cast<BaseEntity>()->SetPositionDirect(pokeNewPos);

            // Face toward the player with smooth turning
            float lookDx = remote.position.fields.x - pokeNewPos.fields.x;
            float lookDz = remote.position.fields.z - pokeNewPos.fields.z;
            float lookDistSq = lookDx * lookDx + lookDz * lookDz;
            if (lookDistSq > 0.01f) {
                float targetYaw = atan2f(lookDx, lookDz) * 180.0f / 3.14159265f;
                float currentYaw = pokeEntity->cast<BaseEntity>()->fields.yawAngle;

                // Shortest-path angular difference (-180 to 180)
                float yawDiff = targetYaw - currentYaw;
                if (yawDiff > 180.0f) yawDiff -= 360.0f;
                if (yawDiff < -180.0f) yawDiff += 360.0f;

                // Smooth turn: ~8x per second lerp rate, faster when running
                float turnRate = (targetPokeClip == 2) ? 12.0f : 8.0f;
                float turnStep = yawDiff * turnRate * deltaTime;
                // Clamp to prevent overshoot
                if (fabsf(turnStep) > fabsf(yawDiff)) turnStep = yawDiff;

                pokeEntity->cast<BaseEntity>()->SetYawAngleDirect(currentYaw + turnStep);
            }

            // Distance-based animation: 0=Idle, 1=Walk, 2=Run
            auto* pokeFpe = (FieldPokemonEntity::Object*)pokeEntity;
            if (pokeFpe != nullptr && pokeFpe->fields._animationPlayer != nullptr) {
                auto* animPlayer = (AnimationPlayer*)pokeFpe->fields._animationPlayer;
                int32_t currentPokeClip = animPlayer->get_currentIndex();
                if (currentPokeClip != targetPokeClip) {
                    animPlayer->Play(targetPokeClip);
                }
            }
        }
    }

    // Spawn follow pokemon for remote players (after player entity exists)
    if (!s_spawnInFlight && spawnQueueEmpty() && !s_pokeSpawnInFlight
        && s_zoneChangeGraceTime <= 0.0f) {
        for (int i = 0; i < OW_MP_MAX_PLAYERS; i++) {
            auto& remote = s_mpContext.remotePlayers[i];
            if (remote.isActive && remote.isSpawned && remote.followPokeActive
                && !remote.followPokeSpawned && remote.followMonsNo != 0) {
                overworldMPSpawnFollowPokemon(i);
                break;  // One at a time
            }
        }
    }
}

// ---------------------------------------------------------------------------
// Zone change handling
// ---------------------------------------------------------------------------

void overworldMPOnAreaChange(int32_t newAreaID) {
    if (!isOverworldMPActive()) {
        return;
    }

    int32_t oldAreaID = s_mpContext.myAreaID;
    s_mpContext.myAreaID = newAreaID;

    Logger::log("[OverworldMP] Area change: %d -> %d\n", oldAreaID, newAreaID);

    // Notify peers of our area change
    overworldMPSendAreaChange(newAreaID);

    // Cancel any in-flight spawns from the old area — they're no longer valid
    spawnQueueClear();

    // Despawn players not in the new area, then queue spawns for those already here
    for (int i = 0; i < OW_MP_MAX_PLAYERS; i++) {
        auto& remote = s_mpContext.remotePlayers[i];
        if (!remote.isActive) {
            continue;
        }

        if (remote.areaID != newAreaID && remote.isSpawned) {
            // Remote player is in a different area — despawn their entity
            overworldMPDespawnEntity(i);
        } else if (remote.areaID == newAreaID && !remote.isSpawned) {
            // Remote player is in the same area — queue their spawn
            overworldMPSpawnEntity(i);
        }
    }
}

// ---------------------------------------------------------------------------
// Player join/leave
// ---------------------------------------------------------------------------

void overworldMPOnPlayerJoin(int32_t stationIndex) {
    if (stationIndex < 0 || stationIndex >= OW_MP_MAX_PLAYERS) {
        return;
    }

    Logger::log("[OverworldMP] Player joined: station %d\n", stationIndex);

    auto& remote = s_mpContext.remotePlayers[stationIndex];
    remote.Clear();
    remote.stationIndex = stationIndex;
    remote.isActive = true;

    // Don't spawn yet — wait for their zone/position data
}

void overworldMPOnPlayerLeave(int32_t stationIndex) {
    if (stationIndex < 0 || stationIndex >= OW_MP_MAX_PLAYERS) {
        return;
    }

    Logger::log("[OverworldMP] Player left: station %d\n", stationIndex);

    if (s_mpContext.remotePlayers[stationIndex].isSpawned) {
        overworldMPDespawnEntity(stationIndex);
    }

    s_mpContext.remotePlayers[stationIndex].Clear();
}

// ---------------------------------------------------------------------------
// Entity spawn/despawn
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// Spawn queue — serial async spawn pipeline
// ---------------------------------------------------------------------------
// The base game uses managed C# closures (DisplayClass) to track per-spawn
// context, allowing fully concurrent asset loads. Since we can't allocate
// managed closure objects from C++, we use a FIFO queue that processes spawns
// serially. For a local wireless session with 4-8 players, the async asset
// load completes fast enough that all players appear within ~1 second.
// ---------------------------------------------------------------------------
static int32_t s_spawnQueue[OW_MP_MAX_PLAYERS];
static int32_t s_spawnQueueHead = 0;   // Next index to dequeue from
static int32_t s_spawnQueueTail = 0;   // Next index to enqueue to
// s_spawnInFlight declared earlier (near forward declarations) for use in overworldMPUpdate
static int32_t s_spawnInFlightStation = -1; // Station being spawned right now

// Follow pokemon spawn state — separate from player spawn queue so they don't block each other
// s_pokeSpawnInFlight declared earlier (near forward declarations) for use in overworldMPUpdate
static int32_t s_pokeSpawnInFlightStation = -1;

static int spawnQueueCount() {
    return (s_spawnQueueTail - s_spawnQueueHead + OW_MP_MAX_PLAYERS) % OW_MP_MAX_PLAYERS;
}

static bool spawnQueueEmpty() {
    return s_spawnQueueHead == s_spawnQueueTail;
}

static void spawnQueueClear() {
    s_spawnQueueHead = 0;
    s_spawnQueueTail = 0;
    s_spawnInFlight = false;
    s_spawnInFlightStation = -1;
    s_pokeSpawnInFlight = false;
    s_pokeSpawnInFlightStation = -1;
}

static bool spawnQueuePush(int32_t stationIndex) {
    int nextTail = (s_spawnQueueTail + 1) % OW_MP_MAX_PLAYERS;
    if (nextTail == s_spawnQueueHead) {
        // Queue is full (shouldn't happen with 8 slots and 8 max players)
        Logger::log("[OverworldMP] Spawn queue full, dropping station %d\n", stationIndex);
        return false;
    }
    s_spawnQueue[s_spawnQueueTail] = stationIndex;
    s_spawnQueueTail = nextTail;
    return true;
}

static int32_t spawnQueuePop() {
    if (spawnQueueEmpty()) return -1;
    int32_t station = s_spawnQueue[s_spawnQueueHead];
    s_spawnQueueHead = (s_spawnQueueHead + 1) % OW_MP_MAX_PLAYERS;
    return station;
}

// Forward declaration — kicks off the next spawn from the queue
static void spawnQueueProcessNext();

// ---------------------------------------------------------------------------
// Async asset load callback
// ---------------------------------------------------------------------------
// Invoked by Action<Object>.Invoke when Dpr.SubContents.Utils.LoadAsset
// completes. Ghidra confirms the closure's b__0 calls:
//   Action<Object>.Invoke(onLoad, loadedAsset, Method_System_Action<Object>_Invoke__)
// Inside Invoke, for static delegate with parameters_count=1, m_target=null,
// the dispatch is 2-arg: method_ptr(loadedAsset, delegate_method).
// This is the same LAB_5674 path as ReceivePacketCallback.Invoke.
// ---------------------------------------------------------------------------
static void onCharacterAssetLoaded(Il2CppObject* loadedAsset, MethodInfo* /*method*/) {
    int32_t stationIndex = s_spawnInFlightStation;
    s_spawnInFlight = false;
    s_spawnInFlightStation = -1;

    if (stationIndex < 0 || stationIndex >= OW_MP_MAX_PLAYERS) {
        Logger::log("[OverworldMP] Asset loaded but no pending spawn\n");
        // Still process next in queue
        spawnQueueProcessNext();
        return;
    }

    auto& ctx = getOverworldMPContext();
    auto& remote = ctx.remotePlayers[stationIndex];

    if (!remote.isActive) {
        Logger::log("[OverworldMP] Asset loaded but station %d no longer active\n", stationIndex);
        spawnQueueProcessNext();
        return;
    }

    if (loadedAsset == nullptr) {
        Logger::log("[OverworldMP] Asset load returned null for station %d\n", stationIndex);
        remote.isSpawned = false;
        spawnQueueProcessNext();
        return;
    }

    // Grace period guard — don't spawn during zone transitions
    if (s_zoneChangeGraceTime > 0.0f) {
        Logger::log("[OverworldMP] Asset loaded during grace period, skipping spawn for station %d\n",
                    stationIndex);
        remote.isSpawned = false;
        spawnQueueProcessNext();
        return;
    }

    // Area guard — only spawn if still in the same area
    FieldManager::getClass()->initIfNeeded();
    auto* fmInst = FieldManager::getClass()->static_fields->_Instance_k__BackingField;
    int32_t myArea = fmInst != nullptr ? fmInst->get_areaID() : -1;
    if (myArea != remote.areaID) {
        Logger::log("[OverworldMP] Area mismatch after load (my=%d, remote=%d), skipping spawn\n",
                    myArea, remote.areaID);
        remote.isSpawned = false;
        spawnQueueProcessNext();
        return;
    }

    Logger::log("[OverworldMP] Asset loaded for station %d: obj=%p klass=%p\n",
                stationIndex, loadedAsset,
                loadedAsset ? *(void**)loadedAsset : nullptr);

    // --- Spawn sequence (simplified from UgOpcManager.CreateCharacter) ---
    // We skip UgOpcController (underground-only, its Awake accesses UG singletons)
    // and handle position updates ourselves via the recv callback.

    // 1. Instantiate the loaded prefab
    Logger::log("[OverworldMP] Step 1: Instantiate\n");
    auto* prefab = (UnityEngine::_Object::Object*)loadedAsset;
    auto* go = (UnityEngine::GameObject::Object*)UnityEngine::_Object::Instantiate(prefab);
    if (go == nullptr) {
        Logger::log("[OverworldMP] Object.Instantiate returned null\n");
        remote.isSpawned = false;
        spawnQueueProcessNext();
        return;
    }
    Logger::log("[OverworldMP] Step 1 done: go=%p\n", go);

    // Entity is NOT marked DontDestroyOnLoad — Unity destroys it during
    // scene transitions. We just null out our references on zone change
    // and spawn fresh entities after the grace period.

    // 2. Set world position via Transform (for Unity rendering)
    auto* transform = go->get_transform();
    transform->set_position(remote.position);

    // 3. Set rotation (euler angles — Y axis rotation for facing direction)
    UnityEngine::Vector3::Object euler = {};
    euler.fields.x = 0.0f;
    euler.fields.y = remote.rotationY;
    euler.fields.z = 0.0f;
    transform->set_eulerAngles(euler);
    Logger::log("[OverworldMP] Step 3 done: pos/rot set\n");

    // 4. GetComponent<FieldObjectEntity> — the prefab already has this
    auto* entity = go->GetComponent<FieldObjectEntity>(
        UnityEngine::GameObject::Method$$FieldObjectEntity$$GetComponent);
    Logger::log("[OverworldMP] Step 4: entity=%p\n", entity);

    if (entity != nullptr) {
        // Network characters should not block local player movement
        Logger::log("[OverworldMP] Step 4a: setting collision ignore\n");
        entity->fields.IsIgnorePlayerCollision = true;

        // NOTE: Do NOT null out EventParams — UpdateSubductionDepth and
        // UpdateSwim dereference it (offset +0xa4 for attribute checks).
        // The interact crash is handled separately by our MP interaction system.

        // Set BaseEntity.worldPosition directly so the interpolation loop
        // doesn't see a huge delta from (0,0,0) to the actual position.
        // Without this, the entity visibly "drags" across the screen on spawn.
        entity->cast<BaseEntity>()->SetPositionDirect(remote.position);
        entity->cast<BaseEntity>()->fields.worldPosition = remote.position;
        entity->cast<BaseEntity>()->SetYawAngleDirect(remote.rotationY);
        Logger::log("[OverworldMP] Step 4a done: pos/rot/collision set\n");
    }

    // 5. Apply color variation (hair color, skin, etc.)
    // The prefab has a ColorVariation component whose OnEnable was already triggered
    // during Instantiate (via the ColorVariation_OnEnable hook in color_variations.cpp).
    // We now set the remote player's colorId and re-trigger OnEnable to re-apply.
    // Use non-generic GetComponent(Type) to avoid RGCTX issues.
    // ColorVariation_TypeInfo @ 0x04C57F80 (from script.json: Address 80060800)
    {
        auto* cvKlass = *(Il2CppClass**)exl::util::modules::GetTargetOffset(0x04C57F80);
        if (cvKlass != nullptr) {
            System::RuntimeTypeHandle::Object cvHandle {};
            cvHandle.fields.value = &cvKlass->_1.byval_arg;
            auto* cvType = System::Type::GetTypeFromHandle(cvHandle);
            auto* cvComp = (ColorVariation::Object*)_ILExternal::external<void*>(0x026a8240, go, cvType);
            if (cvComp != nullptr) {
                cvComp->fields.ColorIndex = remote.colorId;
                // Re-trigger OnEnable to apply the new color index.
                // ColorVariation.OnEnable @ 0x018ecd20 — goes through our hook
                // which calls Orig() + UpdateColorVariation().
                _ILExternal::external<void>(0x018ecd20, cvComp);
                Logger::log("[OverworldMP] ColorVariation applied: colorId=%d\n", remote.colorId);
            } else {
                Logger::log("[OverworldMP] No ColorVariation component on entity\n");
            }
        }
    }

    // 6. Start idle animation on the character
    auto* fce = (FieldCharacterEntity::Object*)entity;
    if (fce != nullptr && fce->fields._animationPlayer != nullptr) {
        fce->fields._animationPlayer->Play(0);  // 0 = Idle
        Logger::log("[OverworldMP] Animation initialized: animPlayer=%p\n",
                    fce->fields._animationPlayer);
    }

    // 7. Store the entity reference for position updates and cleanup
    ctx.spawnedEntities[stationIndex] = entity;
    remote.isSpawned = true;

    Logger::log("[OverworldMP] Entity spawned for station %d (avatar=%d)\n",
                stationIndex, remote.avatarId);

    // Kick off next queued spawn, if any
    spawnQueueProcessNext();
}

// ---------------------------------------------------------------------------
// Start the async asset load for a specific station index.
// Called by spawnQueueProcessNext() when this station is at the front of the queue.
// ---------------------------------------------------------------------------
static void spawnQueueStartLoad(int32_t stationIndex);

static void spawnQueueProcessNext() {
    if (s_spawnInFlight) {
        return; // Already loading one — will chain when it completes
    }

    while (!spawnQueueEmpty()) {
        int32_t next = spawnQueuePop();
        if (next < 0 || next >= OW_MP_MAX_PLAYERS) continue;

        auto& ctx = getOverworldMPContext();
        auto& remote = ctx.remotePlayers[next];

        // Skip if player left, already spawned, or no longer same zone
        if (!remote.isActive || remote.isSpawned) continue;

        FieldManager::getClass()->initIfNeeded();
        auto* fmCheck = FieldManager::getClass()->static_fields->_Instance_k__BackingField;
        if (fmCheck == nullptr || fmCheck->get_areaID() != remote.areaID) continue;

        // This one is valid — start loading
        spawnQueueStartLoad(next);
        return;
    }
}

void overworldMPSpawnEntity(int32_t stationIndex) {
    if (stationIndex < 0 || stationIndex >= OW_MP_MAX_PLAYERS) {
        return;
    }

    auto& remote = s_mpContext.remotePlayers[stationIndex];
    if (!remote.isActive || remote.isSpawned) {
        return;
    }

    // Enqueue the spawn request
    if (!spawnQueuePush(stationIndex)) {
        return; // Queue full — logged inside spawnQueuePush
    }

    Logger::log("[OverworldMP] Queued spawn for station %d (queue depth: %d)\n",
                stationIndex, spawnQueueCount());

    // If nothing is currently loading, start immediately
    spawnQueueProcessNext();
}

// ---------------------------------------------------------------------------
// Actual async asset load — called when this station reaches the front of the queue.
// Mirrors the first phase of UgOpcManager.CreateCharacter (Ghidra @ 0x1B17E30).
// ---------------------------------------------------------------------------
static void spawnQueueStartLoad(int32_t stationIndex) {
    auto& remote = getOverworldMPContext().remotePlayers[stationIndex];

    Logger::log("[OverworldMP] Starting asset load for station %d at (%.1f, %.1f, %.1f)\n",
                stationIndex, remote.position.fields.x, remote.position.fields.y,
                remote.position.fields.z);

    // Get the character dress data to determine the field model asset name
    GameData::DataManager::getClass()->initIfNeeded();
    auto* dressData = GameData::DataManager::GetCharacterDressData(remote.avatarId);
    if (dressData == nullptr) {
        Logger::log("[OverworldMP] No dress data for avatarId %d\n", remote.avatarId);
        spawnQueueProcessNext();
        return;
    }

    // FieldGraphic is the 3rd field (offset 0x18 in SheetData, which is 0x28 in IL2CPP with header)
    auto* assetName = (System::String::Object*)dressData->fields.FieldGraphic;
    if (assetName == nullptr) {
        Logger::log("[OverworldMP] No FieldGraphic for avatarId %d\n", remote.avatarId);
        spawnQueueProcessNext();
        return;
    }

    // Build the full asset path: "persons/field/" + assetName
    auto* prefix = System::String::Create("persons/field/");
    auto* path = System::String::Concat(prefix, assetName);

    // Mark this station as in-flight
    s_spawnInFlight = true;
    s_spawnInFlightStation = stationIndex;
    remote.isSpawned = true; // Mark as "spawning" to prevent duplicate requests

    // Create a delegate for the asset load completion callback.
    // Use il2cpp_object_new + Action.ctor for proper initialization.
    // Ghidra confirms Action.ctor (0x23FEB30) does:
    //   +0x10 = MethodInfo->methodPointer, +0x20 = target, +0x28 = MethodInfo
    //   + GC write barrier. invoke_impl (+0x18) is NOT set by .ctor.
    // Action<Object>.Invoke (0x2630500) doesn't read invoke_impl for the
    // static/null-target path — it dispatches 2-arg: method_ptr(arg, method).
    static MethodInfo* s_assetMethodInfo = nullptr;
    if (s_assetMethodInfo == nullptr) {
        s_assetMethodInfo = (MethodInfo*)nn_malloc(sizeof(MethodInfo));
        memset(s_assetMethodInfo, 0, sizeof(MethodInfo));
        s_assetMethodInfo->methodPointer = (Il2CppMethodPointer)&onCharacterAssetLoaded;
        s_assetMethodInfo->flags = 0x0010;          // METHOD_ATTRIBUTE_STATIC
        s_assetMethodInfo->parameters_count = 1;     // void(Object) = 1 param
    }
    auto* actionClass = System::Action::getClass(System::Action::void_TypeInfo);
    // Set klass so Invoke's internal thunks don't null-deref method->klass
    if (s_assetMethodInfo->klass == nullptr) {
        s_assetMethodInfo->klass = (Il2CppClass*)actionClass;
    }
    auto* action = (System::Action::Object*)il2cpp_object_new((Il2CppClass*)actionClass);
    // Action.ctor(action, target=null, methodInfo) @ 0x023feb30
    _ILExternal::external<void>(0x023feb30, action, nullptr, s_assetMethodInfo);

    // Start the async asset load
    Dpr::SubContents::Utils::getClass()->initIfNeeded();
    SmartPoint::AssetAssistant::Sequencer::getClass()->initIfNeeded();

    auto* coroutine = Dpr::SubContents::Utils::LoadAsset(path, action);
    SmartPoint::AssetAssistant::Sequencer::Start(coroutine);
}

void overworldMPDespawnEntity(int32_t stationIndex) {
    if (stationIndex < 0 || stationIndex >= OW_MP_MAX_PLAYERS) {
        return;
    }

    auto& remote = s_mpContext.remotePlayers[stationIndex];

    // Also despawn follow pokemon if present
    if (remote.followPokeSpawned) {
        overworldMPDespawnFollowPokemon(stationIndex);
    }

    if (!remote.isSpawned) {
        return;
    }

    Logger::log("[OverworldMP] Despawning entity for station %d\n", stationIndex);

    auto* entity = (FieldObjectEntity::Object*)s_mpContext.spawnedEntities[stationIndex];
    if (entity != nullptr) {
        // Remove from EntityManager (same as DestroyLeakObject / RemoveCharacter)
        EntityManager::getClass()->initIfNeeded();
        EntityManager::Remove(entity->cast<BaseEntity>());

        // Destroy the GameObject
        UnityEngine::_Object::getClass()->initIfNeeded();
        auto* go = entity->cast<UnityEngine::Component>()->get_gameObject();
        UnityEngine::_Object::Destroy(go->cast<UnityEngine::_Object>());
    }

    remote.isSpawned = false;
    s_mpContext.spawnedEntities[stationIndex] = nullptr;
}

void overworldMPDespawnAllEntities() {
    // Cancel any in-flight or queued spawns
    spawnQueueClear();

    for (int i = 0; i < OW_MP_MAX_PLAYERS; i++) {
        overworldMPDespawnEntity(i);
    }
}

// ---------------------------------------------------------------------------
// Follow pokemon spawn/despawn
// ---------------------------------------------------------------------------

static void overworldMPDespawnFollowPokemon(int32_t stationIndex) {
    if (stationIndex < 0 || stationIndex >= OW_MP_MAX_PLAYERS) return;

    auto& remote = s_mpContext.remotePlayers[stationIndex];
    if (!remote.followPokeSpawned) return;

    Logger::log("[OverworldMP] Despawning follow pokemon for station %d\n", stationIndex);

    auto* entity = (FieldObjectEntity::Object*)remote.followPokeEntity;
    if (entity != nullptr) {
        EntityManager::getClass()->initIfNeeded();
        EntityManager::Remove(entity->cast<BaseEntity>());

        UnityEngine::_Object::getClass()->initIfNeeded();
        auto* go = entity->cast<UnityEngine::Component>()->get_gameObject();
        UnityEngine::_Object::Destroy(go->cast<UnityEngine::_Object>());
    }

    remote.followPokeSpawned = false;
    remote.followPokeEntity = nullptr;
}

// Async asset load callback for follow pokemon
static void onPokemonAssetLoaded(Il2CppObject* loadedAsset, MethodInfo* /*method*/) {
    int32_t stationIndex = s_pokeSpawnInFlightStation;
    s_pokeSpawnInFlight = false;
    s_pokeSpawnInFlightStation = -1;

    if (stationIndex < 0 || stationIndex >= OW_MP_MAX_PLAYERS) {
        Logger::log("[OverworldMP] Pokemon asset loaded but no pending spawn\n");
        return;
    }

    auto& remote = s_mpContext.remotePlayers[stationIndex];

    if (!remote.isActive || !remote.followPokeActive) {
        Logger::log("[OverworldMP] Pokemon asset loaded but station %d no longer active/follow\n", stationIndex);
        return;
    }

    if (loadedAsset == nullptr) {
        Logger::log("[OverworldMP] Pokemon asset load returned null for station %d\n", stationIndex);
        return;
    }

    // Grace period guard
    if (s_zoneChangeGraceTime > 0.0f) {
        Logger::log("[OverworldMP] Pokemon asset loaded during grace period, skipping for station %d\n", stationIndex);
        return;
    }

    Logger::log("[OverworldMP] Follow pokemon asset loaded for station %d: monsNo=%d\n",
                stationIndex, remote.followMonsNo);

    // 1. Instantiate the loaded prefab
    auto* prefab = (UnityEngine::_Object::Object*)loadedAsset;
    auto* go = (UnityEngine::GameObject::Object*)UnityEngine::_Object::Instantiate(prefab);
    if (go == nullptr) {
        Logger::log("[OverworldMP] Pokemon Object.Instantiate returned null\n");
        return;
    }

    // 2. Start with scale 0 for spawn animation (scales up over 0.3s)
    auto* transform = go->get_transform();
    UnityEngine::Vector3::Object scaleVec = {};
    scaleVec.fields.x = 0.0f;
    scaleVec.fields.y = 0.0f;
    scaleVec.fields.z = 0.0f;
    transform->set_localScale(scaleVec);
    remote.followPokeSpawnTimer = 0.3f;

    // 3. Set initial position behind the player and initialize trail target
    float yawRad = remote.rotationY * 3.14159265f / 180.0f;
    float offsetDist = 1.5f;
    UnityEngine::Vector3::Object pokePos = {};
    pokePos.fields.x = remote.position.fields.x + sinf(yawRad) * offsetDist;
    pokePos.fields.y = remote.position.fields.y;
    pokePos.fields.z = remote.position.fields.z - cosf(yawRad) * offsetDist;
    transform->set_position(pokePos);

    // Initialize trail target to the pokemon's starting position
    remote.followPokeTargetX = pokePos.fields.x;
    remote.followPokeTargetY = pokePos.fields.y;
    remote.followPokeTargetZ = pokePos.fields.z;

    // 4. Face toward the player
    float lookDx = remote.position.fields.x - pokePos.fields.x;
    float lookDz = remote.position.fields.z - pokePos.fields.z;
    float lookYaw = atan2f(lookDx, lookDz) * 180.0f / 3.14159265f;
    UnityEngine::Vector3::Object euler = {};
    euler.fields.y = lookYaw;
    transform->set_eulerAngles(euler);

    // 5. GetComponent<FieldObjectEntity> — the pokemon prefab has this (FieldPokemonEntity inherits)
    auto* entity = go->GetComponent<FieldObjectEntity>(
        UnityEngine::GameObject::Method$$FieldObjectEntity$$GetComponent);

    if (entity != nullptr) {
        // Ignore collision with local player
        entity->fields.IsIgnorePlayerCollision = true;

        // Set BaseEntity.worldPosition directly so interpolation loop starts
        // from the correct position (avoids "drag" from origin)
        entity->cast<BaseEntity>()->SetPositionDirect(pokePos);
        entity->cast<BaseEntity>()->fields.worldPosition = pokePos;
        entity->cast<BaseEntity>()->SetYawAngleDirect(lookYaw);
    }

    // 6. Start idle animation
    auto* fpe = (FieldPokemonEntity::Object*)entity;
    if (fpe != nullptr && fpe->fields._animationPlayer != nullptr) {
        auto* animPlayer = (AnimationPlayer*)fpe->fields._animationPlayer;
        animPlayer->Play(0);  // 0 = Idle
    }

    // 7. Store entity reference
    remote.followPokeEntity = entity;
    remote.followPokeSpawned = true;

    Logger::log("[OverworldMP] Follow pokemon spawned for station %d (monsNo=%d scale=%.2f)\n",
                stationIndex, remote.followMonsNo, remote.followPokeScale);
}

static void overworldMPSpawnFollowPokemon(int32_t stationIndex) {
    if (stationIndex < 0 || stationIndex >= OW_MP_MAX_PLAYERS) return;

    auto& remote = s_mpContext.remotePlayers[stationIndex];
    if (remote.followMonsNo == 0) return;

    Logger::log("[OverworldMP] Spawning follow pokemon for station %d: monsNo=%d formNo=%d\n",
                stationIndex, remote.followMonsNo, (int)remote.followFormNo);

    // Get pokemon catalog for asset name and scale
    GameData::DataManager::getClass()->initIfNeeded();
    auto* catalog = GameData::DataManager::GetPokemonCatalog(
        remote.followMonsNo, remote.followFormNo,
        (Pml::Sex)remote.followSex, remote.followIsRare, false);

    if (catalog == nullptr) {
        Logger::log("[OverworldMP] No pokemon catalog for monsNo=%d\n", remote.followMonsNo);
        return;
    }

    auto* assetBundleName = catalog->fields.AssetBundleName;
    remote.followPokeScale = catalog->fields.FieldWalkingScale;

    if (assetBundleName == nullptr) {
        Logger::log("[OverworldMP] No AssetBundleName for monsNo=%d\n", remote.followMonsNo);
        return;
    }

    // Build the full asset path using the game's method
    Dpr::SubContents::Utils::getClass()->initIfNeeded();
    auto* fullPath = Dpr::SubContents::Utils::GetPokemonAssetbundleName(assetBundleName);

    // Mark in-flight
    s_pokeSpawnInFlight = true;
    s_pokeSpawnInFlightStation = stationIndex;

    // Create Action delegate for the asset load callback (same pattern as player spawn)
    static MethodInfo* s_pokeAssetMethodInfo = nullptr;
    if (s_pokeAssetMethodInfo == nullptr) {
        s_pokeAssetMethodInfo = (MethodInfo*)nn_malloc(sizeof(MethodInfo));
        memset(s_pokeAssetMethodInfo, 0, sizeof(MethodInfo));
        s_pokeAssetMethodInfo->methodPointer = (Il2CppMethodPointer)&onPokemonAssetLoaded;
        s_pokeAssetMethodInfo->flags = 0x0010;          // METHOD_ATTRIBUTE_STATIC
        s_pokeAssetMethodInfo->parameters_count = 1;     // void(Object) = 1 param
    }
    auto* actionClass = System::Action::getClass(System::Action::void_TypeInfo);
    if (s_pokeAssetMethodInfo->klass == nullptr) {
        s_pokeAssetMethodInfo->klass = (Il2CppClass*)actionClass;
    }
    auto* action = (System::Action::Object*)il2cpp_object_new((Il2CppClass*)actionClass);
    _ILExternal::external<void>(0x023feb30, action, nullptr, s_pokeAssetMethodInfo);

    // Start async asset load
    SmartPoint::AssetAssistant::Sequencer::getClass()->initIfNeeded();
    auto* coroutine = Dpr::SubContents::Utils::LoadAsset(fullPath, action);
    SmartPoint::AssetAssistant::Sequencer::Start(coroutine);
}

// ---------------------------------------------------------------------------
// Network send helpers
// ---------------------------------------------------------------------------

void overworldMPSendPosition() {
    // Get local player position
    EntityManager::getClass()->initIfNeeded();
    auto player = EntityManager::getClass()->static_fields->_activeFieldPlayer_k__BackingField;
    if (player == nullptr) {
        return;
    }

    auto pos = player->cast<BaseEntity>()->fields.worldPosition;
    float yaw = player->cast<BaseEntity>()->fields.yawAngle;

    // Get the singleton's unreliable PacketWriter
    void* pw = Dpr::NetworkUtils::NetworkManager::get_PacketWriter();
    if (pw == nullptr) return;

    // Get local player's avatar/fashion ID and color ID
    int32_t myAvatarId = PlayerWork::get_playerFashion();
    int32_t myColorId = getCustomSaveData()->playerColorVariation.playerColorID;

    // Build packet: [DataID:1][posX:4][posY:4][posZ:4][rotY:4][areaID:4][avatarId:4][colorId:4]
    //               [hasFollowPoke:1][monsNo:4][formNo:1][sex:1][isRare:1]
    //               [nameLen:1][nameChars:nameLen*2] = 30-61 bytes
    il2cpp_vcall_void(pw, PW_RESET);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, OWMP_DATA_ID);
    il2cpp_vcall_write_fp32(pw, PW_WRITE_FP32, pos.fields.x);
    il2cpp_vcall_write_fp32(pw, PW_WRITE_FP32, pos.fields.y);
    il2cpp_vcall_write_fp32(pw, PW_WRITE_FP32, pos.fields.z);
    il2cpp_vcall_write_fp32(pw, PW_WRITE_FP32, yaw);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, s_mpContext.myAreaID);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, myAvatarId);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, myColorId);

    // Follow pokemon data — read from FieldWalkingManager
    auto* fwMng = (Dpr::Field::Walking::FieldWalkingManager::Object*)
        FieldManager::getClass()->static_fields->fwMng;
    auto* pokeParamRaw = (fwMng != nullptr)
        ? fwMng->fields._PartnerPokeParam_k__BackingField : nullptr;
    auto* corePoke = (Pml::PokePara::CoreParam::Object*)pokeParamRaw;
    bool hasFollowPoke = (corePoke != nullptr && !corePoke->IsNull());
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, hasFollowPoke ? 1 : 0);
    if (hasFollowPoke) {
        il2cpp_vcall_write_s32(pw, PW_WRITE_S32, corePoke->GetMonsNo());
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, (uint8_t)corePoke->GetFormNo());
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, (uint8_t)corePoke->GetSex());
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE,
            (corePoke->GetRareType() != Pml::PokePara::RareType::NOT_RARE) ? 1 : 0);
    }

    // Player name — send as raw UTF-16 chars (length + char pairs)
    auto* myStatus = PlayerWork::get_playerStatus();
    System::String::Object* myName = (myStatus != nullptr) ? myStatus->fields.name : nullptr;
    if (myName != nullptr && myName->fields.m_stringLength > 0) {
        uint8_t nameLen = (uint8_t)myName->fields.m_stringLength;
        if (nameLen > 12) nameLen = 12; // BDSP max name length
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, nameLen);
        int16_t* chars = &myName->fields.m_firstChar;
        for (int i = 0; i < nameLen; i++) {
            il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, (uint8_t)(chars[i] & 0xFF));
            il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, (uint8_t)((chars[i] >> 8) & 0xFF));
        }
    } else {
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, 0);
    }

    // Send unreliable to all peers on Station transport (type=0)
    Dpr::NetworkUtils::NetworkManager::SendUnReliablePacketToAll(pw, 0);

    if (!s_loggedFirstSend) {
        s_loggedFirstSend = true;
        Logger::log("[OverworldMP] First position sent: area=%d pos=(%.1f,%.1f,%.1f) followPoke=%d\n",
                    s_mpContext.myAreaID, pos.fields.x, pos.fields.y, pos.fields.z,
                    hasFollowPoke ? 1 : 0);
    }
}

void overworldMPSendAreaChange(int32_t areaID) {
    // Get local player position for the area change notification
    EntityManager::getClass()->initIfNeeded();
    auto player = EntityManager::getClass()->static_fields->_activeFieldPlayer_k__BackingField;

    UnityEngine::Vector3::Object pos = {};
    float yaw = 0.0f;
    if (player != nullptr) {
        pos = player->cast<BaseEntity>()->fields.worldPosition;
        yaw = player->cast<BaseEntity>()->fields.yawAngle;
    }

    // Get the singleton's reliable PacketWriterRe
    void* pw = Dpr::NetworkUtils::NetworkManager::get_PacketWriterRe();
    if (pw == nullptr) return;

    // Get local player's avatar/fashion ID and color ID
    int32_t myAvatarId = PlayerWork::get_playerFashion();
    int32_t myColorId = getCustomSaveData()->playerColorVariation.playerColorID;

    // Same format as position packet, but sent reliably
    il2cpp_vcall_void(pw, PW_RESET);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, OWMP_DATA_ID);
    il2cpp_vcall_write_fp32(pw, PW_WRITE_FP32, pos.fields.x);
    il2cpp_vcall_write_fp32(pw, PW_WRITE_FP32, pos.fields.y);
    il2cpp_vcall_write_fp32(pw, PW_WRITE_FP32, pos.fields.z);
    il2cpp_vcall_write_fp32(pw, PW_WRITE_FP32, yaw);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, areaID);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, myAvatarId);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, myColorId);

    // Follow pokemon data — same as position packet
    auto* fwMng2 = (Dpr::Field::Walking::FieldWalkingManager::Object*)
        FieldManager::getClass()->static_fields->fwMng;
    auto* pokeParamRaw2 = (fwMng2 != nullptr)
        ? fwMng2->fields._PartnerPokeParam_k__BackingField : nullptr;
    auto* corePoke2 = (Pml::PokePara::CoreParam::Object*)pokeParamRaw2;
    bool hasFollowPoke2 = (corePoke2 != nullptr && !corePoke2->IsNull());
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, hasFollowPoke2 ? 1 : 0);
    if (hasFollowPoke2) {
        il2cpp_vcall_write_s32(pw, PW_WRITE_S32, corePoke2->GetMonsNo());
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, (uint8_t)corePoke2->GetFormNo());
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, (uint8_t)corePoke2->GetSex());
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE,
            (corePoke2->GetRareType() != Pml::PokePara::RareType::NOT_RARE) ? 1 : 0);
    }

    // Player name — same as position packet
    auto* myStatus2 = PlayerWork::get_playerStatus();
    System::String::Object* myName2 = (myStatus2 != nullptr) ? myStatus2->fields.name : nullptr;
    if (myName2 != nullptr && myName2->fields.m_stringLength > 0) {
        uint8_t nameLen2 = (uint8_t)myName2->fields.m_stringLength;
        if (nameLen2 > 12) nameLen2 = 12;
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, nameLen2);
        int16_t* chars2 = &myName2->fields.m_firstChar;
        for (int i = 0; i < nameLen2; i++) {
            il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, (uint8_t)(chars2[i] & 0xFF));
            il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, (uint8_t)((chars2[i] >> 8) & 0xFF));
        }
    } else {
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, 0);
    }

    // Send reliable to all peers on Station transport (type=0)
    Dpr::NetworkUtils::NetworkManager::SendReliablePacketToAll(pw, 0);
}

// ---------------------------------------------------------------------------
// Proximity detection
// ---------------------------------------------------------------------------

int32_t overworldMPFindNearestPlayer(UnityEngine::Vector3::Object localPos) {
    float closestDist = OW_MP_CONTACT_RADIUS * OW_MP_CONTACT_RADIUS;
    int32_t closestStation = -1;

    for (int i = 0; i < OW_MP_MAX_PLAYERS; i++) {
        auto& remote = s_mpContext.remotePlayers[i];
        if (!remote.isActive || !remote.isSpawned) {
            continue;
        }

        float dx = remote.position.fields.x - localPos.fields.x;
        float dz = remote.position.fields.z - localPos.fields.z;
        float distSq = dx * dx + dz * dz;

        if (distSq < closestDist) {
            closestDist = distSq;
            closestStation = remote.stationIndex;
        }
    }

    return closestStation;
}

// ---------------------------------------------------------------------------
// Emote and interaction send functions
// ---------------------------------------------------------------------------

InteractionState overworldMPGetInteractionState() {
    return s_interactionState;
}

void overworldMPSendEmote(uint8_t emoteId) {
    if (!isOverworldMPActive() || s_mpContext.state != OverworldMPState::Connected) {
        Logger::log("[OverworldMP] Cannot send emote — not connected\n");
        return;
    }

    void* pw = Dpr::NetworkUtils::NetworkManager::get_PacketWriter();
    if (pw == nullptr) return;

    // Packet: [DataID:0xC1][emoteId:1] = 2 bytes
    il2cpp_vcall_void(pw, PW_RESET);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, OWMP_DATA_ID_EMOTE);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, emoteId);

    // Send unreliable to all — emotes are fire-and-forget
    Dpr::NetworkUtils::NetworkManager::SendUnReliablePacketToAll(pw, 0);
    Logger::log("[OverworldMP] Sent emote: id=%d\n", (int)emoteId);
}

void overworldMPSendInteractionRequest(int32_t targetStation, InteractionType type, BattleSubtype subtype) {
    if (!isOverworldMPActive() || s_mpContext.state != OverworldMPState::Connected) {
        Logger::log("[OverworldMP] Cannot send interaction request — not connected\n");
        return;
    }

    void* pw = Dpr::NetworkUtils::NetworkManager::get_PacketWriterRe();
    if (pw == nullptr) return;

    // Packet: [DataID:0xC2][targetStation:4][type:1][subtype:1] = 7 bytes
    il2cpp_vcall_void(pw, PW_RESET);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, OWMP_DATA_ID_INTERACT_REQ);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, targetStation);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, (uint8_t)type);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, (uint8_t)subtype);

    // Send reliable — interaction requests must not be lost
    Dpr::NetworkUtils::NetworkManager::SendReliablePacketToAll(pw, 0);

    s_interactionState = InteractionState::WaitingResponse;
    s_interactionTarget = targetStation;
    s_interactionTimeoutTime = 10.0f; // 10 seconds
    s_pendingInteractionType = type;

    Logger::log("[OverworldMP] Sent interaction request: target=%d type=%d subtype=%d\n",
                targetStation, (int)type, (int)subtype);
}

void overworldMPSendInteractionResponse(int32_t targetStation, bool accepted) {
    if (!isOverworldMPActive() || s_mpContext.state != OverworldMPState::Connected) {
        Logger::log("[OverworldMP] Cannot send interaction response — not connected\n");
        return;
    }

    void* pw = Dpr::NetworkUtils::NetworkManager::get_PacketWriterRe();
    if (pw == nullptr) return;

    // Packet: [DataID:0xC3][targetStation:4][accepted:1] = 6 bytes
    il2cpp_vcall_void(pw, PW_RESET);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, OWMP_DATA_ID_INTERACT_RESP);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, targetStation);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, accepted ? 1 : 0);

    Dpr::NetworkUtils::NetworkManager::SendReliablePacketToAll(pw, 0);

    if (!accepted) {
        s_interactionState = InteractionState::None;
        s_interactionTarget = -1;
    }

    Logger::log("[OverworldMP] Sent interaction response: target=%d accepted=%d\n",
                targetStation, (int)accepted);
}

void overworldMPSendTradePoke(int32_t targetStation, int32_t partySlot, uint8_t* data, int32_t size) {
    if (!isOverworldMPActive() || s_mpContext.state != OverworldMPState::Connected) {
        Logger::log("[OverworldMP] Cannot send trade poke — not connected\n");
        return;
    }

    void* pw = Dpr::NetworkUtils::NetworkManager::get_PacketWriterRe();
    if (pw == nullptr) return;

    // Packet: [DataID:0xC4][targetStation:4][partySlot:1][pokeData:344 as 86 ints]
    il2cpp_vcall_void(pw, PW_RESET);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, OWMP_DATA_ID_TRADE_POKE);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, targetStation);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, (uint8_t)partySlot);

    // Write pokemon data as 86 x int32 (344 / 4 = 86)
    int32_t numInts = size / 4;
    for (int i = 0; i < numInts; i++) {
        int32_t val = 0;
        memcpy(&val, &data[i * 4], 4);
        il2cpp_vcall_write_s32(pw, PW_WRITE_S32, val);
    }

    Dpr::NetworkUtils::NetworkManager::SendReliablePacketToAll(pw, 0);
    Logger::log("[OverworldMP] Sent trade poke: target=%d slot=%d size=%d (%d ints)\n",
                targetStation, partySlot, size, numInts);
}

void overworldMPSendTradeConfirm(int32_t targetStation, bool confirmed) {
    if (!isOverworldMPActive() || s_mpContext.state != OverworldMPState::Connected) {
        Logger::log("[OverworldMP] Cannot send trade confirm — not connected\n");
        return;
    }

    void* pw = Dpr::NetworkUtils::NetworkManager::get_PacketWriterRe();
    if (pw == nullptr) return;

    // Packet: [DataID:0xC5][targetStation:4][confirmed:1]
    il2cpp_vcall_void(pw, PW_RESET);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, OWMP_DATA_ID_TRADE_CONFIRM);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, targetStation);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, confirmed ? 1 : 0);

    Dpr::NetworkUtils::NetworkManager::SendReliablePacketToAll(pw, 0);
    Logger::log("[OverworldMP] Sent trade confirm: target=%d confirmed=%d\n",
                targetStation, (int)confirmed);
}

void overworldMPSendBattleParty(int32_t targetStation, BattleSubtype subtype) {
    if (!isOverworldMPActive() || s_mpContext.state != OverworldMPState::Connected) {
        Logger::log("[OverworldMP] Cannot send battle party — not connected\n");
        return;
    }

    auto* party = PlayerWork::get_playerParty();
    if (party == nullptr) {
        Logger::log("[OverworldMP] Cannot send battle party — party is null\n");
        return;
    }

    void* pw = Dpr::NetworkUtils::NetworkManager::get_PacketWriterRe();
    if (pw == nullptr) return;

    uint32_t memberCount = party->fields.m_memberCount;
    if (memberCount > 6) memberCount = 6;

    // -----------------------------------------------------------------------
    // HEADER sub-packet (type=0): memberCount + MYSTATUS_COMM + subtype
    // ~45 bytes — well within the 340-byte PIA packet limit
    // -----------------------------------------------------------------------
    il2cpp_vcall_void(pw, PW_RESET);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, OWMP_DATA_ID_BATTLE_PARTY);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, targetStation);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, BATTLE_PARTY_SUB_HEADER);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, (uint8_t)memberCount);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, (uint8_t)subtype);

    // Write MYSTATUS_COMM
    auto* playerStatus = PlayerWork::get_playerStatus();
    if (playerStatus != nullptr) {
        il2cpp_vcall_write_s32(pw, PW_WRITE_S32, (int32_t)playerStatus->fields.id);

        auto* nameStr = playerStatus->fields.name;
        if (nameStr != nullptr) {
            int nameLen = nameStr->fields.m_stringLength;
            if (nameLen > 12) nameLen = 12;
            il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, (uint8_t)nameLen);
            for (int i = 0; i < nameLen * 2; i++) {
                uint8_t ch = ((uint8_t*)&nameStr->fields.m_firstChar)[i];
                il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, ch);
            }
        } else {
            il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, 0);
        }

        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, playerStatus->fields.sex ? 1 : 0);
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, playerStatus->fields.region_code); // lang
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, playerStatus->fields.fashion);
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, playerStatus->fields.body_type);
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, 0); // hat (not in MYSTATUS, write 0)
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, 0); // shoes (not in MYSTATUS, write 0)
    } else {
        il2cpp_vcall_write_s32(pw, PW_WRITE_S32, 0); // id
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, 0); // nameLen
        for (int i = 0; i < 6; i++) {
            il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, 0);
        }
    }

    Dpr::NetworkUtils::NetworkManager::SendReliablePacketToAll(pw, 0);
    Logger::log("[OverworldMP] Sent battle HEADER: target=%d members=%d subtype=%d\n",
                targetStation, (int)memberCount, (int)subtype);

    // -----------------------------------------------------------------------
    // Per-Pokemon sub-packets (type=1): one packet per party member
    // ~351 bytes each (1 + 4 + 1 + 1 + 344) — fits in 1024-byte buffer
    // -----------------------------------------------------------------------
    for (uint32_t i = 0; i < memberCount; i++) {
        auto* poke = party->GetMemberPointer(i);

        uint8_t pokeBuf[344];
        memset(pokeBuf, 0, sizeof(pokeBuf));

        if (poke != nullptr) {
            auto* accessor = poke->fields.m_accessor;
            auto* coreParam = (Pml::PokePara::CoreParam*)poke;
            int32_t monsNo = coreParam->GetMonsNo();
            int32_t seikaku = coreParam->GetSeikaku();
            uint32_t level = coreParam->GetLevel();
            Logger::log("[OverworldMP] SendParty[%d]: monsNo=%d seikaku=%d level=%u\n",
                        i, monsNo, seikaku, level);

            if (accessor != nullptr) {
                // Serialize_FullData (raw pointer version) @ 0x24A4470
                _ILExternal::external<void>(0x24A4470, accessor, pokeBuf);

                uint32_t rnd = 0;
                memcpy(&rnd, pokeBuf, 4);
                Logger::log("[OverworldMP] SendParty[%d]: personalRnd=0x%08x\n", i, rnd);
            } else {
                Logger::log("[OverworldMP] SendParty[%d]: accessor NULL — sending zeros\n", i);
            }
        } else {
            Logger::log("[OverworldMP] SendParty[%d]: poke NULL — sending zeros\n", i);
        }

        il2cpp_vcall_void(pw, PW_RESET);
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, OWMP_DATA_ID_BATTLE_PARTY);
        il2cpp_vcall_write_s32(pw, PW_WRITE_S32, targetStation);
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, BATTLE_PARTY_SUB_POKE);
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, (uint8_t)i);

        // Write 86 x int32 (344 bytes)
        for (int j = 0; j < 86; j++) {
            int32_t val = 0;
            memcpy(&val, &pokeBuf[j * 4], 4);
            il2cpp_vcall_write_s32(pw, PW_WRITE_S32, val);
        }

        Dpr::NetworkUtils::NetworkManager::SendReliablePacketToAll(pw, 0);
        Logger::log("[OverworldMP] Sent battle POKE[%d]\n", i);
    }

    Logger::log("[OverworldMP] Battle party send complete: %d packets (1 header + %d pokemon)\n",
                1 + (int)memberCount, (int)memberCount);
}

void overworldMPSendBattleReady(int32_t targetStation) {
    if (!isOverworldMPActive() || s_mpContext.state != OverworldMPState::Connected) {
        Logger::log("[OverworldMP] Cannot send battle ready — not connected\n");
        return;
    }

    void* pw = Dpr::NetworkUtils::NetworkManager::get_PacketWriterRe();
    if (pw == nullptr) return;

    // Packet: [DataID:0xC7][targetStation:4]
    il2cpp_vcall_void(pw, PW_RESET);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, OWMP_DATA_ID_BATTLE_READY);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, targetStation);

    Dpr::NetworkUtils::NetworkManager::SendReliablePacketToAll(pw, 0);
    Logger::log("[OverworldMP] Sent BATTLE_READY to station %d\n", targetStation);
}

// overworldMPShowInteractionMenu is defined in commands/overworld_mp_interact.cpp

// ---------------------------------------------------------------------------
// Hooks
// ---------------------------------------------------------------------------

// Track previous setting value for edge detection
static bool s_prevSettingEnabled = false;

// Hook ShowMessageWindow.Setup to null-guard the instance.
// IE_Start calls Setup(NM._showMessageWindow, ...) but for runtime-created NMs,
// NM+0x98 (_showMessageWindow) is null. The original Setup immediately dereferences
// the instance pointer, so we need to guard it.
// ShowMessageWindow.Setup(instance, msbtFile, msgSpeed, param4) @ 0x18629E0
HOOK_DEFINE_TRAMPOLINE(ShowMessageWindow$$Setup) {
    static void Callback(void* __this, void* msbtFile, int32_t msgSpeed, uint8_t param4) {
        if (__this == nullptr) {
            Logger::log("[OverworldMP] ShowMessageWindow.Setup skipped (null instance)\n");
            return;
        }
        Orig(__this, msbtFile, msgSpeed, param4);
    }
};

// Diagnostic: hook CheckReceivePacketImpl to trace packet reception.
// CheckReceivePacketImpl @ 0x1DEA970
// Signature: void(NM* this, int transportType, int stationIndex, delegate* onRecv, delegate* onRecvEx)
static int32_t s_crpiLogCount = 0;
static int32_t s_crpiType2LogCount = 0;
HOOK_DEFINE_TRAMPOLINE(NetworkManager$$CheckReceivePacketImpl) {
    static void Callback(void* __this, int32_t transportType, int32_t stationIndex,
                         void* onRecvCB, void* onRecvExCB) {
        // Log first few calls and periodically for transportType=0 (our packets)
        if (transportType == 0 && isOverworldMPActive()) {
            s_crpiLogCount++;
            if (s_crpiLogCount <= 5 || s_crpiLogCount % 300 == 0) {
                Logger::log("[NetDiag] CheckReceivePacketImpl: type=%d station=%d onRecv=%p onRecvEx=%p\n",
                            transportType, stationIndex, onRecvCB, onRecvExCB);
            }
        }
        // Diagnostic: log transport type 2 calls (battle packets) to check if
        // the transport exists and the ExCallback is registered.
        if (transportType == 2 && isOverworldMPActive() && overworldMPIsInBattleScene()) {
            s_crpiType2LogCount++;
            if (s_crpiType2LogCount <= 10 || s_crpiType2LogCount % 500 == 0) {
                // Check if transport exists by reading SessionConnector
                void* sc = *(void**)((uintptr_t)__this + 0x30);
                Logger::log("[NetDiag] CRPI type=2 station=%d exCB=%p SC=%p (call #%d)\n",
                            stationIndex, onRecvExCB, sc, s_crpiType2LogCount);
            }
        }
        Orig(__this, transportType, stationIndex, onRecvCB, onRecvExCB);
    }
};

// Hook ReceivePacketCallback.Invoke — intercept packet delivery directly.
// ReceivePacketCallback.Invoke @ 0x1BC5620
// Signature: void Invoke(delegate* this, PacketReader* pr, MethodInfo* method)
//
// CheckReceivePacketImpl calls this for every received packet when
// onReceivePacket (static_fields[0]) is non-null. Instead of creating a
// working delegate with invoke_impl (which failed 3 times due to thunk
// dispatch issues), we hook Invoke itself. During overworld MP, we call
// our handler directly and skip the delegate dispatch. For non-MP paths
// (underground/union room), we pass through to Orig normally.
HOOK_DEFINE_TRAMPOLINE(ReceivePacketCallback$$Invoke) {
    static void Callback(void* __this, void* pr, void* method) {
        if (isOverworldMPActive()) {
            if (s_inBattleScene) {
                // During battle: DON'T read from PacketReader. Both
                // ReceivePacketCallback.Invoke and ReceivePacketExCallback.Invoke
                // share the same PacketReader in CheckReceivePacketImpl. If we
                // read the dataId here, the battle's ExCallback gets a corrupted
                // reader (position off by 1+). Return without touching PR so the
                // ExCallback receives it intact.
                return;
            }
            // Call our packet handler directly — bypasses delegate dispatch
            onOverworldMPReceivePacket(pr, nullptr);
            return;
        }
        // Normal path for underground/union room delegates
        Orig(__this, pr, method);
    }
};

// Hook NM.OnSessionError — suppress error applet during overworld MP.
// NM.OnSessionError @ 0x1DEAC40 — called when PIA detects a session error
// (disconnect, transport failure, etc.). The default handler opens nn::err
// ErrorApplet which halts the game. For overworld MP, we handle disconnects
// gracefully via the SC event hook + session health polling instead.
HOOK_DEFINE_TRAMPOLINE(NetworkManager$$OnSessionError) {
    static void Callback(void* __this, int32_t errorType) {
        if (isOverworldMPActive()) {
            Logger::log("[OverworldMP] NM.OnSessionError SUPPRESSED (errorType=%d)\n", errorType);
            return;  // Don't call Orig — prevents ErrorApplet
        }
        Orig(__this, errorType);
    }
};

// Hook NM.IsShowApplicationErrorDialog — return false during overworld MP.
// IsShowApplicationErrorDialog @ 0x1DEB4D0 — static method checked by UI code
// to decide whether to show a network error dialog. During overworld MP, we
// never want to show error dialogs — we handle disconnects silently.
HOOK_DEFINE_TRAMPOLINE(NetworkManager$$IsShowApplicationErrorDialog) {
    static bool Callback() {
        if (isOverworldMPActive()) {
            return false;
        }
        return Orig();
    }
};

// Hook NetworkManager.Start() — guards against duplicate calls and logs.
// NM.Start() @ 0x1DE62F0
// The manual Start() call in ensureNetworkManagerSingleton sets s_nmStartedInstance.
// When Unity's lifecycle calls Start() again on the same instance, we block it to
// prevent a second IE_Start coroutine from corrupting the already-started session.
HOOK_DEFINE_TRAMPOLINE(NetworkManager$$Start) {
    static void Callback(void* __this) {
        if (__this == s_nmStartedInstance) {
            Logger::log("[OverworldMP] NM.Start() BLOCKED (duplicate for %p)\n", __this);
            return;
        }
        s_nmStartedInstance = __this;
        Logger::log("[OverworldMP] NM.Start() called, this=%p\n", __this);
        Orig(__this);
    }
};

// Hook NetworkManager.OnUpdate — sets s_nmOnUpdateReady on first invocation.
// NM.OnUpdate @ 0x1DEA7F0
// This is the definitive signal that IE_Start has completed: the Sequencer
// only calls OnUpdate after IE_Start registers it via Sequencer.SubscribeUpdate.
// Without this, the critical per-frame chain never fires:
//   NM.OnUpdate → SC.OnUpdate → IlcaNetSession.Update → UpdateCore → PiaPlugin.Dispatch()
//
// IMPORTANT: The NM singleton is hidden during FM.Update → Orig() to prevent the
// game's zone change code from entering underground-specific paths. But NM.OnUpdate
// is called by the Sequencer INSIDE Orig(), and its internal code accesses the
// singleton via static methods (get_PacketWriter, etc.). We temporarily restore
// the singleton for the duration of NM.OnUpdate, then re-hide it.
HOOK_DEFINE_TRAMPOLINE(NetworkManager$$OnUpdate) {
    static void Callback(void* __this) {
        if (!s_nmOnUpdateReady) {
            s_nmOnUpdateReady = true;
            Logger::log("[OverworldMP] NM.OnUpdate first call — Sequencer subscription confirmed\n");
        }

        // Block during grace period as safety measure — prevent PIA ticking
        // while scene state is settling after a zone transition.
        if (s_zoneChangeGraceTime > 0.0f) {
            return;
        }

        // Diagnostic: check what CheckReceivePacket will see (~every 5s)
        if (isOverworldMPActive() && s_mpContext.frameCounter % 150 == 1) {
            void* sc = *(void**)((uintptr_t)__this + 0x30);
            if (sc != nullptr) {
                bool bRunning = *(bool*)((uintptr_t)sc + 0x60);
                void* pr = *(void**)((uintptr_t)__this + 0x40);
                auto* nmKlass = Dpr::NetworkUtils::NetworkManager::getClass();
                void* delegate = nmKlass->static_fields->onReceivePacket;
                Logger::log("[NetDiag] OnUpdate pre-Orig: bRunning=%d PR=%p delegate=%p NM=%p SC=%p\n",
                            (int)bRunning, pr, delegate, __this, sc);
            } else {
                Logger::log("[NetDiag] OnUpdate pre-Orig: SC=NULL!\n");
            }
        }

        // NM singleton is no longer hidden during FM.Update — no restore needed.
        Orig(__this);
    }
};

// Hook NetworkManager.OnLateUpdate — same singleton restore/hide pattern.
// NM.OnLateUpdate @ 0x1DEAB00
// Called by Sequencer during FM.Update Orig(), after OnUpdate.
HOOK_DEFINE_TRAMPOLINE(NetworkManager$$OnLateUpdate) {
    static void Callback(void* __this) {
        // Block during grace period as safety measure
        if (s_zoneChangeGraceTime > 0.0f) {
            return;
        }

        // NM singleton is no longer hidden during FM.Update — no restore needed.
        Orig(__this);
    }
};

// ---------------------------------------------------------------------------
// PIA state machine monitoring hooks (read-only, no behavior modification)
// ---------------------------------------------------------------------------
// The PIA state machine progresses naturally via the Sequencer:
//   NM.OnUpdate → SC.OnUpdate → IlcaNetSession.Update → UpdateCore
//   UpdateCore: ... → UpdateDispatchCore → UpdateSwitchCore → GameState_X
// We just observe state transitions — no synchronous PIA calls or dispatch hacks.

// Set to 0 to DISABLE the IlcaNetSession.Update hook as a test.
// If the crash disappears when this is 0, the trampoline's ADRP relocation is
// corrupting a register used by PlatformInitialize.
#define ENABLE_SESSION_UPDATE_HOOK 0

// Hook IlcaNetSession.Update to monitor state machine progression.
// Returns the current session state, called every frame from NM.OnUpdate → SC.OnUpdate.
// Only logs state CHANGES to avoid spamming the log.
// ALWAYS logs (not just OverworldMP active) so we capture the underground's state too.
// IlcaNetSession.Update @ 0x23BAA90 — static, returns ulong (session state)
#if ENABLE_SESSION_UPDATE_HOOK
static bool s_loggedFirstPostSession = false;
HOOK_DEFINE_TRAMPOLINE(IlcaNetSession$$Update) {
    static uint64_t Callback() {
        // Log ONCE before the first Orig() after session start to confirm crash is inside
        if (s_mpContext.state == OverworldMPState::Searching && !s_loggedFirstPostSession) {
            Logger::log("[NetDiag] IlcaNetSession.Update: first post-session call, entering Orig()...\n");
            s_loggedFirstPostSession = true;
        }
        uint64_t result = Orig();
        uint32_t state = (uint32_t)result;
        if (state != s_lastLoggedPiaState) {
            Logger::log("[NetDiag] IlcaNetSession.Update state: %u -> %u\n",
                        s_lastLoggedPiaState, state);
            s_lastLoggedPiaState = state;
        }
        return result;
    }
};
#endif

// Hook IlcaNetBase.PlatformInitialize @ 0x2CDA460
// DISABLED: untested trampoline may have ADRP relocation issue.
// If re-enabling, search for ENABLE_PLATFORMINIT_HOOK.
#define ENABLE_PLATFORMINIT_HOOK 0
#if ENABLE_PLATFORMINIT_HOOK
HOOK_DEFINE_TRAMPOLINE(IlcaNetBase$$PlatformInitialize) {
    static void Callback(void* resultOut, void* methodInfo) {
        Logger::log("[NetDiag] IlcaNetBase.PlatformInitialize: ENTERING\n");
        auto* ilcaBaseKlass = *(Il2CppClass**)exl::util::modules::GetTargetOffset(0x04C604D8);
        if (ilcaBaseKlass != nullptr) {
            void* sf = ilcaBaseKlass->static_fields;
            if (sf != nullptr) {
                void* settingPtr = *(void**)((uintptr_t)sf + 0x28);
                uint32_t gameId = *(uint32_t*)((uintptr_t)sf + 0x58);
                Logger::log("[NetDiag]   IlcaNetBase statics: setting=%p gameId=%u\n",
                            settingPtr, gameId);
            }
        }
        Orig(resultOut, methodInfo);
        Logger::log("[NetDiag] IlcaNetBase.PlatformInitialize: RETURNED\n");
    }
};
#endif

// ---------------------------------------------------------------------------
// Diagnostic hooks — capture both underground and overworld networking paths
// ---------------------------------------------------------------------------
// These hooks log key parameters at each stage of session initialization so
// we can compare the working underground path with the failing overworld path.
// Tagged with [NetDiag] for easy grep filtering.

// Hook NM.DoStartSession @ 0x1DE69B0
// Called by both underground (via StartSessionRandomJoin) and our overworld code.
// Logs the NetworkParam fields and which path is calling.
// Signature: void DoStartSession(NM* this, Action<StartSessionResult> onComplete)
HOOK_DEFINE_TRAMPOLINE(NetworkManager$$DoStartSession) {
    static void Callback(void* __this, void* onComplete) {
        // Read NetworkParam from NM+0x38
        void* np = *(void**)((uintptr_t)__this + 0x38);
        if (np != nullptr) {
            int32_t networkType = *(int32_t*)((uintptr_t)np + 0x10);
            int32_t netTypeDetail = *(int32_t*)((uintptr_t)np + 0x14);
            int32_t gamingStartMode = *(int32_t*)((uintptr_t)np + 0x18);
            int32_t unknown1c = *(int32_t*)((uintptr_t)np + 0x1c);
            int16_t matchingType = *(int16_t*)((uintptr_t)np + 0x20);
            int16_t maxPlayers = *(int16_t*)((uintptr_t)np + 0x22);
            void* attrA = *(void**)((uintptr_t)np + 0x38);
            void* attrB = *(void**)((uintptr_t)np + 0x40);
            void* attrC = *(void**)((uintptr_t)np + 0x48);
            Logger::log("[NetDiag] DoStartSession: NM=%p onComplete=%p\n", __this, onComplete);
            Logger::log("[NetDiag]   NetworkParam=%p: networkType=%d detail=%d gamingStart=%d\n",
                        np, networkType, netTypeDetail, gamingStartMode);
            Logger::log("[NetDiag]   matchingType=%d maxPlayers=%d unknown1c=%d\n",
                        (int)matchingType, (int)maxPlayers, unknown1c);
            Logger::log("[NetDiag]   attrs: %p %p %p\n", attrA, attrB, attrC);
        } else {
            Logger::log("[NetDiag] DoStartSession: NM=%p NetworkParam=NULL\n", __this);
        }

        // Read SessionConnector state
        void* sc = *(void**)((uintptr_t)__this + 0x30);
        if (sc != nullptr) {
            bool bRunning = *(bool*)((uintptr_t)sc + 0x60);
            void* callObj = *(void**)((uintptr_t)sc + 0x58);
            int32_t sessionState = *(int32_t*)((uintptr_t)sc + 0x50);
            Logger::log("[NetDiag]   SC=%p bRunning=%d callObjPtr=%p sessionState=%d\n",
                        sc, (int)bRunning, callObj, sessionState);
        }

        Orig(__this, onComplete);

        // Log post-call state
        if (sc != nullptr) {
            bool bRunning = *(bool*)((uintptr_t)sc + 0x60);
            Logger::log("[NetDiag]   DoStartSession returned, bRunning=%d\n", (int)bRunning);
        }
    }
};

// Hook SC.StartSession @ 0x1BC6940
// Called by DoStartSession. Logs the SC internal state and NetworkParam being applied.
// Signature: void StartSession(SC* this, NetworkParam* param, Action<StartSessionResult> onComplete)
HOOK_DEFINE_TRAMPOLINE(SessionConnector$$StartSession) {
    static void Callback(void* __this, void* networkParam, void* onComplete) {
        Logger::log("[NetDiag] SC.StartSession: SC=%p NP=%p onComplete=%p\n",
                    __this, networkParam, onComplete);

        // SC state before call
        // SC layout (from header): +0x18=onSessionEvent, +0x48=sessionSetting, +0x50=nowSessionState
        // +0x58=callObjPtr, +0x60=bRunningSession
        int32_t scState = *(int32_t*)((uintptr_t)__this + 0x50);
        bool bRunning = *(bool*)((uintptr_t)__this + 0x60);
        void* callObj = *(void**)((uintptr_t)__this + 0x58);
        void* sessionSetting = *(void**)((uintptr_t)__this + 0x48);
        Logger::log("[NetDiag]   SC pre-state: nowSessionState=%d bRunning=%d callObjPtr=%p setting=%p\n",
                    scState, (int)bRunning, callObj, sessionSetting);

        // Log IlcaNetSession setting object fields (param_1[9] = SC+0x48 = sessionSetting)
        if (sessionSetting != nullptr) {
            int32_t settingNetType = *(int32_t*)((uintptr_t)sessionSetting + 0x44);
            int32_t settingInitMode = *(int32_t*)((uintptr_t)sessionSetting + 0x48);
            int16_t settingMatchType = *(int16_t*)((uintptr_t)sessionSetting + 0x4c);
            int16_t settingMaxPlayers = *(int16_t*)((uintptr_t)sessionSetting + 0x7a);
            Logger::log("[NetDiag]   Setting pre: netType=%d initMode=%d matchType=%d maxPlayers=%d\n",
                        settingNetType, settingInitMode, (int)settingMatchType, (int)settingMaxPlayers);
        }

        Orig(__this, networkParam, onComplete);

        // Post-call state
        int32_t scStateAfter = *(int32_t*)((uintptr_t)__this + 0x50);
        bool bRunningAfter = *(bool*)((uintptr_t)__this + 0x60);
        Logger::log("[NetDiag]   SC post-state: nowSessionState=%d bRunning=%d\n",
                    scStateAfter, (int)bRunningAfter);

        if (sessionSetting != nullptr) {
            int32_t settingNetType = *(int32_t*)((uintptr_t)sessionSetting + 0x44);
            int32_t settingInitMode = *(int32_t*)((uintptr_t)sessionSetting + 0x48);
            int16_t settingMaxPlayers = *(int16_t*)((uintptr_t)sessionSetting + 0x7a);
            int32_t settingGamingStart = *(int32_t*)((uintptr_t)sessionSetting + 0xa0);
            Logger::log("[NetDiag]   Setting post: netType=%d initMode=%d maxPlayers=%d gamingStart=%d\n",
                        settingNetType, settingInitMode, (int)settingMaxPlayers, settingGamingStart);
        }
    }
};

// Hook IlcaNetSession.NetworkInitAsync @ 0x23B9F30
// Called during SC.StartSession to initialize PIA. Logs the setting object state
// which determines how PIA is configured.
// Signature: bool NetworkInitAsync(IlcaNetSessionSetting* setting) — static
HOOK_DEFINE_TRAMPOLINE(IlcaNetSession$$NetworkInitAsync) {
    static uint64_t Callback(void* setting) {
        Logger::log("[NetDiag] IlcaNetSession.NetworkInitAsync: setting=%p\n", setting);

        if (setting != nullptr) {
            // Key fields from Ghidra decompilation of NetworkInitAsync:
            // setting+0x44 = networkType
            // setting+0x48 = initMode
            // setting+0x4c = matchingType
            // setting+0x7a = maxPlayers
            // setting+0x7c = flag1
            // setting+0x7d = flag2
            // setting+0x80 = lastEventLeave
            // setting+0xa0 = gamingStartMode
            int32_t netType = *(int32_t*)((uintptr_t)setting + 0x44);
            int32_t initMode = *(int32_t*)((uintptr_t)setting + 0x48);
            int16_t matchType = *(int16_t*)((uintptr_t)setting + 0x4c);
            int16_t maxPlayers = *(int16_t*)((uintptr_t)setting + 0x7a);
            uint8_t flag1 = *(uint8_t*)((uintptr_t)setting + 0x7c);
            uint8_t flag2 = *(uint8_t*)((uintptr_t)setting + 0x7d);
            int32_t lastEvtLeave = *(int32_t*)((uintptr_t)setting + 0x80);
            int32_t gamingStart = *(int32_t*)((uintptr_t)setting + 0xa0);
            Logger::log("[NetDiag]   netType=%d initMode=%d matchType=%d maxPlayers=%d\n",
                        netType, initMode, (int)matchType, (int)maxPlayers);
            Logger::log("[NetDiag]   flag1=%d flag2=%d lastEvtLeave=%d gamingStart=%d\n",
                        (int)flag1, (int)flag2, lastEvtLeave, gamingStart);

            // Log nn_account_Uid (setting+0x10, 16 bytes)
            uint64_t uid_lo = *(uint64_t*)((uintptr_t)setting + 0x10);
            uint64_t uid_hi = *(uint64_t*)((uintptr_t)setting + 0x18);
            Logger::log("[NetDiag]   uid: %016llx %016llx\n", uid_lo, uid_hi);

            // Log session attributes (setting+0x50, 8 bytes — an object ref)
            void* attrStr = *(void**)((uintptr_t)setting + 0x50);
            Logger::log("[NetDiag]   sessionAttrStr=%p\n", attrStr);
        }

        // Check IlcaNetSession state statics (static_fields+0x08 = updateState, +0x0c = subState)
        // IlcaNetSession_TypeInfo @ 0x4C59E40
        auto* ilcaSessionKlass = *(Il2CppClass**)exl::util::modules::GetTargetOffset(0x04C59E40);
        if (ilcaSessionKlass != nullptr) {
            auto* sf = *(void**)((uintptr_t)ilcaSessionKlass + 0xb8);
            if (sf != nullptr) {
                int32_t sessionUpdateState = *(int32_t*)((uintptr_t)sf + 0x08);
                int32_t sessionSubState = *(int32_t*)((uintptr_t)sf + 0x0c);
                Logger::log("[NetDiag]   IlcaNetSession statics: updateState=%d subState=%d\n",
                            sessionUpdateState, sessionSubState);
            }
        }

        uint64_t result = Orig(setting);
        Logger::log("[NetDiag]   NetworkInitAsync returned: %llu\n", result);
        return result;
    }
};

// Hook IlcaNetSession.Init (2-arg version) @ 0x23B9850
// Called during SC.StartSession before NetworkInitAsync.
// arg1 = callObjPtr (the owning MonoBehaviour, typically NM), stored in IlcaNetBase.static_fields[0]
// arg2 = IlcaNetSessionFinalCallback
// Signature: bool Init(void* callObj, void* finalCallback) — static
HOOK_DEFINE_TRAMPOLINE(IlcaNetSession$$Init) {
    static uint64_t Callback(void* callObj, void* finalCallback) {
        Logger::log("[NetDiag] IlcaNetSession.Init: callObj=%p callback=%p\n",
                    callObj, finalCallback);

        // Check IlcaNetBase.isInit before call (static_fields+0x0A)
        auto* ilcaBaseKlass = *(Il2CppClass**)exl::util::modules::GetTargetOffset(0x04C604D8);
        if (ilcaBaseKlass != nullptr) {
            auto* sf = *(void**)((uintptr_t)ilcaBaseKlass + 0xb8);
            if (sf != nullptr) {
                bool isInit = *(bool*)((uintptr_t)sf + 0x0A);
                Logger::log("[NetDiag]   IlcaNetBase.isInit=%d (before)\n", (int)isInit);
            }
        }

        uint64_t result = Orig(callObj, finalCallback);
        Logger::log("[NetDiag]   Init returned: %llu\n", result);
        return result;
    }
};

HOOK_DEFINE_TRAMPOLINE(FieldManager$$Update) {
    static void Callback(FieldManager::Object* __this) {
        // Log at start of frame when in Searching state to track execution order vs crash
        if (s_mpContext.state == OverworldMPState::Searching && s_searchingFrameCount < 5) {
            Logger::log("[NetDiag] FM.Update TOP — Searching frame %d (pre-Orig)\n",
                        s_searchingFrameCount + 1);
        }

        // IMPORTANT: The NM singleton is NO LONGER hidden during Orig().
        //
        // Previous approach: null the singleton so zone-change code wouldn't see
        // an active networking session. But this CAUSED the crash — game code
        // checks networking state via OTHER flags (IlcaNetSession.isInit,
        // Common.s_updateAlive, etc.), decides networking IS active, then tries
        // to use the NM singleton → finds null → null deref at 0x260270.
        //
        // New approach: let the game see the NM singleton normally. If specific
        // underground-only code paths cause issues, we'll hook those individually
        // rather than hiding the entire singleton.
        //
        // The NM.OnUpdate/OnLateUpdate hooks still temporarily restore the
        // singleton if it's somehow null (belt-and-suspenders for the hidden
        // singleton path that's now removed).

        Orig(__this);

        // Poll the overworld MP setting for toggle detection
        bool enabled = getCustomSaveData()->settings.overworldMultiplayer;
        if (enabled != s_prevSettingEnabled) {
            s_prevSettingEnabled = enabled;
            if (enabled) {
                overworldMPStart();
            } else {
                overworldMPStop();
            }
        }

        // Periodic self-heal: if the setting is on but the session is Disabled
        // (e.g. after a dead session, failed restart, or edge case), restart it.
        // Check every ~10 seconds to avoid spamming.
        if (enabled && s_mpContext.state == OverworldMPState::Disabled) {
            static float s_selfHealAccumulator = 0.0f;
            s_selfHealAccumulator += UnityEngine::Time::get_deltaTime();
            if (s_selfHealAccumulator >= 10.0f) {
                s_selfHealAccumulator = 0.0f;
                Logger::log("[OverworldMP] Self-heal: setting ON but session Disabled, restarting...\n");
                overworldMPStart();
            }
        }

        // Detect area changes (replaces OnZoneChange hook — trampoline on
        // OnZoneChange @ 0x01798CC0 caused crashes in native Unity code).
        // After Orig() returns, the zone transition is complete and the new
        // areaID is available. We deactivate DDOL entities and set a grace
        // period before spawning in the new area.
        if (isOverworldMPActive()) {
            int32_t currentArea = __this->get_areaID();
            if (currentArea != s_mpContext.myAreaID && s_mpContext.myAreaID != 0) {
                Logger::log("[OverworldMP] === ZONE CHANGE DETECTED: %d -> %d ===\n",
                            s_mpContext.myAreaID, currentArea);

                // Log session state at the moment of zone change
                int32_t sessionState = Dpr::NetworkUtils::NetworkManager::get_SessionState();
                Logger::log("[OverworldMP] Session state at zone change: %d\n", sessionState);
                // NM singleton is no longer hidden — no GO deactivation needed.
                // The crash was caused by hiding the NM singleton: game code
                // checked other networking flags, found active, accessed singleton
                // (which we nulled) → null deref at 0x260270.

                spawnQueueClear();
                // Explicitly destroy all remote player entities. Don't rely on
                // Unity's scene transition to clean them up — some zone changes
                // don't trigger a full scene unload, leaving orphan entities
                // that become duplicates when we spawn fresh ones post-grace.
                for (int i = 0; i < OW_MP_MAX_PLAYERS; i++) {
                    // Destroy follow pokemon entity
                    auto& rp = s_mpContext.remotePlayers[i];
                    if (rp.followPokeEntity != nullptr) {
                        auto* pokeEnt = (FieldObjectEntity::Object*)rp.followPokeEntity;
                        auto* pokeGo = pokeEnt->cast<UnityEngine::Component>()->get_gameObject();
                        if (pokeGo != nullptr) {
                            UnityEngine::_Object::Destroy(pokeGo->cast<UnityEngine::_Object>());
                        }
                    }
                    rp.followPokeSpawned = false;
                    rp.followPokeEntity = nullptr;

                    // Destroy player entity
                    if (s_mpContext.spawnedEntities[i] != nullptr) {
                        auto* entity = (FieldObjectEntity::Object*)s_mpContext.spawnedEntities[i];
                        auto* go = entity->cast<UnityEngine::Component>()->get_gameObject();
                        if (go != nullptr) {
                            UnityEngine::_Object::Destroy(go->cast<UnityEngine::_Object>());
                        }
                    }
                    rp.isSpawned = false;
                    s_mpContext.spawnedEntities[i] = nullptr;
                }

                // Reset interaction state on zone change
                if (s_interactionState != InteractionState::None) {
                    Logger::log("[OverworldMP] Cancelling interaction (state=%d) due to zone change\n",
                                (int)s_interactionState);
                    s_interactionState = InteractionState::None;
                    s_interactionTarget = -1;
                    s_interactionTimeoutTime = 0.0f;
                    s_pendingInteractionType = InteractionType::Trade;
                }

                s_mpContext.myAreaID = currentArea;
                s_zoneChangeGraceTime = OW_MP_ZONE_CHANGE_GRACE_SEC;
                Logger::log("[OverworldMP] Grace period started: %.1fs\n",
                            OW_MP_ZONE_CHANGE_GRACE_SEC);
            }
        }

        // Run update when session is active OR initializing (waiting for IE_Start)
        if (s_mpContext.state == OverworldMPState::Disabled ||
            s_mpContext.state == OverworldMPState::Disconnecting ||
            s_mpContext.state == OverworldMPState::Error) {
            return;
        }

        // Get real deltaTime from Unity — works correctly at any framerate
        float deltaTime = UnityEngine::Time::get_deltaTime();

        overworldMPUpdate(deltaTime);
    }
};

// Hook FieldManager.OnZoneChange() to manage area-based visibility.
// The hook fires on every zone transition, but we only act on AREA changes.
// Zone changes within the same area are transparent to the MP system —
// remote entities persist across intra-area zone boundaries.
// FieldManager.OnZoneChange() @ 0x01798CC0
HOOK_DEFINE_TRAMPOLINE(FieldManager$$OnZoneChange) {
    static void Callback(FieldManager::Object* __this) {
        // Entities are NOT DontDestroyOnLoad — they'll be destroyed by Unity
        // during the scene transition. Just null our references.
        if (isOverworldMPActive()) {
            spawnQueueClear();
            for (int i = 0; i < OW_MP_MAX_PLAYERS; i++) {
                s_mpContext.remotePlayers[i].isSpawned = false;
                s_mpContext.remotePlayers[i].followPokeSpawned = false;
                s_mpContext.remotePlayers[i].followPokeEntity = nullptr;
                s_mpContext.spawnedEntities[i] = nullptr;
            }
        }

        Orig(__this);

        if (!isOverworldMPActive()) {
            return;
        }

        int32_t newAreaID = __this->get_areaID();
        if (newAreaID != s_mpContext.myAreaID) {
            Logger::log("[OverworldMP] Local area change: %d -> %d\n",
                        s_mpContext.myAreaID, newAreaID);
            s_mpContext.myAreaID = newAreaID;
        }

        s_zoneChangeGraceTime = OW_MP_ZONE_CHANGE_GRACE_SEC;
    }
};

// Hook SessionConnector.OnSessionEventCallback to intercept join/leave events.
// SC.OnSessionEventCallback @ 0x1BC8670 — called by PIA (INL1_IlcaNetSession)
// when a station joins, leaves, or disconnects. Hooked at the SC level for
// robustness — doesn't depend on the delegate chain from SC to NM.
//
// Raw event types: 0=join, 1=leave, 2=disconnect
// Orig() processes these into eventType 1-6 using ThisStationIndex():
//   1=self_joined, 2=other_joined, 3=self_disconnected,
//   4=other_disconnected, 5=self_left, 6=other_left
// and stores them at SC+0x40 (stationIndex) and SC+0x44 (eventType).
//
// IMPORTANT: For overworld MP disconnect/leave events, we handle them
// BEFORE calling Orig(). Orig() propagates the event through the game's
// normal networking error path (SC → NM → ErrorApplet). For the overworld
// MP we want graceful disconnect, not an error dialog.
HOOK_DEFINE_TRAMPOLINE(SessionConnector$$OnSessionEventCallback) {
    static void Callback(void* __this, int32_t rawEventType, int32_t stationIndex) {
        // Non-MP sessions: pass through to normal game handling
        if (!isOverworldMPActive()) {
            Orig(__this, rawEventType, stationIndex);
            return;
        }

        // For overworld MP, we handle events ourselves to prevent ErrorApplet.
        // Orig() propagates disconnect/leave events through the game's normal
        // error path (SC → NM error handler → ErrorApplet), which blocks the
        // game thread until the user dismisses the dialog. We SKIP Orig() for
        // disconnect/leave events and process them directly.
        //
        // Raw event types: 0=join, 1=leave, 2=disconnect
        // We classify them ourselves without needing Orig()'s processing.

        if (rawEventType == 0) {
            // Join event — safe to call Orig() (no error path)
            Orig(__this, rawEventType, stationIndex);
            int32_t eventType = *(int32_t*)((uintptr_t)__this + 0x44);
            Logger::log("[OverworldMP] Session event: raw=%d station=%d type=%d\n",
                        rawEventType, stationIndex, eventType);
            if (eventType == 2) {
                // Another player joined
                overworldMPOnPlayerJoin(stationIndex);
            }
            // eventType==1 (self_joined) — no action needed
        } else {
            // Leave (1) or disconnect (2) — DO NOT call Orig() to prevent ErrorApplet.
            // Treat all leave/disconnect as "other player left". If it's actually us
            // disconnecting, the session health check will detect state=9 and restart.
            Logger::log("[OverworldMP] Session event (no Orig): raw=%d station=%d\n",
                        rawEventType, stationIndex);
            overworldMPOnPlayerLeave(stationIndex);
        }
    }
};

// ---------------------------------------------------------------------------
// IlcaNetComponent.Update hook — BLOCKS PIA tick during zone transitions
// ---------------------------------------------------------------------------
// IlcaNetComponent.Update() @ 0x2CDC280 — the per-frame PIA tick forwarder.
// Its entire body is: Common.Update(this) → NexPlugin.Common.Dispatch() → PIA tick.
//
// Behaviour.set_enabled(false) did NOT prevent Unity from calling this method
// (confirmed by diagnostic logging — the component kept ticking every frame).
// So we block at the trampoline level: skip Orig() during the grace period.
// This is a guaranteed block since we control the hook dispatch.

// ---------------------------------------------------------------------------
// Battle networking diagnostics — trace whether battle sends/receives work
// ---------------------------------------------------------------------------

// Hook NM.SendReliablePacketToAll @ 0x1DE8BD0
// Signature: int32_t SendReliablePacketToAll(void* pw, int32_t sendType)  [static]
// Internally: gets NM singleton, calls SC.SendAll(sc, pw, 1, sendType)
HOOK_DEFINE_TRAMPOLINE(NetworkManager$$SendReliablePacketToAll) {
    static int32_t Callback(void* pw, int32_t sendType) {
        int32_t result = Orig(pw, sendType);
        if (isOverworldMPActive() && overworldMPIsInBattleScene()) {
            static int32_t s_sendLogCount = 0;
            s_sendLogCount++;
            if (s_sendLogCount <= 20 || s_sendLogCount % 200 == 0) {
                int32_t sessionState = Dpr::NetworkUtils::NetworkManager::get_SessionState();
                Logger::log("[BtlNet] SendReliablePacketToAll: pw=%p sendType=%d result=%d sessState=%d (call #%d)\n",
                            pw, sendType, result, sessionState, s_sendLogCount);
            }
        }
        return result;
    }
};

// Battle comm init step hooks — NONE hooked directly.
// With the BATTLE_READY sync handshake (0xC7), both sides enter battle at the
// same time. The native comm pipeline's ServerVersion/ServerParam exchange works
// because both Net.Clients exist when packets start flowing.
//
// store_party_data, store_player_data, raid: NOT HOOKED — .eh_frame unsafe.
// determine_server: NOT HOOKED — native pipeline works with sync'd timing.
// CalcCorrectedPowerBySeikaku: HOOKED — safety clamp on seikaku to 0-24.
//   No .eh_frame issue because the clamp prevents the exception from being thrown.

// Hook NM.SendReliablePacket (per-station) @ 0x1DE89B0
// This is what the battle system uses — NOT SendReliablePacketToAll.
// Chain: sendServerVersion → Net.Client.SendReliableData<ServerVersion> (RGCTX)
//   → ANetData<ServerVersion>.SendReliableData → NM.SendReliablePacket
//   → SC.SendTo → GetTransport(transportType=2) → IlcaNetSession.static_fields+0x38[station]
// Signature: int32_t SendReliablePacket(void* data, int32_t stationIndex, int32_t transportType) [static]
HOOK_DEFINE_TRAMPOLINE(NetworkManager$$SendReliablePacket) {
    static int32_t Callback(void* data, int32_t stationIndex, int32_t transportType) {
        int32_t result = Orig(data, stationIndex, transportType);
        if (isOverworldMPActive() && overworldMPIsInBattleScene()) {
            static int32_t s_srpLogCount = 0;
            s_srpLogCount++;
            if (s_srpLogCount <= 30 || s_srpLogCount % 200 == 0) {
                Logger::log("[BtlNet] NM.SendReliablePacket: data=%p station=%d type=%d result=%d (call #%d)\n",
                            data, stationIndex, transportType, result, s_srpLogCount);
            }
        }
        return result;
    }
};

// Hook SC.SendTo @ 0x1BC7D70 — per-station send (used by battle networking)
// Signature: int32_t SendTo(SC* this, void* data, int32_t reliable, int32_t stationIndex, int32_t transportType)
HOOK_DEFINE_TRAMPOLINE(SessionConnector$$SendTo) {
    static int32_t Callback(void* __this, void* data, int32_t reliable, int32_t stationIndex, int32_t transportType) {
        int32_t result = Orig(__this, data, reliable, stationIndex, transportType);
        if (isOverworldMPActive() && overworldMPIsInBattleScene()) {
            static int32_t s_stLogCount = 0;
            s_stLogCount++;
            if (s_stLogCount <= 30 || s_stLogCount % 200 == 0) {
                Logger::log("[BtlNet] SC.SendTo: station=%d type=%d reliable=%d result=%d (call #%d)\n",
                            stationIndex, transportType, reliable, result, s_stLogCount);
            }
        }
        return result;
    }
};

// Hook Net.Client.get_Instance @ 0x203D410
// Returns the Net.Client singleton (requires BattleProc + MainModule)
// Signature: void* get_Instance()  [static]
// Chain: SingletonMonoBehaviour<BattleProc>.isInstantiated → BattleProc → +0x20 (mainModule) → +0x118 (netClient)
HOOK_DEFINE_TRAMPOLINE(NetClient$$get_Instance) {
    static void* Callback() {
        void* result = Orig();
        if (isOverworldMPActive() && overworldMPIsInBattleScene()) {
            static int32_t s_giLogCount = 0;
            s_giLogCount++;

            // FIX: If the native singleton chain returns null but we have a
            // cached Net::Client from UpdateInitialize, return the cached value.
            // This fixes determine_server: OnReceivePacketExStatic calls
            // get_Instance() to dispatch received type-2 packets. Without this,
            // all incoming ServerVersion packets are silently dropped.
            if (result == nullptr && s_cachedBattleNetClient != nullptr) {
                result = s_cachedBattleNetClient;
                if (s_giLogCount <= 20 || s_giLogCount % 500 == 0) {
                    Logger::log("[BtlNet] get_Instance: native=null, using cache=%p (call #%d)\n",
                                s_cachedBattleNetClient, s_giLogCount);
                }
            } else if (s_giLogCount <= 5 || s_giLogCount % 1000 == 0) {
                Logger::log("[BtlNet] Net.Client.get_Instance: result=%p (call #%d)\n",
                            result, s_giLogCount);
            }
        }
        return result;
    }
};

// Hook SC.SendAll @ 0x1BC7FA0
// Signature: int32_t SendAll(SC* this, void* pw, int32_t reliable, int32_t transportType)
HOOK_DEFINE_TRAMPOLINE(SessionConnector$$SendAll) {
    static int32_t Callback(void* __this, void* pw, int32_t reliable, int32_t transportType) {
        int32_t result = Orig(__this, pw, reliable, transportType);
        if (isOverworldMPActive() && overworldMPIsInBattleScene() && transportType == 2) {
            static int32_t s_saLogCount = 0;
            s_saLogCount++;
            if (s_saLogCount <= 20 || s_saLogCount % 200 == 0) {
                Logger::log("[BtlNet] SC.SendAll: type=%d reliable=%d result=%d (call #%d)\n",
                            transportType, reliable, result, s_saLogCount);
            }
        }
        return result;
    }
};

// Diagnostic: Net.Client.sendToServerVersionCoreAll @ 0x203DD80
// Traces the ServerVersion exchange over type-2 transports.
// With BATTLE_READY sync, both sides enter battle at the same time,
// so the native exchange should work without bypass.
HOOK_DEFINE_TRAMPOLINE(NetClient$$sendToServerVersionCoreAll) {
    static void Callback(void* __this) {
        if (isOverworldMPActive() && overworldMPIsInBattleScene()) {
            static int32_t s_svLogCount = 0;
            s_svLogCount++;
            if (s_svLogCount <= 30 || s_svLogCount % 200 == 0) {
                void* serverVersions = *(void**)((uintptr_t)__this + 0x18);
                uint8_t detResult = serverVersions ? *(uint8_t*)((uintptr_t)serverVersions + 0x19) : 0xFF;
                uint8_t detFlag = *(uint8_t*)((uintptr_t)__this + 0x90);
                int32_t errorBit = *(int32_t*)((uintptr_t)__this + 0x60);
                Logger::log("[BtlNet] sendToSVCoreAll: detResult=%d detFlag=%d errorBit=%d (call #%d)\n",
                            (int)detResult, (int)detFlag, errorBit, s_svLogCount);
            }
        }
        Orig(__this);
    }
};

// Hook BattleProc.UpdateInitialze @ 0x187D510
// Called each frame during battle init phase (before main loop starts).
// param_1 = BattleProc instance.  +0x20 = mainModule, +0x28 = updateCoreFunc delegate
// mainModule+0x118 = netClient, mainModule+0x120 = init complete flag
HOOK_DEFINE_TRAMPOLINE(BattleProc$$UpdateInitialze) {
    static void Callback(void* __this) {
        static int32_t s_initLogCount = 0;
        static int32_t s_prevMainStep = -1;
        static int32_t s_prevSubStep = -1;
        bool isBattle = isOverworldMPActive() && overworldMPIsInBattleScene();
        int32_t mainStep = -1;
        int32_t subStep = -1;
        uint8_t initFlag = 0;
        uint8_t errorFlag = 0;
        void* netClient = nullptr;
        if (isBattle) {
            s_initLogCount++;
            void* mainModule = *(void**)((uintptr_t)__this + 0x20);
            if (mainModule) {
                mainStep = *(int32_t*)((uintptr_t)mainModule + 0x38);
                subStep = *(int32_t*)((uintptr_t)mainModule + 0x3c);
                initFlag = *(uint8_t*)((uintptr_t)mainModule + 0x120);
                netClient = *(void**)((uintptr_t)mainModule + 0x118);
                errorFlag = *(uint8_t*)((uintptr_t)mainModule + 0xa8);

                // Cache the Net::Client pointer for the get_Instance fallback.
                // InitializeCoroutine creates it once commMode != 0.
                if (netClient != nullptr && s_cachedBattleNetClient == nullptr) {
                    s_cachedBattleNetClient = netClient;
                    Logger::log("[BtlNet] Cached Net::Client=%p from mainModule+0x118\n", netClient);
                }
            }
            // Log on: first 30 calls, every step/substep change, every 200th call
            bool stepChanged = (mainStep != s_prevMainStep || subStep != s_prevSubStep);
            if (s_initLogCount <= 30 || stepChanged || s_initLogCount % 200 == 0) {
                // Step name for readability
                const char* stepName = "?";
                switch (mainStep) {
                    case 0: stepName = "init"; break;
                    case 1: stepName = "det_server"; break;
                    case 2: stepName = "store_party"; break;
                    case 3: stepName = "store_player"; break;
                    case 4: stepName = "create_sc"; break;
                    case 5: stepName = "start_server"; break;
                    case 6: stepName = "DONE(>5)"; break;
                    default: stepName = "unknown"; break;
                }
                Logger::log("[BtlNet] UpdateInit: step=%d(%s) sub=%d initFlag=%d errFlag=%d nc=%p (call #%d)%s\n",
                            mainStep, stepName, subStep, (int)initFlag, (int)errorFlag,
                            netClient, s_initLogCount, stepChanged ? " <<CHANGED>>" : "");
                s_prevMainStep = mainStep;
                s_prevSubStep = subStep;
                // On first few calls, dump setupParam details
                if (mainModule && s_initLogCount <= 10) {
                    void* setupParam = *(void**)((uintptr_t)mainModule + 0x10);
                    if (setupParam) {
                        void* partyArr = *(void**)((uintptr_t)setupParam + 0x58);
                        void* statusArr = *(void**)((uintptr_t)setupParam + 0x68);
                        void* stationArr = *(void**)((uintptr_t)setupParam + 0x48);
                        int32_t partyLen = partyArr ? *(int32_t*)((uintptr_t)partyArr + 0x18) : -1;
                        int32_t statusLen = statusArr ? *(int32_t*)((uintptr_t)statusArr + 0x18) : -1;
                        int32_t stationLen = stationArr ? *(int32_t*)((uintptr_t)stationArr + 0x18) : -1;
                        uint8_t commMode = *(uint8_t*)((uintptr_t)setupParam + 0x38);
                        uint8_t commPos = *(uint8_t*)((uintptr_t)setupParam + 0x3a);
                        int32_t btlRule = *(int32_t*)((uintptr_t)mainModule + 0x70);
                        Logger::log("[BtlNet]   sp=%p partyArr=%p[%d] statusArr=%p[%d] stationArr=%p[%d] commMode=%d commPos=%d btlRule=%d\n",
                                    setupParam, partyArr, partyLen, statusArr, statusLen, stationArr, stationLen,
                                    (int)commMode, (int)commPos, btlRule);
                    }
                }
                // When Net.Client exists, dump extra state at step transitions
                if (mainModule && netClient && stepChanged) {
                    int32_t ncErrorBit = *(int32_t*)((uintptr_t)netClient + 0x60);
                    uint8_t ncDetFlag = *(uint8_t*)((uintptr_t)netClient + 0x90);
                    void* serverVersions = *(void**)((uintptr_t)netClient + 0x18);
                    uint8_t detResult = serverVersions ? *(uint8_t*)((uintptr_t)serverVersions + 0x19) : 0xFF;
                    uint8_t ncNotifyDone = *(uint8_t*)((uintptr_t)netClient + 0x3c);
                    int32_t ncNotifyCount = *(int32_t*)((uintptr_t)netClient + 0x40);
                    Logger::log("[BtlNet]   nc: errBit=%d detFlag=%d detResult=%d notifyDone=%d notifyCount=%d\n",
                                ncErrorBit, (int)ncDetFlag, (int)detResult, (int)ncNotifyDone, ncNotifyCount);
                }
            }
            // FIX: populate trainerParam[i].playerStatus BEFORE store_party_data (step 2).
            // setSrcPartyToBattleEnv → POKECON::SetParty → addPokeParam dereferences this.
            // store_player_data (step 3) normally populates it, but step 2 runs first.
            // Copy setupParam->playerStatus[i] into trainerParam[i].playerStatus (+0x10).
            static void* s_lastFixedMM = nullptr;
            if (mainModule && mainModule != s_lastFixedMM) {
                void* trainerArr = *(void**)((uintptr_t)mainModule + 0xD0);
                void* setupParam = *(void**)((uintptr_t)mainModule + 0x10);
                if (trainerArr && setupParam) {
                    void* statusArr = *(void**)((uintptr_t)setupParam + 0x68);
                    int32_t tLen = *(int32_t*)((uintptr_t)trainerArr + 0x18);
                    int32_t sLen = statusArr ? *(int32_t*)((uintptr_t)statusArr + 0x18) : 0;
                    int32_t fixCount = (tLen < sLen) ? tLen : sLen;
                    if (fixCount > 5) fixCount = 5;
                    for (int i = 0; i < fixCount; i++) {
                        void* td = *(void**)((uintptr_t)trainerArr + 0x20 + i * 8);
                        if (td && *(void**)((uintptr_t)td + 0x10) == nullptr) {
                            void* statusObj = *(void**)((uintptr_t)statusArr + 0x20 + i * 8);
                            if (statusObj) {
                                *(void**)((uintptr_t)td + 0x10) = statusObj;
                                Logger::log("[BtlNet] FIX: trainerParam[%d].playerStatus = %p (from setupParam)\n",
                                            i, statusObj);
                            }
                        }
                    }
                    s_lastFixedMM = mainModule;
                }
            }
            // DEEP DIAGNOSTIC: on first frame where step==2, walk through every
            // pointer that store_party_data → setSrcPartyToBattleEnv → SetParty
            // → addPokeParam → setupPokeParam will access.
            static bool s_deepDiagDone = false;
            if (mainModule && mainStep == 2 && !s_deepDiagDone) {
                s_deepDiagDone = true;
                Logger::log("[BtlNet] === DEEP DIAG (step 2, pre-Orig) ===\n");

                void* setupParam = *(void**)((uintptr_t)mainModule + 0x10);
                int32_t btlRule = *(int32_t*)((uintptr_t)mainModule + 0x70);
                uint8_t multiMode = setupParam ? *(uint8_t*)((uintptr_t)setupParam + 0x39) : 0xFF;
                Logger::log("[BtlNet] DD: btlRule=%d multiMode=%d\n", btlRule, (int)multiMode);

                // Check PokeID static table (used by GetClientPokeId)
                // PokeID TypeInfo at 0x04c5a678 (from ILClass<PokeID, 0x04c5a678>)
                {
                    auto* pokeIdTI = *(void**)exl::util::modules::GetTargetOffset(0x04c5a678);
                    Logger::log("[BtlNet] DD: PokeID_TypeInfo=%p\n", pokeIdTI);
                    if (pokeIdTI) {
                        void* sf = *(void**)((uintptr_t)pokeIdTI + 0xb8);
                        Logger::log("[BtlNet] DD: PokeID statics=%p\n", sf);
                        if (sf) {
                            void* table = *(void**)((uintptr_t)sf);
                            Logger::log("[BtlNet] DD: PokeID s_clientStartPokeIndex=%p\n", table);
                            if (table) {
                                int32_t tLen = *(int32_t*)((uintptr_t)table + 0x18);
                                Logger::log("[BtlNet] DD: PokeID table len=%d vals:", tLen);
                                for (int i = 0; i < tLen && i < 6; i++) {
                                    uint8_t v = *(uint8_t*)((uintptr_t)table + 0x20 + i);
                                    Logger::log(" [%d]=%d", i, (int)v);
                                }
                                Logger::log("\n");
                            }
                        }
                    }
                }

                // Check both BattleEnv POKECONs
                void* battleEnv1 = *(void**)((uintptr_t)mainModule + 0x108);
                void* battleEnv2 = *(void**)((uintptr_t)mainModule + 0x110);
                for (int envIdx = 0; envIdx < 2; envIdx++) {
                    void* env = (envIdx == 0) ? battleEnv1 : battleEnv2;
                    Logger::log("[BtlNet] DD: battleEnv[%d]=%p\n", envIdx, env);
                    if (!env) continue;
                    void* pokecon = *(void**)((uintptr_t)env + 0x10);
                    Logger::log("[BtlNet] DD:   pokecon=%p\n", pokecon);
                    if (!pokecon) continue;
                    void* partyArr = *(void**)((uintptr_t)pokecon + 0x18);
                    void* activeArr = *(void**)((uintptr_t)pokecon + 0x20);
                    void* storedArr = *(void**)((uintptr_t)pokecon + 0x28);
                    int32_t partyLen = partyArr ? *(int32_t*)((uintptr_t)partyArr + 0x18) : -1;
                    int32_t activeLen = activeArr ? *(int32_t*)((uintptr_t)activeArr + 0x18) : -1;
                    int32_t storedLen = storedArr ? *(int32_t*)((uintptr_t)storedArr + 0x18) : -1;
                    Logger::log("[BtlNet] DD:   m_party=%p[%d] active=%p[%d] stored=%p[%d]\n",
                                partyArr, partyLen, activeArr, activeLen, storedArr, storedLen);
                    // Check first 12 stored pool elements (need to be non-null for activatePokeParam)
                    if (storedArr && storedLen >= 12) {
                        for (int i = 0; i < 12; i++) {
                            void* elem = *(void**)((uintptr_t)storedArr + 0x20 + i * 8);
                            Logger::log("[BtlNet] DD:   stored[%d]=%p\n", i, elem);
                        }
                    }
                }

                // Check m_srcParty and m_srcPartyForServer element validity
                void* srcPartyArr = *(void**)((uintptr_t)mainModule + 0x48);
                void* srcServerArr = *(void**)((uintptr_t)mainModule + 0x50);
                Logger::log("[BtlNet] DD: m_srcParty=%p m_srcPartyForServer=%p\n", srcPartyArr, srcServerArr);
                for (int arrIdx = 0; arrIdx < 2; arrIdx++) {
                    void* arr = (arrIdx == 0) ? srcPartyArr : srcServerArr;
                    const char* name = (arrIdx == 0) ? "srcParty" : "srcServer";
                    if (!arr) continue;
                    int32_t len = *(int32_t*)((uintptr_t)arr + 0x18);
                    for (int i = 0; i < len && i < 5; i++) {
                        void* elem = *(void**)((uintptr_t)arr + 0x20 + i * 8);
                        Logger::log("[BtlNet] DD: %s[%d]=%p\n", name, i, elem);
                    }
                }

                // Check pokeDesc[j].defaultPowerUpDesc for partyDesc[0] and [1]
                if (setupParam) {
                    void* pdArr = *(void**)((uintptr_t)setupParam + 0x60);
                    if (pdArr) {
                        for (int ci = 0; ci < 2; ci++) {
                            void* pd = *(void**)((uintptr_t)pdArr + 0x20 + ci * 8);
                            if (!pd) { Logger::log("[BtlNet] DD: partyDesc[%d]=null\n", ci); continue; }
                            void* pokeDescArr = *(void**)((uintptr_t)pd + 0x10);
                            if (!pokeDescArr) { Logger::log("[BtlNet] DD: partyDesc[%d].pokeDesc=null\n", ci); continue; }
                            int32_t pdLen = *(int32_t*)((uintptr_t)pokeDescArr + 0x18);
                            for (int j = 0; j < pdLen && j < 6; j++) {
                                void* pokeDesc = *(void**)((uintptr_t)pokeDescArr + 0x20 + j * 8);
                                void* dpuDesc = nullptr;
                                if (pokeDesc) dpuDesc = *(void**)((uintptr_t)pokeDesc + 0x10);
                                Logger::log("[BtlNet] DD: partyDesc[%d].pokeDesc[%d]=%p .dpuDesc=%p\n",
                                            ci, j, pokeDesc, dpuDesc);
                            }
                        }
                    }
                }
                Logger::log("[BtlNet] === END DEEP DIAG ===\n");
            }
        }
        Orig(__this);
        // Post-Orig state tracking
        if (isBattle) {
            void* mmPost = *(void**)((uintptr_t)__this + 0x20);
            if (mmPost) {
                int32_t postStep = *(int32_t*)((uintptr_t)mmPost + 0x38);
                int32_t postSub = *(int32_t*)((uintptr_t)mmPost + 0x3c);
                uint8_t postInitFlag = *(uint8_t*)((uintptr_t)mmPost + 0x120);
                uint8_t postErrFlag = *(uint8_t*)((uintptr_t)mmPost + 0xa8);
                uint8_t postErrFlag2 = *(uint8_t*)((uintptr_t)mmPost + 0xa9);
                // Log if step changed, or initFlag set, or on first 5 calls
                if (postStep != mainStep || postSub != subStep || postInitFlag != initFlag ||
                    postErrFlag != errorFlag || s_initLogCount <= 5) {
                    Logger::log("[BtlNet] UpdateInit POST: step %d->%d sub %d->%d init %d->%d err %d->%d err2=%d (call #%d)\n",
                                mainStep, postStep, subStep, postSub,
                                (int)initFlag, (int)postInitFlag,
                                (int)errorFlag, (int)postErrFlag, (int)postErrFlag2,
                                s_initLogCount);
                }
                // When stuck at store_party (step=2), dump internal arrays once
                if (mainStep == 2 && postStep == 2 && s_initLogCount == 30) {
                    // mainModule+0x48 = m_srcParty, +0x50 = m_srcPartyForServer
                    void* srcPartyArr = *(void**)((uintptr_t)mmPost + 0x48);
                    void* srcPartyBkp = *(void**)((uintptr_t)mmPost + 0x50);
                    void* battleEnv1 = *(void**)((uintptr_t)mmPost + 0x108);
                    void* battleEnv2 = *(void**)((uintptr_t)mmPost + 0x110);
                    void* pSubProc = *(void**)((uintptr_t)mmPost + 0x30);
                    // mainModule+0xD0 = m_trainerParam (TRAINER_DATA[])
                    void* trainerArr = *(void**)((uintptr_t)mmPost + 0xD0);
                    int32_t srcLen = srcPartyArr ? *(int32_t*)((uintptr_t)srcPartyArr + 0x18) : -1;
                    int32_t bkpLen = srcPartyBkp ? *(int32_t*)((uintptr_t)srcPartyBkp + 0x18) : -1;
                    int32_t trainerLen = trainerArr ? *(int32_t*)((uintptr_t)trainerArr + 0x18) : -1;
                    Logger::log("[BtlNet] STUCK DIAG: srcParty=%p[%d] srcBkp=%p[%d] env1=%p env2=%p proc=%p\n",
                                srcPartyArr, srcLen, srcPartyBkp, bkpLen,
                                battleEnv1, battleEnv2, pSubProc);
                    // Check party[0-1], partyDesc, trainerParam in setupParam
                    void* setupParam = *(void**)((uintptr_t)mmPost + 0x10);
                    if (setupParam) {
                        void* partyArr = *(void**)((uintptr_t)setupParam + 0x58);
                        void* partyDescArr = *(void**)((uintptr_t)setupParam + 0x60);
                        uint8_t multiMode = *(uint8_t*)((uintptr_t)setupParam + 0x39);
                        int32_t btlRule = *(int32_t*)((uintptr_t)mmPost + 0x70);
                        int32_t pdLen = partyDescArr ? *(int32_t*)((uintptr_t)partyDescArr + 0x18) : -1;
                        Logger::log("[BtlNet] STUCK DIAG: multiMode=%d btlRule=%d partyDesc=%p[%d] trainerParam=%p[%d]\n",
                                    (int)multiMode, btlRule, partyDescArr, pdLen, trainerArr, trainerLen);
                        // Dump party[] with CORRECT offset 0x18 for m_memberCount
                        if (partyArr) {
                            int32_t pLen = *(int32_t*)((uintptr_t)partyArr + 0x18);
                            for (int i = 0; i < pLen && i < 5; i++) {
                                void* p = *(void**)((uintptr_t)partyArr + 0x20 + i * 8);
                                int32_t memCount = -1;
                                void* memberArr = nullptr;
                                if (p) {
                                    memberArr = *(void**)((uintptr_t)p + 0x10); // m_member array
                                    memCount = *(int32_t*)((uintptr_t)p + 0x18); // m_memberCount (FIXED: was 0x20)
                                }
                                Logger::log("[BtlNet] STUCK DIAG: party[%d]=%p m_member=%p m_memberCount=%d\n",
                                            i, p, memberArr, memCount);
                            }
                        }
                        // Dump srcParty[] with CORRECT offset
                        if (srcPartyArr) {
                            for (int i = 0; i < srcLen && i < 5; i++) {
                                void* sp = *(void**)((uintptr_t)srcPartyArr + 0x20 + i * 8);
                                int32_t memCount = -1;
                                if (sp) memCount = *(int32_t*)((uintptr_t)sp + 0x18); // FIXED offset
                                Logger::log("[BtlNet] STUCK DIAG: srcParty[%d]=%p members=%d\n", i, sp, memCount);
                            }
                        }
                        // Dump partyDesc elements + inner pokeDesc array
                        if (partyDescArr && pdLen > 0) {
                            for (int i = 0; i < pdLen && i < 5; i++) {
                                void* pd = *(void**)((uintptr_t)partyDescArr + 0x20 + i * 8);
                                void* pokeDescArr = nullptr;
                                int32_t pokeDescLen = -1;
                                if (pd) {
                                    pokeDescArr = *(void**)((uintptr_t)pd + 0x10); // PartyDesc.pokeDesc
                                    if (pokeDescArr) pokeDescLen = *(int32_t*)((uintptr_t)pokeDescArr + 0x18);
                                }
                                Logger::log("[BtlNet] STUCK DIAG: partyDesc[%d]=%p pokeDesc=%p[%d]\n",
                                            i, pd, pokeDescArr, pokeDescLen);
                            }
                        }
                        // Dump trainerParam elements — setSrcPartyToBattleEnv reads trainerParam[i]+0x10
                        if (trainerArr && trainerLen > 0) {
                            for (int i = 0; i < trainerLen && i < 5; i++) {
                                void* tp = *(void**)((uintptr_t)trainerArr + 0x20 + i * 8);
                                void* tpField = nullptr;
                                if (tp) tpField = *(void**)((uintptr_t)tp + 0x10);
                                Logger::log("[BtlNet] STUCK DIAG: trainerParam[%d]=%p ->0x10=%p\n", i, tp, tpField);
                            }
                        }
                        // Dump battleEnv pokecon (battleEnv+0x10)
                        if (battleEnv1) {
                            void* pokecon1 = *(void**)((uintptr_t)battleEnv1 + 0x10);
                            Logger::log("[BtlNet] STUCK DIAG: battleEnv1->pokecon=%p\n", pokecon1);
                        }
                        if (battleEnv2) {
                            void* pokecon2 = *(void**)((uintptr_t)battleEnv2 + 0x10);
                            Logger::log("[BtlNet] STUCK DIAG: battleEnv2->pokecon=%p\n", pokecon2);
                        }
                    }
                }
            } else if (s_initLogCount <= 3) {
                Logger::log("[BtlNet] UpdateInit POST: mm=null (call #%d)\n", s_initLogCount);
            }
        }
    }
};

// REMOVED: InitCoroutine$$MoveNext hook (was at 0x203BB00)
// REMOVED: MainModule$$store_party_data hook (was at 0x20336A0)
// Both are in the IL2CPP exception path. Our hook modules (RomBase_Diamond)
// lack proper .eh_frame unwind tables, so libunwind can't parse them. This
// turns normally-catchable IL2CppExceptionWrapper into fatal process termination.

// Safety hook: CalcCorrectedPowerBySeikaku @ 0x24AD2C0
// Clamps seikaku (nature) to 0-24 range to prevent IndexOutOfRangeException
// in NatureGrowTable bounds check. If the deserialized partner party has
// corrupt personality data, this prevents the managed exception that would
// silently kill the InitializeCoroutine (or, with our old hooks, crash fatally).
// Signature: uint32_t CalcCorrectedPowerBySeikaku(uint32_t power, uint16_t seikaku, int32_t statType)
HOOK_DEFINE_TRAMPOLINE(CalcTool$$CalcCorrectedPowerBySeikaku) {
    static uint32_t Callback(uint32_t power, uint16_t seikaku, int32_t statType) {
        if (seikaku >= 25) {
            // Log the first few occurrences to diagnose the root cause
            static int32_t s_clampCount = 0;
            s_clampCount++;
            if (s_clampCount <= 20) {
                Logger::log("[BtlNet] CLAMP: CalcCorrectedPowerBySeikaku seikaku=%d (>24!) power=%u stat=%d — returning power unchanged\n",
                            (int)seikaku, power, statType);
            }
            return power;  // No personality modifier for invalid seikaku
        }
        return Orig(power, seikaku, statType);
    }
};

// Hook BattleProc.UpdateMainRun @ 0x187D810
// Called each frame during the actual battle (after init completes).
// If this never fires, initialization is stuck.
HOOK_DEFINE_TRAMPOLINE(BattleProc$$UpdateMainRun) {
    static void Callback(void* __this) {
        if (isOverworldMPActive() && overworldMPIsInBattleScene()) {
            static int32_t s_mainLogCount = 0;
            s_mainLogCount++;
            if (s_mainLogCount <= 5 || s_mainLogCount % 1000 == 0) {
                Logger::log("[BtlNet] BattleProc.UpdateMainRun: this=%p (call #%d) — BATTLE LOOP ACTIVE\n",
                            __this, s_mainLogCount);
            }
        }
        Orig(__this);
    }
};

HOOK_DEFINE_TRAMPOLINE(IlcaNetComponent$$Update) {
    static void Callback(void* __this) {
        // CRASH FIX: Skip the PIA tick entirely during zone change grace period.
        // PIA's Dispatch() may access resources invalidated by the scene change,
        // causing a delayed crash at ~700-950ms. By not calling Orig(), we prevent
        // Common.Update() → NexPlugin.Common.Dispatch() from running.
        if (s_zoneChangeGraceTime > 0.0f) {
            return;  // Do NOT call Orig() — prevent PIA tick
        }

        Orig(__this);

        // Log first 3 ticks after grace period ends to confirm resume
        if (s_ilcaNetUpdateLogCount > 0 && s_ilcaNetUpdateLogCount <= 3) {
            Logger::log("[OverworldMP] IlcaNetComponent.Update() resumed (tick #%d after grace)\n",
                        s_ilcaNetUpdateLogCount);
            s_ilcaNetUpdateLogCount++;
        }
    }
};

// ---------------------------------------------------------------------------
// Feature entry point
// ---------------------------------------------------------------------------

void exl_overworld_multiplayer_main() {
    s_mpContext.Initialize();

    // Hook NetworkManager.Start() — guards against duplicate Start() calls.
    NetworkManager$$Start::InstallAtOffset(0x1DE62F0);

    // Hook NetworkManager.OnUpdate() — sets s_nmOnUpdateReady on first call,
    // proving IE_Start completed and Sequencer subscriptions are active.
    // Also temporarily restores NM singleton during execution (hidden by FM.Update).
    NetworkManager$$OnUpdate::InstallAtOffset(0x1DEA7F0);

    // Hook NetworkManager.OnLateUpdate() — same singleton restore/hide pattern
    // as OnUpdate, ensures NM.OnLateUpdate works while singleton is hidden.
    NetworkManager$$OnLateUpdate::InstallAtOffset(0x1DEAB00);

    // Hook ShowMessageWindow.Setup to null-guard the instance parameter.
    // IE_Start calls Setup(NM._showMessageWindow, ...) but NM+0x98 is null for
    // runtime-created NMs. Without this guard, Setup dereferences null and crashes.
    ShowMessageWindow$$Setup::InstallAtOffset(0x18629E0);

    // Hook ReceivePacketCallback.Invoke — intercept packets directly at the
    // call site inside CheckReceivePacketImpl. Bypasses the broken delegate
    // dispatch (invoke_impl/method_ptr) by calling our handler from the hook.
    ReceivePacketCallback$$Invoke::InstallAtOffset(0x1BC5620);

    // Diagnostic: hook CheckReceivePacketImpl to trace packet reception path.
    NetworkManager$$CheckReceivePacketImpl::InstallAtOffset(0x1DEA970);

    // Suppress error applet during overworld MP — disconnect/errors are handled
    // gracefully via session health polling + SC event hook.
    NetworkManager$$OnSessionError::InstallAtOffset(0x1DEAC40);
    NetworkManager$$IsShowApplicationErrorDialog::InstallAtOffset(0x1DEB4D0);

    // PIA state machine monitoring (read-only, no behavior changes).
    // Logs ALL state changes (not just OverworldMP) to capture underground path too.
#if ENABLE_SESSION_UPDATE_HOOK
    IlcaNetSession$$Update::InstallAtOffset(0x23BAA90);
    Logger::log("[OverworldMP] IlcaNetSession.Update hook ENABLED\n");
#else
    Logger::log("[OverworldMP] IlcaNetSession.Update hook DISABLED (testing trampoline theory)\n");
#endif

    // Diagnostic hooks — capture networking flow for both underground and overworld
    NetworkManager$$DoStartSession::InstallAtOffset(0x1DE69B0);
    SessionConnector$$StartSession::InstallAtOffset(0x1BC6940);
    IlcaNetSession$$NetworkInitAsync::InstallAtOffset(0x23B9F30);
    IlcaNetSession$$Init::InstallAtOffset(0x23B9850);

    // PlatformInitialize hook — pinpoints whether crash is before, during, or after PIA init
#if ENABLE_PLATFORMINIT_HOOK
    IlcaNetBase$$PlatformInitialize::InstallAtOffset(0x2CDA460);
    Logger::log("[OverworldMP] IlcaNetBase.PlatformInitialize hook ENABLED\n");
#else
    Logger::log("[OverworldMP] IlcaNetBase.PlatformInitialize hook DISABLED\n");
#endif

    // IlcaNetComponent.Update diagnostic hook — detects if PIA ticks during grace period.
    // If the disable via Behaviour.set_enabled(false) works, this hook should NOT fire
    // during grace frames. If it does, the log will flag it as a WARNING.
    IlcaNetComponent$$Update::InstallAtOffset(0x2CDC280);
    Logger::log("[OverworldMP] IlcaNetComponent.Update hook installed (diagnostic)\n");

    // Battle networking — fully native pipeline with BATTLE_READY sync handshake.
    // No comm init step hooks (determine_server, store_party, etc.) — all run natively.
    // Diagnostic hooks on send/receive paths for visibility.
    // Send/receive path diagnostics
    NetworkManager$$SendReliablePacketToAll::InstallAtOffset(0x1DE8BD0);
    NetworkManager$$SendReliablePacket::InstallAtOffset(0x1DE89B0);
    NetClient$$get_Instance::InstallAtOffset(0x203D410);
    SessionConnector$$SendAll::InstallAtOffset(0x1BC7FA0);
    SessionConnector$$SendTo::InstallAtOffset(0x1BC7D70);
    // BattleProc lifecycle — trace init → main loop transition
    BattleProc$$UpdateInitialze::InstallAtOffset(0x187D510);
    BattleProc$$UpdateMainRun::InstallAtOffset(0x187D810);
    // Net.Client ServerVersion exchange diagnostics
    NetClient$$sendToServerVersionCoreAll::InstallAtOffset(0x203DD80);
    // Safety: clamp seikaku in CalcCorrectedPowerBySeikaku to prevent
    // IndexOutOfRangeException during store_party_data → stat calculation
    CalcTool$$CalcCorrectedPowerBySeikaku::InstallAtOffset(0x24AD2C0);
    Logger::log("[OverworldMP] Battle networking (diagnostic hooks, native pipeline, CalcTool safety) installed\n");

    Logger::log("[OverworldMP] All hooks installed (NM.Start, ShowMessageWindow, state monitor, diagnostics)\n");

    // Hook FieldManager.Update()
    FieldManager$$Update::InstallAtOffset(0x0179A080);

    // OnZoneChange hook DISABLED — trampoline at 0x01798CC0 caused crashes
    // in native Unity code (~300ms after zone transition). Area changes are
    // now detected in FieldManager.Update by polling get_areaID().
    // FieldManager$$OnZoneChange::InstallAtOffset(0x01798CC0);

    // Hook SessionConnector.OnSessionEventCallback to intercept player join/leave
    // (hooked at SC level instead of NM.OnSessionEvent — more reliable since it
    //  doesn't depend on the delegate chain from SC to NM being set up)
    SessionConnector$$OnSessionEventCallback::InstallAtOffset(0x1BC8670);

    // Setting toggle detection: we poll the save data setting each frame in
    // FieldManager.Update. When the value changes, start/stop is triggered.

    Logger::log("[OverworldMP] Feature hooks installed\n");
}
