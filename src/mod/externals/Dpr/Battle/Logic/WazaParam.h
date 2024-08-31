#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/PokeTypePair.h"

namespace Dpr::Battle::Logic {
    struct WazaParam : ILClass<WazaParam, 0x04c5ab90> {
        struct Fields {
            int32_t wazaID;
            int32_t orgWazaID;
            int32_t gSrcWazaID;
            PokeTypePair::Object userType;
            uint8_t wazaType;
            int32_t damageType;
            bool touchFlag;
            int32_t targetType;
            int32_t wazaPri;
            uint16_t wazaPower;
            bool fMagicCoat;
            bool fYokodori;
            bool fItazuraGokoro;
            bool fInvalidMessageDisable;
        };

        inline void ctor() {
            external<void>(0x01f73610, this);
        }

        inline void CopyFrom(WazaParam::Object* src) {
            external<void>(0x01f734c0, this, src);
        }
    };
}
