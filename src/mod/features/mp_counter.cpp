#include "exlaunch.hpp"
#include "features/mp_log.h"
#include "features/mp_net.h"
#include "features/mp_counter.h"
#include "features/mp_tower.h"
#include "features/mp_contest.h"
#include "features/mp_minigames.h"
#include "features/overworld_multiplayer.h"
#include "features/team_up.h"

#include "externals/Dpr/NetworkUtils/NetworkManager.h"

#include <cstdint>
#include <cstdio>
#include <cstring>

// ---------------------------------------------------------------------------
// Counter check-in rendezvous — see mp_counter.h.
// ---------------------------------------------------------------------------

static constexpr int32_t COUNTER_ACTIVITY_COUNT = 2;
// A lone check-in expires after this long (partner never showed).
static constexpr float COUNTER_CHECKIN_TTL_SEC = 120.0f;

struct CounterState {
    bool  localCheckin[COUNTER_ACTIVITY_COUNT] = {};
    bool  partnerCheckin[COUNTER_ACTIVITY_COUNT] = {};
    float localTtl[COUNTER_ACTIVITY_COUNT] = {};

    void Clear() {
        for (int i = 0; i < COUNTER_ACTIVITY_COUNT; i++) {
            localCheckin[i] = false;
            partnerCheckin[i] = false;
            localTtl[i] = 0.0f;
        }
    }
};
static CounterState s_cnt;

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

static int32_t counterPartner() {
    auto& tu = overworldMPGetTeamUpState();
    return tu.isTeamedUp ? tu.partnerStation : -1;
}

static void counterSendCheckin(MpCounterActivity activity, bool on) {
    int32_t partner = counterPartner();
    if (partner < 0) return;
    auto* pw = Dpr::NetworkUtils::NetworkManager::get_PacketWriterRe();
    if (pw == nullptr) return;
    il2cpp_vcall_void(pw, PW_RESET);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, OWMP_DATA_ID_COUNTER_CHECKIN);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, partner);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, (uint8_t)activity);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, on ? 1 : 0);
    Dpr::NetworkUtils::NetworkManager::SendReliablePacketToAll(pw, 0);
    MP_LOG("[Counter] Sent checkin: activity=%d on=%d\n", (int)activity, (int)on);
}

// Both players are checked in — clear the flags and launch the activity.
static void counterRendezvous(MpCounterActivity activity) {
    int32_t partner = counterPartner();
    int32_t a = (int32_t)activity;
    s_cnt.localCheckin[a] = false;
    s_cnt.partnerCheckin[a] = false;
    s_cnt.localTtl[a] = 0.0f;
    if (partner < 0) return;

    if (activity == MpCounterActivity::Tower) {
        // Exactly one side initiates the run (lots round 1, sends 0xD6); the
        // other activates on the round packet — deterministic by station index.
        if (mpThisStationIndex() < partner) {
            mpTowerStartAsInitiator(partner);
        }
        overworldMPShowAreaText("Multi Battle Room — your challenge begins!");
    } else {
        mpContestArm(partner);
        overworldMPShowAreaText("Contest Show ready! Register at the contest desk.");
    }
    MP_LOG("[Counter] Rendezvous complete: activity=%d partner=%d\n", a, partner);
}

// ---------------------------------------------------------------------------
// Ev-command API
// ---------------------------------------------------------------------------

MpCounterStatus mpCounterStatus(MpCounterActivity activity) {
    auto& ctx = getOverworldMPContext();
    if (ctx.state != OverworldMPState::Connected) return MpCounterStatus::NoSession;

    int32_t partner = counterPartner();
    if (partner < 0 || !ctx.remotePlayers[partner].isActive)
        return MpCounterStatus::NotTeamed;

    auto& tu = overworldMPGetTeamUpState();
    if (mpTowerIsActive() || mpMinigameIsActive() || tu.battlePending ||
        overworldMPIsInBattleScene())
        return MpCounterStatus::Busy;

    int32_t a = (int32_t)activity;
    if (activity == MpCounterActivity::Tower) {
        if (!mpTowerLocalPartyOk())    return MpCounterStatus::PartyNotOk;
        if (!mpTowerLocalPartyLegal()) return MpCounterStatus::PartyIllegal;
    } else {
        if (mpContestIsArmed()) return MpCounterStatus::Armed;
    }

    if (s_cnt.localCheckin[a])   return MpCounterStatus::Waiting;
    if (s_cnt.partnerCheckin[a]) return MpCounterStatus::PartnerWaiting;
    return MpCounterStatus::Ready;
}

MpCounterCheckin mpCounterCheckin(MpCounterActivity activity) {
    MpCounterStatus st = mpCounterStatus(activity);
    if (st == MpCounterStatus::Waiting) return MpCounterCheckin::NowWaiting;
    if (st != MpCounterStatus::Ready && st != MpCounterStatus::PartnerWaiting)
        return MpCounterCheckin::Failed;

    int32_t a = (int32_t)activity;
    if (s_cnt.partnerCheckin[a]) {
        // Partner beat us here — rendezvous completes right now. Tell them our
        // check-in so their console launches too.
        counterSendCheckin(activity, true);
        counterRendezvous(activity);
        return MpCounterCheckin::Started;
    }

    s_cnt.localCheckin[a] = true;
    s_cnt.localTtl[a] = COUNTER_CHECKIN_TTL_SEC;
    counterSendCheckin(activity, true);
    return MpCounterCheckin::NowWaiting;
}

void mpCounterCancel(MpCounterActivity activity) {
    int32_t a = (int32_t)activity;
    if (s_cnt.localCheckin[a]) {
        s_cnt.localCheckin[a] = false;
        s_cnt.localTtl[a] = 0.0f;
        counterSendCheckin(activity, false);
    }
    // Contest desk cancel also drops an armed-but-unused pairing.
    if (activity == MpCounterActivity::Contest && mpContestIsArmed()) {
        mpContestDisarm();
    }
}

// ---------------------------------------------------------------------------
// 0xDA receive
// ---------------------------------------------------------------------------

void mpCounterOnCheckinReceived(void* pr) {
    int32_t fromStation = il2cpp_vcall_int(pr, PR_FROM_STATION);
    int32_t target = 0;
    uint8_t activity = 0, on = 0;
    il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &target);
    il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &activity);
    il2cpp_vcall_read_out(pr, PR_READ_BYTE_OUT, &on);

    if (target != mpThisStationIndex()) return;
    if (activity >= COUNTER_ACTIVITY_COUNT) return;
    if (fromStation != counterPartner()) return;

    if (!on) {
        s_cnt.partnerCheckin[activity] = false;
        MP_LOG("[Counter] Partner cancelled checkin (activity=%d)\n", (int)activity);
        return;
    }

    if (s_cnt.localCheckin[activity]) {
        // We were waiting at the counter — rendezvous complete on this side.
        counterRendezvous((MpCounterActivity)activity);
    } else {
        s_cnt.partnerCheckin[activity] = true;
        auto& remote = getOverworldMPContext().remotePlayers[fromStation];
        const char* name = remote.playerNameSet ? remote.playerNameBuf : "Your friend";
        char buf[96];
        snprintf(buf, sizeof(buf), "%s is waiting at the %s!",
                 name,
                 activity == (uint8_t)MpCounterActivity::Tower
                     ? "Battle Tower counter" : "Contest Hall multi desk");
        overworldMPShowAreaText(buf);
        MP_LOG("[Counter] Partner waiting at counter (activity=%d)\n", (int)activity);
    }
}

// ---------------------------------------------------------------------------
// Tick / cleanup
// ---------------------------------------------------------------------------

void mpCounterTick(float deltaTime) {
    for (int a = 0; a < COUNTER_ACTIVITY_COUNT; a++) {
        if (!s_cnt.localCheckin[a]) continue;
        s_cnt.localTtl[a] -= deltaTime;
        if (s_cnt.localTtl[a] <= 0.0f) {
            MP_LOG("[Counter] Checkin TTL expired (activity=%d)\n", a);
            s_cnt.localCheckin[a] = false;
            counterSendCheckin((MpCounterActivity)a, false);
            overworldMPShowAreaText("Your friend didn't check in — reservation cancelled.");
        }
    }
}

void mpCounterReset() {
    s_cnt.Clear();
}

void mpCounterOnPeerLeft(int32_t stationIndex) {
    // Pair activities only — any peer leaving that was our partner voids all
    // checkins (partner check happens against the team-up state, which the
    // leave path clears before/alongside this).
    (void)stationIndex;
    s_cnt.Clear();
}
