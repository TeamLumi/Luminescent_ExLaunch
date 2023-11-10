#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "externals/Dpr/Battle/Logic/PokeAction.h"
#include "externals/Dpr/Battle/Logic/PokeActionCategory.h"

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
    };
}
