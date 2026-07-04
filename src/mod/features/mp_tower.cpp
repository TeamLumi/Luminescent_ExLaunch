#include "exlaunch.hpp"
#include "features/mp_log.h"
#include "features/mp_net.h"
#include "features/mp_poke_validate.h"
#include "features/mp_tower.h"
#include "features/overworld_multiplayer.h"
#include "features/team_up.h"

#include "externals/Dpr/Battle/Logic/BATTLE_SETUP_PARAM.h"
#include "externals/Dpr/EncountTools.h"
#include "externals/Dpr/NetworkUtils/NetworkManager.h"
#include "externals/DPData/MYSTATUS.h"
#include "externals/FieldCanvas.h"
#include "externals/FieldManager.h"
#include "externals/GameManager.h"
#include "externals/MYSTATUS_COMM.h"
#include "externals/PlayerWork.h"
#include "externals/Pml/PokeParty.h"
#include "externals/Pml/PokePara/CoreParam.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/System/String.h"

#include "save/save.h"

#include <cstdio>
#include <cstring>

// ---------------------------------------------------------------------------
// Game externals (RVAs cross-checked against il2cpp_dump/dump.cs and the
// Ghidra pseudocode in tools/ghidra_all/_Global/{TrainerSystem,TowerLotResult,
// BattleTowerLot,BtlTowerWork}.txt)
// ---------------------------------------------------------------------------
// BtlTowerWork::GetRank(uint mode) @0x18DF1F0 — class_data[mode]+0x30.
// mode 1 = the reserved NORMAL_TAG slot (no vanilla code path touches it).
static inline int32_t towerGetRank(uint32_t mode) {
    return _ILExternal::external<int32_t>(0x18DF1F0, mode);
}
// TrainerSystem::CreateTowerLotResult(TowerLotRule lotRule, TowerLotCls lotCls,
//   int rank, int round, ulong seed) @0x2CC0320. Does its own lazy TypeInfo
// init (system_load_typeinfo 0x8c65 at entry). Returns null when no
// TowerMatching row exists for the composed matchingID.
static inline void* towerCreateLotResult(int32_t lotRule, int32_t lotCls,
                                         int32_t rank, int32_t round, uint64_t seed) {
    return _ILExternal::external<void*>(0x2CC0320, lotRule, lotCls, rank, round, seed);
}
// TowerLotResult::GetTrainerID(int index) @0x2CB7070
static inline int32_t towerLotGetTrainerID(void* lot, uint32_t index) {
    return _ILExternal::external<int32_t>(0x2CB7070, lot, index);
}
// TowerLotResult::CreatePokeParty(int trainerIndex) @0x2CB7340 — builds the
// trainer's PokeParty from the double-stock sheet + the lot's personalRand.
static inline Pml::PokeParty::Object* towerLotCreatePokeParty(void* lot, uint32_t index) {
    return (Pml::PokeParty::Object*)_ILExternal::external<void*>(0x2CB7340, lot, index);
}
// TrainerSystem::GetTowerTrainerData(TowerTrID) @0x2CB7230 — used as a
// validity pre-check: Setup::instTrainer does NOT null-check this lookup.
static inline void* towerGetTrainerData(int32_t towerTrID) {
    return _ILExternal::external<void*>(0x2CB7230, towerTrID);
}
// Dpr.Battle.Logic.Setup::instTrainer(BATTLE_SETUP_PARAM, BTL_CLIENT_ID,
//   TowerTrID, PokeParty, SealTemplateID[], BtlRule) @0x1F70160 — the game's
// own tower-enemy slot fill (the tower analog of normalTrainer @0x1F6EE00):
// setupParty into party[clientID] + BSP_TRAINER_DATA (tower AI bits 0x4F for
// doubles, name label from the TowerTrainerTable sheet).
static inline void towerInstTrainer(Dpr::Battle::Logic::BATTLE_SETUP_PARAM::Object* bsp,
                                    uint32_t clientId, int32_t towerTrID,
                                    Pml::PokeParty::Object* party) {
    _ILExternal::external<void>(0x1F70160, bsp, clientId, towerTrID,
                                party, (void*)nullptr, (int32_t)1 /*BTL_RULE_DOUBLE*/);
}

// TowerLotRule / TowerLotCls (dump.cs TypeDefIndex 3842/3843). NORMAL_TAG lots
// draw from the DOUBLE stock at NORMAL class (BattleTowerLot::LotTrainer's
// towerclass table maps mode 1 -> (Double, Normal)).
static constexpr int32_t TOWER_LOT_RULE_DOUBLE = 2;
static constexpr int32_t TOWER_LOT_CLS_NORMAL  = 1;

// BattleSetupEffectId 0x49 — the two-tower-trainer intro/BGM the vanilla
// SetupBattleTowerTrainer picks for a double battle with enemy2 != -1.
static constexpr int32_t TOWER_EFFECT_DOUBLE_TAG = 0x49;

// ---------------------------------------------------------------------------
// Tuning
// ---------------------------------------------------------------------------
static constexpr float TOWER_ARM_DELAY_SEC    = 0.35f; // packet ctx -> battle setup
static constexpr float TOWER_NET_TIMEOUT_SEC  = 20.0f; // round/ack exchange timeout
static constexpr float TOWER_POST_ROUND_SEC   = 1.5f;  // finalize grace after scene end
static constexpr int32_t TOWER_RANK_MIN = 1;
static constexpr int32_t TOWER_RANK_MAX = 10;          // TowerMatching rank column range

// ---------------------------------------------------------------------------
// State
// ---------------------------------------------------------------------------
enum class TowerPhase : int32_t {
    Idle       = 0,
    AwaitAck   = 1,  // initiator sent TOWER_ROUND, waiting for partner's ACK
    AwaitRound = 2,  // partner waiting for the next round packet
    ArmBattle  = 3,  // round data complete on this console — battle starts shortly
    InBattle   = 4,  // comm double battle running
    PostRound  = 5,  // battle scene over — waiting out finalization, then heal
};

struct TowerState {
    bool active = false;
    bool isInitiator = false;
    int32_t partner = -1;
    TowerPhase phase = TowerPhase::Idle;
    float timer = 0.0f;

    int32_t round = 0;              // 1..TOWER_ROUND_COUNT
    int32_t rank = TOWER_RANK_MIN;

    // Current round's lotted opponents (initiator: from the lot pipeline;
    // partner: from the TOWER_ROUND packet)
    int32_t trainerID0 = -1;
    int32_t trainerID1 = -1;
    uint8_t enemy1Buf[6 * POKE_FULL_DATA_SIZE] = {};
    int32_t enemy1Count = 0;
    uint8_t enemy2Buf[6 * POKE_FULL_DATA_SIZE] = {};
    int32_t enemy2Count = 0;

    // Partner's battle party (refreshed every round: ROUND carries the
    // initiator's, the ACK carries the partner's)
    uint8_t partnerPartyBuf[TOWER_PARTY_LIMIT * POKE_FULL_DATA_SIZE] = {};
    int32_t partnerPartyCount = 0;
    uint8_t partnerMystatusBuf[64] = {};
    int32_t partnerMystatusLen = 0;

    // No-repeat relot memory (vanilla LotTrainer keeps a per-set TowerTrID list)
    int32_t seenTrainers[TOWER_ROUND_COUNT] = {};
    int32_t seenCount = 0;

    // Chunk accumulation for the in-flight 0xD6 packet
    bool accumActive = false;
    bool accumIsAck = false;
    uint8_t expectOwn = 0, gotOwn = 0;
    uint8_t expectE1 = 0, gotE1 = 0;
    uint8_t expectE2 = 0, gotE2 = 0;

    // Partner's round packet finished while we were still in/After a battle
    bool pendingArm = false;

    // Round result from team_up's storeBattleResult hook
    bool resultReceived = false;
    int32_t roundResult = -1;
    bool roundAbnormal = false;

    // Run-start snapshot: the between-round "full heal" restores to this
    // (and undoes any battle-engine party writes — no exp in the tower).
    uint8_t snapshotBuf[6 * POKE_FULL_DATA_SIZE] = {};
    int32_t snapshotCount = 0;
    bool snapshotValid = false;

    int32_t savedWazaeffMode = -1;

    // memset instead of `*this = TowerState{}` — the buffers make this struct
    // several KB and the temporary would live on the caller's stack.
    void Clear() {
        memset(this, 0, sizeof(*this));
        partner = -1;
        rank = TOWER_RANK_MIN;
        trainerID0 = -1;
        trainerID1 = -1;
        roundResult = -1;
        savedWazaeffMode = -1;
        phase = TowerPhase::Idle;
    }
};
static TowerState s_tw;

bool mpTowerIsActive() { return s_tw.active; }

// ---------------------------------------------------------------------------
// HUD helpers (same FieldCanvas route as mp_minigames)
// ---------------------------------------------------------------------------
static void towerHudMsg(const char* text) {
    FieldCanvas::Debug_ShowDisplayMessage(System::String::Create(text));
}

static void towerHudMsgL(const char* label, const char* fallback) {
    towerHudMsg(overworldMPGetMessageCStr(label, fallback));
}

// "Multi Tower — Round N/7" (raw snprintf text for v1; SS_mp_TowerRound label
// lands with the companion message-bundle work)
static void towerHudRound() {
    char buf[64];
    snprintf(buf, sizeof(buf), "Multi Tower \xe2\x80\x94 Round %d/%d",
             s_tw.round, TOWER_ROUND_COUNT);
    towerHudMsg(buf);
}

// ---------------------------------------------------------------------------
// Party helpers
// ---------------------------------------------------------------------------

// First TOWER_PARTY_LIMIT slots must be battle-able (non-egg, HP>0): the comm
// battle takes them verbatim, and a vanilla tower set never fields an egg.
static bool towerLocalPartyOk() {
    auto* party = PlayerWork::get_playerParty();
    if (party == nullptr) return false;
    if (party->fields.m_memberCount < TOWER_PARTY_LIMIT) return false;
    for (int i = 0; i < TOWER_PARTY_LIMIT; i++) {
        auto* member = party->GetMemberPointer(i);
        if (member == nullptr) return false;
        auto* core = member->cast<Pml::PokePara::CoreParam>();
        if (core == nullptr) return false;
        if (core->IsEgg(Pml::PokePara::EggCheckType::BOTH_EGG) || core->IsHpZero()) {
            return false;
        }
    }
    return true;
}

static void towerSnapshotParty() {
    s_tw.snapshotCount = 0;
    s_tw.snapshotValid = false;
    auto* party = PlayerWork::get_playerParty();
    if (party == nullptr) return;
    int32_t count = party->fields.m_memberCount;
    if (count > 6) count = 6;
    for (int i = 0; i < count; i++) {
        auto* poke = party->GetMemberPointer(i);
        if (poke == nullptr || poke->fields.m_accessor == nullptr) {
            count = i;
            break;
        }
        poke->fields.m_accessor->Serialize_FullData(
            &s_tw.snapshotBuf[i * POKE_FULL_DATA_SIZE]);
    }
    s_tw.snapshotCount = count;
    s_tw.snapshotValid = count > 0;
    MP_LOG("[Tower] Run snapshot saved (%d members)\n", count);
}

// The vanilla tower fully heals between rounds; restoring the run-start
// snapshot is that heal AND discards any battle-engine writes (no exp/no
// damage persists — tower battles run on flat copies, like vanilla).
static void towerRestoreSnapshot() {
    if (!s_tw.snapshotValid) return;
    auto* party = PlayerWork::get_playerParty();
    if (party == nullptr) return;
    party->fields.m_memberCount = s_tw.snapshotCount;
    for (int i = 0; i < s_tw.snapshotCount; i++) {
        auto* poke = party->GetMemberPointer(i);
        if (poke != nullptr && poke->fields.m_accessor != nullptr) {
            poke->fields.m_accessor->Deserialize_FullData(
                &s_tw.snapshotBuf[i * POKE_FULL_DATA_SIZE]);
        }
    }
    MP_LOG("[Tower] Party restored to run-start state (%d members)\n",
                s_tw.snapshotCount);
}

// Build a fresh PokeParty from a serialized buffer, validating each member.
// Same load-bearing construction as team_up's deserializeTeamUpParty: reuse
// the live party's klass + raw ctor — newInstance()'s lazy generic class-init
// null-derefs in the battle setup context.
static Pml::PokeParty::Object* towerDeserializeParty(const uint8_t* buf, int32_t bufCapacity,
                                                     int32_t count, int32_t maxCount) {
    if (count <= 0 || count > maxCount) return nullptr;

    auto* localParty = PlayerWork::get_playerParty();
    if (localParty == nullptr) return nullptr;

    auto* party = (Pml::PokeParty::Object*)il2cpp_object_new((Il2CppClass*)localParty->klass);
    _ILExternal::external<void>(0x2055D10, party); // PokeParty::ctor()

    int32_t validCount = 0;
    for (int i = 0; i < count; i++) {
        int32_t offset = i * POKE_FULL_DATA_SIZE;
        if (offset + POKE_FULL_DATA_SIZE > bufCapacity) break;

        auto* slotPoke = party->GetMemberPointer(i);
        if (slotPoke == nullptr) continue;
        auto* accessor = slotPoke->fields.m_accessor;
        if (accessor != nullptr) {
            accessor->Deserialize_FullData(const_cast<uint8_t*>(&buf[offset]));
        }
        if (!mpValidatePokemonParam((Pml::PokePara::CoreParam*)slotPoke)) {
            MP_LOG("[Tower] REJECT poke[%d] — failed validation\n", i);
            break;
        }
        validCount++;
    }
    if (validCount == 0) return nullptr;
    party->fields.m_memberCount = validCount;
    return party;
}

// Trim the live party to its first `limit` members (fresh managed copy).
static Pml::PokeParty::Object* towerCloneOwnParty(int32_t limit) {
    auto* myParty = PlayerWork::get_playerParty();
    if (myParty == nullptr) return nullptr;
    auto* trimmed = (Pml::PokeParty::Object*)il2cpp_object_new((Il2CppClass*)myParty->klass);
    _ILExternal::external<void>(0x2055D10, trimmed); // PokeParty::ctor()
    int32_t count = myParty->fields.m_memberCount;
    if (count > limit) count = limit;
    for (int i = 0; i < count; i++) {
        auto* src = myParty->GetMemberPointer(i);
        auto* dst = trimmed->GetMemberPointer(i);
        if (src && dst && src->fields.m_accessor && dst->fields.m_accessor) {
            uint8_t tmp[POKE_FULL_DATA_SIZE];
            src->fields.m_accessor->Serialize_FullData(tmp);
            dst->fields.m_accessor->Deserialize_FullData(tmp);
        }
    }
    trimmed->fields.m_memberCount = count;
    return trimmed;
}

// Deserialize MYSTATUS_COMM from the header blob (same layout the 0xC9
// exchange uses: [id:4][nameLen:1][chars][sex][lang][fashion][body][hat][shoes])
static MYSTATUS_COMM::Object towerDeserializeStatus(uint8_t* buf, int32_t size) {
    MYSTATUS_COMM::Object status = {};
    int32_t offset = 0;

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
// Run teardown
// ---------------------------------------------------------------------------
static void towerEndRun(const char* label, const char* fallback) {
    if (fallback != nullptr) {
        towerHudMsgL(label, fallback);
    }
    MP_LOG("[Tower] Run ended (round=%d, phase=%d)\n", s_tw.round, (int)s_tw.phase);
    s_tw.Clear();
}

// ---------------------------------------------------------------------------
// Round lot (initiator only) — per-round CreateTowerLotResult with the vanilla
// no-repeat relot (LotTrainer relots on a repeated GetTrainerID(0), <=10 tries,
// advancing the seed by (attempt + stride) each miss).
// ---------------------------------------------------------------------------
static bool towerTrainerSeen(int32_t towerTrID) {
    for (int i = 0; i < s_tw.seenCount; i++) {
        if (s_tw.seenTrainers[i] == towerTrID) return true;
    }
    return false;
}

static int32_t towerSerializeParty(Pml::PokeParty::Object* party, uint8_t* outBuf,
                                   int32_t maxCount) {
    if (party == nullptr) return 0;
    int32_t count = party->fields.m_memberCount;
    if (count > maxCount) count = maxCount;
    int32_t written = 0;
    for (int i = 0; i < count; i++) {
        auto* poke = party->GetMemberPointer(i);
        if (poke == nullptr || poke->fields.m_accessor == nullptr) break;
        poke->fields.m_accessor->Serialize_FullData(&outBuf[i * POKE_FULL_DATA_SIZE]);
        written++;
    }
    return written;
}

static bool towerLotRound() {
    // RandomGroupWork::Value @0x199F420 — the game's general random
    uint64_t seed = ((uint64_t)_ILExternal::external<uint32_t>(0x199F420) << 32) |
                    (uint64_t)_ILExternal::external<uint32_t>(0x199F420);
    uint32_t stride = ((uint32_t)seed & 0x1f) + 1;

    for (int attempt = 0; attempt <= 9; attempt++) {
        void* lot = towerCreateLotResult(TOWER_LOT_RULE_DOUBLE, TOWER_LOT_CLS_NORMAL,
                                         s_tw.rank, s_tw.round, seed);
        if (lot == nullptr) {
            MP_LOG("[Tower] CreateTowerLotResult returned null (rank=%d round=%d)\n",
                        s_tw.rank, s_tw.round);
            return false;
        }
        int32_t tid0 = towerLotGetTrainerID(lot, 0);
        int32_t tid1 = towerLotGetTrainerID(lot, 1);
        if (tid0 < 0 || tid1 < 0 ||
            towerGetTrainerData(tid0) == nullptr || towerGetTrainerData(tid1) == nullptr) {
            MP_LOG("[Tower] Lot produced invalid trainer IDs %d/%d\n", tid0, tid1);
            return false;
        }
        if (attempt < 9 && towerTrainerSeen(tid0)) {
            seed += (uint64_t)((uint32_t)(attempt + 1) + stride);
            continue;
        }

        auto* p0 = towerLotCreatePokeParty(lot, 0);
        auto* p1 = towerLotCreatePokeParty(lot, 1);
        if (p0 == nullptr || p1 == nullptr) {
            MP_LOG("[Tower] CreatePokeParty failed (tid %d/%d)\n", tid0, tid1);
            return false;
        }
        s_tw.enemy1Count = towerSerializeParty(p0, s_tw.enemy1Buf, 6);
        s_tw.enemy2Count = towerSerializeParty(p1, s_tw.enemy2Buf, 6);
        if (s_tw.enemy1Count == 0 || s_tw.enemy2Count == 0) {
            MP_LOG("[Tower] Lotted enemy party empty\n");
            return false;
        }
        s_tw.trainerID0 = tid0;
        s_tw.trainerID1 = tid1;
        if (s_tw.seenCount < TOWER_ROUND_COUNT) {
            s_tw.seenTrainers[s_tw.seenCount++] = tid0;
        }
        MP_LOG("[Tower] Round %d lot: trainers %d/%d, %d+%d pokes (rank=%d, attempt=%d)\n",
                    s_tw.round, tid0, tid1, s_tw.enemy1Count, s_tw.enemy2Count,
                    s_tw.rank, attempt);
        return true;
    }
    return false;
}

// ---------------------------------------------------------------------------
// 0xD6 send — HEADER + OWN_POKE chunks (+ enemy chunks on the initiator side).
// Same chunking rationale as 0xC6/0xC9: a full party blows the PIA
// PacketWriter budget, one Pokemon per packet does not.
// ---------------------------------------------------------------------------
static void towerSendPokeChunk(void* pw, uint8_t sub, uint8_t idx, const uint8_t* data) {
    il2cpp_vcall_void(pw, PW_RESET);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, OWMP_DATA_ID_TOWER_ROUND);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, s_tw.partner);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, sub);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, idx);
    for (int j = 0; j < POKE_FULL_DATA_INTS; j++) {
        int32_t val = 0;
        memcpy(&val, &data[j * 4], 4);
        il2cpp_vcall_write_s32(pw, PW_WRITE_S32, val);
    }
    Dpr::NetworkUtils::NetworkManager::SendReliablePacketToAll(
        (INL1::PacketWriterRe::Object*)pw, 0);
}

static void towerSendRound(bool isAck) {
    auto* pw = Dpr::NetworkUtils::NetworkManager::get_PacketWriterRe();
    if (pw == nullptr) return;
    auto* party = PlayerWork::get_playerParty();
    if (party == nullptr) return;

    int32_t ownCount = party->fields.m_memberCount;
    if (ownCount > TOWER_PARTY_LIMIT) ownCount = TOWER_PARTY_LIMIT;
    uint8_t e1 = isAck ? 0 : (uint8_t)s_tw.enemy1Count;
    uint8_t e2 = isAck ? 0 : (uint8_t)s_tw.enemy2Count;

    // --- HEADER ---
    il2cpp_vcall_void(pw, PW_RESET);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, OWMP_DATA_ID_TOWER_ROUND);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, s_tw.partner);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, TOWER_SUB_HEADER);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, isAck ? 1 : 0);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, (uint8_t)s_tw.round);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, (uint8_t)ownCount);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, e1);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, e2);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, s_tw.trainerID0);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, s_tw.trainerID1);

    // MYSTATUS blob (same field set the 0xC9 header carries)
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
        for (int i = 0; i < 6; i++) {
            il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, 0);
        }
    }
    Dpr::NetworkUtils::NetworkManager::SendReliablePacketToAll(pw, 0);
    MP_LOG("[Tower] Sent %s HEADER: round=%d own=%d e1=%d e2=%d trainers=%d/%d\n",
                isAck ? "ACK" : "ROUND", s_tw.round, ownCount, (int)e1, (int)e2,
                s_tw.trainerID0, s_tw.trainerID1);

    // --- OWN_POKE chunks (live party, first TOWER_PARTY_LIMIT) ---
    for (int32_t i = 0; i < ownCount; i++) {
        uint8_t pokeBuf[POKE_FULL_DATA_SIZE];
        memset(pokeBuf, 0, sizeof(pokeBuf));
        auto* poke = party->GetMemberPointer(i);
        if (poke != nullptr && poke->fields.m_accessor != nullptr) {
            poke->fields.m_accessor->Serialize_FullData(pokeBuf);
        }
        towerSendPokeChunk(pw, TOWER_SUB_OWN_POKE, (uint8_t)i, pokeBuf);
    }

    // --- Enemy chunks (initiator's ROUND only) ---
    if (!isAck) {
        for (int32_t i = 0; i < s_tw.enemy1Count; i++) {
            towerSendPokeChunk(pw, TOWER_SUB_ENEMY1_POKE, (uint8_t)i,
                               &s_tw.enemy1Buf[i * POKE_FULL_DATA_SIZE]);
        }
        for (int32_t i = 0; i < s_tw.enemy2Count; i++) {
            towerSendPokeChunk(pw, TOWER_SUB_ENEMY2_POKE, (uint8_t)i,
                               &s_tw.enemy2Buf[i * POKE_FULL_DATA_SIZE]);
        }
    }
}

// ---------------------------------------------------------------------------
// Battle setup + start — the exact PP_AA dance team_up's ACK handler performs
// (SetupBattleComm -> restore tr_data -> slot 1<->2 swap -> multiMode=2 ->
// enemy slot fill -> stations fix), with the game's own tower-enemy fill
// (Setup::instTrainer) in place of normalTrainer, and the PvP-style
// scriptless battle transition at the end.
// ---------------------------------------------------------------------------
static bool towerSetupAndStartBattle() {
    auto& tw = s_tw;
    auto& tu = overworldMPGetTeamUpState();

    auto* bsp = PlayerWork::get_battleSetupParam();
    if (bsp == nullptr) {
        MP_LOG("[Tower] ERROR: battleSetupParam is null\n");
        return false;
    }

    // instTrainer does not null-check the TowerTrainerTable lookup — reject
    // bad (or hostile) trainer IDs before they can crash the setup.
    if (towerGetTrainerData(tw.trainerID0) == nullptr ||
        towerGetTrainerData(tw.trainerID1) == nullptr) {
        MP_LOG("[Tower] ERROR: invalid tower trainer IDs %d/%d\n",
                    tw.trainerID0, tw.trainerID1);
        return false;
    }

    // Fresh full-party save + battle-result sync reset. The team-up post-battle
    // machinery (overworldMPHandleTeamUpPostBattle) runs for this battle too;
    // the fresh save keeps its slot-3+ restore consistent, and the tower's own
    // run snapshot restore afterwards makes the final party state authoritative.
    overworldMPSaveFullParty();

    auto* myTrimmedParty = towerCloneOwnParty(TOWER_PARTY_LIMIT);
    if (myTrimmedParty == nullptr || myTrimmedParty->fields.m_memberCount < 1) {
        MP_LOG("[Tower] ERROR: could not build own battle party\n");
        return false;
    }

    auto* partnerParty = towerDeserializeParty(tw.partnerPartyBuf, sizeof(tw.partnerPartyBuf),
                                               tw.partnerPartyCount, TOWER_PARTY_LIMIT);
    auto* enemyParty0 = towerDeserializeParty(tw.enemy1Buf, sizeof(tw.enemy1Buf),
                                              tw.enemy1Count, 6);
    auto* enemyParty1 = towerDeserializeParty(tw.enemy2Buf, sizeof(tw.enemy2Buf),
                                              tw.enemy2Count, 6);
    if (partnerParty == nullptr || enemyParty0 == nullptr || enemyParty1 == nullptr) {
        MP_LOG("[Tower] ERROR: party deserialize failed (partner=%p e0=%p e1=%p)\n",
                    partnerParty, enemyParty0, enemyParty1);
        return false;
    }

    MYSTATUS_COMM::Object myStatus = {};
    myStatus.SetupFromPlayerWork();
    MYSTATUS_COMM::Object partnerStatus =
        towerDeserializeStatus(tw.partnerMystatusBuf, tw.partnerMystatusLen);

    // TODO(regulation): v1 skips the vanilla multi-room rules gate — flat Lv50
    // (PokeRegulation.ModifyLevelPokeParty @0x1BCD5E0, operates on the battle
    // copy), legendary ban (CheckLegend @0x1BCD400 / CheckSubLegend), and the
    // species/item clauses over the combined four (CheckBothPoke @0x1BCD8A0,
    // CheckBothItem @0x1BCDAE0). See docs/superpowers/specs/multi-battle-tower.md.
    uint8_t regulation[4] = { 1, 6, 2, 0x07 };
    static uint8_t s_towerEmptyCapsule[32] = {};
    void* emptyCapsule = (void*)s_towerEmptyCapsule;

    // Comm battles sync animation settings from the server — save the local
    // preference and restore it after the round.
    auto* myConfig = PlayerWork::get_config();
    if (myConfig != nullptr) {
        tw.savedWazaeffMode = myConfig->fields.wazaeff_mode;
    }

    // MP battle color override must be armed BEFORE SetupBattleComm (StoreCore
    // fires inside it).
    {
        extern bool g_owmpBattleColorActive;
        g_owmpBattleColorActive = true;
    }

    // Save the tr_data array pointer across SetupBattleComm (Clear nulls
    // entries in place but may swap the array object) — same guard team_up uses.
    auto* preFields = &bsp->instance()->fields;
    void* savedTrData = preFields->tr_data;

    int32_t myStation = mpThisStationIndex();
    if (tw.isInitiator) {
        Dpr::EncountTools::SetupBattleComm(bsp, 0, 1/*double*/, 0/*commPos*/, regulation,
            myStation, myTrimmedParty, &myStatus, emptyCapsule,
            tw.partner, partnerParty, &partnerStatus, emptyCapsule,
            -1, nullptr, nullptr, nullptr,
            -1, nullptr, nullptr, nullptr,
            nullptr, nullptr, 0, 0);
    } else {
        Dpr::EncountTools::SetupBattleComm(bsp, 0, 1/*double*/, 1/*commPos*/, regulation,
            tw.partner, partnerParty, &partnerStatus, emptyCapsule,
            myStation, myTrimmedParty, &myStatus, emptyCapsule,
            -1, nullptr, nullptr, nullptr,
            -1, nullptr, nullptr, nullptr,
            nullptr, nullptr, 0, 0);
    }

    auto* fields = &bsp->instance()->fields;

    // Keep commMode=1 (comm double routing) but force the PP_AA shape:
    // rule DOUBLE, humans at 0/2, tower AI at 1/3.
    fields->tr_data = savedTrData;
    fields->rule = Dpr::Battle::Logic::BtlRule::BTL_RULE_DOUBLE;
    if (fields->party != nullptr && fields->party->max_length > 2) {
        auto* tmp = fields->party->m_Items[1];
        fields->party->m_Items[1] = fields->party->m_Items[2];
        fields->party->m_Items[2] = tmp;
    }
    if (fields->playerStatus != nullptr && fields->playerStatus->max_length > 2) {
        auto* tmp = fields->playerStatus->m_Items[1];
        fields->playerStatus->m_Items[1] = fields->playerStatus->m_Items[2];
        fields->playerStatus->m_Items[2] = tmp;
    }
    if (fields->stations != nullptr && fields->stations->max_length > 2) {
        auto tmp = fields->stations->m_Items[1];
        fields->stations->m_Items[1] = fields->stations->m_Items[2];
        fields->stations->m_Items[2] = tmp;
    }
    fields->multiMode = TEAMUP_MULTIMODE_PP_AA;
    fields->commPos = tw.isInitiator ? 0 : 2;
    // competitor stays COMM(3) from SetupBattleComm — seq_EXIT_COMM syncs the
    // result to both consoles (the tower reads it via the storeBattleResult hook).

    // Fill the tower AI slots through the game's own tower setup path.
    towerInstTrainer(bsp, 1, tw.trainerID0, enemyParty0);
    towerInstTrainer(bsp, 3, tw.trainerID1, enemyParty1);

    // AI clients have no PIA stations.
    if (fields->stations != nullptr && fields->stations->max_length > 3) {
        fields->stations->m_Items[1] = -1;
        fields->stations->m_Items[3] = -1;
    }

    // Tower two-trainer intro + BGM (what vanilla SetupBattleTowerTrainer picks
    // for a double with enemy2 != -1). SetUpBattleEffectComponentData @0x0187B7E0.
    if (fields->btlEffComponent != nullptr) {
        _ILExternal::external<void>(0x0187B7E0, fields->btlEffComponent,
            TOWER_EFFECT_DOUBLE_TAG, (int32_t)-1, (int32_t)0, (uint64_t)0);
    }

    // Arena: SetupBattleComm hardcodes Union Room 0x2b — use the current zone's
    // battle background instead (same override the PvP path applies).
    {
        GameManager::getClass()->initIfNeeded();
        auto zoneID = PlayerWork::get_zoneID();
        auto* mapInfo = GameManager::get_mapInfo();
        if (mapInfo != nullptr) {
            auto* zoneData = mapInfo->get_Item(zoneID);
            if (zoneData != nullptr && zoneData->fields.BattleBg != nullptr &&
                zoneData->fields.BattleBg->max_length > 0) {
                int32_t arenaID = zoneData->fields.BattleBg->m_Items[0];
                void* fieldSit = fields->fieldSituation;
                if (fieldSit != nullptr) {
                    void* bgComp = *(void**)((uintptr_t)fieldSit + 0x10);
                    if (bgComp != nullptr) {
                        // BgComponentData::SetUpBgComponentData @0x188A6C0
                        _ILExternal::external<void>(0x188A6C0, bgComp, arenaID, 0);
                    }
                }
            }
        }
    }

    // Human trainer colors (PP_AA layout: initiator local=0/partner=2,
    // joiner local=2/partner=0) — shared team-up implementation.
    if (tw.partner >= 0 && tw.partner < OW_MP_MAX_PLAYERS) {
        int32_t localColor = getCustomSaveData()->playerColorVariation.playerColorID;
        auto& remote = getOverworldMPContext().remotePlayers[tw.partner];
        int32_t localSlot = tw.isInitiator ? 0 : 2;
        int32_t partnerSlot = tw.isInitiator ? 2 : 0;
        owmpApplyTeamUpBattleColors(fields->tr_data, fields->playerStatus, remote,
                                    localColor, localSlot, partnerSlot);
    }

    // Drive the team-up battle hooks (storeBattleResult result caching, comm
    // error suppression, AI client creation, PP_AA server-start bypass): the
    // tower round IS a team-up-shaped comm trainer battle. The tower-specific
    // differences (no exp, no money, comm exit on loss) are gated inside
    // team_up.cpp on mpTowerIsActive().
    tu.battleType = 1;
    tu.battleMultiMode = TEAMUP_MULTIMODE_PP_AA;
    tu.isInitiator = tw.isInitiator;
    tu.battleResult = -1;
    owmpTeamUpSetCommClientHookActive(true);

    overworldMPSetActiveBattlePartner(tw.partner);

    // Scriptless comm battle transition (shared with the PvP path).
    overworldMPBeginCommBattleTransition(bsp);

    MP_LOG("[Tower] Round %d battle started (initiator=%d, commPos=%d, trainers=%d/%d)\n",
                tw.round, (int)tw.isInitiator, (int)fields->commPos,
                tw.trainerID0, tw.trainerID1);
    return true;
}

// ---------------------------------------------------------------------------
// Run start (initiator side; partner activates on the first ROUND header)
// ---------------------------------------------------------------------------
void mpTowerStartAsInitiator(int32_t partnerStation) {
    if (s_tw.active) return;
    if (!overworldMPIsTeamedUp() ||
        overworldMPGetTeamUpState().partnerStation != partnerStation) {
        MP_LOG("[Tower] Start rejected: not teamed with station %d\n", partnerStation);
        return;
    }
    if (overworldMPIsInBattleScene()) return;
    if (!towerLocalPartyOk()) {
        towerHudMsgL("SS_mp_TowerNeedTwo",
                     "Multi Tower needs 2 healthy Pok\xc3\xa9mon leading your party!");
        return;
    }

    s_tw.Clear();
    s_tw.active = true;
    s_tw.isInitiator = true;
    s_tw.partner = partnerStation;
    s_tw.round = 1;

    // Difficulty: the reserved NORMAL_TAG rank slot (mode 1), floored at 1.
    // The spec's min(rankA, rankB) needs the partner's rank in the handshake —
    // v1 uses the initiator's TAG rank (both start at 1 until
    // TODO(tag-persistence) writes ranks back).
    int32_t rank = towerGetRank(1);
    if (rank < TOWER_RANK_MIN) rank = TOWER_RANK_MIN;
    if (rank > TOWER_RANK_MAX) rank = TOWER_RANK_MAX;
    s_tw.rank = rank;

    towerSnapshotParty();

    if (!towerLotRound()) {
        towerEndRun("SS_mp_TowerNoLot", "Multi Tower \xe2\x80\x94 couldn't draw opponents.");
        return;
    }
    towerSendRound(false);
    towerHudRound();
    s_tw.phase = TowerPhase::AwaitAck;
    s_tw.timer = TOWER_NET_TIMEOUT_SEC;
    MP_LOG("[Tower] Run started as initiator (partner=%d, rank=%d)\n",
                partnerStation, s_tw.rank);
}

// ---------------------------------------------------------------------------
// 0xD6 receive
// ---------------------------------------------------------------------------
static void towerOnExchangeComplete() {
    s_tw.accumActive = false;

    // Validate everything we'll battle with BEFORE committing (mirror team_up:
    // never ACK data we can't fight on).
    auto* partnerParty = towerDeserializeParty(s_tw.partnerPartyBuf, sizeof(s_tw.partnerPartyBuf),
                                               s_tw.partnerPartyCount, TOWER_PARTY_LIMIT);
    auto* e0 = towerDeserializeParty(s_tw.enemy1Buf, sizeof(s_tw.enemy1Buf), s_tw.enemy1Count, 6);
    auto* e1 = towerDeserializeParty(s_tw.enemy2Buf, sizeof(s_tw.enemy2Buf), s_tw.enemy2Count, 6);
    if (partnerParty == nullptr || e0 == nullptr || e1 == nullptr ||
        towerGetTrainerData(s_tw.trainerID0) == nullptr ||
        towerGetTrainerData(s_tw.trainerID1) == nullptr) {
        MP_LOG("[Tower] Round data failed validation — abandoning run\n");
        towerEndRun("SS_mp_TowerBadData", "Multi Tower \xe2\x80\x94 bad round data; run abandoned.");
        return;
    }

    if (!s_tw.accumIsAck && !s_tw.isInitiator) {
        // Partner side: echo our battle party, then arm.
        towerSendRound(true);
        towerHudRound();
        if (s_tw.phase == TowerPhase::AwaitRound || s_tw.phase == TowerPhase::Idle) {
            s_tw.phase = TowerPhase::ArmBattle;
            s_tw.timer = TOWER_ARM_DELAY_SEC;
        } else {
            // Still finishing the previous round locally — arm once it's done.
            s_tw.pendingArm = true;
        }
    } else if (s_tw.accumIsAck && s_tw.isInitiator &&
               s_tw.phase == TowerPhase::AwaitAck) {
        s_tw.phase = TowerPhase::ArmBattle;
        s_tw.timer = TOWER_ARM_DELAY_SEC;
    }
}

void mpTowerOnRoundPacket(void* pr) {
    int32_t fromStation = il2cpp_vcall_int(pr, PR_FROM_STATION);
    if (fromStation < 0 || fromStation >= OW_MP_MAX_PLAYERS) return;

    int32_t target = 0;
    il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &target);
    if (target != mpThisStationIndex()) return;

    uint8_t sub = 0;
    il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &sub);

    if (sub == TOWER_SUB_HEADER) {
        uint8_t isAck = 0, round = 0, ownCount = 0, e1Count = 0, e2Count = 0;
        int32_t tid0 = -1, tid1 = -1;
        il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &isAck);
        il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &round);
        il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &ownCount);
        il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &e1Count);
        il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &e2Count);
        il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &tid0);
        il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &tid1);

        // MYSTATUS blob → partnerMystatusBuf (serialized layout, parsed at setup)
        uint8_t mystBuf[64] = {};
        int32_t mystOff = 0;
        int32_t statusId = 0;
        il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &statusId);
        memcpy(&mystBuf[mystOff], &statusId, 4);
        mystOff += 4;
        uint8_t nameLen = 0;
        il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &nameLen);
        if (nameLen > 12) nameLen = 12;
        mystBuf[mystOff++] = nameLen;
        for (int nc = 0; nc < nameLen * 2; nc++) {
            uint8_t ch = 0;
            il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &ch);
            if (mystOff < (int32_t)sizeof(mystBuf)) mystBuf[mystOff++] = ch;
        }
        for (int fi = 0; fi < 6; fi++) {
            uint8_t val = 0;
            il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &val);
            if (mystOff < (int32_t)sizeof(mystBuf)) mystBuf[mystOff++] = val;
        }

        MP_LOG("[Tower] %s HEADER from %d: round=%d own=%d e1=%d e2=%d trainers=%d/%d\n",
                    isAck ? "ACK" : "ROUND", fromStation, (int)round,
                    (int)ownCount, (int)e1Count, (int)e2Count, tid0, tid1);

        auto& tu = overworldMPGetTeamUpState();
        if (!tu.isTeamedUp || tu.partnerStation != fromStation) {
            MP_LOG("[Tower] Ignoring 0xD6 from non-partner station %d\n", fromStation);
            return;
        }

        if (!s_tw.active) {
            // Run activation on the partner side: only a fresh round-1 ROUND
            // from the teamed partner starts a run.
            if (isAck || round != 1) return;
            if (overworldMPIsInBattleScene()) return;
            if (!towerLocalPartyOk()) {
                towerHudMsgL("SS_mp_TowerNeedTwo",
                             "Multi Tower needs 2 healthy Pok\xc3\xa9mon leading your party!");
                return;
            }
            s_tw.Clear();
            s_tw.active = true;
            s_tw.isInitiator = false;
            s_tw.partner = fromStation;
            towerSnapshotParty();
            MP_LOG("[Tower] Run started as partner (initiator=%d)\n", fromStation);
        }

        if (isAck) {
            if (!s_tw.isInitiator || s_tw.phase != TowerPhase::AwaitAck) return;
        } else {
            if (s_tw.isInitiator) return;  // both sides think they initiate — drop
            if (round < 1 || round > TOWER_ROUND_COUNT) return;
            s_tw.round = round;
            s_tw.trainerID0 = tid0;
            s_tw.trainerID1 = tid1;
            s_tw.enemy1Count = (e1Count > 6) ? 6 : e1Count;
            s_tw.enemy2Count = (e2Count > 6) ? 6 : e2Count;
            memset(s_tw.enemy1Buf, 0, sizeof(s_tw.enemy1Buf));
            memset(s_tw.enemy2Buf, 0, sizeof(s_tw.enemy2Buf));
            if (s_tw.enemy1Count == 0 || s_tw.enemy2Count == 0) return;
        }

        memcpy(s_tw.partnerMystatusBuf, mystBuf, sizeof(mystBuf));
        s_tw.partnerMystatusLen = mystOff;
        memset(s_tw.partnerPartyBuf, 0, sizeof(s_tw.partnerPartyBuf));
        s_tw.partnerPartyCount = (ownCount > TOWER_PARTY_LIMIT) ? TOWER_PARTY_LIMIT : ownCount;
        if (s_tw.partnerPartyCount == 0) return;

        s_tw.accumActive = true;
        s_tw.accumIsAck = isAck != 0;
        s_tw.expectOwn = (uint8_t)s_tw.partnerPartyCount;
        s_tw.gotOwn = 0;
        s_tw.expectE1 = isAck ? 0 : (uint8_t)s_tw.enemy1Count;
        s_tw.gotE1 = 0;
        s_tw.expectE2 = isAck ? 0 : (uint8_t)s_tw.enemy2Count;
        s_tw.gotE2 = 0;
        return;
    }

    // --- Pokemon chunks ---
    if (!s_tw.active || !s_tw.accumActive || fromStation != s_tw.partner) return;

    uint8_t idx = 0;
    il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &idx);

    uint8_t* destBuf = nullptr;
    int32_t destCap = 0;
    uint8_t* gotCounter = nullptr;
    uint8_t expect = 0;
    if (sub == TOWER_SUB_OWN_POKE) {
        destBuf = s_tw.partnerPartyBuf;
        destCap = sizeof(s_tw.partnerPartyBuf);
        gotCounter = &s_tw.gotOwn;
        expect = s_tw.expectOwn;
    } else if (sub == TOWER_SUB_ENEMY1_POKE && !s_tw.accumIsAck) {
        destBuf = s_tw.enemy1Buf;
        destCap = sizeof(s_tw.enemy1Buf);
        gotCounter = &s_tw.gotE1;
        expect = s_tw.expectE1;
    } else if (sub == TOWER_SUB_ENEMY2_POKE && !s_tw.accumIsAck) {
        destBuf = s_tw.enemy2Buf;
        destCap = sizeof(s_tw.enemy2Buf);
        gotCounter = &s_tw.gotE2;
        expect = s_tw.expectE2;
    } else {
        return;
    }
    if (idx >= expect) return;

    int32_t bufOffset = idx * POKE_FULL_DATA_SIZE;
    if (bufOffset + POKE_FULL_DATA_SIZE > destCap) return;
    for (int j = 0; j < POKE_FULL_DATA_INTS; j++) {
        int32_t val = 0;
        il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &val);
        memcpy(&destBuf[bufOffset + j * 4], &val, 4);
    }
    (*gotCounter)++;

    if (s_tw.gotOwn >= s_tw.expectOwn &&
        s_tw.gotE1 >= s_tw.expectE1 &&
        s_tw.gotE2 >= s_tw.expectE2) {
        MP_LOG("[Tower] %s exchange complete (own=%d e1=%d e2=%d)\n",
                    s_tw.accumIsAck ? "ACK" : "ROUND",
                    (int)s_tw.gotOwn, (int)s_tw.gotE1, (int)s_tw.gotE2);
        towerOnExchangeComplete();
    }
}

// ---------------------------------------------------------------------------
// Round completion / progression
// ---------------------------------------------------------------------------
static void towerFinishRound() {
    // Between-round heal: restore the run-start snapshot (also discards the
    // battle engine's party writes — no exp/damage carries, like vanilla).
    towerRestoreSnapshot();

    // Restore the local animation preference the comm battle synced over.
    if (s_tw.savedWazaeffMode >= 0) {
        auto* config = PlayerWork::get_config();
        if (config != nullptr) {
            config->fields.wazaeff_mode = s_tw.savedWazaeffMode;
        }
        s_tw.savedWazaeffMode = -1;
    }

    bool win = s_tw.resultReceived && !s_tw.roundAbnormal && s_tw.roundResult == 1;
    MP_LOG("[Tower] Round %d finished: result=%d abnormal=%d received=%d\n",
                s_tw.round, s_tw.roundResult, (int)s_tw.roundAbnormal,
                (int)s_tw.resultReceived);

    if (!win) {
        if (!s_tw.resultReceived || s_tw.roundAbnormal) {
            towerEndRun("SS_mp_TowerAbort",
                        "Multi Tower \xe2\x80\x94 battle ended abnormally; run over.");
        } else {
            char buf[80];
            snprintf(buf, sizeof(buf),
                     "Multi Tower \xe2\x80\x94 defeated in Round %d/%d.",
                     s_tw.round, TOWER_ROUND_COUNT);
            towerHudMsg(buf);
            MP_LOG("[Tower] Run ended in defeat (round=%d)\n", s_tw.round);
            s_tw.Clear();
        }
        return;
    }

    if (s_tw.round >= TOWER_ROUND_COUNT) {
        // TODO(tag-persistence): bank streak/rounds into the reserved NORMAL_TAG
        // class_data slot — SetRound @0x18E0980 / RoundAdd @0x18E0AD0 /
        // UpdateRenshou @0x18DFD00 with mode 1; GameClear @0x18E0CD0 is already
        // clear_flag==1 || tower_round>6. Blocked on the save serializer
        // round-trip test of class_data[1]/[3] (spec, 2026-07-03 findings).
        // TODO(bp-reward): pay BP from the TowerBattlePoint table
        // (GameData.DataManager static +0xA8, TowerBattlePoint.Sheetpoint) to
        // each player's own save.
        towerEndRun("SS_mp_TowerClear", "Multi Tower cleared! (7 wins)");
        return;
    }

    s_tw.resultReceived = false;
    s_tw.roundResult = -1;
    s_tw.roundAbnormal = false;

    if (s_tw.isInitiator) {
        s_tw.round++;
        if (!towerLotRound()) {
            towerEndRun("SS_mp_TowerNoLot", "Multi Tower \xe2\x80\x94 couldn't draw opponents.");
            return;
        }
        towerSendRound(false);
        towerHudRound();
        s_tw.phase = TowerPhase::AwaitAck;
        s_tw.timer = TOWER_NET_TIMEOUT_SEC;
    } else {
        if (s_tw.pendingArm) {
            // The next round's packet already landed while we were finishing.
            s_tw.pendingArm = false;
            s_tw.phase = TowerPhase::ArmBattle;
            s_tw.timer = TOWER_ARM_DELAY_SEC;
        } else {
            s_tw.phase = TowerPhase::AwaitRound;
            s_tw.timer = TOWER_NET_TIMEOUT_SEC;
        }
    }
}

// ---------------------------------------------------------------------------
// Tick / events
// ---------------------------------------------------------------------------
void mpTowerTick(float deltaTime) {
    if (!s_tw.active) return;

    // Partner gone / team disbanded — but let an in-flight battle (and its
    // post-round restore) finish first: the battle's own abnormal ending flows
    // through storeBattleResult, and PostRound still owns the snapshot restore.
    if (s_tw.phase != TowerPhase::InBattle && s_tw.phase != TowerPhase::PostRound) {
        auto& ctx = getOverworldMPContext();
        if (s_tw.partner < 0 || !ctx.remotePlayers[s_tw.partner].isActive ||
            !overworldMPIsTeamedUp()) {
            towerEndRun("SS_mp_Disconnected", "Your friend disconnected \xe2\x80\x94 game over.");
            return;
        }
    }

    switch (s_tw.phase) {
    case TowerPhase::AwaitAck:
    case TowerPhase::AwaitRound:
        s_tw.timer -= deltaTime;
        if (s_tw.timer <= 0.0f) {
            towerEndRun("SS_mp_TowerTimeout",
                        "Multi Tower \xe2\x80\x94 no response; run abandoned.");
        }
        break;

    case TowerPhase::ArmBattle:
        s_tw.timer -= deltaTime;
        if (s_tw.timer <= 0.0f) {
            if (!towerSetupAndStartBattle()) {
                towerEndRun("SS_mp_TowerAbort",
                            "Multi Tower \xe2\x80\x94 battle setup failed; run over.");
                return;
            }
            s_tw.phase = TowerPhase::InBattle;
            s_tw.resultReceived = false;
            s_tw.roundResult = -1;
            s_tw.roundAbnormal = false;
        }
        break;

    case TowerPhase::InBattle:
        // The interact tick clears the battle-scene flag once
        // FieldManager._updateType returns to 0 (and runs the team-up
        // post-battle handler in the same breath) — then we take over.
        if (!overworldMPIsInBattleScene()) {
            s_tw.phase = TowerPhase::PostRound;
            s_tw.timer = TOWER_POST_ROUND_SEC;
        }
        break;

    case TowerPhase::PostRound:
        s_tw.timer -= deltaTime;
        if (s_tw.timer <= 0.0f) {
            towerFinishRound();
        }
        break;

    case TowerPhase::Idle:
    default:
        break;
    }
}

void mpTowerOnBattleResult(int32_t result, bool abnormal) {
    if (!s_tw.active || s_tw.phase != TowerPhase::InBattle) return;
    s_tw.roundResult = result;
    s_tw.roundAbnormal = abnormal;
    s_tw.resultReceived = true;
    MP_LOG("[Tower] Battle result: %d (abnormal=%d)\n", result, (int)abnormal);
}

void mpTowerOnPeerLeft(int32_t stationIndex) {
    if (!s_tw.active || stationIndex != s_tw.partner) return;
    if (s_tw.phase == TowerPhase::InBattle || s_tw.phase == TowerPhase::PostRound) {
        // The battle layer handles the mid-battle disconnect; the round then
        // finishes abnormally and PostRound ends the run with the restore done.
        MP_LOG("[Tower] Partner left mid-battle — deferring run end to PostRound\n");
        return;
    }
    towerEndRun("SS_mp_Disconnected", "Your friend disconnected \xe2\x80\x94 game over.");
}

void mpTowerReset() {
    s_tw.Clear();
}
