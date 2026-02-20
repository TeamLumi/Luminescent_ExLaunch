#include "exlaunch.hpp"

#include "features/team_up.h"
#include "features/overworld_multiplayer.h"

#include "externals/Dpr/Battle/Logic/BATTLE_SETUP_PARAM.h"
#include "externals/Dpr/EncountTools.h"
#include "externals/Dpr/NetworkUtils/NetworkManager.h"
#include "externals/DPData/MYSTATUS.h"
#include "externals/FieldManager.h"
#include "externals/FlagWork.h"
#include "externals/FlagWork_Enums.h"
#include "externals/GameManager.h"
#include "externals/MYSTATUS_COMM.h"
#include "externals/PlayerWork.h"
#include "externals/TrainerWork.h"
#include "externals/Pml/PokePara/CoreParam.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/Pml/PokeParty.h"
#include "externals/System/String.h"
#include "externals/Audio/AudioManager.h"

#include "externals/il2cpp-api.h"
#include "logger/logger.h"

// ---------------------------------------------------------------------------
// IL2CPP vtable dispatch helpers (same as overworld_multiplayer.cpp)
// ---------------------------------------------------------------------------
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

// PacketWriter vtable offsets
static constexpr uint32_t PW_RESET      = 0x1b0;
static constexpr uint32_t PW_WRITE_BYTE = 0x270;
static constexpr uint32_t PW_WRITE_S32  = 0x2D0;

// PokePara sizes
static constexpr int32_t POKE_FULL_DATA_SIZE = 344;

// ---------------------------------------------------------------------------
// Global state
// ---------------------------------------------------------------------------
static TeamUpState s_teamUpState;

// Accumulation buffers for chunked TEAMUP_BATTLE / TEAMUP_BATTLE_ACK packets
// Non-static: accessed from overworld_multiplayer.cpp packet dispatcher via extern
int32_t s_tuAccumFromStation = -1;
uint8_t s_tuAccumMemberCount = 0;
uint8_t s_tuAccumReceivedCount = 0;
uint8_t s_tuAccumBuf[6 * POKE_FULL_DATA_SIZE + 128] = {};
int32_t s_tuAccumBufSize = 0;
uint8_t s_tuAccumBattleType = 0;  // 1=trainer (only trainer team-up supported)
bool    s_tuAccumIsAck = false;    // true if accumulating ACK (not initial BATTLE)

// normalTrainer @ 0x1F6EE00 — fills a BSP party slot with trainer AI data
static inline void normalTrainer(Dpr::Battle::Logic::BATTLE_SETUP_PARAM::Object* bsp,
                                  int32_t clientId, int32_t trainerID) {
    _ILExternal::external<void>(0x1F6EE00, bsp, clientId, trainerID);
}

// Split trainer party between two AI slots (defined in trainer_double_battle.cpp)
extern void splitTrainerParty(Dpr::Battle::Logic::BATTLE_SETUP_PARAM::Object* bsp,
                               int slot1, int slot3);

// Cached BSP pointer for Player A's in-flight battle modification
// Non-static: also accessed from trainer_double_battle.cpp via extern
Dpr::Battle::Logic::BATTLE_SETUP_PARAM::Object* s_teamUpBSP = nullptr;

// Guard flag for CreateLocalClient hook
static bool s_teamUpCreateLocalClientActive = false;

TeamUpState& overworldMPGetTeamUpState() {
    return s_teamUpState;
}

bool overworldMPIsTeamedUp() {
    return s_teamUpState.isTeamedUp;
}

// ---------------------------------------------------------------------------
// Team-Up handshake
// ---------------------------------------------------------------------------
void overworldMPTeamUp(int32_t partnerStation) {
    s_teamUpState.Clear();
    s_teamUpState.isTeamedUp = true;
    s_teamUpState.partnerStation = partnerStation;
    Logger::log("[TeamUp] Teamed up with station %d\n", partnerStation);
}

// ---------------------------------------------------------------------------
// Disband
// ---------------------------------------------------------------------------
static void sendTeamUpDisband(int32_t targetStation) {
    if (!isOverworldMPActive()) return;

    void* pw = Dpr::NetworkUtils::NetworkManager::get_PacketWriterRe();
    if (pw == nullptr) return;

    il2cpp_vcall_void(pw, PW_RESET);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, OWMP_DATA_ID_TEAMUP_DISBAND);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, targetStation);

    Dpr::NetworkUtils::NetworkManager::SendReliablePacketToAll(pw, 0);
    Logger::log("[TeamUp] Sent DISBAND to station %d\n", targetStation);
}

void overworldMPTeamUpDisband() {
    if (!s_teamUpState.isTeamedUp) return;

    int32_t partner = s_teamUpState.partnerStation;
    sendTeamUpDisband(partner);
    s_teamUpState.Clear();
    Logger::log("[TeamUp] Disbanded (was teamed with station %d)\n", partner);
}

void overworldMPOnTeamUpDisbandReceived(int32_t fromStation) {
    if (!s_teamUpState.isTeamedUp) return;
    if (s_teamUpState.partnerStation != fromStation) return;

    s_teamUpState.Clear();
    Logger::log("[TeamUp] Partner station %d disbanded\n", fromStation);
}

// ---------------------------------------------------------------------------
// Auto-disband check (called each tick)
// ---------------------------------------------------------------------------
void overworldMPTeamUpAutoDisband() {
    if (!s_teamUpState.isTeamedUp) return;

    // NPC companion active (Riley, Cheryl, etc.) → disband
    if (PlayerWork::GetSystemFlag((int32_t)FlagWork_SysFlag::SYS_FLAG_PAIR)) {
        Logger::log("[TeamUp] Auto-disband: SYS_FLAG_PAIR is set\n");
        overworldMPTeamUpDisband();
        return;
    }

    // Partner disconnected → disband
    auto& ctx = getOverworldMPContext();
    int32_t ps = s_teamUpState.partnerStation;
    if (ps < 0 || ps >= OW_MP_MAX_PLAYERS || !ctx.remotePlayers[ps].isActive) {
        Logger::log("[TeamUp] Auto-disband: partner station %d disconnected\n", ps);
        s_teamUpState.Clear();
        return;
    }
}

// ---------------------------------------------------------------------------
// Send party data using chunked protocol (reuses 0xC6 pattern)
// dataId = OWMP_DATA_ID_TEAMUP_BATTLE or OWMP_DATA_ID_TEAMUP_BATTLE_ACK
// ---------------------------------------------------------------------------
static void sendTeamUpPartyChunked(int32_t targetStation, uint8_t dataId,
                                    uint8_t battleType, int32_t arenaID,
                                    int32_t weatherType, int32_t trainerID) {
    void* pw = Dpr::NetworkUtils::NetworkManager::get_PacketWriterRe();
    if (pw == nullptr) return;

    auto* party = PlayerWork::get_playerParty();
    if (party == nullptr) return;

    int32_t memberCount = party->fields.m_memberCount;
    if (memberCount > TEAMUP_PARTY_LIMIT) memberCount = TEAMUP_PARTY_LIMIT;

    // --- HEADER sub-packet ---
    // Format: [dataId][target:4][sub:HEADER][battleType:1][memberCount:1][wildCount:1(always 0)]
    //         [arenaID:4][weatherType:4][trainerID:4][trainerID2:4][effectID:4][MYSTATUS fields]
    il2cpp_vcall_void(pw, PW_RESET);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, dataId);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, targetStation);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, TEAMUP_SUB_HEADER);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, battleType);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, (uint8_t)memberCount);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, 0); // wildCount (always 0, trainer-only)
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, arenaID);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, weatherType);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, trainerID);

    // Write trainerID2 and battleEffectID (from TeamUpState)
    auto& tuSend = overworldMPGetTeamUpState();
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, tuSend.battleTrainerID2);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, tuSend.battleEffectID);

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
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, playerStatus->fields.region_code);
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, playerStatus->fields.fashion);
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, playerStatus->fields.body_type);
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, 0); // hat
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, 0); // shoes
    } else {
        il2cpp_vcall_write_s32(pw, PW_WRITE_S32, 0);
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, 0);
        for (int i = 0; i < 6; i++)
            il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, 0);
    }

    Dpr::NetworkUtils::NetworkManager::SendReliablePacketToAll(pw, 0);
    Logger::log("[TeamUp] Sent %s HEADER: target=%d members=%d battle=%d\n",
                dataId == OWMP_DATA_ID_TEAMUP_BATTLE ? "BATTLE" : "ACK",
                targetStation, memberCount, (int)battleType);

    // --- Per-Pokemon sub-packets ---
    for (int32_t i = 0; i < memberCount; i++) {
        auto* poke = party->GetMemberPointer(i);
        uint8_t pokeBuf[344];
        memset(pokeBuf, 0, sizeof(pokeBuf));

        if (poke != nullptr && poke->fields.m_accessor != nullptr) {
            _ILExternal::external<void>(0x24A4470, poke->fields.m_accessor, pokeBuf);
        }

        il2cpp_vcall_void(pw, PW_RESET);
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, dataId);
        il2cpp_vcall_write_s32(pw, PW_WRITE_S32, targetStation);
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, TEAMUP_SUB_POKE);
        il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, (uint8_t)i);

        for (int j = 0; j < 86; j++) {
            int32_t val = 0;
            memcpy(&val, &pokeBuf[j * 4], 4);
            il2cpp_vcall_write_s32(pw, PW_WRITE_S32, val);
        }

        Dpr::NetworkUtils::NetworkManager::SendReliablePacketToAll(pw, 0);
        Logger::log("[TeamUp] Sent %s POKE[%d]\n",
                    dataId == OWMP_DATA_ID_TEAMUP_BATTLE ? "BATTLE" : "ACK", i);
    }
}

// ---------------------------------------------------------------------------
// Public send functions
// ---------------------------------------------------------------------------
void overworldMPSendTeamUpBattleTrainer(int32_t targetStation,
                                         int32_t enemyID0, int32_t arenaID,
                                         int32_t weatherType) {
    sendTeamUpPartyChunked(targetStation, OWMP_DATA_ID_TEAMUP_BATTLE,
                           1, arenaID, weatherType, enemyID0);
}

void overworldMPSendTeamUpBattleAck(int32_t targetStation) {
    // ACK sends our party but no encounter data
    sendTeamUpPartyChunked(targetStation, OWMP_DATA_ID_TEAMUP_BATTLE_ACK,
                           0, 0, 0, 0);
}

// ---------------------------------------------------------------------------
// Deserialize partner party from accumulated buffer
// (Same logic as overworld_mp_interact.cpp::deserializeBattleParty)
// ---------------------------------------------------------------------------
static Pml::PokeParty::Object* deserializeTeamUpParty(uint8_t* buf, int32_t size, int32_t count) {
    if (count <= 0 || count > 6) return nullptr;

    auto* localParty = PlayerWork::get_playerParty();
    if (localParty == nullptr) return nullptr;

    auto* partyKlass = (Il2CppClass*)localParty->klass;
    if (partyKlass == nullptr) return nullptr;

    auto* party = (Pml::PokeParty::Object*)il2cpp_object_new(partyKlass);
    _ILExternal::external<void>(0x2055D10, party); // PokeParty::ctor()

    int32_t validCount = 0;
    for (int i = 0; i < count; i++) {
        int32_t offset = i * POKE_FULL_DATA_SIZE;
        if (offset + POKE_FULL_DATA_SIZE > size) break;

        auto* slotPoke = party->GetMemberPointer(i);
        if (slotPoke == nullptr) continue;

        auto* accessor = slotPoke->fields.m_accessor;
        if (accessor != nullptr) {
            _ILExternal::external<void>(0x24A4550, accessor, &buf[offset]);
        }

        // Validate
        auto* coreParam = (Pml::PokePara::CoreParam*)slotPoke;
        int32_t monsNo = coreParam->GetMonsNo();
        int32_t seikaku = coreParam->GetSeikaku();
        uint32_t level = coreParam->GetLevel();

        if (monsNo <= 0 || monsNo > 905 || seikaku < 0 || seikaku >= 25 || level == 0 || level > 100) {
            Logger::log("[TeamUp] REJECT partner poke[%d]: mons=%d seikaku=%d lv=%u\n",
                        i, monsNo, seikaku, level);
            break;
        }
        validCount++;
    }

    if (validCount == 0) return nullptr;
    party->fields.m_memberCount = validCount;
    Logger::log("[TeamUp] Deserialized partner party: %d members\n", validCount);
    return party;
}

// Deserialize MYSTATUS_COMM from buffer at given offset
static MYSTATUS_COMM::Object deserializeTeamUpStatus(uint8_t* buf, int32_t size, int32_t& offset) {
    MYSTATUS_COMM::Object status = {};

    if (offset + 4 > size) return status;
    memcpy(&status.fields.id, &buf[offset], 4);
    offset += 4;

    if (offset + 1 > size) return status;
    uint8_t nameLen = buf[offset++];
    if (nameLen > 0 && nameLen <= 12 && offset + nameLen * 2 <= size) {
        status.fields.name = System::String::fromUnicodeBytes(&buf[offset], nameLen * 2);
        offset += nameLen * 2;
    } else {
        status.fields.name = System::String::Create("Trainer");
    }

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
// Modify BSP for PP_AA team-up battle
// ---------------------------------------------------------------------------
void overworldMPModifyBSPForTeamUp(Dpr::Battle::Logic::BATTLE_SETUP_PARAM::Object* bsp,
                                    TeamUpState& tu, bool isInitiator) {
    auto* fields = &bsp->instance()->fields;

    int32_t myStation = _ILExternal::external<int32_t>(0x23BC000); // ThisStationIndex

    // 1. Set comm battle mode
    fields->commMode = 1;    // BTL_COMM_WIRELESS
    fields->multiMode = 2;   // BTL_MULTIMODE_PP_AA (slots 0,2 = human, 1,3 = AI)
    fields->competitor = (Dpr::Battle::Logic::BtlCompetitor)3; // BTL_COMPETITOR_COMM

    if (isInitiator) {
        fields->commPos = 0;  // We are slot 0 (PLAYER)
    } else {
        fields->commPos = 2;  // We are slot 2 (PARTNER)
    }

    // 2. Set stations array
    if (fields->stations != nullptr && fields->stations->max_length >= 4) {
        if (isInitiator) {
            fields->stations->m_Items[0] = myStation;
            fields->stations->m_Items[1] = -1;  // AI
            fields->stations->m_Items[2] = tu.partnerStation;
            fields->stations->m_Items[3] = -1;  // AI
        } else {
            fields->stations->m_Items[0] = tu.partnerStation;
            fields->stations->m_Items[1] = -1;  // AI
            fields->stations->m_Items[2] = myStation;
            fields->stations->m_Items[3] = -1;  // AI
        }
    } else {
        Logger::log("[TeamUp] WARNING: stations array too small or null (max_length=%d)\n",
                    fields->stations ? (int)fields->stations->max_length : -1);
    }

    // 3. Inject partner party into slot 2 (or slot 0 for non-initiator)
    int32_t partnerSlot = isInitiator ? 2 : 0;
    if (fields->party != nullptr && fields->party->max_length > (uint64_t)partnerSlot) {
        auto* partnerParty = deserializeTeamUpParty(
            tu.partnerPartyBuf, tu.partnerPartyBufSize, tu.partnerPartyCount);
        if (partnerParty != nullptr && fields->party->m_Items[partnerSlot] != nullptr) {
            // Copy partner's Pokemon into the BSP party slot
            auto* destParty = fields->party->m_Items[partnerSlot];
            int32_t count = partnerParty->fields.m_memberCount;
            if (count > TEAMUP_PARTY_LIMIT) count = TEAMUP_PARTY_LIMIT;

            for (int i = 0; i < count; i++) {
                auto* srcPoke = partnerParty->GetMemberPointer(i);
                auto* dstPoke = destParty->GetMemberPointer(i);
                if (srcPoke != nullptr && dstPoke != nullptr &&
                    srcPoke->fields.m_accessor != nullptr && dstPoke->fields.m_accessor != nullptr) {
                    uint8_t tmpBuf[344];
                    _ILExternal::external<void>(0x24A4470, srcPoke->fields.m_accessor, tmpBuf);
                    _ILExternal::external<void>(0x24A4550, dstPoke->fields.m_accessor, tmpBuf);
                }
            }
            destParty->fields.m_memberCount = count;
            Logger::log("[TeamUp] Injected partner party into slot %d (%d members)\n",
                        partnerSlot, count);
        }
    }

    // 4. Inject partner MYSTATUS into the correct slot
    if (fields->playerStatus != nullptr && fields->playerStatus->max_length > (uint64_t)partnerSlot) {
        int32_t mystOff = 0;
        MYSTATUS_COMM::Object partnerCommStatus = deserializeTeamUpStatus(
            tu.partnerMystatusBuf, tu.partnerMystatusLen, mystOff);

        auto* statusSlot = fields->playerStatus->m_Items[partnerSlot];
        if (statusSlot != nullptr) {
            // Copy fields from MYSTATUS_COMM into MyStatus (different layouts)
            statusSlot->fields.name = partnerCommStatus.fields.name;
            statusSlot->fields.sex = partnerCommStatus.fields.sex;
            statusSlot->fields.lang = partnerCommStatus.fields.lang;
            statusSlot->fields.id = partnerCommStatus.fields.id;
            statusSlot->fields.fashion = partnerCommStatus.fields.fashion;
            statusSlot->fields.body_type = partnerCommStatus.fields.body_type;
            statusSlot->fields.hat = partnerCommStatus.fields.hat;
            statusSlot->fields.shoes = partnerCommStatus.fields.shoes;
            Logger::log("[TeamUp] Injected partner MYSTATUS into slot %d\n", partnerSlot);
        }
    }

    Logger::log("[TeamUp] BSP modified: commMode=%d multiMode=%d commPos=%d stations=[%d,%d,%d,%d]\n",
                (int)fields->commMode, (int)fields->multiMode, (int)fields->commPos,
                fields->stations ? fields->stations->m_Items[0] : -99,
                fields->stations ? fields->stations->m_Items[1] : -99,
                fields->stations ? fields->stations->m_Items[2] : -99,
                fields->stations ? fields->stations->m_Items[3] : -99);
}

// ---------------------------------------------------------------------------
// Packet receive handlers (called from overworld_multiplayer.cpp dispatcher)
// ---------------------------------------------------------------------------

// TEAMUP_BATTLE header accumulated — store encounter + party data
void overworldMPOnTeamUpBattleReceived(int32_t fromStation, uint8_t* data, int32_t size) {
    auto& tu = s_teamUpState;
    if (!tu.isTeamedUp || tu.partnerStation != fromStation) {
        Logger::log("[TeamUp] Ignoring TEAMUP_BATTLE from non-partner station %d\n", fromStation);
        return;
    }

    // Already parsed by accumulator — tu fields are set by the accum handler.
    // Send ACK with our own party, then enter battle
    Logger::log("[TeamUp] Received TEAMUP_BATTLE: type=%d trainer=%d — sending ACK\n",
                (int)tu.battleType, tu.battleTrainerID);

    tu.isInitiator = false; // Player B — no event script
    overworldMPSendTeamUpBattleAck(fromStation);
    tu.battlePending = true;
    tu.partnerPartyValid = true;

    // Set the interaction state to enter battle on next tick
    overworldMPSetTeamUpBattleStarting();
}

// TEAMUP_BATTLE_ACK accumulated — partner responded with their party.
// Player A builds a full comm battle BSP using SetupBattleComm, replacing the
// trainer-battle BSP that Orig created. This gives us proper comm arrays (stations,
// playerStatus) that the trainer BSP doesn't have.
void overworldMPOnTeamUpBattleAckReceived(int32_t fromStation, uint8_t* data, int32_t size) {
    auto& tu = s_teamUpState;
    if (!tu.isTeamedUp || tu.partnerStation != fromStation) {
        Logger::log("[TeamUp] Ignoring TEAMUP_BATTLE_ACK from non-partner station %d\n", fromStation);
        return;
    }

    tu.partnerPartyValid = true;
    Logger::log("[TeamUp] Received TEAMUP_BATTLE_ACK: partner party valid, %d members\n",
                tu.partnerPartyCount);

    auto* bsp = s_teamUpBSP;
    if (bsp == nullptr) {
        Logger::log("[TeamUp] WARNING: s_teamUpBSP is null, cannot modify BSP\n");
        return;
    }

    // Deserialize partner party
    auto* partnerParty = deserializeTeamUpParty(
        tu.partnerPartyBuf, tu.partnerPartyBufSize, tu.partnerPartyCount);
    if (partnerParty == nullptr) {
        Logger::log("[TeamUp] ERROR: failed to deserialize partner party for BSP\n");
        s_teamUpBSP = nullptr;
        tu.battlePending = false;
        return;
    }

    // Get our own party (trimmed to TEAMUP_PARTY_LIMIT)
    auto* myParty = PlayerWork::get_playerParty();
    if (myParty == nullptr) {
        Logger::log("[TeamUp] ERROR: local party is null\n");
        s_teamUpBSP = nullptr;
        tu.battlePending = false;
        return;
    }

    auto* myTrimmedParty = (Pml::PokeParty::Object*)il2cpp_object_new((Il2CppClass*)myParty->klass);
    _ILExternal::external<void>(0x2055D10, myTrimmedParty);
    int32_t myCount = myParty->fields.m_memberCount;
    if (myCount > TEAMUP_PARTY_LIMIT) myCount = TEAMUP_PARTY_LIMIT;
    for (int i = 0; i < myCount; i++) {
        auto* src = myParty->GetMemberPointer(i);
        auto* dst = myTrimmedParty->GetMemberPointer(i);
        if (src && dst && src->fields.m_accessor && dst->fields.m_accessor) {
            uint8_t tmp[344];
            _ILExternal::external<void>(0x24A4470, src->fields.m_accessor, tmp);
            _ILExternal::external<void>(0x24A4550, dst->fields.m_accessor, tmp);
        }
    }
    myTrimmedParty->fields.m_memberCount = myCount;

    // Build MYSTATUS_COMM for both players
    MYSTATUS_COMM::Object myStatus = {};
    myStatus.SetupFromPlayerWork();

    int32_t mystOff = 0;
    MYSTATUS_COMM::Object partnerStatus = deserializeTeamUpStatus(
        tu.partnerMystatusBuf, tu.partnerMystatusLen, mystOff);

    uint8_t regulation[4] = { 1, 6, 2, 0x07 };
    static uint8_t s_emptyCapsuleArray2[32] = {};
    void* emptyCapsule = (void*)s_emptyCapsuleArray2;

    int32_t myStation = _ILExternal::external<int32_t>(0x23BC000);

    // Call SetupBattleComm (commRule=1 double, commPos=0, us=slot0 partner=slot1)
    // This REPLACES the trainer BSP with a proper comm-battle BSP that has
    // stations/playerStatus arrays.
    Dpr::EncountTools::SetupBattleComm(bsp, 0, 1/*double*/, 0/*commPos*/, regulation,
        myStation, myTrimmedParty, &myStatus, emptyCapsule,             // slot 0: us
        tu.partnerStation, partnerParty, &partnerStatus, emptyCapsule,  // slot 1: partner
        -1, nullptr, nullptr, nullptr,                                   // slot 2: empty
        -1, nullptr, nullptr, nullptr,                                   // slot 3: empty
        nullptr, nullptr, 0, 0);

    Logger::log("[TeamUp] Player A: SetupBattleComm complete, rearranging for PP_AA...\n");

    // Rearrange from PvP layout [us, partner, empty, empty]
    // to PP_AA layout [us, AI_trainer1, partner, AI_trainer2]
    auto* fields = &bsp->instance()->fields;

    // Move slot 1 (partner) → slot 2
    if (fields->party != nullptr && fields->party->max_length >= 4) {
        auto* slot1Party = fields->party->m_Items[1];
        auto* slot2Party = fields->party->m_Items[2];
        if (slot1Party != nullptr) {
            if (slot2Party == nullptr) {
                slot2Party = (Pml::PokeParty::Object*)il2cpp_object_new((Il2CppClass*)myParty->klass);
                _ILExternal::external<void>(0x2055D10, slot2Party);
                fields->party->m_Items[2] = slot2Party;
            }
            int32_t cnt = slot1Party->fields.m_memberCount;
            for (int i = 0; i < cnt && i < 6; i++) {
                auto* s = slot1Party->GetMemberPointer(i);
                auto* d = slot2Party->GetMemberPointer(i);
                if (s && d && s->fields.m_accessor && d->fields.m_accessor) {
                    uint8_t tmp[344];
                    _ILExternal::external<void>(0x24A4470, s->fields.m_accessor, tmp);
                    _ILExternal::external<void>(0x24A4550, d->fields.m_accessor, tmp);
                }
            }
            slot2Party->fields.m_memberCount = cnt;
        }
    }

    // Move stations: slot 1 → slot 2
    if (fields->stations != nullptr && fields->stations->max_length >= 4) {
        fields->stations->m_Items[2] = fields->stations->m_Items[1];
        fields->stations->m_Items[1] = -1;
        fields->stations->m_Items[3] = -1;
    }

    // Move playerStatus: slot 1 → slot 2
    if (fields->playerStatus != nullptr && fields->playerStatus->max_length >= 4) {
        auto* st1 = fields->playerStatus->m_Items[1];
        auto* st2 = fields->playerStatus->m_Items[2];
        if (st1 != nullptr && st2 != nullptr) {
            st2->fields.name = st1->fields.name;
            st2->fields.sex = st1->fields.sex;
            st2->fields.lang = st1->fields.lang;
            st2->fields.id = st1->fields.id;
            st2->fields.fashion = st1->fields.fashion;
            st2->fields.body_type = st1->fields.body_type;
            st2->fields.hat = st1->fields.hat;
            st2->fields.shoes = st1->fields.shoes;
        }
    }

    // Manual PP_AA setup
    fields->multiMode = 2;  // PP_AA
    fields->commPos = 0;    // We (Player A) control slot 0

    // Trainer: competitor=TRAINER
    fields->competitor = (Dpr::Battle::Logic::BtlCompetitor)1;
    // normalTrainer for BOTH slots — slot 3 needs full infrastructure (partyDesc, tr_data)
    // or BtlNet server init crashes at step 5
    _ILExternal::external<void>(0x1F6EE00, bsp, (int32_t)1, tu.battleTrainerID);
    if (tu.battleTrainerID2 > 0) {
        // Already-double: second trainer in slot 3
        _ILExternal::external<void>(0x1F6EE00, bsp, (int32_t)3, tu.battleTrainerID2);
    } else {
        // Single trainer: set up full slot 3, then split Pokemon between slots
        _ILExternal::external<void>(0x1F6EE00, bsp, (int32_t)3, tu.battleTrainerID);
        splitTrainerParty(bsp, 1, 3);
    }
    // btlEffComponent: use the trainer's BattleEffectID for correct intro/BGM
    int32_t effIdx = (tu.battleEffectID >= 0) ? tu.battleEffectID : 0x10;
    if (fields->btlEffComponent != nullptr)
        _ILExternal::external<void>(0x0187B7E0, fields->btlEffComponent,
            effIdx, (int32_t)-1, (int32_t)0, (uint64_t)0);
    Logger::log("[TeamUp] Player A: trainer PP_AA setup (trainer=%d/%d, effect=%d)\n",
                tu.battleTrainerID, tu.battleTrainerID2, effIdx);

    // Override arena (SetupBattleComm hardcodes Union Room arena 0x2b)
    if (tu.battleArenaID > 0) {
        void* fieldSit = fields->fieldSituation;
        if (fieldSit != nullptr) {
            void* bgComp = *(void**)((uintptr_t)fieldSit + 0x10);
            if (bgComp != nullptr) {
                _ILExternal::external<void>(0x188A6C0, bgComp, tu.battleArenaID);
            }
        }
    }

    // Enable CreateLocalClient hook — the battle engine will call it during
    // setupseq_comm_create_server_client to create clients. We need local AI
    // clients for PP_AA slots 1,3 regardless of isRemoteServer.
    s_teamUpCreateLocalClientActive = true;

    // NOW suppress overworld packet processing — BSP is ready
    overworldMPSetInBattleScene(true);

    s_teamUpBSP = nullptr;
    tu.battlePending = false;
    Logger::log("[TeamUp] Player A: PP_AA BSP complete, commMode=1 (wireless)\n");
}

// ---------------------------------------------------------------------------
// Partner-side (Player B): setup BSP and enter battle scene
// Called from the interaction tick when TeamUpBattleStarting state is reached
// ---------------------------------------------------------------------------
void overworldMPSetupTeamUpBattle() {
    auto& tu = s_teamUpState;

    Logger::log("[TeamUp] Player B: setting up team-up trainer battle (trainer=%d)\n",
                tu.battleTrainerID);

    auto* bsp = PlayerWork::get_battleSetupParam();
    if (bsp == nullptr) {
        Logger::log("[TeamUp] ERROR: battleSetupParam is null\n");
        tu.battlePending = false;
        return;
    }

    int32_t myStation = _ILExternal::external<int32_t>(0x23BC000);

    // Deserialize partner's party (the initiator)
    auto* partnerParty = deserializeTeamUpParty(
        tu.partnerPartyBuf, tu.partnerPartyBufSize, tu.partnerPartyCount);
    if (partnerParty == nullptr) {
        Logger::log("[TeamUp] ERROR: failed to deserialize partner party\n");
        tu.battlePending = false;
        return;
    }

    // Get our party (first 3 members)
    auto* myParty = PlayerWork::get_playerParty();
    if (myParty == nullptr) {
        Logger::log("[TeamUp] ERROR: local party is null\n");
        tu.battlePending = false;
        return;
    }

    // Build a trimmed local party (first TEAMUP_PARTY_LIMIT members)
    auto* myTrimmedParty = (Pml::PokeParty::Object*)il2cpp_object_new((Il2CppClass*)myParty->klass);
    _ILExternal::external<void>(0x2055D10, myTrimmedParty); // PokeParty::ctor()
    int32_t myCount = myParty->fields.m_memberCount;
    if (myCount > TEAMUP_PARTY_LIMIT) myCount = TEAMUP_PARTY_LIMIT;
    for (int i = 0; i < myCount; i++) {
        auto* srcPoke = myParty->GetMemberPointer(i);
        auto* dstPoke = myTrimmedParty->GetMemberPointer(i);
        if (srcPoke != nullptr && dstPoke != nullptr &&
            srcPoke->fields.m_accessor != nullptr && dstPoke->fields.m_accessor != nullptr) {
            uint8_t tmpBuf[344];
            _ILExternal::external<void>(0x24A4470, srcPoke->fields.m_accessor, tmpBuf);
            _ILExternal::external<void>(0x24A4550, dstPoke->fields.m_accessor, tmpBuf);
        }
    }
    myTrimmedParty->fields.m_memberCount = myCount;

    // Setup MYSTATUS_COMM for local player
    MYSTATUS_COMM::Object myStatus = {};
    myStatus.SetupFromPlayerWork();

    // Deserialize partner MYSTATUS_COMM
    int32_t mystOff = 0;
    MYSTATUS_COMM::Object partnerStatus = deserializeTeamUpStatus(
        tu.partnerMystatusBuf, tu.partnerMystatusLen, mystOff);

    // Build regulation (no restrictions)
    uint8_t regulation[4] = { 1, 6, 2, 0x07 };

    // Empty capsule array (avoids null deref)
    static uint8_t s_emptyCapsuleArray[32] = {};
    void* emptyCapsule = (void*)s_emptyCapsuleArray;

    // Call SetupBattleComm like PvP: 2 active players in slots 0/1, commRule=1 (double).
    Dpr::EncountTools::SetupBattleComm(bsp, 0, 1/*double*/, 1/*commPos*/, regulation,
        tu.partnerStation, partnerParty, &partnerStatus, emptyCapsule,  // slot 0: partner
        myStation, myTrimmedParty, &myStatus, emptyCapsule,             // slot 1: us (temp)
        -1, nullptr, nullptr, nullptr,                                   // slot 2: empty
        -1, nullptr, nullptr, nullptr,                                   // slot 3: empty
        nullptr, nullptr, 0, 0);

    Logger::log("[TeamUp] Player B: SetupBattleComm complete, rearranging for PP_AA...\n");

    // Rearrange BSP from PvP layout [partner, us, empty, empty]
    // to PP_AA layout [partner, AI_trainer1, us, AI_trainer2]
    auto* bspFields = &bsp->instance()->fields;

    // Move slot 1 (us) → slot 2
    if (bspFields->party != nullptr && bspFields->party->max_length >= 4) {
        auto* slot1Party = bspFields->party->m_Items[1];
        auto* slot2Party = bspFields->party->m_Items[2];
        if (slot1Party != nullptr) {
            if (slot2Party == nullptr) {
                slot2Party = (Pml::PokeParty::Object*)il2cpp_object_new((Il2CppClass*)myParty->klass);
                _ILExternal::external<void>(0x2055D10, slot2Party);
                bspFields->party->m_Items[2] = slot2Party;
            }
            int32_t cnt = slot1Party->fields.m_memberCount;
            for (int i = 0; i < cnt && i < 6; i++) {
                auto* src = slot1Party->GetMemberPointer(i);
                auto* dst = slot2Party->GetMemberPointer(i);
                if (src && dst && src->fields.m_accessor && dst->fields.m_accessor) {
                    uint8_t tmp[344];
                    _ILExternal::external<void>(0x24A4470, src->fields.m_accessor, tmp);
                    _ILExternal::external<void>(0x24A4550, dst->fields.m_accessor, tmp);
                }
            }
            slot2Party->fields.m_memberCount = cnt;
        }
    }

    // Move station: slot 1 → slot 2
    if (bspFields->stations != nullptr && bspFields->stations->max_length >= 4) {
        bspFields->stations->m_Items[2] = bspFields->stations->m_Items[1];
        bspFields->stations->m_Items[1] = -1;
        bspFields->stations->m_Items[3] = -1;
    }

    // Move playerStatus: slot 1 → slot 2
    if (bspFields->playerStatus != nullptr && bspFields->playerStatus->max_length >= 4) {
        auto* status1 = bspFields->playerStatus->m_Items[1];
        auto* status2 = bspFields->playerStatus->m_Items[2];
        if (status1 != nullptr && status2 != nullptr) {
            status2->fields.name = status1->fields.name;
            status2->fields.sex = status1->fields.sex;
            status2->fields.lang = status1->fields.lang;
            status2->fields.id = status1->fields.id;
            status2->fields.fashion = status1->fields.fashion;
            status2->fields.body_type = status1->fields.body_type;
            status2->fields.hat = status1->fields.hat;
            status2->fields.shoes = status1->fields.shoes;
        }
    }

    // Manual PP_AA setup — commPos=2 for Player B
    bspFields->multiMode = 2;  // PP_AA
    bspFields->commPos = 2;    // We (Player B) control slot 2

    // Trainer: competitor=TRAINER
    bspFields->competitor = (Dpr::Battle::Logic::BtlCompetitor)1;
    // normalTrainer for BOTH slots — slot 3 needs full infrastructure (partyDesc, tr_data)
    _ILExternal::external<void>(0x1F6EE00, bsp, (int32_t)1, tu.battleTrainerID);
    if (tu.battleTrainerID2 > 0) {
        // Already-double: second trainer in slot 3
        _ILExternal::external<void>(0x1F6EE00, bsp, (int32_t)3, tu.battleTrainerID2);
    } else {
        // Single trainer: set up full slot 3, then split Pokemon between slots
        _ILExternal::external<void>(0x1F6EE00, bsp, (int32_t)3, tu.battleTrainerID);
        splitTrainerParty(bsp, 1, 3);
    }
    // btlEffComponent: use the trainer's BattleEffectID (forwarded from Player A)
    int32_t effIdx = (tu.battleEffectID >= 0) ? tu.battleEffectID : 0x10;
    if (bspFields->btlEffComponent != nullptr)
        _ILExternal::external<void>(0x0187B7E0, bspFields->btlEffComponent,
            effIdx, (int32_t)-1, (int32_t)0, (uint64_t)0);
    Logger::log("[TeamUp] Player B: trainer PP_AA setup (trainer=%d/%d, effect=%d)\n",
                tu.battleTrainerID, tu.battleTrainerID2, effIdx);

    // Override arena with received arenaID
    if (tu.battleArenaID > 0) {
        void* fieldSit = bsp->instance()->fields.fieldSituation;
        if (fieldSit != nullptr) {
            void* bgComp = *(void**)((uintptr_t)fieldSit + 0x10);
            if (bgComp != nullptr) {
                _ILExternal::external<void>(0x188A6C0, bgComp, tu.battleArenaID);
                Logger::log("[TeamUp] Arena override: arenaID=%d\n", tu.battleArenaID);
            }
        }
    }

    // Enable CreateLocalClient hook for PP_AA AI slot creation
    s_teamUpCreateLocalClientActive = true;

    // Set up btlEffComponent on PlayerWork's BSP so the encounter update
    // can read the correct battle BGM. Our locally-created BSP isn't in PlayerWork,
    // but the encounter update reads from PlayerWork's BSP for audio.
    auto* pwBsp = PlayerWork::get_battleSetupParam();
    if (pwBsp != nullptr && pwBsp->instance()->fields.btlEffComponent != nullptr) {
        _ILExternal::external<void>(0x0187B7E0, pwBsp->instance()->fields.btlEffComponent,
            effIdx, (int32_t)-1, (int32_t)0, (uint64_t)0);
        Logger::log("[TeamUp] Player B: set btlEff on PlayerWork BSP (effIdx=%d)\n", effIdx);
    }

    // Audio transition: stop field BGM and start battle BGM manually.
    // EncountStart handles the visual transition but the BGM change normally
    // happens in the event script flow which Player B doesn't have.
    {
        auto* amInstance = Audio::AudioManager::get_Instance();
        if (amInstance != nullptr) {
            // Stop field BGM
            _ILExternal::external<uint32_t>(0x021EB100, amInstance,
                (uint32_t)0x35f93cfe, (uint32_t)0x100009, (bool)false);
            Logger::log("[TeamUp] Player B: stopped field BGM\n");

            // Start battle BGM from btlEffComponent
            if (bspFields->btlEffComponent != nullptr) {
                auto* bgmStr = (System::String::Object*)
                    _ILExternal::external<void*>(0x0187B510, bspFields->btlEffComponent);
                if (bgmStr != nullptr && bgmStr->fields.m_stringLength > 0) {
                    uint32_t bgmId = _ILExternal::external<uint32_t>(0x02465FA0, bgmStr);
                    _ILExternal::external<uint32_t>(0x021EB100, amInstance,
                        bgmId, (uint32_t)0x100009, (bool)false);
                    Logger::log("[TeamUp] Player B: started battle BGM (id=0x%08x)\n", bgmId);
                } else {
                    Logger::log("[TeamUp] Player B: btlEff has no BGM string\n");
                }
            } else {
                Logger::log("[TeamUp] Player B: no btlEffComponent for BGM\n");
            }
        }
    }

    // Battle transition: use the same trainer encounter flow as Player A.
    // The trainer entity exists in Player B's field (same-area check guarantees this).
    auto* fm = FieldManager::getClass()->static_fields->_Instance_k__BackingField;
    overworldMPSetInBattleScene(true);

    if (fm != nullptr) {
        fm->EncountStart(1, tu.battleTrainerID, tu.battleTrainerID2);
        Logger::log("[TeamUp] Player B: EncountStart(1, %d, %d) — normal trainer transition\n",
                    tu.battleTrainerID, tu.battleTrainerID2);
    }

    tu.battlePending = false;
    tu.partnerPartyValid = false;
}

// ---------------------------------------------------------------------------
// Copy battle-modified party back to player's original party (EXP/HP/PP persistence)
// ---------------------------------------------------------------------------
static void copyBackBattleParty(Dpr::Battle::Logic::BATTLE_SETUP_PARAM::Object* bsp) {
    if (bsp == nullptr) {
        Logger::log("[TeamUp] copyBack: BSP is null!\n");
        return;
    }
    auto* fields = &bsp->instance()->fields;
    if (fields->party == nullptr) {
        Logger::log("[TeamUp] copyBack: BSP party array is null!\n");
        return;
    }

    auto& tu = overworldMPGetTeamUpState();
    int32_t mySlot = tu.isInitiator ? 0 : 2;  // commPos

    Logger::log("[TeamUp] copyBack: isInitiator=%d mySlot=%d partyLen=%d\n",
                (int)tu.isInitiator, mySlot, fields->party->max_length);

    if ((uint64_t)mySlot >= fields->party->max_length) {
        Logger::log("[TeamUp] copyBack: mySlot %d >= party max_length %d!\n",
                    mySlot, fields->party->max_length);
        return;
    }
    auto* battleParty = fields->party->m_Items[mySlot];
    if (battleParty == nullptr) {
        Logger::log("[TeamUp] copyBack: battleParty at slot %d is null!\n", mySlot);
        return;
    }

    auto* origParty = PlayerWork::get_playerParty();
    if (origParty == nullptr) {
        Logger::log("[TeamUp] copyBack: origParty is null!\n");
        return;
    }

    int32_t count = battleParty->fields.m_memberCount;
    if (count > TEAMUP_PARTY_LIMIT) count = TEAMUP_PARTY_LIMIT;

    Logger::log("[TeamUp] copyBack: battleParty members=%d, origParty members=%d, copying %d\n",
                battleParty->fields.m_memberCount, origParty->fields.m_memberCount, count);

    for (int i = 0; i < count && i < origParty->fields.m_memberCount; i++) {
        auto* src = battleParty->GetMemberPointer(i);
        auto* dst = origParty->GetMemberPointer(i);
        if (src && dst && src->fields.m_accessor && dst->fields.m_accessor) {
            uint8_t tmp[344];
            _ILExternal::external<void>(0x24A4470, src->fields.m_accessor, tmp);
            _ILExternal::external<void>(0x24A4550, dst->fields.m_accessor, tmp);
        } else {
            Logger::log("[TeamUp] copyBack: skip poke %d (src=%p dst=%p)\n", i, src, dst);
        }
    }
    Logger::log("[TeamUp] Copied %d battle-modified Pokemon back to player party\n", count);
}

// ---------------------------------------------------------------------------
// Post-battle handler: party copy-back + Player B rewards/whiteout
// ---------------------------------------------------------------------------
void overworldMPHandleTeamUpPostBattle() {
    auto& tu = overworldMPGetTeamUpState();
    if (tu.battleType != 1) return;  // not a trainer team-up battle

    // NOTE: copyBackBattleParty() is called from TeamUpStoreBattleResult hook
    // (before BSP::Clear wipes party data during finalization).

    if (tu.isInitiator) {
        // Player A — event script handles rewards/whiteout
        tu.battleType = 0;
        tu.battleResult = -1;
        return;
    }

    // --- Player B post-battle ---
    if (tu.battleResult == 1) {  // WIN
        // Give money
        int32_t current = PlayerWork::GetMoney();
        int32_t newMoney = current + tu.battleGetMoney;
        if (newMoney < 0) newMoney = 0;
        if (newMoney > 999999) newMoney = 999999;
        PlayerWork::SetMoney(newMoney);

        // Set trainer flag (if not already set)
        if (!TrainerWork::GetWinFlag(tu.battleTrainerID)) {
            TrainerWork::SetWinFlag(tu.battleTrainerID);
            if (tu.battleTrainerID2 > 0)
                TrainerWork::SetWinFlag(tu.battleTrainerID2);
        }
        Logger::log("[TeamUp] Player B WIN: +%d money, trainer %d flagged\n",
                    tu.battleGetMoney, tu.battleTrainerID);

    } else if (tu.battleResult == 0) {  // LOSS
        // Trigger whiteout (heals party, deducts money, warps to Pokemon Center)
        auto* fm = FieldManager::getClass()->static_fields->_Instance_k__BackingField;
        if (fm != nullptr) {
            int32_t zoneID = PlayerWork::get_zoneID();
            _ILExternal::external<void>(0x1790B60, fm, zoneID);
            Logger::log("[TeamUp] Player B LOSS: triggered whiteout\n");
        }
    }

    // Clear battle state
    tu.battleType = 0;
    tu.battleResult = -1;
    tu.battleGetMoney = 0;
}

// ---------------------------------------------------------------------------
// Post-battle cleanup
// ---------------------------------------------------------------------------
void overworldMPClearTeamUpBattleState() {
    s_teamUpCreateLocalClientActive = false;
    s_teamUpBSP = nullptr;
    Logger::log("[TeamUp] Battle state cleared\n");
}

// ---------------------------------------------------------------------------
// MainModule::GetEscapeMode hook
// ---------------------------------------------------------------------------
// In team-up PvE battles, SetupBattleComm sets competitor=COMM (3) which maps
// to BTL_ESCAPE_MODE_CONFIRM (2) — showing a "Forfeit" button. Our later
// competitor override to TRAINER (1) doesn't take effect because MainModule
// caches the value during initialization.
//
// Fix: return BTL_ESCAPE_MODE_NG (1) for team-up trainer battles to disable
// the forfeit option entirely.
//
// GetEscapeMode @ 0x2034BB0

HOOK_DEFINE_TRAMPOLINE(TeamUpGetEscapeMode) {
    static int32_t Callback(void* mainModule, MethodInfo* mi) {
        if (overworldMPIsTeamedUp() && overworldMPGetTeamUpState().battleType == 1) {
            return 1; // BTL_ESCAPE_MODE_NG — can't run/forfeit
        }
        return Orig(mainModule, mi);
    }
};

// ---------------------------------------------------------------------------
// MainModule::storeBattleResult hook
// ---------------------------------------------------------------------------
// After the battle engine stores the result, check if this was a team-up
// trainer battle where a disconnect, comm error, or escape occurred. If so,
// override the result to LOSS (0) so the event script doesn't mark the
// trainer as defeated.
//
// storeBattleResult @ 0x202D560
// BSP is at MainModule+0x10 (managed BATTLE_SETUP_PARAM object)
// EscapeInfo is at MainModule+0x110 → +0x98
// EscapeInfo::GetCount @ 0x1D11EB0

HOOK_DEFINE_TRAMPOLINE(TeamUpStoreBattleResult) {
    static void Callback(void* mainModule, MethodInfo* mi) {
        Orig(mainModule, mi);

        bool teamed = overworldMPIsTeamedUp();
        auto& tu = overworldMPGetTeamUpState();
        Logger::log("[TeamUp] storeBattleResult hook: teamed=%d battleType=%d isInitiator=%d\n",
                    (int)teamed, (int)tu.battleType, (int)tu.isInitiator);
        if (!teamed) return;
        if (tu.battleType != 1) return; // only trainer team-up battles

        // Get BSP from MainModule+0x10
        auto* bsp = *(Dpr::Battle::Logic::BATTLE_SETUP_PARAM::Object**)
            ((uintptr_t)mainModule + 0x10);
        if (bsp == nullptr) return;

        auto* fields = &bsp->instance()->fields;

        // Check escape count via EscapeInfo (MainModule+0x110 → +0x98)
        int32_t escapeCount = 0;
        void* ctx = *(void**)((uintptr_t)mainModule + 0x110);
        if (ctx != nullptr) {
            void* escapeInfo = *(void**)((uintptr_t)ctx + 0x98);
            if (escapeInfo != nullptr) {
                escapeCount = _ILExternal::external<int32_t>(0x1D11EB0, escapeInfo);
            }
        }

        Logger::log("[TeamUp] storeBattleResult: result=%d disconnect=%d commError=%d escapes=%d\n",
                    fields->result, (int)fields->isDisconnectOccur, fields->commError, escapeCount);

        // Override result to LOSS if battle ended abnormally
        bool abnormal = fields->isDisconnectOccur || fields->commError != 0 || escapeCount > 0;
        if (abnormal && fields->result != 0) {
            fields->result = 0;     // LOSS — trainer not defeated
            fields->getMoney = 0;   // no money gain from invalid battle
            Logger::log("[TeamUp] Overrode trainer result to LOSS (abnormal ending)\n");
        }

        // PP_AA commPos=2 fix: the battle engine computes the result relative to
        // commPos. In PP_AA, commPos 0 and 2 are on the same team, but the engine
        // treats commPos=2 as the opposing side, inverting win/loss. Fix by flipping.
        if (!tu.isInitiator && fields->commPos == 2) {
            int32_t origResult = fields->result;
            if (fields->result == 0) fields->result = 1;
            else if (fields->result == 1) fields->result = 0;
            if (origResult != fields->result) {
                fields->getMoney = -fields->getMoney; // flip money sign too
                Logger::log("[TeamUp] Player B result fix: %d -> %d (commPos=2 inversion)\n",
                            origResult, fields->result);
            }
        }

        // Cache final result for post-battle handler (Player B rewards/whiteout)
        tu.battleResult = fields->result;
        tu.battleGetMoney = fields->getMoney;

        // Copy battle-modified party back NOW — BSP::Clear will wipe party data
        // during finalization, so this must happen before we leave storeBattleResult.
        // Pass the actual battle BSP (mainModule+0x10), NOT PlayerWork's BSP —
        // Player B's battle BSP is a locally-created object, not stored in PlayerWork.
        copyBackBattleParty(bsp);
    }
};

// ---------------------------------------------------------------------------
// MainModule::setupseq_comm_start_server hook
// ---------------------------------------------------------------------------
// The vanilla game blocks commMode=1 (wireless) + multiMode=2 (PP_AA) at step 3
// of the comm server startup sequence. This combination never occurs in vanilla,
// but our team-up feature uses it for cooperative PvE battles over local wireless.
//
// The check at step 3 (RVA 0x2033AF0, case 3):
//   if (multiMode == 2 && commMode == 1) { errorFlag = 1; return 1; }
//
// Fix: when step==3 and it's our PP_AA+wireless combination, skip the check
// by advancing the step counter to 4, which causes the sequence to complete
// normally (default case returns 1 without setting the error flag).
//
// setupseq_comm_start_server @ 0x2033AF0

HOOK_DEFINE_TRAMPOLINE(TeamUpCommStartServer) {
    static uint64_t Callback(void* mainModule, int32_t* stepPtr) {
        if (*stepPtr == 3) {
            int32_t multiMode = *(int32_t*)((uintptr_t)mainModule + 0x70);
            if (multiMode == 2) {
                void* battleEnv = *(void**)((uintptr_t)mainModule + 0x10);
                if (battleEnv != nullptr) {
                    uint8_t commMode = *(uint8_t*)((uintptr_t)battleEnv + 0x38);
                    if (commMode == 1) {
                        Logger::log("[TeamUp] Bypassing step 3 PP_AA+wireless check\n");
                        *stepPtr = 4;
                        return 0;
                    }
                }
            }
        }
        return Orig(mainModule, stepPtr);
    }
};

// ---------------------------------------------------------------------------
// ServerClientFactory::CreateLocalClient hook
// ---------------------------------------------------------------------------
// The vanilla game never uses PP_AA (multiMode=2) with commMode!=0 (wireless).
// In wireless comm battles, CreateLocalClient only creates local AI clients
// when isRemoteServer=true (the server side). The not-server side skips them,
// and CreateRemoteClients then creates wireless adapters for AI slots with
// station=-1 → crash.
//
// Fix: after Orig returns 0 (no client), check if this is a PP_AA + wireless
// AI slot, and if so force-create a local AI client (type=1, commMode forced
// to 0 inside CreateClientObject).
//
// ServerClientFactory::CreateLocalClient @ 0x1F1CF50
// ServerClientFactory::CreateClientObject @ 0x1F1D010
// BattleRule::IsClientAi @ 0x1881600
//
// Input struct layout (from setupCommon_CreateServerClient):
//   +0x10: battleRuleObj (long, managed BattleRule*)
//   +0x18: btlRule (uint32, enum value)
//   +0x1c: commMode (uint8)
//   +0x1d: multiMode/stationType (uint8)
//   +0x24: isReplay (bool/char)
//   +0x28: commPos (uint32)
//   +0x2c: isRemoteServer (bool/char)

HOOK_DEFINE_TRAMPOLINE(TeamUpCreateLocalClient) {
    static long Callback(void** inputHolder, void* param_2, uint64_t slot, MethodInfo* mi) {
        long result = Orig(inputHolder, param_2, slot, mi);
        if (result != 0) return result;  // Orig created a client — done

        // Only intervene for team-up PP_AA + wireless
        if (!s_teamUpCreateLocalClientActive) return 0;

        uintptr_t input = (uintptr_t)*inputHolder;
        uint8_t commMode  = *(uint8_t*)(input + 0x1c);
        uint8_t multiMode = *(uint8_t*)(input + 0x1d);

        if (commMode == 0 || multiMode != 2) return 0;  // Not PP_AA + wireless

        // PP_AA double has exactly 4 clients (0-3). SetupBattleComm may allocate
        // 5 slots (commRule+3), but slot 4 is not a real participant.
        // IsClientAi creates a bool[4] — calling it with slot>=4 would OOB crash.
        if (slot >= 4) return 0;

        // Check if this slot is AI via BattleRule::IsClientAi
        uint32_t btlRule = *(uint32_t*)(input + 0x18);
        bool isAi = _ILExternal::external<bool>(0x1881600,
            btlRule, commMode, multiMode, (uint32_t)(slot & 0xFFFFFFFF), (MethodInfo*)nullptr);
        if (!isAi) return 0;  // Human slot — let CreateRemoteClients handle it

        // Force-create local AI client (type=1 → commMode forced to 0 inside CreateClientObject)
        // CreateClientObject(Input** inputHolder, MethodInfo* unused, uint slot, char clientType)
        long aiClient = _ILExternal::external<long>(0x1F1D010,
            inputHolder, (void*)nullptr, (uint32_t)(slot & 0xFFFFFFFF), (int8_t)1);

        Logger::log("[TeamUp] Force-created local AI client for slot %d (PP_AA + wireless)\n",
                    (int)(slot & 0xFF));
        return aiClient;
    }
};

// ---------------------------------------------------------------------------
// Hook installation
// ---------------------------------------------------------------------------
void exl_team_up_main() {
    s_teamUpState.Clear();

    // Hook CreateLocalClient to fix PP_AA + wireless AI slot creation
    TeamUpCreateLocalClient::InstallAtOffset(0x1F1CF50);

    // Hook setupseq_comm_start_server to bypass step 3 PP_AA+wireless block
    TeamUpCommStartServer::InstallAtOffset(0x2033AF0);

    // Hook GetEscapeMode to disable forfeit in PP_AA PvE battles
    TeamUpGetEscapeMode::InstallAtOffset(0x2034BB0);

    // Hook storeBattleResult to prevent trainer defeat on disconnect/escape
    TeamUpStoreBattleResult::InstallAtOffset(0x202D560);

    Logger::log("[TeamUp] Hooks installed\n");
}
