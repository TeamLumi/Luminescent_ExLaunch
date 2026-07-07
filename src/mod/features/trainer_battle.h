#pragma once

#include "externals/Dpr/EvScript/EvDataManager.h"

// Sentinel for StartTrainerBattle's `rule`: let EvDataManager::BattleTrainer derive
// the rule itself (SINGLE for one enemy trainer, DOUBLE when a second one is present).
constexpr int32_t BTL_RULE_DERIVE = -1;

// Sentinel for StartTrainerBattle's `multiMode`: leave whatever the game's setup path
// chose. BATTLE_SETUP_PARAM.multiMode only needs forcing for the single-opponent modes.
constexpr int32_t BTL_MULTIMODE_KEEP = -1;

// BATTLE_SETUP_PARAM.multiMode value for a "player + AI partner vs a single AI enemy
// that sends out 2 Pokemon" battle. Its position-cover table [0,1,2,1,5] is the only
// baked layout that maps our slots correctly (see teamup_single_trainer_paa2 notes).
constexpr int32_t BTL_MULTIMODE_PA_A2 = 6;

// Starts a trainer battle through the game's own EvDataManager::BattleTrainer, exactly
// like the vanilla _TRAINER_BTL_SET / _TRAINER_MULTI_BTL_SET commands do (SetBattleReturn
// followed by BattleTrainer). This replaces the old FlagWork + static-variable + two-step
// mechanism: a single command now sets up AND starts the battle.
//
//   enemyID0   first (required) enemy trainer ID
//   enemyID1   second enemy trainer ID, or 0 for a single opponent
//   partnerID  ally trainer ID, or 0 for none
//   rule       BtlRule to force, or BTL_RULE_DERIVE to let BattleTrainer decide
//   multiMode  multiMode to force after setup, or BTL_MULTIMODE_KEEP
//
// The rule/multiMode overrides exist because BattleTrainer derives the rule purely from
// whether a 2nd enemy trainer is present, and never sets multiMode. A single enemy trainer
// that must fight a DOUBLE / PA_A2 battle therefore needs the nudge, which the slim
// SetupBattleTrainer hook applies as a consume-once override.
void StartTrainerBattle(Dpr::EvScript::EvDataManager::Object* manager,
                        int32_t enemyID0, int32_t enemyID1, int32_t partnerID,
                        int32_t rule, int32_t multiMode);
