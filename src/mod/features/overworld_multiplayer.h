#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/NetworkUtils/SessionConnector.h"
#include "externals/OpcState.h"
#include "externals/UnityEngine/Vector3.h"

// Maximum players in an overworld multiplayer session (PIA mesh limit is 16 stations)
static constexpr int32_t OW_MP_MAX_PLAYERS = 16;

// Position list length for interpolation (matches OpcController.POS_LIST_LENGTH)
static constexpr int32_t OW_MP_POS_LIST_LENGTH = 20;

// Proximity radius for interaction prompt (in world units)
static constexpr float OW_MP_CONTACT_RADIUS = 2.0f;

// Position sync interval in seconds (~20 Hz)
static constexpr float OW_MP_POS_SYNC_INTERVAL_SEC = 0.05f;

// Zone change grace period in seconds — defer all MP processing after a zone transition.
// Crash window observed at 700-950ms; 1.0s provides sufficient margin.
static constexpr float OW_MP_ZONE_CHANGE_GRACE_SEC = 1.0f;

// Network data IDs (matching existing ANetData<T> DataID constants)
static constexpr uint8_t NET_DATA_ID_POS = 2;        // NetPosData
static constexpr uint8_t NET_DATA_ID_ZONE = 23;       // NetZoneData (0x17)
static constexpr uint8_t NET_DATA_ID_POS_ZONE = 67;   // NetPosZoneData (0x43)

// Custom overworld MP packet DataIDs (0xC0+ range, avoids existing 2-67)
static constexpr uint8_t OWMP_DATA_ID_POSITION      = 0xC0; // Position/avatar broadcast
static constexpr uint8_t OWMP_DATA_ID_EMOTE          = 0xC1; // Emote broadcast
static constexpr uint8_t OWMP_DATA_ID_INTERACT_REQ   = 0xC2; // Interaction request (targeted)
static constexpr uint8_t OWMP_DATA_ID_INTERACT_RESP  = 0xC3; // Interaction response (targeted)
static constexpr uint8_t OWMP_DATA_ID_TRADE_POKE     = 0xC4; // Trade pokemon data (targeted)
static constexpr uint8_t OWMP_DATA_ID_TRADE_CONFIRM   = 0xC5; // Trade confirmation (targeted)
static constexpr uint8_t OWMP_DATA_ID_BATTLE_PARTY    = 0xC6; // Battle party data (targeted, chunked)
static constexpr uint8_t OWMP_DATA_ID_BATTLE_READY    = 0xC7; // Battle scene sync (targeted)
static constexpr uint8_t OWMP_DATA_ID_TEAMUP_DISBAND  = 0xC8; // Team-up disband notification
static constexpr uint8_t OWMP_DATA_ID_TEAMUP_BATTLE   = 0xC9; // Team-up battle initiation
static constexpr uint8_t OWMP_DATA_ID_TEAMUP_BATTLE_ACK = 0xCA; // Team-up battle acknowledgement
static constexpr uint8_t OWMP_DATA_ID_TEAMUP_SYNC_WAIT   = 0xCB; // Team-up sync wait (trainer rendezvous)
static constexpr uint8_t OWMP_DATA_ID_TEAMUP_SYNC_CANCEL  = 0xCC; // Team-up sync cancel (going solo)
static constexpr uint8_t OWMP_DATA_ID_CUSTOM_COLORS       = 0xCD; // Custom color data (reliable, on join/zone change)
static constexpr uint8_t OWMP_DATA_ID_MAP_INFO            = 0xCE; // Town Map location broadcast (zone + grid cell)
static constexpr uint8_t OWMP_DATA_ID_MAP_PIN             = 0xCF; // Meet-up pin place/clear
static constexpr uint8_t OWMP_DATA_ID_CARD_REQUEST        = 0xD0; // Trainer card request (targeted)
static constexpr uint8_t OWMP_DATA_ID_CARD_DATA           = 0xD1; // Trainer card blob (targeted)
static constexpr uint8_t OWMP_DATA_ID_EVENT_START         = 0xD2; // Minigame start (targeted)
static constexpr uint8_t OWMP_DATA_ID_EVENT_END           = 0xD3; // Minigame end: found/timeout/forfeit (targeted)
static constexpr uint8_t OWMP_DATA_ID_EVENT_RESULT        = 0xD4; // Minigame result exchange (targeted)
static constexpr uint8_t OWMP_DATA_ID_WORLD_DAILIES       = 0xD5; // Host's daily world values (broadcast)
static constexpr uint8_t OWMP_DATA_ID_TOWER_ROUND          = 0xD6; // Multi Tower round data (targeted, chunked)
static constexpr uint8_t OWMP_DATA_ID_POFFIN_RESULT       = 0xD7; // Group Poffin cook-end stats (targeted)
static constexpr uint8_t OWMP_DATA_ID_CONTEST_RESULT      = 0xD8; // Contest section/result score exchange (targeted)
static constexpr uint8_t OWMP_DATA_ID_CONTEST_ENTRY       = 0xD9; // Contest entry data: partner's Pokémon + name (targeted)
static constexpr uint8_t OWMP_DATA_ID_COUNTER_CHECKIN     = 0xDA; // Receptionist counter check-in on/off (targeted)

// 0xC6 sub-packet types — battle party is chunked because a full party (2100+ bytes)
// exceeds the PIA PacketWriter buffer limit (~340 bytes user data, 1024 total).
static constexpr uint8_t BATTLE_PARTY_SUB_HEADER = 0;  // Header: memberCount + MYSTATUS + subtype
static constexpr uint8_t BATTLE_PARTY_SUB_POKE   = 1;  // Single Pokemon: pokeIndex + 344 bytes

// Dpr::Battle::Logic::MyStatus colorID field. Verified via MyStatus::GetColorID
// @0x203D3F0 which is literally `return *(int8_t*)(this + 0x25)`. It's a signed byte,
// so a custom-color player (-1) is stored/read as (uint8)-1 == 0xFF.
static constexpr uintptr_t MYSTATUS_COLORID_OFFSET = 0x25;
static constexpr uint8_t   MYSTATUS_COLORID_CUSTOM = 0xFF;  // == (int8)-1 : custom colors

// Per-remote-player state tracked by the overworld multiplayer system
struct FieldPlayerNetData {
    int32_t stationIndex;
    int32_t areaID;
    UnityEngine::Vector3::Object position;
    float rotationY;
    OpcState::OnlineState onlineState;
    bool isActive;           // true if this slot is occupied
    bool isSpawned;          // true if entity is currently in the scene
    bool isMoving;           // true if position changed between last two packets
    bool isRunning;          // true if movement speed suggests running (vs walking)
    int32_t avatarId;
    int32_t colorId;
    int32_t trainerId;       // MYSTATUS trainer ID (via 0xCE MAP_INFO; 0 = unknown)
    bool hasCustomColors;          // true if 0xCD custom color packet received
    float customFieldColors[18];   // 6 field colors × RGB: SkinFace, SkinMouth, Eyes, Eyebrows, SkinBody, Hair
    float customBattleColors[18];  // 6 battle colors × RGB: SkinFace, HairExtra, EyeLeft, EyeRight, SkinBody, Hair
    char playerNameBuf[52];  // native ASCII copy (max 12 chars + null + padding)
    bool playerNameSet;      // true once we've received the name
    bool isBicycle;          // remote player is riding a bicycle
    // Previous received position — used for movement detection
    float prevPosX;
    float prevPosZ;

    // Deferred color refresh — Unity renderers may not be ready on the same
    // frame as Instantiate, so we re-apply UpdateColorVariation after a short delay.
    float colorRefreshTimer;

    // Follow pokemon (Walk Together) state
    int32_t followMonsNo;       // 0 = no follow pokemon
    uint8_t followFormNo;
    uint8_t followSex;
    bool    followIsRare;
    bool    followPokeActive;    // remote player has a follow pokemon
    bool    followPokeSpawned;   // we've spawned their pokemon entity
    float   followPokeScale;     // FieldWalkingScale from catalog
    float   followPokeSpawnTimer; // scale-in animation countdown (0.3s → 0)
    float   followPokeTargetX;   // trail target position (player's previous position)
    float   followPokeTargetY;
    float   followPokeTargetZ;
    void*   followPokeEntity;    // FieldPokemonEntity::Object*
    void*   colorVariationComp;  // ColorVariation::Object* captured during OnEnable

    void Clear() {
        stationIndex = -1;
        areaID = 0;
        position = {};
        rotationY = 0.0f;
        onlineState = OpcState::OnlineState::NONE;
        isActive = false;
        isSpawned = false;
        isMoving = false;
        isRunning = false;
        isBicycle = false;
        avatarId = 0;
        colorId = 0;
        trainerId = 0;
        hasCustomColors = false;
        memset(customFieldColors, 0, sizeof(customFieldColors));
        memset(customBattleColors, 0, sizeof(customBattleColors));
        memset(playerNameBuf, 0, sizeof(playerNameBuf));
        playerNameSet = false;
        prevPosX = 0.0f;
        prevPosZ = 0.0f;
        colorRefreshTimer = 0.0f;
        followMonsNo = 0;
        followFormNo = 0;
        followSex = 0;
        followIsRare = false;
        followPokeActive = false;
        followPokeSpawned = false;
        followPokeScale = 1.0f;
        followPokeSpawnTimer = 0.0f;
        followPokeTargetX = 0.0f;
        followPokeTargetY = 0.0f;
        followPokeTargetZ = 0.0f;
        followPokeEntity = nullptr;
        colorVariationComp = nullptr;
    }
};

// Interaction state for the local player
enum class InteractionState : int32_t {
    None = 0,               // No interaction in progress
    MenuOpen = 1,           // Context menu is showing
    WaitingResponse = 2,    // Sent interaction request, waiting for response
    ReceivedRequest = 3,    // Received request, showing accept/reject dialog
    InTrade = 4,            // In trade screen
    InBattle = 5,           // In battle
};

// Interaction request types (sent in 0xC2 packet)
enum class InteractionType : uint8_t {
    Battle = 0,
    Trade = 1,
    TeamUp = 2,
    Minigame = 3,   // subtype byte carries MinigameKind
    Tower = 4,      // Multi Battle Tower invite (teamed-up pairs only)
    Poffin = 5,     // Group Poffin cooking invite (teamed-up pairs only)
    Contest = 6,    // Multiplayer Super Contest invite (teamed-up pairs only)
};

// Battle subtypes
enum class BattleSubtype : uint8_t {
    Single = 0,
    Double = 1,
};

// Emote IDs (matching underground OnlinePlayerCharacter constants)
static constexpr uint8_t EMOTE_ID_TALK        = 9;
static constexpr uint8_t EMOTE_ID_BATTLE      = 15;
static constexpr uint8_t EMOTE_ID_TRADE       = 16;
static constexpr uint8_t EMOTE_ID_CROSS       = 20;  // rejection / no
static constexpr uint8_t EMOTE_ID_LIKES       = 28;  // heart / yes
static constexpr uint8_t EMOTE_ID_EXCLAMATION = 29;

// Overall state of the overworld multiplayer system
enum class OverworldMPState : int32_t {
    Disabled = 0,            // Setting is off
    Initializing = 1,        // Starting network session
    Searching = 2,           // Session created, waiting for peers
    Connected = 3,           // Active session with peer communication
    Disconnecting = 4,       // Tearing down session
    Error = 5,               // Network error occurred
};

// Global overworld multiplayer state
struct OverworldMPContext {
    OverworldMPState state;
    int32_t myAreaID;
    int32_t frameCounter;
    FieldPlayerNetData remotePlayers[OW_MP_MAX_PLAYERS];
    void* spawnedEntities[OW_MP_MAX_PLAYERS]; // FieldObjectEntity::Object*

    void Initialize() {
        state = OverworldMPState::Disabled;
        myAreaID = 0;
        frameCounter = 0;
        for (int i = 0; i < OW_MP_MAX_PLAYERS; i++) {
            remotePlayers[i].Clear();
            spawnedEntities[i] = nullptr;
        }
    }
};

// Access the global context
OverworldMPContext& getOverworldMPContext();

// Check if overworld MP is currently enabled and active
bool isOverworldMPActive();

// Cross-module state used by color_variations.cpp during Instantiate.
// Set before Instantiate so ColorVariation_OnEnable applies the remote preset.
extern bool g_owmpSkipCustomColorOverride;
extern int32_t g_owmpRemoteColorId;

// Start/stop the overworld multiplayer session
void overworldMPStart();
void overworldMPStop();

// Called each frame from FieldManager.Update hook
void overworldMPUpdate(float deltaTime);

// Called on area change from FieldManager.OnZoneChange hook
void overworldMPOnAreaChange(int32_t newAreaID);

// Handle a remote player joining/leaving
void overworldMPOnPlayerJoin(int32_t stationIndex);
void overworldMPOnPlayerLeave(int32_t stationIndex);

// Spawn/despawn a remote player's entity in the scene
void overworldMPSpawnEntity(int32_t stationIndex);
void overworldMPDespawnEntity(int32_t stationIndex);
void overworldMPDespawnAllEntities();

// Toggle a spawned remote entity's visibility (GameObject.SetActive on the
// entity + follow pokemon). Position sync continues while hidden.
void overworldMPSetEntityVisible(int32_t stationIndex, bool visible);

// Color self-heal: if cvComp is a spawned remote's captured ColorVariation,
// schedule that remote's deferred color refresh (any SetActive cycle — battle
// return, cutscene — resets colors via OnEnable). Returns true if matched.
bool overworldMPHealRemoteColors(void* cvComp);

// Send local player's position to peers
void overworldMPSendPosition();

// Send area change notification to peers
void overworldMPSendAreaChange(int32_t areaID);

// Send custom color data (reliable, only when colorId == -1)
void overworldMPSendCustomColors();

// Find the closest remote player entity within contact radius, returns station index or -1
int32_t overworldMPFindNearestPlayer(UnityEngine::Vector3::Object localPos);

// Interaction menu entry point
void overworldMPShowInteractionMenu(int32_t targetStationIndex);

// Send an emote to all peers
void overworldMPSendEmote(uint8_t emoteId);

// Display emote balloon above a remote player's entity
void overworldMPShowRemoteEmote(int32_t stationIndex, uint8_t emoteId);

// Send an interaction request to a specific player
void overworldMPSendInteractionRequest(int32_t targetStation, InteractionType type, BattleSubtype subtype);

// Send an interaction response to a specific player
void overworldMPSendInteractionResponse(int32_t targetStation, bool accepted);

// Get current interaction state
InteractionState overworldMPGetInteractionState();

// Load a ss_multiplayer message label as a C string, with English fallback
// when the label is missing from the deployed bundle. NOT reentrant: result
// points into a shared static buffer — copy before the next call.
const char* overworldMPGetMessageCStr(const char* label, const char* fallback);

// Reset network-level interaction state back to None (call after interaction completes)
void overworldMPResetInteractionState();

// Show incoming request dialog to the local player (called from 0xC2 receive handler)
void overworldMPShowIncomingRequestDialog(int32_t fromStation, InteractionType type, BattleSubtype battleSubtype = BattleSubtype::Single);

// Called when the partner accepts/declines our request (from 0xC3 receive handler)
void overworldMPOnRequestAccepted(int32_t partnerStation);
void overworldMPOnRequestDeclined(int32_t partnerStation);

// Check for Y-button interaction with nearby remote players (called from overworldMPUpdate)
void overworldMPCheckInteraction();

// Tick emote balloon timers and delete expired balloons (called from overworldMPUpdate)
void overworldMPTickBalloons(float deltaTime);

// Clear all tracked emote balloons. deleteFromCanvas=true releases them through the
// FieldCanvas (use while it's alive, e.g. battle transition); false only drops the
// tracking pointers (use on zone change, when the canvas/balloons are already gone).
void overworldMPClearAllBalloons(bool deleteFromCanvas);

// Start trade flow after handshake accept (called from overworldMPOnRequestAccepted)
void overworldMPStartTrade(int32_t partnerStation);

// Send trade pokemon data to partner
void overworldMPSendTradePoke(int32_t targetStation, int32_t partySlot, uint8_t* data, int32_t size);

// Send trade confirmation to partner
void overworldMPSendTradeConfirm(int32_t targetStation, bool confirmed);

// Called from 0xC4/0xC5 receive handlers to notify the trade state machine
void overworldMPOnTradePokeReceived(int32_t fromStation, int32_t partySlot, uint8_t* data, int32_t size);
void overworldMPOnTradeConfirmReceived(int32_t fromStation, bool confirmed);

// Battle: send local party data to partner (0xC6 packet)
void overworldMPSendBattleParty(int32_t targetStation, BattleSubtype subtype);

// Battle: called from 0xC6 receive handler
void overworldMPOnBattlePartyReceived(int32_t fromStation, uint8_t* data, int32_t size);

// Battle: start battle flow after handshake accept
void overworldMPStartBattle(int32_t partnerStation, BattleSubtype subtype);

// Battle: send BATTLE_READY sync packet to partner (0xC7 packet)
void overworldMPSendBattleReady(int32_t targetStation);

// Battle: called from 0xC7 receive handler
void overworldMPOnBattleReadyReceived(int32_t fromStation);

// Battle scene flag — suppresses overworld packet reading so the battle
// system's ReceivePacketExCallback gets clean PacketReaders.
void overworldMPSetInBattleScene(bool inBattle);
bool overworldMPIsInBattleScene();

// Scriptless comm-battle transition (extracted from the PvP battle start):
// clears balloons, sets the battle-scene flag, runs EncountStart's setup
// side-effects, bypasses the encounter-effect state machine, swaps BGM, fades
// to black and flags the Sequencer to load the battle scene. `bsp` is the
// fully prepared BATTLE_SETUP_PARAM::Object* (void* to keep this header light).
void overworldMPBeginCommBattleTransition(void* bsp);

// Restore local party to pre-battle state (HP/PP/status) after a PvP battle.
// Called when the battle scene ends and _updateType returns to 0.
void overworldMPRestorePartyAfterBattle();

// Check if a specific station index is still connected
bool overworldMPIsStationConnected(int32_t stationIndex);

// Abort any in-progress interaction when a partner disconnects
void overworldMPOnInteractionPartnerLeft(int32_t stationIndex);

// Track which station is our battle partner during the battle scene.
// Set before battle starts, checked in onPlayerLeave to detect mid-battle disconnect.
void overworldMPSetActiveBattlePartner(int32_t stationIndex);
int32_t overworldMPGetActiveBattlePartner();

// True if the battle partner disconnected during the battle scene.
// Checked by BattleProc hooks to stop error suppression and signal disconnect.
bool overworldMPIsBattlePartnerDisconnected();
