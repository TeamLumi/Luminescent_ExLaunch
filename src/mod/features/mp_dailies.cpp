#include "exlaunch.hpp"
#include "features/mp_log.h"
#include "features/mp_net.h"
#include "features/mp_dailies.h"
#include "features/overworld_multiplayer.h"

#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Dpr/NetworkUtils/NetworkManager.h"

#include <cstdint>

// ---------------------------------------------------------------------------
// RVAs
// ---------------------------------------------------------------------------
static constexpr uintptr_t RVA_GET_DAILY_RANDOM   = 0x19A0BA0; // RandomGroupWork.GetDailyRandom (hooked)
static constexpr uintptr_t RVA_GET_URAYAMA_INDEX  = 0x1F09DE0; // EncountDataWork.GetUrayamaIndex (hooked)
static constexpr uintptr_t RVA_LOTTERY_CHECK_POKE = 0x1797650; // FieldLotteryWork.CheckPokemon (hooked)
static constexpr uintptr_t RVA_EV_GET_WORK        = 0x2C47CC0; // EvDataManager.GetWork(index)
static constexpr uintptr_t RVA_EV_SET_WORK        = 0x2C42840; // EvDataManager.SetWork(index, value)
static constexpr int32_t   EV_WORK_LOTTERY        = 0x104;     // daily lottery number work slot

static constexpr float BROADCAST_INTERVAL = 10.0f;

// ---------------------------------------------------------------------------
// State
// ---------------------------------------------------------------------------
static bool    s_overlayValid = false;   // guest: host values received
static int32_t s_hostDailyRandom = 0;
static int32_t s_hostLotteryWork = 0;
static int32_t s_hostUrayama[2] = {0, 0};
static int32_t s_appliedLotteryWork = -1;
static float   s_timer = 0.0f;

static bool iAmHost() { return mpThisStationIndex() == 0; }

static int32_t evGetWork(int32_t index) {
    auto* ev = Dpr::EvScript::EvDataManager::get_Instanse();
    if (ev == nullptr) return 0;
    return _ILExternal::external<int32_t>(RVA_EV_GET_WORK, ev, index);
}

static void evSetWork(int32_t index, int32_t value) {
    auto* ev = Dpr::EvScript::EvDataManager::get_Instanse();
    if (ev == nullptr) return;
    _ILExternal::external<void>(RVA_EV_SET_WORK, ev, index, value);
}

// ---------------------------------------------------------------------------
// Broadcast / receive
// ---------------------------------------------------------------------------

static void broadcastWorld() {
    auto* pw = Dpr::NetworkUtils::NetworkManager::get_PacketWriterRe();
    if (pw == nullptr) return;

    int32_t dailyRandom = _ILExternal::external<int32_t>(RVA_GET_DAILY_RANDOM);
    int32_t lotteryWork = evGetWork(EV_WORK_LOTTERY);
    int32_t ura0 = _ILExternal::external<int32_t>(RVA_GET_URAYAMA_INDEX, 0);
    int32_t ura1 = _ILExternal::external<int32_t>(RVA_GET_URAYAMA_INDEX, 1);

    il2cpp_vcall_void(pw, PW_RESET);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, OWMP_DATA_ID_WORLD_DAILIES);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, dailyRandom);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, lotteryWork);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, ura0);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, ura1);
    Dpr::NetworkUtils::NetworkManager::SendReliablePacketToAll(pw, 0);
}

void mpDailiesOnWorldReceived(void* pr) {
    int32_t fromStation = il2cpp_vcall_int(pr, PR_FROM_STATION);
    if (fromStation != 0) return;  // only the host's world counts
    if (iAmHost()) return;

    bool wasValid = s_overlayValid;
    int32_t oldRandom = s_hostDailyRandom;
    il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &s_hostDailyRandom);
    il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &s_hostLotteryWork);
    il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &s_hostUrayama[0]);
    il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &s_hostUrayama[1]);
    s_overlayValid = true;

    if (!wasValid || oldRandom != s_hostDailyRandom) {
        MP_LOG("[Dailies] Host world: dailyRandom=%d lottery=%d urayama=%d/%d\n",
                    s_hostDailyRandom, s_hostLotteryWork,
                    s_hostUrayama[0], s_hostUrayama[1]);
    }
}

void mpDailiesClear() {
    s_overlayValid = false;
    s_appliedLotteryWork = -1;
    // Local daily values recompute from the own save on next read; the
    // lottery work slot stays at the host's value until the next in-game
    // day refresh (RAM-only, never saved as-is).
}

void mpDailiesTick(float deltaTime) {
    s_timer += deltaTime;
    if (s_timer >= BROADCAST_INTERVAL) {
        s_timer = 0.0f;
        if (iAmHost()) {
            broadcastWorld();
        }
    }

    // Guest: keep the lottery work slot pinned to the host's number.
    if (!iAmHost() && s_overlayValid && s_hostLotteryWork != s_appliedLotteryWork) {
        evSetWork(EV_WORK_LOTTERY, s_hostLotteryWork);
        s_appliedLotteryWork = s_hostLotteryWork;
        MP_LOG("[Dailies] Applied host lottery number (work 0x104 = %d)\n",
                    s_hostLotteryWork);
    }
}

// ---------------------------------------------------------------------------
// Overlay hooks
// ---------------------------------------------------------------------------

// RandomGroupWork.GetDailyRandom @0x19A0BA0 — the daily world seed.
// While linked as a guest, the host's seed is the world's seed.
HOOK_DEFINE_TRAMPOLINE(RandomGroupWork$$GetDailyRandom) {
    static int32_t Callback() {
        int32_t v = Orig();
        if (s_overlayValid && !iAmHost() && isOverworldMPActive()) {
            return s_hostDailyRandom;
        }
        return v;
    }
};

// EncountDataWork.GetUrayamaIndex @0x1F09DE0 — Trophy Garden daily guests.
// Read-only overlay; the guest's saved index is never written.
HOOK_DEFINE_TRAMPOLINE(EncountDataWork$$GetUrayamaIndex) {
    static int32_t Callback(int32_t slot) {
        int32_t v = Orig(slot);
        if (s_overlayValid && !iAmHost() && isOverworldMPActive() &&
            slot >= 0 && slot <= 1) {
            return s_hostUrayama[slot];
        }
        return v;
    }
};

// FieldLotteryWork.CheckPokemon @0x1797650 — Loto-ID pooling: lobby members'
// trainer IDs count toward your draw. Returns the match tier 0..5.
static int32_t digitMatchTier(uint32_t winNumber, uint32_t trainerId) {
    uint32_t win = winNumber % 100000;
    int32_t tier = 0;
    uint32_t w = win, t = trainerId;
    for (int d = 0; d < 5; d++) {
        if (w % 10 != t % 10) break;
        tier++;
        w /= 10;
        t /= 10;
    }
    return tier;
}

HOOK_DEFINE_TRAMPOLINE(FieldLotteryWork$$CheckPokemon) {
    static int32_t Callback(void* pokemonParam) {
        int32_t best = Orig(pokemonParam);
        if (!isOverworldMPActive()) return best;

        uint32_t win = (uint32_t)evGetWork(EV_WORK_LOTTERY);
        auto& ctx = getOverworldMPContext();
        for (int i = 0; i < OW_MP_MAX_PLAYERS; i++) {
            auto& remote = ctx.remotePlayers[i];
            if (!remote.isActive || remote.trainerId == 0) continue;
            int32_t tier = digitMatchTier(win, (uint32_t)remote.trainerId);
            if (tier > best) {
                best = tier;
                MP_LOG("[Dailies] Loto pool: station %d's ID raises tier to %d\n", i, tier);
            }
        }
        return best;
    }
};

void exl_mp_dailies_hooks() {
    RandomGroupWork$$GetDailyRandom::InstallAtOffset(RVA_GET_DAILY_RANDOM);
    EncountDataWork$$GetUrayamaIndex::InstallAtOffset(RVA_GET_URAYAMA_INDEX);
    FieldLotteryWork$$CheckPokemon::InstallAtOffset(RVA_LOTTERY_CHECK_POKE);
    MP_LOG("[Dailies] Hooks installed (GetDailyRandom, GetUrayamaIndex, CheckPokemon)\n");
}
