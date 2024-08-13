#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BattleEnv.h"
#include "externals/Dpr/Battle/Logic/MainModule.h"

namespace Dpr::Battle::Logic {
    struct FrontPokeAccessor : ILClass<FrontPokeAccessor, 0x04c5a8e0> {
        struct Fields {
            MainModule::Object* m_pMainModule;
            BattleEnv::Object* m_pBattleEnv;
            uint8_t m_clientID;
            uint8_t m_pokeIndex;
            bool m_endFlag;
        };

        inline void ctor(MainModule::Object* pMainModule, BattleEnv::Object* pBattleEnv) {
            external<void>(0x018fbce0, this, pMainModule, pBattleEnv);
        }

        inline bool GetNext(BTL_POKEPARAM::Object** bpp) {
            return external<bool>(0x018fc040, this, bpp);
        }
    };
}
