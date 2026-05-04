#include "exlaunch.hpp"

#include "externals/FlagWork.h"

#include "logger/logger.h"

HOOK_DEFINE_TRAMPOLINE(DailyEventWork$$ResetScriptFlag) {
    static void Callback() {
        Logger::log("[DailyEventWork$$ResetScriptFlag] New Day! Resetting time flags\n");
        Orig();

        // Any extra time flags to add
        FlagWork::SetFlag(FlagWork_Flag::TMFLG_LUMI_000, false);
        FlagWork::SetFlag(FlagWork_Flag::TMFLG_LUMI_001, false);
        FlagWork::SetFlag(FlagWork_Flag::TMFLG_LUMI_002, false);
        FlagWork::SetFlag(FlagWork_Flag::TMFLG_LUMI_003, false);
        FlagWork::SetFlag(FlagWork_Flag::TMFLG_LUMI_004, false);
        FlagWork::SetFlag(FlagWork_Flag::TMFLG_LUMI_005, false);
        FlagWork::SetFlag(FlagWork_Flag::TMFLG_LUMI_006, false);
        FlagWork::SetFlag(FlagWork_Flag::TMFLG_LUMI_007, false);
        FlagWork::SetFlag(FlagWork_Flag::TMFLG_LUMI_008, false);
        FlagWork::SetFlag(FlagWork_Flag::TMFLG_LUMI_009, false);
        FlagWork::SetFlag(FlagWork_Flag::TMFLG_LUMI_010, false);
        FlagWork::SetFlag(FlagWork_Flag::TMFLG_LUMI_011, false);
        FlagWork::SetFlag(FlagWork_Flag::TMFLG_LUMI_012, false);
        FlagWork::SetFlag(FlagWork_Flag::TMFLG_LUMI_013, false);
        FlagWork::SetFlag(FlagWork_Flag::TMFLG_LUMI_014, false);
        FlagWork::SetFlag(FlagWork_Flag::TMFLG_LUMI_015, false);
        FlagWork::SetFlag(FlagWork_Flag::TMFLG_LUMI_016, false);
        FlagWork::SetFlag(FlagWork_Flag::TMFLG_LUMI_017, false);
        FlagWork::SetFlag(FlagWork_Flag::TMFLG_LUMI_018, false);
        FlagWork::SetFlag(FlagWork_Flag::TMFLG_LUMI_019, false);
    }
};

void exl_time_flags_main() {
    DailyEventWork$$ResetScriptFlag::InstallAtOffset(0x01ab43f0);
}
