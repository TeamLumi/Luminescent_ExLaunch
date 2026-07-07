#include "exlaunch.hpp"
#include "logger/logger.h"
#include "externals/il2cpp-api.h"
#include "externals/Dpr/Battle/Logic/BATTLE_SETUP_PARAM.h"
#include "externals/PlayerWork.h"
#include "externals/Dpr/Battle/Logic/BtlRule.h"
#include "externals/FlagWork.h"
#include "externals/FlagWork_Enums.h"

#include "features/trainer_battle.h"

using namespace Dpr::Battle::Logic;
using Dpr::EvScript::EvDataManager;

// RVAs of the two game methods every trainer-battle command funnels through.
constexpr int64_t RVA_SetBattleReturn = 0x02C45C90;  // EvDataManager::SetBattleReturn(this)
constexpr int64_t RVA_BattleTrainer   = 0x02C4FC10;  // EvDataManager::BattleTrainer(this, e0, e1, partner)

// One-shot override consumed by the SetupBattleTrainer hook below. StartTrainerBattle sets
// it immediately before invoking BattleTrainer, which synchronously calls SetupBattleTrainer
// exactly once, so a plain static (no persistent FlagWork state) is enough and self-clears.
struct PendingOverride {
    bool active;
    int32_t rule;
    int32_t multiMode;
};
static PendingOverride s_pending = { false, BTL_RULE_DERIVE, BTL_MULTIMODE_KEEP };

HOOK_DEFINE_TRAMPOLINE(SetupBattleTrainer) {
    static void Callback(BATTLE_SETUP_PARAM::Object *battleSetupParam, int32_t arenaID,
                         int32_t mapAttrib, int32_t weatherType, int32_t rule, int32_t enemyID0,
                         int32_t enemyID1, int32_t partnerID, MethodInfo *method)
    {
        // Snapshot and consume the one-shot override up front so a re-entrant call can't
        // reuse it.
        PendingOverride pending = s_pending;
        s_pending.active = false;

        if (pending.active)
        {
            // New command path: honor whatever rule the caller asked for.
            if (pending.rule != BTL_RULE_DERIVE)
                rule = pending.rule;
        }
        else if (FlagWork::GetFlag(FlagWork_Flag::FLAG_TRAINER_DOUBLE))
        {
            // Legacy path: baked event scripts (gyms, common_scr, dungeons) set flag 2196
            // before a normal _TRAINER_BTL_SET to force a double battle vs a single trainer.
            // TODO: remove once all baked scripts migrate to the _TRAINER_DOUBLE_BTL command.
            rule = (int32_t) BtlRule::BTL_RULE_DOUBLE;
        }

        Orig(battleSetupParam, arenaID, mapAttrib, weatherType, rule, enemyID0, enemyID1, partnerID, method);

        // multiMode is set inside the game's setup helpers; override it afterwards for the
        // single-opponent modes (e.g. PA_A2) that the default path wouldn't select.
        if (pending.active && pending.multiMode != BTL_MULTIMODE_KEEP && battleSetupParam != nullptr)
            battleSetupParam->fields.multiMode = (uint8_t) pending.multiMode;
    }
};

void StartTrainerBattle(EvDataManager::Object* manager,
                        int32_t enemyID0, int32_t enemyID1, int32_t partnerID,
                        int32_t rule, int32_t multiMode)
{
    // Only arm the override when the caller actually wants to steer rule or multiMode;
    // otherwise BattleTrainer derives everything on its own (vanilla behavior).
    s_pending.active = (rule != BTL_RULE_DERIVE) || (multiMode != BTL_MULTIMODE_KEEP);
    s_pending.rule = rule;
    s_pending.multiMode = multiMode;

    EvDataManager::external<void>(RVA_SetBattleReturn, manager);
    EvDataManager::external<void>(RVA_BattleTrainer, manager, enemyID0, enemyID1, partnerID);
}

void exl_trainer_double_battles_main() {
    SetupBattleTrainer::InstallAtOffset(0x02c3b800);
}
