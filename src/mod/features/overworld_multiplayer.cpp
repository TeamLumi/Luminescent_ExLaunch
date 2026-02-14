#include "exlaunch.hpp"

#include "features/overworld_multiplayer.h"

#include "data/settings.h"
#include "data/utils.h"

#include "externals/BaseEntity.h"
#include "externals/ColorVariation.h"
#include "externals/EntityManager.h"
#include "externals/FieldCharacterEntity.h"
#include "externals/FieldManager.h"
#include "externals/FieldObjectEntity.h"
#include "externals/FieldPlayerEntity.h"
#include "externals/GameData/DataManager.h"
#include "externals/PlayerWork.h"
#include "externals/XLSXContent/CharacterDressData.h"

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
static int32_t s_interactionTimeoutFrames = 0;  // countdown for request timeout

// NM.Start() is called manually after AddComponent since Unity's non-generic
// AddComponent(Type) doesn't trigger Start(). The NM.Start hook logs all calls.

OverworldMPContext& getOverworldMPContext() {
    return s_mpContext;
}

bool isOverworldMPActive() {
    return s_mpContext.state == OverworldMPState::Connected ||
           s_mpContext.state == OverworldMPState::Searching;
}

// ---------------------------------------------------------------------------
// NM init-wait helpers
// ---------------------------------------------------------------------------
// After ensureNetworkManagerSingleton(), NM.Start() runs on the next frame
// and creates the IE_Start coroutine. We poll for IE_Start completion by
// checking SC.callObjPtr (set during SC.Initialize inside the coroutine).

static int32_t s_initWaitFrames = 0;

// NM.OnUpdate readiness flag — set true on first invocation of NM.OnUpdate,
// proving IE_Start completed and Sequencer subscriptions are active.
static bool s_nmOnUpdateReady = false;

// Guard against duplicate NM.Start() calls (manual + Unity lifecycle)
static void* s_nmStartedInstance = nullptr;

// PIA state machine monitoring — only logs on state CHANGE to avoid spam
static uint32_t s_lastLoggedPiaState = 0xFFFFFFFF;
static int32_t s_searchingFrameCount = 0;

// Zone change grace period — after a zone transition, game systems (Sequencer,
// EntityManager, asset loader) need time to stabilize. During grace frames,
// all MP processing (position sends, spawning) is deferred.
static int32_t s_zoneChangeGraceFrames = 0;

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

        int32_t oldArea = remote.areaID;
        remote.areaID = areaID;

        // Handle area change: spawn/despawn based on whether remote is in our area.
        // Skip during zone change grace period — game systems are unstable.
        if (oldArea != areaID && s_zoneChangeGraceFrames <= 0) {
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

        // TODO: Display emote above remote player entity.
        // Needs research into UgEmoticonSet or direct prefab instantiation.
        // For now, just log it — the packet protocol is in place.
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
            Logger::log("[OverworldMP] Interaction request queued for display (from station %d, type=%d)\n",
                        fromStation, (int)interactType);
            // TODO: Show accept/reject dialog to local player.
            // For now, auto-log. The interaction framework is in place.
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
            if (accepted) {
                Logger::log("[OverworldMP] Interaction ACCEPTED by station %d\n", fromStation);
                // TODO: Launch trade/battle screen based on pending request type.
            } else {
                Logger::log("[OverworldMP] Interaction REJECTED by station %d\n", fromStation);
                s_interactionState = InteractionState::None;
                s_interactionTarget = -1;
                s_interactionTimeoutFrames = 0;
            }
        } else {
            Logger::log("[OverworldMP] Ignoring interaction response — state=%d target=%d\n",
                        (int)s_interactionState, s_interactionTarget);
        }
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
    s_initWaitFrames = 0;
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
    if (s_zoneChangeGraceFrames > 0) {
        Logger::log("[OverworldMP] Stop during grace period (frames remaining: %d)\n",
                    s_zoneChangeGraceFrames);
        s_zoneChangeGraceFrames = 0;
    }

    // Reset interaction state
    s_interactionState = InteractionState::None;
    s_interactionTarget = -1;
    s_interactionTimeoutFrames = 0;

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
        s_initWaitFrames++;

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
            Logger::log("[OverworldMP] IE_Start completed after %d frames (NM.OnUpdate fired)\n",
                        s_initWaitFrames);

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
        } else if (s_initWaitFrames > 600) {  // ~20 second timeout
            Logger::log("[OverworldMP] IE_Start timeout after %d frames\n", s_initWaitFrames);
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
    if (s_zoneChangeGraceFrames > 0) {
        s_zoneChangeGraceFrames--;

        // Log every 5 frames during grace to track countdown + session health
        if (s_zoneChangeGraceFrames % 5 == 0 || s_zoneChangeGraceFrames <= 3) {
            int32_t sessionState = Dpr::NetworkUtils::NetworkManager::get_SessionState();
            Logger::log("[OverworldMP] Grace countdown: %d frames left, sessionState=%d, area=%d\n",
                        s_zoneChangeGraceFrames, sessionState, s_mpContext.myAreaID);
        }

        if (s_zoneChangeGraceFrames == 0) {
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

    // Session health check — detect error/crash states and stop gracefully.
    // This catches disconnects that bypass the session event callback
    // (e.g., PIA internal errors, transport failures).
    if (s_mpContext.state == OverworldMPState::Connected) {
        int32_t sessionState = Dpr::NetworkUtils::NetworkManager::get_SessionState();
        if (sessionState == 7 || sessionState == 9) { // GamingError or Crash
            Logger::log("[OverworldMP] Session error detected (state=%d), stopping gracefully\n",
                        sessionState);
            overworldMPStop();
            return;
        }
    }

    // Periodic Connected state diagnostics (~every 5 seconds)
    if (s_mpContext.state == OverworldMPState::Connected &&
        s_mpContext.frameCounter % 150 == 0) {
        int activeCount = 0;
        for (int i = 0; i < OW_MP_MAX_PLAYERS; i++)
            if (s_mpContext.remotePlayers[i].isActive) activeCount++;
        Logger::log("[OverworldMP] Connected: frame=%d recvCB=%d activePlayers=%d\n",
                    s_mpContext.frameCounter, s_recvCallbackCount, activeCount);
    }

    // Send position at sync rate (~10 Hz)
    if (s_mpContext.state == OverworldMPState::Connected &&
        s_mpContext.frameCounter % OW_MP_POS_SYNC_INTERVAL == 0) {
        overworldMPSendPosition();
    }

    // Interaction timeout — if waiting for a response too long, cancel
    if (s_interactionState == InteractionState::WaitingResponse) {
        if (s_interactionTimeoutFrames > 0) {
            s_interactionTimeoutFrames--;
        }
        if (s_interactionTimeoutFrames == 0) {
            Logger::log("[OverworldMP] Interaction request timed out (target station %d)\n",
                        s_interactionTarget);
            s_interactionState = InteractionState::None;
            s_interactionTarget = -1;
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
    }
}

// Forward declarations for spawn queue (defined after entity spawn section)
static void spawnQueueClear();
static void spawnQueueProcessNext();

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
static bool    s_spawnInFlight = false; // True while an asset load is pending
static int32_t s_spawnInFlightStation = -1; // Station being spawned right now

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
    if (s_zoneChangeGraceFrames > 0) {
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

    // 2. Set world position
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
        Logger::log("[OverworldMP] Step 4a done\n");
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

    // Build packet: [DataID:1][posX:4][posY:4][posZ:4][rotY:4][areaID:4][avatarId:4][colorId:4] = 29 bytes
    il2cpp_vcall_void(pw, PW_RESET);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, OWMP_DATA_ID);
    il2cpp_vcall_write_fp32(pw, PW_WRITE_FP32, pos.fields.x);
    il2cpp_vcall_write_fp32(pw, PW_WRITE_FP32, pos.fields.y);
    il2cpp_vcall_write_fp32(pw, PW_WRITE_FP32, pos.fields.z);
    il2cpp_vcall_write_fp32(pw, PW_WRITE_FP32, yaw);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, s_mpContext.myAreaID);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, myAvatarId);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, myColorId);

    // Send unreliable to all peers on Station transport (type=0)
    Dpr::NetworkUtils::NetworkManager::SendUnReliablePacketToAll(pw, 0);

    if (!s_loggedFirstSend) {
        s_loggedFirstSend = true;
        Logger::log("[OverworldMP] First position sent: area=%d pos=(%.1f,%.1f,%.1f)\n",
                    s_mpContext.myAreaID, pos.fields.x, pos.fields.y, pos.fields.z);
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
    s_interactionTimeoutFrames = 300; // 10 seconds at 30fps

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
        if (s_zoneChangeGraceFrames > 0) {
            if (s_zoneChangeGraceFrames % 10 == 0 || s_zoneChangeGraceFrames <= 3) {
                Logger::log("[OverworldMP] NM.OnUpdate BLOCKED during grace (frames left: %d)\n",
                            s_zoneChangeGraceFrames);
            }
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
        if (s_zoneChangeGraceFrames > 0) {
            if (s_zoneChangeGraceFrames % 10 == 0 || s_zoneChangeGraceFrames <= 3) {
                Logger::log("[OverworldMP] NM.OnLateUpdate BLOCKED during grace (frames left: %d)\n",
                            s_zoneChangeGraceFrames);
            }
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
                    if (s_mpContext.spawnedEntities[i] != nullptr) {
                        auto* entity = (FieldObjectEntity::Object*)s_mpContext.spawnedEntities[i];
                        auto* go = entity->cast<UnityEngine::Component>()->get_gameObject();
                        if (go != nullptr) {
                            UnityEngine::_Object::Destroy(go->cast<UnityEngine::_Object>());
                        }
                    }
                    s_mpContext.remotePlayers[i].isSpawned = false;
                    s_mpContext.spawnedEntities[i] = nullptr;
                }

                // Reset interaction state on zone change
                if (s_interactionState != InteractionState::None) {
                    Logger::log("[OverworldMP] Cancelling interaction (state=%d) due to zone change\n",
                                (int)s_interactionState);
                    s_interactionState = InteractionState::None;
                    s_interactionTarget = -1;
                    s_interactionTimeoutFrames = 0;
                }

                s_mpContext.myAreaID = currentArea;
                s_zoneChangeGraceFrames = OW_MP_ZONE_CHANGE_GRACE_FRAMES;
                Logger::log("[OverworldMP] Grace period started: %d frames (~%.1fs)\n",
                            OW_MP_ZONE_CHANGE_GRACE_FRAMES,
                            OW_MP_ZONE_CHANGE_GRACE_FRAMES / 30.0f);
            }
        }

        // Run update when session is active OR initializing (waiting for IE_Start)
        if (s_mpContext.state == OverworldMPState::Disabled ||
            s_mpContext.state == OverworldMPState::Disconnecting ||
            s_mpContext.state == OverworldMPState::Error) {
            return;
        }

        // Approximate deltaTime — Update runs at ~30fps, so deltaTime ~= 0.033s
        float deltaTime = 0.03333334f;

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

        s_zoneChangeGraceFrames = OW_MP_ZONE_CHANGE_GRACE_FRAMES;
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
        // For overworld MP: let Orig() process all events (updates SC internal
        // state tables). The session event/error/finish delegates on SC are
        // permanently nulled during session setup, so Orig() can't propagate
        // errors to NM's handler (no ErrorApplet). We handle events ourselves.
        if (!isOverworldMPActive()) {
            Orig(__this, rawEventType, stationIndex);
            return;
        }

        Orig(__this, rawEventType, stationIndex);

        // Read the processed eventType stored by Orig() at SC+0x44
        int32_t eventType = *(int32_t*)((uintptr_t)__this + 0x44);
        Logger::log("[OverworldMP] Session event: raw=%d station=%d type=%d\n",
                    rawEventType, stationIndex, eventType);

        if (eventType == 2) {
            // Another player joined
            overworldMPOnPlayerJoin(stationIndex);
        } else if (eventType == 4 || eventType == 6) {
            // Other player disconnected (4) or left (6)
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
HOOK_DEFINE_TRAMPOLINE(IlcaNetComponent$$Update) {
    static void Callback(void* __this) {
        // CRASH FIX: Skip the PIA tick entirely during zone change grace period.
        // PIA's Dispatch() may access resources invalidated by the scene change,
        // causing a delayed crash at ~700-950ms. By not calling Orig(), we prevent
        // Common.Update() → NexPlugin.Common.Dispatch() from running.
        if (s_zoneChangeGraceFrames > 0) {
            // Log periodically (not every frame) to confirm blocking works
            if (s_zoneChangeGraceFrames % 10 == 0 || s_zoneChangeGraceFrames <= 3) {
                Logger::log("[OverworldMP] IlcaNetComponent.Update() BLOCKED (grace frames left: %d)\n",
                            s_zoneChangeGraceFrames);
            }
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
