#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BTL_CLIENT.h"
#include "externals/Dpr/Battle/Logic/MainModule.h"

namespace Dpr::Battle::View::Systems {
    struct BTLV_WAZA_EFF_PARAM : ILStruct<BTLV_WAZA_EFF_PARAM> {
        #pragma pack(push, 4)
        struct Fields {
            uint8_t m_atkPos;
            uint8_t m_defPos;
            int32_t m_id;
            int32_t m_range;
            int32_t m_turnType;
            int32_t m_continueCount;
            bool m_syncDamageEffect;
            int32_t m_damageAbout;
            bool m_isGShockOccur;
            bool m_isGShortWaza;
            bool m_isDefGPoke;
            bool m_setData;
            int32_t m_frame;
            float m_elaspedTime;
            bool m_enableKeyAdjust;
            bool m_isPlayMotion;
            int32_t m_keyDiffFrame;
            int32_t m_attackMotion;
            int32_t m_startFrame;
        };
        #pragma pack(pop)

        static_assert(offsetof(Fields, m_startFrame) == 0x34);
        static_assert(sizeof(Fields) == 0x38);
    };
}
