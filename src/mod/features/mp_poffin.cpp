#include "exlaunch.hpp"
#include "features/mp_log.h"
#include "features/mp_net.h"
#include "features/mp_poffin.h"
#include "features/overworld_multiplayer.h"
#include "features/team_up.h"

#include "externals/PlayerWork.h"
#include "externals/Dpr/NetworkUtils/NetworkManager.h"

#include <cstdint>
#include <cstdio>

// ---------------------------------------------------------------------------
// Game RVAs (Poffin cooking — Dpr.FureaiHiroba.PofinManager and friends)
// ---------------------------------------------------------------------------
// PofinManager.EndCooking(this) @0x1BF8510 — builds the PofinCookModel from the
// manager's stats + berry list and runs the completion. We defer it for co-op.
static constexpr uintptr_t RVA_ENDCOOKING       = 0x1BF8510;
// KinomiWork.GetKinomiData(int tagNo) @0x1AEDF70 -> KinomiData.SheetData.
static constexpr uintptr_t RVA_GET_KINOMI_DATA  = 0x1AEDF70;
// List<KinomiData.SheetData>.Add(item) @0x2A3C680.
static constexpr uintptr_t RVA_KINOMI_LIST_ADD  = 0x2A3C680;

// PofinManager field offsets (Ghidra: EndCooking / OnUpdate).
static constexpr uintptr_t PM_BERRY_LIST   = 0x68;  // List<KinomiData.SheetData>
static constexpr uintptr_t PM_GAMECALC     = 0xB0;  // PofinGameCalc
static constexpr uintptr_t PM_COOKING_FLAG = 0xC0;  // byte: 1 while cooking, 0 = done/paused
// PofinGameCalc stat offsets.
static constexpr uintptr_t GC_BURNS      = 0x1C;  // int
static constexpr uintptr_t GC_SPILLS     = 0x20;  // int
static constexpr uintptr_t GC_SMOOTHNESS = 0x24;  // float
static constexpr uintptr_t GC_TIME       = 0x44;  // float (capped 60)
// KinomiData.SheetData.TagNo @+0x10.
static constexpr uintptr_t SHEET_TAGNO   = 0x10;
// C# List<T>: backing array @+0x10, _size @+0x18.
static constexpr uintptr_t LIST_ITEMS = 0x10;
static constexpr uintptr_t LIST_SIZE  = 0x18;

static constexpr float POFFIN_WAIT_TIMEOUT_SEC = 30.0f; // partner never cooks → solo fallback

// ---------------------------------------------------------------------------
// State
// ---------------------------------------------------------------------------
struct PoffinResult {
    bool     valid = false;
    int32_t  berryTag = 0;
    int32_t  burns = 0;
    int32_t  spills = 0;
    float    time = 0.0f;
    float    smoothness = 0.0f;
};

struct PoffinState {
    bool    armed = false;
    int32_t partner = -1;

    // Deferred cook-end: manager pointer stalled until the partner's result.
    bool    pendingComplete = false;
    void*   pendingManager = nullptr;
    float   waitTimer = 0.0f;

    PoffinResult mine;
    PoffinResult peer;

    void ClearRound() {
        pendingComplete = false;
        pendingManager = nullptr;
        waitTimer = 0.0f;
        mine = PoffinResult{};
        peer = PoffinResult{};
    }
};
static PoffinState s_pf;

bool mpPoffinIsArmed() { return s_pf.armed; }

// Poffin Case = ItemNo 449 (POFINKEESU). Owning it = Poffins unlocked.
bool mpPoffinUnlocked() {
    return PlayerWork::GetItem(449).fields.Count > 0;
}

void mpPoffinArm(int32_t partnerStation) {
    s_pf.armed = true;
    s_pf.partner = partnerStation;
    s_pf.ClearRound();
    MP_LOG("[Poffin] Co-op armed with station %d\n", partnerStation);
}

void mpPoffinDisarm() {
    if (!s_pf.armed) return;
    MP_LOG("[Poffin] Co-op disarmed\n");
    s_pf.armed = false;
    s_pf.partner = -1;
    s_pf.ClearRound();
}

void mpPoffinOnPeerLeft(int32_t stationIndex) {
    if (s_pf.armed && stationIndex == s_pf.partner) {
        mpPoffinDisarm();
    }
}

// ---------------------------------------------------------------------------
// HUD (top-of-screen toast, shared with team_up/minigames)
// ---------------------------------------------------------------------------
static void poffinToast(const char* text) {
    overworldMPShowAreaText(text);
}

// ---------------------------------------------------------------------------
// Stat read/write on the manager's PofinGameCalc
// ---------------------------------------------------------------------------
static void* gameCalc(void* mgr) {
    return *(void**)((uintptr_t)mgr + PM_GAMECALC);
}

static void readMyStats(void* mgr, PoffinResult* out) {
    void* gc = gameCalc(mgr);
    out->burns      = *(int32_t*)((uintptr_t)gc + GC_BURNS);
    out->spills     = *(int32_t*)((uintptr_t)gc + GC_SPILLS);
    out->smoothness = *(float*)((uintptr_t)gc + GC_SMOOTHNESS);
    out->time       = *(float*)((uintptr_t)gc + GC_TIME);

    // My berry TagNo = first entry of the berry list.
    void* list = *(void**)((uintptr_t)mgr + PM_BERRY_LIST);
    out->berryTag = 0;
    if (list != nullptr) {
        void* items = *(void**)((uintptr_t)list + LIST_ITEMS);
        int32_t size = *(int32_t*)((uintptr_t)list + LIST_SIZE);
        if (items != nullptr && size > 0) {
            // items is KinomiData.SheetData[]; element 0 at array+0x20.
            void* sheet0 = *(void**)((uintptr_t)items + 0x20);
            if (sheet0 != nullptr) {
                out->berryTag = *(int32_t*)((uintptr_t)sheet0 + SHEET_TAGNO);
            }
        }
    }
    out->valid = true;
}

// Inject the partner's berry into the manager's list + write combined stats.
static void applyCombined(void* mgr) {
    // Add the partner's berry (KinomiWork.GetKinomiData(tag) → SheetData → List.Add).
    void* list = *(void**)((uintptr_t)mgr + PM_BERRY_LIST);
    if (list != nullptr && s_pf.peer.berryTag != 0) {
        void* sheet = _ILExternal::external<void*>(RVA_GET_KINOMI_DATA, s_pf.peer.berryTag);
        if (sheet != nullptr) {
            _ILExternal::external<void>(RVA_KINOMI_LIST_ADD, list, sheet);
        }
    }

    // Combine stats: spills/burns/smoothness sum, time = min (best of the two).
    void* gc = gameCalc(mgr);
    int32_t burns  = s_pf.mine.burns + s_pf.peer.burns;
    int32_t spills = s_pf.mine.spills + s_pf.peer.spills;
    float smooth   = s_pf.mine.smoothness + s_pf.peer.smoothness;
    float t        = (s_pf.mine.time < s_pf.peer.time) ? s_pf.mine.time : s_pf.peer.time;
    *(int32_t*)((uintptr_t)gc + GC_BURNS)      = burns;
    *(int32_t*)((uintptr_t)gc + GC_SPILLS)     = spills;
    *(float*)((uintptr_t)gc + GC_SMOOTHNESS)   = smooth;
    *(float*)((uintptr_t)gc + GC_TIME)         = t;
    MP_LOG("[Poffin] Combined: burns=%d spills=%d time=%.1f smooth=%.1f + partner berry %d\n",
                burns, spills, t, smooth, s_pf.peer.berryTag);
}

// ---------------------------------------------------------------------------
// Packet
// ---------------------------------------------------------------------------
static void sendMyResult() {
    auto* pw = Dpr::NetworkUtils::NetworkManager::get_PacketWriterRe();
    if (pw == nullptr) return;
    il2cpp_vcall_void(pw, PW_RESET);
    il2cpp_vcall_write_byte(pw, PW_WRITE_BYTE, OWMP_DATA_ID_POFFIN_RESULT);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, s_pf.partner);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, s_pf.mine.berryTag);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, s_pf.mine.burns);
    il2cpp_vcall_write_s32(pw, PW_WRITE_S32, s_pf.mine.spills);
    il2cpp_vcall_write_fp32(pw, PW_WRITE_FP32, s_pf.mine.time);
    il2cpp_vcall_write_fp32(pw, PW_WRITE_FP32, s_pf.mine.smoothness);
    Dpr::NetworkUtils::NetworkManager::SendReliablePacketToAll(pw, 0);
    MP_LOG("[Poffin] Sent my result: berry=%d burns=%d spills=%d time=%.1f\n",
                s_pf.mine.berryTag, s_pf.mine.burns, s_pf.mine.spills, s_pf.mine.time);
}

// Forward decl: resume a deferred cook-end (defined via the hook struct below).
static void poffinResumeComplete();

void mpPoffinOnResultReceived(void* pr) {
    int32_t fromStation = il2cpp_vcall_int(pr, PR_FROM_STATION);
    int32_t target = 0;
    il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &target);
    int32_t berryTag = 0, burns = 0, spills = 0;
    float t = 0.0f, smooth = 0.0f;
    il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &berryTag);
    il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &burns);
    il2cpp_vcall_read_out(pr, PR_READ_S32_OUT, &spills);
    il2cpp_vcall_read_out(pr, PR_READ_FP32_OUT, &t);
    il2cpp_vcall_read_out(pr, PR_READ_FP32_OUT, &smooth);
    if (target != mpThisStationIndex()) return;
    if (!s_pf.armed || fromStation != s_pf.partner) return;

    // Range sanity (distrust posture): a phase-bounded cook can't exceed these.
    if (burns < 0 || burns > 999 || spills < 0 || spills > 999 ||
        t < 0.0f || t > 120.0f) {
        MP_LOG("[Poffin] Rejected implausible partner result\n");
        return;
    }

    s_pf.peer.valid = true;
    s_pf.peer.berryTag = berryTag;
    s_pf.peer.burns = burns;
    s_pf.peer.spills = spills;
    s_pf.peer.time = t;
    s_pf.peer.smoothness = smooth;
    MP_LOG("[Poffin] Partner result: berry=%d burns=%d spills=%d time=%.1f\n",
                berryTag, burns, spills, t);

    // If our cook already finished and is waiting, complete it now.
    if (s_pf.pendingComplete) {
        poffinResumeComplete();
    }
}

// ---------------------------------------------------------------------------
// EndCooking deferral hook
// ---------------------------------------------------------------------------
HOOK_DEFINE_TRAMPOLINE(PofinManager$$EndCooking) {
    static void Callback(void* __this) {
        // Solo cooking (not armed / no partner) — untouched.
        if (!s_pf.armed || s_pf.partner < 0) {
            Orig(__this);
            return;
        }

        // Capture our stats + berry and broadcast them.
        readMyStats(__this, &s_pf.mine);
        sendMyResult();

        if (s_pf.peer.valid) {
            // Partner already finished — merge and complete immediately.
            applyCombined(__this);
            s_pf.ClearRound();
            Orig(__this);
            return;
        }

        // Stall: pause the cook loop (OnUpdate early-returns on flag 0) and wait
        // for the partner's result — resumed from poffinResumeComplete().
        *(uint8_t*)((uintptr_t)__this + PM_COOKING_FLAG) = 0;
        s_pf.pendingManager = __this;
        s_pf.pendingComplete = true;
        s_pf.waitTimer = POFFIN_WAIT_TIMEOUT_SEC;
        poffinToast("Waiting for your partner to finish cooking...");
        MP_LOG("[Poffin] Cook finished — waiting for partner\n");
    }
};

static void poffinResumeComplete() {
    if (!s_pf.pendingComplete || s_pf.pendingManager == nullptr) return;
    void* mgr = s_pf.pendingManager;
    bool merged = s_pf.peer.valid;
    if (merged) {
        applyCombined(mgr);
    } else {
        MP_LOG("[Poffin] Timeout — completing solo (no partner result)\n");
        poffinToast("Your partner didn't finish — cooking solo.");
    }
    s_pf.ClearRound();
    PofinManager$$EndCooking::Orig(mgr);
    MP_LOG("[Poffin] Deferred cook completed (%s)\n", merged ? "merged" : "solo");
}

// ---------------------------------------------------------------------------
// Tick
// ---------------------------------------------------------------------------
void mpPoffinTick(float deltaTime) {
    if (!s_pf.pendingComplete) return;

    // Partner gone → complete solo so the local player isn't stuck.
    auto& ctx = getOverworldMPContext();
    if (s_pf.partner < 0 || !ctx.remotePlayers[s_pf.partner].isActive) {
        poffinResumeComplete();
        return;
    }

    s_pf.waitTimer -= deltaTime;
    if (s_pf.waitTimer <= 0.0f) {
        poffinResumeComplete();
    }
}

void exl_mp_poffin_hooks() {
    PofinManager$$EndCooking::InstallAtOffset(RVA_ENDCOOKING);
    MP_LOG("[Poffin] EndCooking hook installed\n");
}
