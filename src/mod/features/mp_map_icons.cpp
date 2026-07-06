#include "exlaunch.hpp"
#include "features/mp_log.h"
#include "features/mp_net.h"
#include "features/mp_map_icons.h"
#include "features/mp_minigames.h"
#include "features/overworld_multiplayer.h"

#include "helpers/InputHelper.h"

#include "externals/FieldManager.h"
#include "externals/PlayerWork.h"
#include "externals/Dpr/NetworkUtils/NetworkManager.h"
#include "externals/Dpr/UI/UIManager.h"
#include "externals/System/String.h"
#include "externals/System/Type.h"
#include "externals/UnityEngine/_Object.h"
#include "externals/UnityEngine/Component.h"
#include "externals/UnityEngine/GameObject.h"
#include "externals/UnityEngine/Transform.h"
#include "externals/UnityEngine/RectTransform.h"
#include "externals/UnityEngine/UI/Image.h"
#include "externals/UnityEngine/Color.h"
#include "externals/UnityEngine/Vector2.h"
#include "externals/UnityEngine/Vector3.h"

#include <cstdio>
#include <cstring>
#include <cmath>

// ---------------------------------------------------------------------------
// Game function RVAs (Dpr.UI.Townmap and friends)
// ---------------------------------------------------------------------------
// FieldManager.GetTownMapPos(out ZoneID, out Vector3, bool isForcePrevious)
static constexpr uintptr_t RVA_FM_GET_TOWNMAP_POS   = 0x17A43B0;
// FieldObjectEntity.PositionToGrid(Vector3) -> Vector2Int (static; floats in s0-s2)
static constexpr uintptr_t RVA_POSITION_TO_GRID     = 0x1D54AE0;
// Townmap.GetData(ZoneID, Vector2Int) -> TownMapTable.SheetData (instance)
static constexpr uintptr_t RVA_TOWNMAP_GET_DATA     = 0x184CE10;
// Townmap.Setup — hook target (this in x3, see trampoline note below)
static constexpr uintptr_t RVA_TOWNMAP_SETUP        = 0x184B7B0;
// TownmapWindow.OnCellChanged(Cell) / OnUpdate — hook targets
static constexpr uintptr_t RVA_WINDOW_ON_CELL_CHANGED = 0x1850E20;
static constexpr uintptr_t RVA_WINDOW_ON_UPDATE       = 0x18504E0;

// Field offsets (verified in Ghidra pseudocode, see docs/superpowers/specs/map-player-icons.md)
static constexpr uintptr_t TOWNMAP_CELLROOT_OFFSET  = 0x20; // RectTransform*
static constexpr uintptr_t TOWNMAP_PLAYER_OFFSET    = 0x28; // TownmapPlayerIcon*
static constexpr uintptr_t PLAYERICON_IMAGE_OFFSET  = 0x20; // UnityEngine.UI.Image*
static constexpr uintptr_t CELL_SHEETDATA_OFFSET    = 0x10; // Townmap.Cell -> SheetData
static constexpr uintptr_t SHEETDATA_NOWPOS_OFFSET  = 0x24; // SheetData.NowPosXZ (Vector2: 2 floats)

// Vanilla ToCellIconPos math: screen = (cellX*24 + root.x + 12, root.y - cellY*24 - 12, root.z)
static constexpr float MAP_CELL_SIZE   = 24.0f;
static constexpr float MAP_HALF_CELL   = 12.0f;

// Peer icons render smaller than the local player's marker, and players
// sharing a map cell fan out horizontally so nobody is buried.
static constexpr float PEER_ICON_SCALE = 0.65f;
static constexpr float STACK_FAN_PX    = 10.0f;

// ---------------------------------------------------------------------------
// State
// ---------------------------------------------------------------------------

struct PeerMapInfo {
    bool    valid;
    int32_t zoneID;
    int32_t gridX;
    int32_t gridZ;
};

struct MapPin {
    bool  active;
    float cellX;   // SheetData.NowPosXZ of the pinned cell
    float cellZ;
};

static PeerMapInfo s_peerMapInfo[OW_MP_MAX_PLAYERS] = {};
static MapPin      s_peerPins[OW_MP_MAX_PLAYERS] = {};
static MapPin      s_myPin = {};

// Local broadcast state
static bool    s_myInfoValid = false;
static int32_t s_myZone = 0;
static int32_t s_myGridX = 0;
static int32_t s_myGridZ = 0;
static float   s_sendTimer = 0.0f;
static constexpr float MAP_INFO_SEND_INTERVAL = 2.0f;

// Live Town Map window state (valid between Townmap.Setup and window close;
// only dereferenced from inside the window's own callbacks)
static void* s_curTownmap = nullptr;
// Marker spawn is deferred from Townmap.Setup to the first TownmapWindow.OnUpdate:
// at Setup-return the player-icon template's GameObject hierarchy isn't always
// finished initializing, and cloning it then intermittently crashed inside
// UnityEngine.Object.Instantiate. By the first Update the map is fully built.
static bool  s_markersPending = false;
// Hovered cell cache (from TownmapWindow.OnCellChanged)
static bool  s_hoverValid = false;
static float s_hoverCellX = 0.0f;
static float s_hoverCellZ = 0.0f;
static bool  s_prevZL = false;
// My-pin icon clone (respawned live on pin placement while the map is open)
static void* s_myPinClone = nullptr;

// ---------------------------------------------------------------------------
// Local map position
// ---------------------------------------------------------------------------

static bool getMyMapCell(int32_t* outZone, int32_t* outGridX, int32_t* outGridZ) {
    FieldManager::getClass()->initIfNeeded();
    auto fm = FieldManager::getClass()->static_fields->_Instance_k__BackingField;
    if (fm == nullptr) return false;

    int32_t zone = 0;
    UnityEngine::Vector3::Fields pos = {};
    // FieldManager.GetTownMapPos(out zone, out pos, isForcePrevious=false)
    _ILExternal::external<void>(RVA_FM_GET_TOWNMAP_POS, fm, &zone, &pos, false);

    // FieldObjectEntity.PositionToGrid(pos) -> Vector2Int packed {x lo32, y hi32}
    uint64_t packed = _ILExternal::external<uint64_t>(RVA_POSITION_TO_GRID, pos.x, pos.y, pos.z);
    *outZone  = zone;
    *outGridX = (int32_t)(packed & 0xFFFFFFFF);
    *outGridZ = (int32_t)(packed >> 32);
    return true;
}

// ---------------------------------------------------------------------------
// Packet send
// ---------------------------------------------------------------------------

static void sendMapInfo() {
    if (!s_myInfoValid) return;
    auto* pw = Dpr::NetworkUtils::NetworkManager::get_PacketWriterRe();
    if (pw == nullptr) return;

    // Trainer ID rides along for Loto-ID pooling (MYSTATUS +0x8)
    int32_t myTrainerId = 0;
    auto* status = PlayerWork::get_playerStatus();
    if (status != nullptr) {
        myTrainerId = *(int32_t*)((uintptr_t)status + 0x8);
    }

    il2cpp_vcall_void(pw, PW_RESET);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, OWMP_DATA_ID_MAP_INFO);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, s_myZone);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, s_myGridX);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, s_myGridZ);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, myTrainerId);
    Dpr::NetworkUtils::NetworkManager::SendReliablePacketToAll(pw, 0);
}

static void sendMyPin() {
    auto* pw = Dpr::NetworkUtils::NetworkManager::get_PacketWriterRe();
    if (pw == nullptr) return;

    il2cpp_vcall_void(pw, PW_RESET);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, OWMP_DATA_ID_MAP_PIN);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, s_myPin.active ? 1 : 0);
    il2cpp_vcall_write_fp32(pw, PW_WRITE_FP32, s_myPin.cellX);
    il2cpp_vcall_write_fp32(pw, PW_WRITE_FP32, s_myPin.cellZ);
    Dpr::NetworkUtils::NetworkManager::SendReliablePacketToAll(pw, 0);
    MP_LOG("[MapIcons] Sent pin (active=%d cell=%.0f,%.0f)\n",
                (int)s_myPin.active, s_myPin.cellX, s_myPin.cellZ);
}

// ---------------------------------------------------------------------------
// Tick / receive
// ---------------------------------------------------------------------------

void mpMapIconsTick(float deltaTime) {
    s_sendTimer += deltaTime;
    if (s_sendTimer < MAP_INFO_SEND_INTERVAL) return;
    s_sendTimer = 0.0f;

    // Hide-and-seek: while hiding, broadcasting my map location would hand
    // the seeker a wallhack — go dark until the game ends.
    if (mpMinigameAmHiding()) return;

    int32_t zone = 0, gx = 0, gz = 0;
    if (getMyMapCell(&zone, &gx, &gz)) {
        s_myInfoValid = true;
        s_myZone = zone;
        s_myGridX = gx;
        s_myGridZ = gz;
        sendMapInfo();
    }
}

void mpMapIconsOnInfoReceived(void* pr) {
    int32_t fromStation = il2cpp_vcall_int(pr, PR_FROM_STATION);
    if (fromStation < 0 || fromStation >= OW_MP_MAX_PLAYERS) return;

    auto& info = s_peerMapInfo[fromStation];
    il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &info.zoneID);
    il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &info.gridX);
    il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &info.gridZ);
    info.valid = true;

    // Trainer ID for Loto-ID pooling
    int32_t tid = 0;
    il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &tid);
    getOverworldMPContext().remotePlayers[fromStation].trainerId = tid;
}

void mpMapIconsOnPinReceived(void* pr) {
    int32_t fromStation = il2cpp_vcall_int(pr, PR_FROM_STATION);
    if (fromStation < 0 || fromStation >= OW_MP_MAX_PLAYERS) return;

    auto& pin = s_peerPins[fromStation];
    uint8_t active = 0;
    il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &active);
    il2cpp_vcall_read_out(pr, PR_READ_FP32_OUT, &pin.cellX);
    il2cpp_vcall_read_out(pr, PR_READ_FP32_OUT, &pin.cellZ);
    pin.active = (active != 0);
    MP_LOG("[MapIcons] Pin from station %d (active=%d cell=%.0f,%.0f)\n",
                fromStation, (int)pin.active, pin.cellX, pin.cellZ);
}

void mpMapIconsOnPeerLeft(int32_t stationIndex) {
    if (stationIndex < 0 || stationIndex >= OW_MP_MAX_PLAYERS) return;
    s_peerMapInfo[stationIndex] = {};
    s_peerPins[stationIndex] = {};
}

void mpMapIconsClearAll() {
    for (int i = 0; i < OW_MP_MAX_PLAYERS; i++) {
        s_peerMapInfo[i] = {};
        s_peerPins[i] = {};
    }
    s_myPin = {};
    s_myInfoValid = false;
    s_curTownmap = nullptr;
    s_myPinClone = nullptr;
    s_hoverValid = false;
}

// ---------------------------------------------------------------------------
// Icon rendering (inside the Town Map)
// ---------------------------------------------------------------------------

// Vanilla marker math from Townmap.ToCellIconPos @0x184CAB0.
static UnityEngine::Vector3::Object cellToWorldPos(void* townmap, float cellX, float cellZ) {
    auto* cellRoot = *(UnityEngine::Transform::Object**)((uintptr_t)townmap + TOWNMAP_CELLROOT_OFFSET);
    auto rootPos = cellRoot->cast<UnityEngine::Transform>()->get_position();
    UnityEngine::Vector3::Object out = {};
    out.fields.x = cellX * MAP_CELL_SIZE + rootPos.fields.x + MAP_HALF_CELL;
    out.fields.y = rootPos.fields.y - cellZ * MAP_CELL_SIZE - MAP_HALF_CELL;
    out.fields.z = rootPos.fields.z;
    return out;
}

// Per-render-pass occupancy tracking: how many markers already sit on a
// given cell. Reset at the start of each spawnAllMarkers pass.
struct CellOccupancy { float x, z; int32_t count; };
static CellOccupancy s_occupancy[OW_MP_MAX_PLAYERS * 2 + 1];
static int32_t s_occupancyCount = 0;

static void resetOccupancy() { s_occupancyCount = 0; }

// Returns the stack index (0 = first marker on this cell) and records the marker.
static int32_t occupyCell(float cellX, float cellZ) {
    for (int32_t i = 0; i < s_occupancyCount; i++) {
        if (fabsf(s_occupancy[i].x - cellX) < 0.5f &&
            fabsf(s_occupancy[i].z - cellZ) < 0.5f) {
            return s_occupancy[i].count++;
        }
    }
    if (s_occupancyCount < (int32_t)(sizeof(s_occupancy) / sizeof(s_occupancy[0]))) {
        s_occupancy[s_occupancyCount] = { cellX, cellZ, 1 };
        s_occupancyCount++;
    }
    return 0;
}

// Symmetric horizontal fan: 0, +10, -10, +20, -20, ... pixels.
static float stackFanOffset(int32_t stackIndex) {
    if (stackIndex == 0) return 0.0f;
    int32_t step = (stackIndex + 1) / 2;
    return (stackIndex % 2 == 1) ? STACK_FAN_PX * step : -STACK_FAN_PX * step;
}

// Deterministic distinct badge color keyed to a peer's synced colorId, so a
// given player keeps the same map color across the session. This is what tells
// multiple peers apart. colorId < 0 (our own meet-up pin) -> gold.
static UnityEngine::Color::Fields peerBadgeColor(int32_t colorId) {
    static const float palette[8][3] = {
        {0.95f, 0.30f, 0.30f}, // red
        {0.30f, 0.55f, 0.95f}, // blue
        {0.35f, 0.80f, 0.40f}, // green
        {0.95f, 0.80f, 0.25f}, // yellow
        {0.75f, 0.45f, 0.90f}, // purple
        {0.95f, 0.55f, 0.25f}, // orange
        {0.35f, 0.85f, 0.85f}, // cyan
        {0.95f, 0.50f, 0.75f}, // pink
    };
    UnityEngine::Color::Fields c{};
    if (colorId < 0) { c.r = 1.0f; c.g = 0.85f; c.b = 0.30f; c.a = 1.0f; return c; }
    const float* p = palette[colorId & 7];
    c.r = p[0]; c.g = p[1]; c.b = p[2]; c.a = 1.0f;
    return c;
}

// Build a System.Type from a LIVE instance's runtime class (first field = klass).
// Lets us name RectTransform/Image without a compile-time TypeInfo address.
static System::Type::Object* typeOfInstance(void* inst) {
    if (inst == nullptr) return nullptr;
    Il2CppClass* klass = *(Il2CppClass**)inst;
    System::RuntimeTypeHandle::Object handle{};
    handle.fields.value = &((UnityEngine::GameObject::Class*)klass)->_1.byval_arg;
    return System::Type::GetTypeFromHandle(handle);
}

static UnityEngine::Vector2::Object vec2(float x, float y) {
    UnityEngine::Vector2::Object v{}; v.fields.x = x; v.fields.y = y; return v;
}

// Create a fresh UI GameObject (RectTransform + Image), NO Instantiate. Passing
// a RectTransform type to the GameObject ctor makes the transform a RectTransform
// (a plain-Transform GO can't have one added after). AddComponentByType(Image)
// auto-adds CanvasRenderer via RequireComponent and returns the Image.
static UnityEngine::UI::Image::Object* makeUiImage(
        System::Type::Object* rectType, System::Type::Object* imageType,
        const char* name, float w, float h) {
    if (rectType == nullptr || imageType == nullptr) return nullptr;
    auto* typeArr = (System::Type::Array*)system_array_new(
        (Il2CppClass*)System::Type::getClass(), 1L);
    if (typeArr == nullptr) return nullptr;
    typeArr->m_Items[0] = rectType;
    UnityEngine::GameObject::getClass()->initIfNeeded();
    auto* go = (UnityEngine::GameObject::Object*)il2cpp_object_new(
        (Il2CppClass*)UnityEngine::GameObject::getClass());
    if (go == nullptr) return nullptr;
    go->cast<UnityEngine::GameObject>()->ctor(System::String::Create(name), typeArr);
    auto* img = (UnityEngine::UI::Image::Object*)
        go->cast<UnityEngine::GameObject>()->AddComponentByType(imageType);
    if (img == nullptr) return nullptr;
    auto* tf = ((UnityEngine::Component*)img)->get_transform();
    auto* rt = reinterpret_cast<UnityEngine::RectTransform::Object*>(tf);
    rt->set_anchorMin(vec2(0.5f, 0.5f));
    rt->set_anchorMax(vec2(0.5f, 0.5f));
    rt->set_pivot(vec2(0.5f, 0.5f));
    rt->set_sizeDelta(vec2(w, h));
    return img;
}

// Build a from-scratch peer marker at a cell: a colored badge (peer's synced
// color, for telling peers apart) with the trainer face on top. Returns the
// badge root (owned by the window hierarchy — dies with the window). No
// Instantiate anywhere: cloning this in-scene town map object faults at the
// UnityEngine engine level on Ryujinx, so we compose the marker by hand.
// Peer-icon RENDERING is disabled: on Ryujinx, every way of materialising a
// marker faults in a different part of the UI subsystem — Instantiate of the
// vanilla icon deep-clones an Animator (engine crash), Instantiate of the child
// Image faults at the engine level (scene-clone path), and even the from-scratch
// path crashes just reading the town map's own _image via get_transform, i.e.
// the town map's live UI objects aren't safe to touch at this lifecycle point.
// The meet-up PIN DATA (coordinate sharing) is unaffected and stays live; only
// the on-map dots are suppressed. Flip this to re-enable a future attempt that
// builds markers on an isolated overlay canvas we own end-to-end (never reading
// town map internals).
static constexpr bool MP_MAP_PEER_ICONS_ENABLED = false;

static void* spawnIconClone(void* townmap, float cellX, float cellZ,
                            int32_t fashionId, int32_t colorId) {
    if (!MP_MAP_PEER_ICONS_ENABLED) return nullptr;
    MP_LOG("[MapIcons] spawnIconClone ENTER townmap=%p fashion=%d color=%d\n", townmap, fashionId, colorId);
    auto* playerIcon = *(UnityEngine::_Object::Object**)((uintptr_t)townmap + TOWNMAP_PLAYER_OFFSET);
    if (playerIcon == nullptr || !UnityEngine::_Object::op_Inequality(playerIcon, nullptr)) return nullptr;
    auto* srcImage = *(UnityEngine::_Object::Object**)((uintptr_t)playerIcon + PLAYERICON_IMAGE_OFFSET);
    if (srcImage == nullptr || !UnityEngine::_Object::op_Inequality(srcImage, nullptr)) return nullptr;

    // Sample the RectTransform + Image runtime types from the live vanilla icon.
    auto* srcTf = ((UnityEngine::Component*)srcImage)->get_transform();
    auto* rectType  = typeOfInstance(srcTf);
    auto* imageType = typeOfInstance(srcImage);
    if (rectType == nullptr || imageType == nullptr) return nullptr;

    // Size off the vanilla marker; peers render a bit smaller, badge a bit bigger.
    auto srcSize = reinterpret_cast<UnityEngine::RectTransform::Object*>(srcTf)->get_sizeDelta();
    float baseW = (srcSize.fields.x > 1.0f) ? srcSize.fields.x : 24.0f;
    float baseH = (srcSize.fields.y > 1.0f) ? srcSize.fields.y : 24.0f;
    float faceW = baseW * PEER_ICON_SCALE, faceH = baseH * PEER_ICON_SCALE;
    float badgeW = faceW * 1.30f, badgeH = faceH * 1.30f;

    auto* cellRoot = *(UnityEngine::Transform::Object**)((uintptr_t)townmap + TOWNMAP_CELLROOT_OFFSET);

    // Badge (colored backing quad — Image with no sprite renders solid color).
    auto* badge = makeUiImage(rectType, imageType, "MPPeerBadge", badgeW, badgeH);
    if (badge == nullptr) return nullptr;
    badge->cast<UnityEngine::UI::Image>()->virtual_set_color(peerBadgeColor(colorId));
    auto* badgeTf = ((UnityEngine::Component*)badge)->get_transform();
    badgeTf->cast<UnityEngine::Transform>()->SetParent((UnityEngine::Transform*)cellRoot, false);
    auto pos = cellToWorldPos(townmap, cellX, cellZ);
    pos.fields.x += stackFanOffset(occupyCell(cellX, cellZ));
    badgeTf->cast<UnityEngine::Transform>()->set_position(pos);

    // Face (child of badge, centered, drawn on top). Peers use their fashion
    // face sprite; our own pin (fashion < 0) reuses the live local face sprite.
    auto* face = makeUiImage(rectType, imageType, "MPPeerFace", faceW, faceH);
    if (face != nullptr) {
        auto* faceTf = ((UnityEngine::Component*)face)->get_transform();
        faceTf->cast<UnityEngine::Transform>()->SetParent((UnityEngine::Transform*)badgeTf, false);
        UnityEngine::Vector3::Object zero{}; zero.fields.x = 0; zero.fields.y = 0; zero.fields.z = 0;
        faceTf->cast<UnityEngine::Transform>()->set_localPosition(zero);
        UnityEngine::Color::Fields white{}; white.r = white.g = white.b = white.a = 1.0f;
        face->cast<UnityEngine::UI::Image>()->virtual_set_color(white);

        UnityEngine::Sprite::Object* sprite = nullptr;
        if (fashionId >= 0) {
            char nameBuf[48];
            int32_t safeColor = (colorId < 0) ? 0 : colorId;
            snprintf(nameBuf, sizeof(nameBuf), "prefab_npc_%d_%d", fashionId, safeColor);
            sprite = Dpr::UI::UIManager::get_Instance()->GetAtlasSprite(
                (SpriteAtlasID)3, System::String::Create(nameBuf));
        } else {
            sprite = ((UnityEngine::UI::Image::Object*)srcImage)->cast<UnityEngine::UI::Image>()->get_sprite();
        }
        if (sprite != nullptr) {
            face->cast<UnityEngine::UI::Image>()->set_sprite(sprite);
        }
    }
    MP_LOG("[MapIcons] spawnIconClone OK badge=%p fashion=%d\n", badge, fashionId);
    return badge;
}

// Resolve a peer's (zone, grid) to the map cell floats via Townmap.GetData.
static bool resolvePeerCell(void* townmap, const PeerMapInfo& info, float* outX, float* outZ) {
    uint64_t packed = ((uint64_t)(uint32_t)info.gridZ << 32) | (uint32_t)info.gridX;
    void* sheetData = _ILExternal::external<void*>(RVA_TOWNMAP_GET_DATA, townmap, info.zoneID, packed);
    if (sheetData == nullptr) return false;
    *outX = *(float*)((uintptr_t)sheetData + SHEETDATA_NOWPOS_OFFSET);
    *outZ = *(float*)((uintptr_t)sheetData + SHEETDATA_NOWPOS_OFFSET + 4);
    return true;
}

static void spawnAllMarkers(void* townmap) {
    auto& ctx = getOverworldMPContext();
    resetOccupancy();

    // Seed the occupancy with the LOCAL player's cell so peers standing on
    // top of you fan out instead of hiding under the vanilla marker.
    {
        int32_t zone = 0, gx = 0, gz = 0;
        if (getMyMapCell(&zone, &gx, &gz)) {
            PeerMapInfo mine = { true, zone, gx, gz };
            float cx = 0.0f, cz = 0.0f;
            if (resolvePeerCell(townmap, mine, &cx, &cz)) {
                occupyCell(cx, cz);
            }
        }
    }

    // Peer icons. During hide-and-seek the hider is never drawn on the
    // seeker's map (their last-known cell would still leak the trail).
    int32_t hiddenStation = mpMinigameHiddenStation();
    for (int i = 0; i < OW_MP_MAX_PLAYERS; i++) {
        auto& remote = ctx.remotePlayers[i];
        auto& info = s_peerMapInfo[i];
        if (!remote.isActive || !info.valid) continue;
        if (i == hiddenStation) continue;

        float cx = 0.0f, cz = 0.0f;
        if (!resolvePeerCell(townmap, info, &cx, &cz)) {
            MP_LOG("[MapIcons] No map cell for station %d (zone=%d grid=%d,%d)\n",
                        i, info.zoneID, info.gridX, info.gridZ);
            continue;
        }
        spawnIconClone(townmap, cx, cz, remote.avatarId, remote.colorId);
        MP_LOG("[MapIcons] Icon for station %d at cell %.0f,%.0f (zone=%d)\n",
                    i, cx, cz, info.zoneID);
    }

    // Peer pins
    for (int i = 0; i < OW_MP_MAX_PLAYERS; i++) {
        auto& remote = ctx.remotePlayers[i];
        auto& pin = s_peerPins[i];
        if (!remote.isActive || !pin.active) continue;
        spawnIconClone(townmap, pin.cellX, pin.cellZ, -1, 0);
    }

    // My pin
    s_myPinClone = nullptr;
    if (s_myPin.active) {
        s_myPinClone = spawnIconClone(townmap, s_myPin.cellX, s_myPin.cellZ, -1, 0);
    }
}

// ---------------------------------------------------------------------------
// Hooks
// ---------------------------------------------------------------------------

// Townmap.Setup @0x184B7B0
// Register layout (from Ghidra): 16-byte by-value first param in x0:x1,
// param_2 in x2, THIS in x3, showGoal in x4, guideData in x5, callback in x6.
// We mirror x0-x7 as int64 pass-throughs and take `this` from the 4th slot.
HOOK_DEFINE_TRAMPOLINE(Townmap$$Setup) {
    static void Callback(uint64_t a0, uint64_t a1, uint64_t a2, void* __this,
                         uint64_t a4, uint64_t a5, uint64_t a6, uint64_t a7) {
        Orig(a0, a1, a2, __this, a4, a5, a6, a7);

        s_curTownmap = __this;
        s_hoverValid = false;
        s_myPinClone = nullptr;
        s_markersPending = false;

        if (!isOverworldMPActive()) return;
        // Defer the actual spawn to the first OnUpdate (see s_markersPending) —
        // spawning here races the player-icon template's initialization.
        s_markersPending = true;
    }
};

// TownmapWindow.OnCellChanged @0x1850E20 — cache the hovered cell's map coords.
HOOK_DEFINE_TRAMPOLINE(TownmapWindow$$OnCellChanged) {
    static void Callback(void* __this, void* cell) {
        Orig(__this, cell);

        s_hoverValid = false;
        if (cell == nullptr) return;
        void* sheetData = *(void**)((uintptr_t)cell + CELL_SHEETDATA_OFFSET);
        if (sheetData == nullptr) return;
        s_hoverCellX = *(float*)((uintptr_t)sheetData + SHEETDATA_NOWPOS_OFFSET);
        s_hoverCellZ = *(float*)((uintptr_t)sheetData + SHEETDATA_NOWPOS_OFFSET + 4);
        s_hoverValid = true;
    }
};

// TownmapWindow.OnUpdate @0x18504E0 — ZL toggles a pin at the hovered cell.
HOOK_DEFINE_TRAMPOLINE(TownmapWindow$$OnUpdate) {
    static void Callback(void* __this) {
        Orig(__this);

        if (!isOverworldMPActive()) { s_prevZL = false; return; }

        // Deferred peer-marker spawn (moved out of Townmap.Setup to dodge the
        // Instantiate crash on a not-yet-ready icon template).
        if (s_markersPending) {
            s_markersPending = false;
            if (s_curTownmap != nullptr) spawnAllMarkers(s_curTownmap);
        }

        nn::hid::NpadBaseState padState = InputHelper::readNpadStateDirect();
        bool zlHeld = padState.mButtons.isBitSet(nn::hid::NpadButton::ZL);
        bool zlPressed = zlHeld && !s_prevZL;
        s_prevZL = zlHeld;
        if (!zlPressed || !s_hoverValid) return;

        // Toggle: pressing on (approximately) the current pin clears it;
        // anywhere else places/moves the pin.
        bool samePinCell = s_myPin.active &&
                           fabsf(s_myPin.cellX - s_hoverCellX) < 0.5f &&
                           fabsf(s_myPin.cellZ - s_hoverCellZ) < 0.5f;
        if (samePinCell) {
            s_myPin.active = false;
        } else {
            s_myPin.active = true;
            s_myPin.cellX = s_hoverCellX;
            s_myPin.cellZ = s_hoverCellZ;
        }
        sendMyPin();

        // Live-refresh my pin icon on the open map.
        if (s_curTownmap != nullptr) {
            if (s_myPinClone != nullptr) {
                // s_myPinClone is the cloned Image component; destroy its whole
                // GameObject so the old marker's visual goes away (destroying the
                // component alone would leave the graphic on the map).
                if (UnityEngine::_Object::op_Inequality(
                        (UnityEngine::_Object::Object*)s_myPinClone, nullptr)) {
                    auto* go = ((UnityEngine::Component*)s_myPinClone)->get_gameObject();
                    UnityEngine::_Object::Destroy((UnityEngine::_Object::Object*)go);
                }
                s_myPinClone = nullptr;
            }
            if (s_myPin.active) {
                s_myPinClone = spawnIconClone(s_curTownmap, s_myPin.cellX, s_myPin.cellZ, -1, 0);
            }
        }
    }
};

void exl_mp_map_icons_hooks() {
    Townmap$$Setup::InstallAtOffset(RVA_TOWNMAP_SETUP);
    TownmapWindow$$OnCellChanged::InstallAtOffset(RVA_WINDOW_ON_CELL_CHANGED);
    TownmapWindow$$OnUpdate::InstallAtOffset(RVA_WINDOW_ON_UPDATE);
    MP_LOG("[MapIcons] Hooks installed (Townmap.Setup, OnCellChanged, OnUpdate)\n");
}
