// trainer_flag_bypass.cpp — Bypass _IF_TR_FLAGON_JUMP/CALL for team-up sync
//
// When Player B receives a SYNC_WAIT from Player A at a trainer they've already
// defeated, bypassTrainerFlag is set true. This makes the event script's
// "_IF_TR_FLAGON_JUMP" / "_IF_TR_FLAGON_CALL" NOT take the "already defeated"
// branch, so Player B's script proceeds to the battle setup as if it's fresh.
// The bypass is narrow: cleared when sync resolves, cancels, or partner disconnects.

#include "externals/Dpr/EvScript/EvDataManager.h"

#include "features/team_up.h"

// Returns true (bypass active, skip the jump/call) or false (not bypassing,
// caller should fall through to Orig).
bool IfTrFlagonBypass(Dpr::EvScript::EvDataManager::Object* manager) {
    if (!overworldMPIsTeamedUp()) return false;
    auto& tu = overworldMPGetTeamUpState();
    if (tu.bypassTrainerFlag) {
        // Bypass: pretend trainer flag is NOT set — don't jump/call.
        // Return true = command complete, script advances to next command.
        return true;
    }
    // Not bypassing — return false to signal caller should use Orig()
    return false;
}
