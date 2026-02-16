#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/NetworkUtils/SessionConnector.h"
#include "externals/OpcState.h"
#include "externals/UnityEngine/Vector3.h"

// Maximum players in an overworld multiplayer session (same as underground)
static constexpr int32_t OW_MP_MAX_PLAYERS = 8;

// Position list length for interpolation (matches OpcController.POS_LIST_LENGTH)
static constexpr int32_t OW_MP_POS_LIST_LENGTH = 20;

// Proximity radius for interaction prompt (in world units)
static constexpr float OW_MP_CONTACT_RADIUS = 2.0f;

// Position sync interval in seconds (~20 Hz)
static constexpr float OW_MP_POS_SYNC_INTERVAL_SEC = 0.05f;

// Zone change grace period in seconds — defer all MP processing after a zone transition.
// 1.5s covers the full crash window (700-950ms observed) plus safety margin.
static constexpr float OW_MP_ZONE_CHANGE_GRACE_SEC = 1.5f;

// Network data IDs (matching existing ANetData<T> DataID constants)
static constexpr uint8_t NET_DATA_ID_POS = 2;        // NetPosData
static constexpr uint8_t NET_DATA_ID_ZONE = 23;       // NetZoneData (0x17)
static constexpr uint8_t NET_DATA_ID_POS_ZONE = 67;   // NetPosZoneData (0x43)

// Custom overworld MP packet DataIDs (0xC0+ range, avoids existing 2-67)
static constexpr uint8_t OWMP_DATA_ID_POSITION      = 0xC0; // Position/avatar broadcast
static constexpr uint8_t OWMP_DATA_ID_EMOTE          = 0xC1; // Emote broadcast
static constexpr uint8_t OWMP_DATA_ID_INTERACT_REQ   = 0xC2; // Interaction request (targeted)
static constexpr uint8_t OWMP_DATA_ID_INTERACT_RESP  = 0xC3; // Interaction response (targeted)

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
    System::String::Object* playerName;
    // Previous received position — used for movement detection
    float prevPosX;
    float prevPosZ;

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
        avatarId = 0;
        colorId = 0;
        playerName = nullptr;
        prevPosX = 0.0f;
        prevPosZ = 0.0f;
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

// Send local player's position to peers
void overworldMPSendPosition();

// Send area change notification to peers
void overworldMPSendAreaChange(int32_t areaID);

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

// Check for A-button interaction with nearby remote players (called from overworldMPUpdate)
void overworldMPCheckInteraction();

// Tick emote balloon timers and delete expired balloons (called from overworldMPUpdate)
void overworldMPTickBalloons(float deltaTime);
