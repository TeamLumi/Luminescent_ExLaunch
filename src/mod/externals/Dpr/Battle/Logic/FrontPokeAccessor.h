#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BattleEnv.h"
#include "externals/Dpr/Battle/Logic/MainModule.h"

namespace Dpr::Battle::Logic {
    struct FrontPokeAccessor : ILClass<FrontPokeAccessor, game::Address::Dpr_Battle_Logic_FrontPokeAccessor_TypeInfo> {
        struct Fields {
            MainModule::Object* m_pMainModule;
            BattleEnv::Object* m_pBattleEnv;
            uint8_t m_clientID;
            uint8_t m_pokeIndex;
            bool m_endFlag;
        };

        inline void ctor(MainModule::Object* pMainModule, BattleEnv::Object* pBattleEnv) {
            external<void>(game::Address::Dpr_Battle_Logic_FrontPokeAccessor_ctor, this, pMainModule, pBattleEnv);
        }

        inline bool GetNext(BTL_POKEPARAM::Object** bpp) {
            return external<bool>(game::Address::Dpr_Battle_Logic_FrontPokeAccessor_GetNext, this, bpp);
        }
    };
}
