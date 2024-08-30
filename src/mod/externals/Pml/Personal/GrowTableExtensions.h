#pragma once

#include "externals/il2cpp-api.h"

#include "externals/XLSXContent/GrowTable.h"

namespace Pml::Personal {
    struct GrowTableExtensions : ILClass<GrowTableExtensions, 0x04c64410> {
        struct Fields {};

        static inline uint32_t GetMinExp(XLSXContent::GrowTable::SheetData::Object* data, uint8_t level) {
            return external<uint32_t>(0x024a0880, data, level);
        }
    };
}
