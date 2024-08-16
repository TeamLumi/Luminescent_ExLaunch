#pragma once

#include "externals/il2cpp-api.h"

#include "externals/DPData/MYSTATUS.h"
#include "externals/Pml/PokePara/CoreParam.h"
#include "externals/poketool/poke_memo/DataType.h"

namespace poketool::poke_memo {
    struct poketool_poke_memo : ILClass<poketool_poke_memo> {
        struct Fields {
            // TODO
        };

        static inline void SetFromCapture(Pml::PokePara::CoreParam::Object* pParam, DPData::MYSTATUS::Object* pMystatus, uint32_t placeNo) {
            external<void>(0x02bacb10, pParam, pMystatus, placeNo);
        }

        static inline void ClearPlaceTime(Pml::PokePara::CoreParam::Object* pParam, poketool::poke_memo::DataType type) {
            external<void>(0x02bacca0, pParam, type);
        }

        static inline void SetPlaceTime(Pml::PokePara::CoreParam::Object* pParam, uint32_t placeNo, poketool::poke_memo::DataType dataType) {
            external<void>(0x02baccc0, pParam, placeNo, dataType);
        }

        static inline void SetGetLevel(Pml::PokePara::CoreParam::Object* pParam) {
            external<void>(0x02bace70, pParam);
        }

        static inline void SetVersion(Pml::PokePara::CoreParam::Object* pParam) {
            external<void>(0x02baceb0, pParam);
        }
    };
}
