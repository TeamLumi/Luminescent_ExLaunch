#pragma once

#include "externals/il2cpp-api.h"

#include "externals/XLSXContent/GrowTable.h"

namespace Pml::Personal {
    struct GrowTableExtensions : ILClass<GrowTableExtensions, game::Address::Pml_PokePara_CalcTool_TypeInfo> {
        struct Fields {};

        static inline uint32_t GetMinExp(XLSXContent::GrowTable::SheetData::Object* data, uint8_t level) {
            return external<uint32_t>(game::Address::Pml_Personal_GrowTableExtensions_GetMinExp, data, level);
        }
    };
}
