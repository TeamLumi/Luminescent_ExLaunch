#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/String.h"

namespace Dpr::Battle::View {
    struct BTLV_WAZA_INFO : ILStruct<BTLV_WAZA_INFO> {
        struct Fields {
            uint8_t PokeType;
            int32_t WazaNo;
            System::String::Object* WazaName;
            uint16_t PP;
            uint16_t MaxPP;
            int32_t WazaDamageType;
            uint32_t Power;
            uint16_t Dex;
            System::String::Object* Description;
        };

        static_assert(offsetof(Fields, Description) == 0x20);
    };
}
