#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/EventVar.h"

namespace Dpr::Battle::Logic {
    struct BattleEnv;
    struct MainModule;
    struct PokeActionContainer;
    struct SectionContainer;
    struct SectionSharedData;

    struct EventSystem : ILClass<EventSystem> {
        struct Fields {
            MainModule* m_pMainModule;
            BattleEnv* m_pBattleEnv;
            PokeActionContainer* m_pPokeActionContainer;
            void* m_pPokeChangeRequest; // Dpr_Battle_Logic_PokeChangeRequest_o*
            SectionContainer* m_pSectionContainer;
            SectionSharedData* m_pSectionSharedData;
            void* m_variableStack; // Dpr_Battle_Logic_EventVarStack_o*
            void* m_variableSetStack; // Dpr_Battle_Logic_EventVarSetStack_o*
            void* m_pCurrentEventVarSet; // Dpr_Battle_Logic_EventVarSet_o*
            uint32_t m_eventLevel;
        };

        inline void EVENTVAR_Push() {
            external<void>(0x018f5510, this);
        }

        inline void EVENTVAR_SetConstValue(EventVar::Label label, int32_t value) {
            external<void>(0x018f5b60, this, label, value);
        }

        inline void EVENTVAR_SetRewriteOnceValue(EventVar::Label label, int32_t value) {
            external<void>(0x018f5c60, this, label, value);
        }

        inline void CallEvent(EventID eventID) {
            external<void>(0x018f4a00, this, eventID);
        }

        inline int32_t EVENTVAR_GetValue(EventVar::Label label) {
            return external<int32_t>(0x018f60a0, this, label);
        }

        inline void EVENTVAR_Pop() {
            external<void>(0x018f56a0, this);
        }
    };
}
