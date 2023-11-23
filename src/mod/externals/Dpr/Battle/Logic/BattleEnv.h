#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Dpr/Battle/Logic/FieldStatus.h"
#include "externals/Dpr/Battle/Logic/POKECON.h"
#include "externals/Dpr/Battle/Logic/PosPoke.h"

namespace Dpr::Battle::Logic {
    struct BattleEnv : ILClass<BattleEnv> {
        struct Fields {
            POKECON::Object* m_pokecon;
            FieldStatus::Object* m_fieldStatus; // Dpr_Battle_Logic_FieldStatus_o
            void* m_sideEffectManager; // Dpr_Battle_Logic_SideEffectManager_o
            void* m_posEffectManager; // Dpr_Battle_Logic_PosEffectManager_o
            void* m_eventFactorContainer; // Dpr_Battle_Logic_EventFactorContainer_o
            PosPoke::Object* m_posPoke;
            void* m_deadRec; // Dpr_Battle_Logic_DeadRec_o
            void* m_wazaRec; // Dpr_Battle_Logic_WazaRec_o
            void* m_affCounter; // Dpr_Battle_Logic_AffCounter_o
            void* m_actionRecorder; // Dpr_Battle_Logic_ActionRecorder_o
            void* m_actionSerialNoManager; // Dpr_Battle_Logic_ActionSerialNoManager_o
            void* m_timeLimit; // Dpr_Battle_Logic_TimeLimit_o
            void* m_gRightsManager; // Dpr_Battle_Logic_GRightsManager_o
            void* m_gGauge; // Dpr_Battle_Logic_GGauge_array
            void* m_raidBattleStatus; // Dpr_Battle_Logic_RaidBattleStatus_o
            void* m_flags; // Dpr_Battle_Logic_BattleFlags_o
            void* m_counter; // Dpr_Battle_Logic_BattleCounter_o
            void* m_escapeInfo; // Dpr_Battle_Logic_EscapeInfo_o
            void* m_lastExecutedWaza; // Dpr_Battle_Logic_WazaParam_o
            void* m_tamaHiroiData; // Dpr_Battle_Logic_TamaHiroiData_o
        };
    };
}
