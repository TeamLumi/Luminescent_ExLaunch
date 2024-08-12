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
            external<void>(0x01f2ab90, this, poke, counterID, value);
        }

        inline void InsertWazaInfo(uint8_t pokeID, bool isTokuseiWindowDisplay, StrParam::Object** message) {
            external<void>(0x01f2d0c0, this, pokeID, isTokuseiWindowDisplay, message);
        }

        inline void SetContFlag(BTL_POKEPARAM::Object* poke, ContFlag flag) {
            external<void>(0x01f187e0, this, poke, flag);
        }

        inline bool RemoveFieldEffect(int32_t effect) {
            return external<bool>(0x01f2a0b0, this, effect);
        }

        inline bool RemoveFieldEffect_DependPoke(BTL_POKEPARAM::Object* causedPoke, int32_t fieldEffect) {
            return external<bool>(0x01f2a260, this, causedPoke, fieldEffect);
        }

        inline void RemoveFieldHandler(int32_t effect) {
            external<void>(0x01f305e0, this, effect);
        }

        inline void Message_Std(uint16_t strID) {
            external<void>(0x01f289e0, this, strID);
        }

        inline void Message_Set(BTL_POKEPARAM::Object* poke, uint16_t strID) {
            external<void>(0x01f17ab0, this, poke, strID);
        }

        inline bool Message(StrParam::Object** strParam) {
            return external<bool>(0x01f27ec0, this, strParam);
        }
    };
}
