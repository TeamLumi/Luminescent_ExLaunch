#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "externals/Dpr/Battle/Logic/ContFlag.h"

namespace Dpr::Battle::Logic {
    struct BattleEnv;
    struct MainModule;

    struct ServerCommandPutter : ILClass<ServerCommandPutter> {
        struct Fields {
            MainModule* m_pMainModule;
            BattleEnv* m_pBattleEnv;
            void* m_pQueue; // Dpr_Battle_Logic_ServerCommandQueue_o*
            void* m_pExecutor; // Dpr_Battle_Logic_ServerCommandExecutor_o*
        };

        inline void SetBppCounter(BTL_POKEPARAM::Object* poke, BTL_POKEPARAM::Counter counterID, uint8_t value) {
            external<void>(0x01f2ab90, this, counterID, value);
        }

        inline void InsertWazaInfo(uint8_t pokeID, bool isTokuseiWindowDisplay, StrParam::Object** message) {
            external<void>(0x01f2d0c0, this, pokeID, isTokuseiWindowDisplay, message);
        }

        inline void SetContFlag(BTL_POKEPARAM::Object* poke, ContFlag flag) {
            external<void>(0x01f187e0, this, poke, flag);
        }
    };
}
