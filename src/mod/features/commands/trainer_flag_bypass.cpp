// trainer_flag_bypass.cpp — Bypass _IF_TR_FLAGON_JUMP/CALL for team-up sync
//
// When Player B receives a SYNC_WAIT from Player A at a trainer they've already
// defeated, bypassTrainerFlag is set true. This makes the event script's
// "_IF_TR_FLAGON_JUMP" / "_IF_TR_FLAGON_CALL" NOT take the "already defeated"
// branch, so Player B's script proceeds to the battle setup as if it's fresh.
// The bypass is narrow: cleared when sync resolves, cancels, or partner disconnects.
//
// Uses proper Replace hooks on EvMacro_IF_TR_FLAGON_JUMP (0x2C61C30) and
// EvMacro_IF_TR_FLAGON_CALL (0x2C61E50) instead of command-level intercepts.

#include "exlaunch.hpp"

#include "externals/Dpr/EvScript/EvDataManager.h"
#include "features/team_up.h"

// EvMacro_IF_TR_FLAGON_JUMP @ 0x2C61C30
HOOK_DEFINE_TRAMPOLINE(IfTrFlagonJump) {
    static bool Callback(Dpr::EvScript::EvDataManager::Object* __this, MethodInfo* mi) {
        if (overworldMPIsTeamedUp() && overworldMPGetTeamUpState().bypassTrainerFlag) {
            // Bypass: pretend trainer flag is NOT set — don't take the jump.
            // Return true = command complete, script advances to next command.
            return true;
        }
        return Orig(__this, mi);
    }
};

// EvMacro_IF_TR_FLAGON_CALL @ 0x2C61E50
HOOK_DEFINE_TRAMPOLINE(IfTrFlagonCall) {
    static bool Callback(Dpr::EvScript::EvDataManager::Object* __this, MethodInfo* mi) {
        if (overworldMPIsTeamedUp() && overworldMPGetTeamUpState().bypassTrainerFlag) {
            return true;
        }
        return Orig(__this, mi);
    }
};

void exl_trainer_flag_bypass_main() {
    IfTrFlagonJump::InstallAtOffset(0x2C61C30);
    IfTrFlagonCall::InstallAtOffset(0x2C61E50);
}
