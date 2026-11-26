#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/Primitives.h"

namespace Pml::PokePara {
    struct CalcTool : ILClass<CalcTool, game::Address::Pml_PokePara_CalcTool_TypeInfo> {
        static inline uint8_t GetArceusType(uint32_t itemno) {
            return external<uint8_t>(game::Address::Pml_PokePara_CalcTool_GetAruseusuType, itemno);
        }

        static inline bool IsRareColor(uint32_t id, uint32_t rnd) {
            return external<bool>(game::Address::Pml_PokePara_CalcTool_IsRareColor, id, rnd);
        }

        static inline uint32_t CorrectColorRndForRareType(uint32_t id, uint32_t rnd, uint8_t type) {
            return external<uint32_t>(game::Address::Pml_PokePara_CalcTool_CorrectColorRndForRareType, id, rnd, type);
        }

        static inline bool IsSeikakuHigh(int32_t seikaku) {
            return external<bool>(game::Address::Pml_PokePara_CalcTool_IsSeikakuHigh, seikaku);
        }

        static inline bool IsSeikakuLow(int32_t seikaku) {
            return external<bool>(game::Address::Pml_PokePara_CalcTool_IsSeikakuLow, seikaku);
        }

        static inline System::Int32_array* GetSeikakuHigh(uint8_t* pNum) {
            return external<System::Int32_array*>(game::Address::Pml_PokePara_CalcTool_GetSeikakuHigh, pNum);
        }

        static inline System::Int32_array* GetSeikakuLow(uint8_t* pNum) {
            return external<System::Int32_array*>(game::Address::Pml_PokePara_CalcTool_GetSeikakuLow, pNum);
        }

        static inline uint16_t GetTokuseiNo(int32_t monsno, uint16_t formno, uint8_t tokuseiIndex) {
            return external<uint16_t>(game::Address::Pml_PokePara_CalcTool_GetTokuseiNo, monsno, formno, tokuseiIndex);
        }
    };
}
