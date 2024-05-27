#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/System/Primitives.h"

namespace Dpr::Battle::Logic {
    struct BTL_POKEPARAM : ILClass<BTL_POKEPARAM> {
        struct Fields {
            void* m_coreParam;
            void* m_baseParam;
            void* m_varyParam;
            void* m_doryokuParam;
            void* m_waza;
            uint16_t m_tokusei;
            uint16_t m_weight;
            uint8_t m_wazaCnt;
            uint8_t m_formNo;
            uint8_t m_friendship;
            uint8_t m_criticalRank;
            uint8_t m_usedWazaCount;
            uint8_t m_prevWazaType;
            uint8_t m_spActPriority;
            uint16_t m_turnCount;
            uint16_t m_appearedTurn;
            uint16_t m_wazaContCounter;
            uint8_t m_prevTargetPos;
            int32_t m_prevActWazaID;
            int32_t m_prevSelectWazaID;
            int32_t m_prevDamagedWaza;
            System::Byte_array * m_turnFlag;
            System::Byte_array * m_contFlag;
            System::Byte_array * m_permFlag;
            System::Byte_array * m_counter;
            System::UInt32_array * m_permCounter;
            void * m_wazaDamageRec;
            System::Byte_array * m_dmgrecCount;
            uint8_t m_dmgrecTurnPtr;
            uint8_t m_dmgrecPtr;
            uint16_t m_migawariHP;
            int32_t m_combiWazaID;
            uint8_t m_combiPokeID;
            void * m_fldSim;
        };

        inline bool IsFightEnable() {
            return external<bool>(0x01fd9750, this);
        }

        inline Pml::PokePara::PokemonParam* GetSrcDataConst() {
            return external<Pml::PokePara::PokemonParam*>(0x01fdd5c0, this);
        }

        inline uint16_t GetMonsNo() {
            return external<uint16_t>(0x01fdcfb0, this);
        }

        inline int32_t GetValue(int32_t vid) {
            return external<int32_t>(0x01fdd010, this, vid);
        }

        inline int32_t GetID() {
            return external<int32_t>(0x01fd8a50, this);
        }

        inline bool CONFRONT_REC_IsMatch(int32_t pokeID) {
            return external<bool>(0x01fe39f0, this, pokeID);
        }

        inline uint16_t GetItem() {
            return external<uint16_t>(0x01fdfd50, this);
        }

        inline void COUNTER_Inc(int32_t cnt) {
            external<void>(0x01fe2cd0, this, cnt);
        }

        inline uint8_t COUNTER_Get(int32_t cnt) {
            return external<uint8_t>(0x01fe0140, this, cnt);
        }

        inline Pml::PokePara::PokemonParam::Object* GetSrcData() {
            return external<Pml::PokePara::PokemonParam::Object*>(0x01fdd5b0, this);
        }

        inline bool isGMode() {
            return external<bool>(0x01fdb9a0, this);
        }
    };
}
