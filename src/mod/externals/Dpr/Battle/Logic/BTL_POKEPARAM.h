#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Dpr/Battle/Logic/PokeTypePair.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/Pml/WazaData/WazaSick.h"
#include "externals/System/Primitives.h"

namespace Dpr::Battle::Logic {
    struct BTL_POKEPARAM : ILClass<BTL_POKEPARAM> {
        enum class NemuriCheckMode : int32_t {
            NEMURI_CHECK_ONLY_SICK = 0,
            NEMURI_CHECK_INCLUDE_ZETTAINEMURI = 1,
        };

        enum class ValueID : int32_t {
            BPP_VALUE_NULL = 0,
            BPP_ATTACK_RANK = 1,
            BPP_DEFENCE_RANK = 2,
            BPP_SP_ATTACK_RANK = 3,
            BPP_SP_DEFENCE_RANK = 4,
            BPP_AGILITY_RANK = 5,
            BPP_HIT_RATIO = 6,
            BPP_AVOID_RATIO = 7,
            BPP_ATTACK = 8,
            BPP_DEFENCE = 9,
            BPP_SP_ATTACK = 10,
            BPP_SP_DEFENCE = 11,
            BPP_AGILITY = 12,
            BPP_HP = 13,
            BPP_HP_BEFORE_G = 14,
            BPP_MAX_HP = 15,
            BPP_MAX_HP_BEFORE_G = 16,
            BPP_LEVEL = 17,
            BPP_TOKUSEI = 18,
            BPP_TOKUSEI_EFFECTIVE = 19,
            BPP_SEX = 20,
            BPP_SEIKAKU = 21,
            BPP_PERSONAL_RAND = 22,
            BPP_EXP = 23,
            BPP_MONS_POW = 24,
            BPP_MONS_AGILITY = 25,
            BPP_RANKVALUE_START = 1,
            BPP_RANKVALUE_END = 7,
            BPP_RANKVALUE_RANGE = 7,
        };
        
        enum class Counter : int32_t {
            COUNTER_TAKUWAERU = 0,
            COUNTER_TAKUWAERU_DEF = 1,
            COUNTER_TAKUWAERU_SPDEF = 2,
            COUNTER_MAMORU = 3,
            COUNTER_FREEFALL = 4,
            COUNTER_TURN_FROM_GWALL_BROKEN = 5,
            COUNTER_MAX = 6,
        };

        enum class TurnFlag : int32_t {
            TURNFLG_ACTION_START = 0,
            TURNFLG_ACTION_DONE = 1,
            TURNFLG_DAMAGED = 2,
            TURNFLG_WAZAPROC_DONE = 3,
            TURNFLG_SHRINK = 4,
            TURNFLG_KIAI_READY = 5,
            TURNFLG_KIAI_SHRINK = 6,
            TURNFLG_MAMORU = 7,
            TURNFLG_ITEM_CONSUMED = 8,
            TURNFLG_ITEM_CANT_USE = 9,
            TURNFLG_COMBIWAZA_READY = 10,
            TURNFLG_TAMEHIDE_OFF = 11,
            TURNFLG_MOVED = 12,
            TURNFLG_TURNCHECK_SICK_PASSED = 13,
            TURNFLG_HITRATIO_UP = 14,
            TURNFLG_NAGETUKERU_USING = 15,
            TURNFLG_MAMORU_ONLY_DAMAGE_WAZA = 16,
            TURNFLG_RESERVE_ITEM_SPEND = 17,
            TURNFLG_APPEARED_BY_POKECHANGE = 18,
            TURNFLG_CANT_ACTION = 19,
            TURNFLG_TRAPPSHELL_READY = 20,
            TURNFLG_GWALL_BROKEN = 21,
            TURNFLG_RAIDBOSS_REINFORCE_DONE = 22,
            TURNFLG_RAIDBOSS_ANGRY = 23,
            TURNFLG_RAIDBOSS_ANGRY_WAZA_ADD_DONE = 24,
            TURNFLG_RANK_UP = 25,
            TURNFLG_RANK_DOWN = 26,
            TURNFLG_MAX = 27,
        };

        enum class ExTypeCause : int32_t {
            EXTYPE_CAUSE_NONE = 0,
            EXTYPE_CAUSE_HALLOWEEN = 1,
            EXTYPE_CAUSE_MORINONOROI = 2,
        };

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

        inline int32_t GetValue(BTL_POKEPARAM::ValueID vid) {
            return external<int32_t>(0x01fdd010, this, vid);
        }

        inline uint8_t GetID() {
            return external<uint8_t>(0x01fd8a50, this);
        }

        inline bool CONFRONT_REC_IsMatch(int32_t pokeID) {
            return external<bool>(0x01fe39f0, this, pokeID);
        }

        inline uint16_t GetItem() {
            return external<uint16_t>(0x01fdfd50, this);
        }

        inline void COUNTER_Inc(Counter cnt) {
            external<void>(0x01fe2cd0, this, cnt);
        }

        inline uint8_t COUNTER_Get(Counter cnt) {
            return external<uint8_t>(0x01fe0140, this, cnt);
        }

        inline Pml::PokePara::PokemonParam::Object* GetSrcData() {
            return external<Pml::PokePara::PokemonParam::Object*>(0x01fdd5b0, this);
        }

        inline bool CheckSick(Pml::WazaData::WazaSick sickType) {
            return external<bool>(0x01fd8b30, this, sickType);
        }

        inline bool CheckNemuri(NemuriCheckMode checkMode) {
            return external<bool>(0x01fd8ab0, this, checkMode);
        }

        inline uint8_t GetFriendship() {
            return external<uint8_t>(0x01fda180, this);
        }

        inline bool IsDead() {
            return external<bool>(0x01fd8a60, this);
        }

        inline void TURNFLAG_Set(TurnFlag flagID) {
            external<void>(0x01fe0d90, this, flagID);
        }

        inline int32_t WAZA_GetID(uint8_t idx) {
            return external<int32_t>(0x01fdeb70, this, idx);
        }

        inline bool IsMatchType(uint8_t type) {
            return external<bool>(0x01fdfad0, this, type);
        }

        inline uint8_t WAZA_SearchIdx(int32_t waza) {
            return external<uint8_t>(0x01fdf880, this, waza);
        }

        inline uint16_t WAZA_GetPP(uint8_t wazaIdx) {
            return external<uint16_t>(0x01fdf140, this, wazaIdx);
        }

        inline bool TURNFLAG_Get(TurnFlag flagID) {
            return external<bool>(0x01fdfe70, this, flagID);
        }

        inline PokeTypePair::Object GetPokeType() {
            return external<PokeTypePair::Object>(0x01fdfa80, this);
        }

        inline bool isGMode() {
            return external<bool>(0x01fdb9a0, this);
        }

        inline int32_t WAZA_GetID(uint8_t idx) {
            return external<int32_t>(0x01fdeb70, this, idx);
        }

        inline uint8_t WAZA_GetCount() {
            return external<int32_t>(0x01fde9c0, this);
        }

        inline bool MIGAWARI_IsExist() {
            return external<bool>(0x01fe38a0, this);
        }

        inline bool IsWazaHide() {
            return external<bool>(0x01fe0060, this);
        }

        inline static uint8_t PokeIDtoFreeFallCounter(uint8_t pokeID) {
            return external<uint8_t>(0x01fda190, pokeID);
        }
    };
}
