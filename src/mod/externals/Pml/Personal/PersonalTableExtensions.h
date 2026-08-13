#pragma once

#include "externals/il2cpp-api.h"

#include "externals/XLSXContent/PersonalTable.h"

namespace Pml::Personal {
    struct PersonalTableExtensions : ILClass<PersonalTableExtensions, 0x04c64410> {
        struct Fields {};

        static inline uint32_t GetParam(XLSXContent::PersonalTable::SheetPersonal::Object* data, int32_t paramId) {
            return external<uint32_t>(0x024a0ff0, data, paramId);
        }
    };
}
