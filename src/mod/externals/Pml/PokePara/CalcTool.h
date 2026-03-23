#pragma once

#include "externals/il2cpp-api.h"

namespace Pml::PokePara {
    struct CalcTool : ILClass<CalcTool, 0x04c64410> {
        static inline uint8_t GetArceusType(uint32_t itemno) {
            return external<uint8_t>(0x024ae710, itemno);
        }

        static inline bool IsRareColor(uint32_t id, uint32_t rnd) {
            return external<bool>(0x024ad790, id, rnd);
        }

        static inline uint32_t CorrectColorRndForRareType(uint32_t id, uint32_t rnd, uint8_t type) {
            return external<uint32_t>(0x024adce0, id, rnd, type);
        }

        static inline bool IsSeikakuHigh(int32_t seikaku) {
            return external<bool>(0x024adf10, seikaku);
        }

        static inline bool IsSeikakuLow(int32_t seikaku) {
            return external<bool>(0x024ae000, seikaku);
        }

        static inline uint16_t GetTokuseiNo(int32_t monsno, uint16_t formno, uint8_t tokuseiIndex) {
            return external<uint16_t>(0x024ae1f0, monsno, formno, tokuseiIndex);
        }
    };
}
