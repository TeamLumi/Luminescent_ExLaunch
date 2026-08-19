#pragma once

#include "externals/Dpr/EvScript/EvDataManager.h"

constexpr int32_t BTL_RULE_DERIVE = -1;
constexpr int32_t BTL_MULTIMODE_KEEP = -1;

// This is the battle mode that lets the enemy trainer use 2 pokemon against the player and one partner
constexpr int32_t BTL_MULTIMODE_PA_A2 = 6;

// one enemy trainer battles that are multi battles require this override
void StartTrainerBattle(Dpr::EvScript::EvDataManager::Object* manager,
                        int32_t enemyID0, int32_t enemyID1, int32_t partnerID,
                        int32_t rule, int32_t multiMode);
