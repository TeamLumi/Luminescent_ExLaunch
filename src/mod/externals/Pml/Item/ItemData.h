#pragma once

#include "externals/il2cpp-api.h"

namespace Pml::Item {
    struct ItemData : ILClass<ItemData> {
        enum class PrmID : int32_t {
            ITEMNUMBER = 0,
            PRICE = 1,
            WAT_PRICE = 2,
            BP_PRICE = 3,
            ICONID = 4,
            EQUIP = 5,
            ATTACK = 6,
            TUIBAMU_EFF = 7,
            NAGE_EFF = 8,
            NAGE_ATC = 9,
            SIZEN_ATC = 10,
            SIZEN_TYPE = 11,
            IMP = 12,
            CNV = 13,
            F_POCKET = 14,
            F_FUNC = 15,
            B_FUNC = 16,
            WORK_TYPE = 17,
            ITEM_TYPE = 18,
            SPEND = 19,
            USE_SPEND = 20,
            SORT = 21,
            GROUP = 22,
            GROUPID = 23,
            SET_TO_POKE = 24,
            B_SELECTABLE = 25,
            INACTIVE = 26,
            WORK = 27,
            SLEEP_RCV = 27,
            POISON_RCV = 28,
            BURN_RCV = 29,
            ICE_RCV = 30,
            PARALYZE_RCV = 31,
            PANIC_RCV = 32,
            MEROMERO_RCV = 33,
            ABILITY_GUARD = 34,
            DEATH_RCV = 35,
            ALL_DEATH_RCV = 36,
            LV_UP = 37,
            EVOLUTION = 38,
            ATTACK_UP = 39,
            DEFENCE_UP = 40,
            SP_ATTACK_UP = 41,
            SP_DEFENCE_UP = 42,
            AGILITY_UP = 43,
            HIT_UP = 44,
            CRITICAL_UP = 45,
            PP_UP = 46,
            PP_3UP = 47,
            PP_RCV = 48,
            ALL_PP_RCV = 49,
            HP_RCV = 50,
            HP_EXP = 51,
            POWER_EXP = 52,
            DEFENCE_EXP = 53,
            AGILITY_EXP = 54,
            SP_ATTACK_EXP = 55,
            SP_DEFENCE_EXP = 56,
            EXP_LIMIT_FLAG = 57,
            FRIEND1 = 58,
            FRIEND2 = 59,
            FRIEND3 = 60,
            HP_EXP_POINT = 61,
            POWER_EXP_POINT = 62,
            DEFENCE_EXP_POINT = 63,
            AGILITY_EXP_POINT = 64,
            SP_ATTACK_EXP_POINT = 65,
            SP_DEFENCE_EXP_POINT = 66,
            HP_RCV_POINT = 67,
            PP_RCV_POINT = 68,
            FRIEND1_POINT = 69,
            FRIEND2_POINT = 70,
            FRIEND3_POINT = 71,
        };

        static inline uint32_t GetWazaMachineNo(uint16_t item) {
            return external<uint32_t>(0x0249d4e0, item);
        }

        static inline int32_t GetParam(uint16_t itemno, PrmID prmID) {
            return external<int32_t>(0x0249b950, itemno, prmID);
        }
    };
}
