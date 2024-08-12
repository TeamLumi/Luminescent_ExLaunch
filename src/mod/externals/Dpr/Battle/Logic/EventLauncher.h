#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "externals/Dpr/Battle/Logic/PokeAction.h"
#include "externals/Dpr/Battle/Logic/PokeActionCategory.h"
#include "externals/Dpr/Battle/Logic/WazaParam.h"

namespace Dpr::Battle::Logic {
    struct BattleEnv;
    struct EventSystem;
    struct MainModule;

    struct EventLauncher : ILClass<EventLauncher> {
        struct Fields {
            MainModule* m_pMainModule;
            EventSystem* m_pEventSystem;
            BattleEnv* m_pBattleEnv;
        };

        inline void Event_ActProcCanceled(PokeAction::Object* action) {
            external<void>(0x01d17cc0, this, action);
        }

        inline void Event_ActProcStart(PokeAction::Object* action) {
            external<void>(0x01d17b80, this, action);
        }

        inline void Event_ActProcEnd(BTL_POKEPARAM::Object* bpp, PokeActionCategory actionCmd) {
            external<void>(0x01d17c30, this, bpp, actionCmd);
        }

        inline void Event_GetWazaParam(int32_t waza, int32_t orgWaza, int32_t gSrcWaza, int32_t wazaPri, BTL_POKEPARAM::Object* attacker, WazaParam::Object* param) {
            external<void>(0x01d14030, this, waza, orgWaza, gSrcWaza, wazaPri, attacker, param);
        }

        inline bool Event_Check_FreeFallStart_Guard(BTL_POKEPARAM::Object* attacker, BTL_POKEPARAM::Object* target, WazaParam::Object* wazaParam, StrParam::Object* strParam) {
            return external<bool>(0x01d19890, this, attacker, target, wazaParam, strParam);
        }
    };
}
