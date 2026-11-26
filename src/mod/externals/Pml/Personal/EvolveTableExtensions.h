#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Pml/Personal/EvolveCond.h"
#include "externals/XLSXContent/EvolveTable.h"

namespace Pml::Personal {
    struct EvolveTableExtensions : ILClass<EvolveTableExtensions> {
        static inline Pml::Personal::EvolveCond GetEvolutionCondition(XLSXContent::EvolveTable::SheetEvolve::Object* self, int32_t index) {
            return external<Pml::Personal::EvolveCond>(game::Address::Pml_Personal_EvolveTableExtensions_GetEvolutionCondition, self, index);
        }

        static inline uint16_t GetEvolutionParam(XLSXContent::EvolveTable::SheetEvolve::Object* self, int32_t index) {
            return external<uint16_t>(game::Address::Pml_Personal_EvolveTableExtensions_GetEvolutionParam, self, index);
        }

        static inline int32_t GetEvolvedMonsNo(XLSXContent::EvolveTable::SheetEvolve::Object* self, int32_t index) {
            return external<uint32_t>(game::Address::Pml_Personal_EvolveTableExtensions_GetEvolvedMonsNo, self, index);
        }

        static inline uint8_t GetEvolveEnableLevel(XLSXContent::EvolveTable::SheetEvolve::Object* self, int32_t index) {
            return external<uint8_t>(game::Address::Pml_Personal_EvolveTableExtensions_GetEvolveEnableLevel, self, index);
        }
    };
}