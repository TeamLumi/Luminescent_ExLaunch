#include "exlaunch.hpp"
#include "logger/logger.h"
#include "externals/il2cpp-api.h"
#include "externals/Dpr/Battle/Logic/BATTLE_SETUP_PARAM.h"
#include "externals/PlayerWork.h"
#include "externals/Dpr/Battle/Logic/BtlRule.h"
#include "externals/FlagWork.h"
#include "externals/FlagWork_Enums.h"

#include "features/team_up.h"
#include "features/overworld_multiplayer.h"

using namespace Dpr::Battle::Logic;

// normalTrainer @ 0x1F6EE00 — fills a BSP party slot with trainer AI data
// Signature: void normalTrainer(BATTLE_SETUP_PARAM* bsp, int32_t clientId, int32_t trainerID)
static inline void normalTrainer(BATTLE_SETUP_PARAM::Object* bsp, int32_t clientId, int32_t trainerID) {
    _ILExternal::external<void>(0x1F6EE00, bsp, clientId, trainerID);
}

// Split trainer party between two AI slots (for single-trainer team-up)
// Non-static: also called from team_up.cpp for ACK handler and Player B setup
// If slot3's party doesn't exist, creates one (so normalTrainer is not needed for slot3)
void splitTrainerParty(BATTLE_SETUP_PARAM::Object* bsp, int slot1, int slot3) {
    auto* fields = &bsp->instance()->fields;
    if (fields->party == nullptr || (uint32_t)slot3 >= fields->party->max_length) return;

    auto* party1 = fields->party->m_Items[slot1];
    if (party1 == nullptr) return;

    auto* party3 = fields->party->m_Items[slot3];
    if (party3 == nullptr) {
        // Create a new PokeParty for slot 3 (no normalTrainer was called for this slot)
        party3 = (Pml::PokeParty::Object*)il2cpp_object_new((Il2CppClass*)party1->klass);
        _ILExternal::external<void>(0x2055D10, party3); // PokeParty::ctor()
        fields->party->m_Items[slot3] = party3;
        Logger::log("[TeamUp] Created new party for slot %d\n", slot3);
    }

    int32_t total = party1->fields.m_memberCount;

    if (total <= 1) {
        // Only 1 Pokemon: keep it in slot 1, copy to slot 3
        party1->fields.m_memberCount = 1;
        auto* src = party1->GetMemberPointer(0);
        auto* dst = party3->GetMemberPointer(0);
        if (src != nullptr && dst != nullptr &&
            src->fields.m_accessor != nullptr && dst->fields.m_accessor != nullptr) {
            uint8_t tmpBuf[344];
            _ILExternal::external<void>(0x24A4470, src->fields.m_accessor, tmpBuf);
            _ILExternal::external<void>(0x24A4550, dst->fields.m_accessor, tmpBuf);
        }
        party3->fields.m_memberCount = 1;
        Logger::log("[TeamUp] Trainer has 1 Pokemon — duplicated to both AI slots\n");
        return;
    }

    // Split: slot 1 gets first ceil(N/2), slot 3 gets remaining
    int32_t half = (total + 1) / 2;

    // Copy Pokemon from party1[half..N] into party3[0..]
    for (int i = 0; i < total - half; i++) {
        auto* src = party1->GetMemberPointer(half + i);
        auto* dst = party3->GetMemberPointer(i);
        if (src != nullptr && dst != nullptr &&
            src->fields.m_accessor != nullptr && dst->fields.m_accessor != nullptr) {
            uint8_t tmpBuf[344];
            _ILExternal::external<void>(0x24A4470, src->fields.m_accessor, tmpBuf);
            _ILExternal::external<void>(0x24A4550, dst->fields.m_accessor, tmpBuf);
        }
    }
    party1->fields.m_memberCount = half;
    party3->fields.m_memberCount = total - half;

    Logger::log("[TeamUp] Split trainer party: slot %d=%d, slot %d=%d (total %d)\n",
                slot1, half, slot3, total - half, total);
}

HOOK_DEFINE_TRAMPOLINE(SetupBattleTrainer) {
    static void Callback(BATTLE_SETUP_PARAM::Object *battleSetupParam, int32_t arenaID,
                         int32_t mapAttrib, int32_t weatherType, int32_t rule, int32_t enemyID0, int32_t enemyID1,
                         int32_t partnerID, MethodInfo *method)
    {
        if (FlagWork::GetFlag(FlagWork_Flag::FLAG_TRAINER_DOUBLE))
        {
            rule = (int32_t) BtlRule::BTL_RULE_DOUBLE;
        }

        auto& tu = overworldMPGetTeamUpState();
        bool doTeamUp = tu.isTeamedUp && partnerID == 0 &&
                        !PlayerWork::GetSystemFlag((int32_t)FlagWork_SysFlag::SYS_FLAG_PAIR);

        // Same-area check: only convert to double if partner is in the same area
        if (doTeamUp) {
            auto& ctx = getOverworldMPContext();
            int32_t ps = tu.partnerStation;
            if (ps >= 0 && ps < OW_MP_MAX_PLAYERS && ctx.remotePlayers[ps].isActive) {
                if (ctx.remotePlayers[ps].areaID != ctx.myAreaID) {
                    doTeamUp = false;
                    Logger::log("[TeamUp] Skipping: partner in different area (%d vs %d)\n",
                                ctx.remotePlayers[ps].areaID, ctx.myAreaID);
                }
            }
        }

        if (doTeamUp) {
            rule = (int32_t) BtlRule::BTL_RULE_DOUBLE;
            Logger::log("[TeamUp] SetupBattleTrainer: forcing double for team-up (enemy=%d)\n", enemyID0);
        }

        Orig(battleSetupParam, arenaID, mapAttrib, weatherType, rule, enemyID0, enemyID1, partnerID, method);

        if (!doTeamUp) return;

        // Orig has set up the BSP with the trainer's party in slot 1
        // (and slot 3 if enemyID1 != 0 for already-double trainers).
        // Do NOT duplicate the trainer into slot 3 for single trainers —
        // the ACK handler will set up slots properly.

        // Cache BSP pointer — it will be modified when partner ACK arrives
        extern Dpr::Battle::Logic::BATTLE_SETUP_PARAM::Object* s_teamUpBSP;
        s_teamUpBSP = battleSetupParam;

        // Store trainer battle info
        tu.battleType = 1; // trainer
        tu.battleTrainerID = enemyID0;
        tu.battleTrainerID2 = enemyID1; // 0 for single trainer
        tu.isInitiator = true; // Player A has event script

        // Capture BattleEffectID from tr_data for correct intro/BGM
        tu.battleEffectID = -1;
        auto* fields = &battleSetupParam->instance()->fields;
        if (fields->tr_data != nullptr) {
            uint8_t* trArr = (uint8_t*)fields->tr_data;
            uint32_t trLen = *(uint32_t*)(trArr + 0x18);
            Logger::log("[TeamUp] tr_data len=%d\n", trLen);
            // Scan all entries for first non-null trainer data with valid effect
            for (uint32_t i = 0; i < trLen; i++) {
                void* td = *(void**)(trArr + 0x20 + i * 8);
                if (td != nullptr) {
                    int32_t eff = _ILExternal::external<int32_t>(0x1AC45B0, td);
                    Logger::log("[TeamUp] tr_data[%d] effect=%d\n", i, eff);
                    if (tu.battleEffectID < 0) {
                        tu.battleEffectID = eff;
                    }
                }
            }
            if (tu.battleEffectID >= 0) {
                Logger::log("[TeamUp] Captured BattleEffectID=%d from trainer %d\n",
                            tu.battleEffectID, enemyID0);
            }
        }

        // Send TEAMUP_BATTLE to partner with our party + encounter info
        overworldMPSendTeamUpBattleTrainer(tu.partnerStation, enemyID0, arenaID, weatherType);

        // Mark battle pending
        tu.battlePending = true;

        Logger::log("[TeamUp] SetupBattleTrainer: enemy0=%d enemy1=%d effectID=%d, partner notified\n",
                    enemyID0, enemyID1, tu.battleEffectID);
    }
};

void exl_trainer_double_battles_main() {
    SetupBattleTrainer::InstallAtOffset(0x02c3b800);
}
