#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/MainModule.h"
#include "externals/FlagWork.h"
#include "externals/PlayerWork.h"

#include "logger/logger.h"

const FlagWork_Work TURN_COUNTER_WORK = FlagWork_Work::WK_LAST_BATTLE_TURN_COUNTER;

HOOK_DEFINE_INLINE(BattleProc_FinalizeCoroutine__EndOfBattleTurnCounter) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        Logger::log("BattleProc_FinalizeCoroutine__EndOfBattleTurnCounter\n");
        auto mainModule = (Dpr::Battle::Logic::MainModule::Object*)ctx->X[0];

        Logger::log("Getting turn count...\n");
        int32_t turnCount = mainModule->fields.m_battleEnvForServer->fields.m_counter->Get(0);
        Logger::log("Turn count is %d\n", turnCount);
        PlayerWork::SetInt((int32_t)TURN_COUNTER_WORK, turnCount);

        Logger::log("Calling original line of code\n");
        ctx->X[0] = (uint64_t)mainModule->LeavenOnErrorCoroutine();
        Logger::log("BattleProc_FinalizeCoroutine__EndOfBattleTurnCounter done!\n");
    }
};

void exl_turn_counter_main() {
    BattleProc_FinalizeCoroutine__EndOfBattleTurnCounter::InstallAtOffset(0x0187e22c);
}
