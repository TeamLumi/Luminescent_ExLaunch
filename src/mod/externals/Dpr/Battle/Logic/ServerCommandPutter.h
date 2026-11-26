#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "externals/Dpr/Battle/Logic/ContFlag.h"
#include "externals/Dpr/Battle/Logic/StrParam.h"

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
            external<void>(game::Address::Dpr_Battle_Logic_ServerCommandPutter_SetBppCounter, this, poke, counterID, value);
        }

        inline void InsertWazaInfo(uint8_t pokeID, bool isTokuseiWindowDisplay, StrParam::Object** message) {
            external<void>(game::Address::Dpr_Battle_Logic_ServerCommandPutter_InsertWazaInfo, this, pokeID, isTokuseiWindowDisplay, message);
        }

        inline void SetContFlag(BTL_POKEPARAM::Object* poke, ContFlag flag) {
            external<void>(game::Address::Dpr_Battle_Logic_ServerCommandPutter_SetContFlag, this, poke, flag);
        }

        inline bool RemoveFieldEffect(int32_t effect) {
            return external<bool>(game::Address::Dpr_Battle_Logic_ServerCommandPutter_RemoveFieldEffect, this, effect);
        }

        inline bool RemoveFieldEffect_DependPoke(BTL_POKEPARAM::Object* causedPoke, int32_t fieldEffect) {
            return external<bool>(game::Address::Dpr_Battle_Logic_ServerCommandPutter_RemoveFieldEffect_DependPoke, this, causedPoke, fieldEffect);
        }

        inline void RemoveFieldHandler(int32_t effect) {
            external<void>(game::Address::Dpr_Battle_Logic_ServerCommandPutter_RemoveFieldHandler, this, effect);
        }

        inline void Message_Std(uint16_t strID) {
            external<void>(game::Address::Dpr_Battle_Logic_ServerCommandPutter_Message_Std, this, strID);
        }

        inline void Message_Set(BTL_POKEPARAM::Object* poke, uint16_t strID) {
            external<void>(game::Address::Dpr_Battle_Logic_ServerCommandPutter_Message_Set, this, poke, strID);
        }

        inline bool Message(StrParam::Object** strParam) {
            return external<bool>(game::Address::Dpr_Battle_Logic_ServerCommandPutter_Message, this, strParam);
        }

        inline void Act_ChangeForm(uint8_t pokeID) {
            external<void>(game::Address::Dpr_Battle_Logic_ServerCommandPutter_Act_ChangeForm, this, pokeID);
        }

        inline void FakeDisable(uint8_t pokeID) {
            external<void>(game::Address::Dpr_Battle_Logic_ServerCommandPutter_FakeDisable, this, pokeID);
        }

        inline void Act_FakeDisable(uint8_t pokeID) {
            external<void>(game::Address::Dpr_Battle_Logic_ServerCommandPutter_Act_FakeDisable, this, pokeID);
        }

        inline void PublishClientInformation_AppeardPokemon(BTL_POKEPARAM::Object** appeardPoke) {
            external<void>(game::Address::Dpr_Battle_Logic_ServerCommandPutter_PublishClientInformation_AppeardPokemon, this, appeardPoke);
        }

        inline void Message_Set(uint16_t strID, int32_t arg1) {
            external<void>(game::Address::Dpr_Battle_Logic_ServerCommandPutter_Message_Set_UInt16_Int32, this, strID, arg1);
        }
    };
}
