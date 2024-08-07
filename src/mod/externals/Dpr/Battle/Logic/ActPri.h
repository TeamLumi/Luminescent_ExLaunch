#pragma once
#include "externals/il2cpp-api.h"

namespace Dpr::Battle::Logic {
    struct ActPri : ILClass<ActPri> {
        static inline uint32_t Make(uint8_t dominantPri, uint8_t operationPri, uint8_t wazaPri, uint8_t spPri,
                                    uint16_t agility) {
            return external<uint32_t>(0x01abd360, dominantPri, operationPri, wazaPri, spPri, agility);
        }
    };
}