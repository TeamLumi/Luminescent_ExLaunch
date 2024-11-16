#pragma once

#include "externals/il2cpp-api.h"

namespace DPData {
    struct MV_POKE_DATA : ILStruct<MV_POKE_DATA, 0x04c5a478> {
        #pragma pack(push, 4)
        struct Fields {
            int32_t ZoneIDIndex;
            uint64_t RndSeed;
            uint32_t MonsNo;
            uint32_t Hp;
            uint8_t Lv;
            uint32_t Sick;
            uint8_t EncountStatus;
        };
        #pragma pack(pop)

        static_assert(offsetof(Fields, MonsNo) == 0xc);
        static_assert(offsetof(Fields, Lv) == 0x14);
        static_assert(sizeof(Fields) == 0x20);
    };
}
