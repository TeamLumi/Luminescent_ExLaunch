#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Battle::Logic {
    struct EventVar : ILClass<EventVar> {
        struct Fields {
            uint16_t m_label;
            uint8_t m_rewriteState;
            int32_t m_value;
            int32_t m_mulMin;
            int32_t m_mulMax;
            Il2CppObject* m_address;
        };

        enum class Label : uint16_t {
            INVALID = 0,
            SYS_SEPARATE = 1,
            POKEID = 2,
            POKEID_ATK = 3,
            POKEID_DEF = 4,
            TARGET_POKECNT = 5,
            POKEID_TARGET1 = 6,
            POKEID_TARGET2 = 7,
            POKEID_TARGET3 = 8,
            POKEID_TARGET4 = 9,
            POKEID_TARGET5 = 10,
            POKEID_TARGET6 = 11,
            ACTION = 12,
            POKEPOS = 13,
            POKEPOS_ORG = 14,
            TOKUSEI_PREV = 15,
            TOKUSEI_NEXT = 16,
            SP_PRIORITY = 17,
            WAZAID = 18,
            EFFECT_WAZAID = 19,
            ORG_WAZAID = 20,
            POKE_TYPE = 21,
            WAZA_TYPE = 22,
            WAZA_TYPE_EX = 23,
            WAZA_IDX = 24,
            WAZA_PRI = 25,
            WAZA_SERIAL = 26,
            DAMAGE_TYPE = 27,
            TARGET_TYPE = 28,
            USER_TYPE = 29,
            SICKID = 30,
            SICK_CONT_ADRS = 31,
            SICK_CONT_ADRS_HIGH = 32,
            SICK_CONT_ADRS_LOW = 33,
            STATUS_TYPE = 34,
            VOLUME = 35,
            POKE_HIDE = 36,
            FAIL_CAUSE = 37,
            CONF_DMG = 38,
            TURN_COUNT = 39,
            ADD_DEFAULT_PER = 40,
            ADD_PER = 41,
            HIT_RANK = 42,
            AVOID_RANK = 43,
            HITCOUNT_MAX = 44,
            HITCOUNT = 45,
            HIT_PER = 46,
            CRITICAL_RANK = 47,
            ITEM = 48,
            ITEM_REACTION = 49,
            AGILITY = 50,
            WAZA_POWER = 51,
            WAZA_POWER_RATIO = 52,
            DAMAGE = 53,
            POWER = 54,
            GUARD = 55,
            RATIO = 56,
            RATIO_EX = 57,
            FIX_DAMAGE = 58,
            FIX_DAMAGE_FLAG = 59,
            ZERO_DAMAGE_FLAG = 60,
            TYPEAFF = 61,
            WEATHER = 62,
            KORAERU_CAUSE = 63,
            SWAP_POKEID = 64,
            VID = 65,
            VID_SWAP_CNT = 66,
            ENABLE_MODE = 67,
            WORK_ADRS = 68,
            WORK_ADRS_HIGH = 69,
            WORK_ADRS_LOW = 70,
            NOEFFECT_FLAG = 71,
            FAIL_FLAG = 72,
            AVOID_FLAG = 73,
            ALMOST_FLAG = 74,
            TYPEMATCH_FLAG = 75,
            CRITICAL_FLAG = 76,
            MIGAWARI_FLAG = 77,
            RINPUNGUARD_FLG = 78,
            TIKARAZUKU_FLG = 79,
            ITEMUSE_FLAG = 80,
            TRICK_FLAG = 81,
            FLAT_FLAG = 82,
            FLATMASTER_FLAG = 83,
            DELAY_ATTACK_FLAG = 84,
            MAGICCOAT_FLAG = 85,
            MSG_FLAG = 86,
            FIX_FLAG = 87,
            KINOMI_FLAG = 88,
            GEN_FLAG = 89,
            DISABLE_YAKEDO_FLAG = 90,
            SIDE = 91,
            SIDE_EFFECT = 92,
            AURA_BREAK_FLAG = 93,
            AURA_RESERVE_FLAG = 94,
            EFFECT_NO = 95,
            KINOMI_EFFECT_UP = 96,
            TEMPT_TARGET_PRIORITY = 97,
            TEMPT_TARGET_CAUSE = 98,
            WAZA_TOUCH_FLAG = 99,
            WALL_EFFECT_FLAG_PHYSIC = 100,
            WALL_EFFECT_FLAG_SPECIAL = 101,
            GROUND = 102,
            FIELD = 103,
            RANKEFFECT_CAUSE = 104,
            DAMAGE_CAUSE = 105,
            DAMAGE_CAUSE_POKEID = 106,
            SICK_CAUSE = 107,
            TOKUSEI_CHANGE_CAUSE = 108,
            ITAZURAGOKORO_EFFECT = 109,
            OIUTI_INTERRUPT_FLAG = 110,
            ACTION_DESC_IS_YOKODORI_ROB_ACTION = 111,
            ACTION_DESC_IS_MAGICCOAT_REACTION = 112,
            ACTION_DESC_IS_ODORIKO_REACTION = 113,
            ACTION_DESC_IS_OIUTI_INTERRUPT_ACTION = 114,
            ENABLE_FLAG = 115,
            REFLECT_FLAG = 116,
            MESSAGE_DISPLAYED = 117,
            TOKUSEI_WINDOW_DISPLAY = 118,
            CAN_AIM_TARGET = 119,
            SUCCESS_FLAG = 120,
            CHANGE_FLAG = 121,
            INVALID_MESSAGE_FLAG = 122,
            NEED_ITEM_CONSUME_MESSAGE_DISPLAY = 123,
            NUM = 124,
            MEGA_EVOLUTION = 125,
        };
    };
}