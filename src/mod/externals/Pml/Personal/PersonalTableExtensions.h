#pragma once

#include "externals/il2cpp-api.h"

#include "externals/XLSXContent/PersonalTable.h"

namespace Pml::Personal {
    struct PersonalTableExtensions : ILClass<PersonalTableExtensions, game::Address::Pml_PokePara_CalcTool_TypeInfo> {
        struct Fields {};

        static inline uint32_t GetParam(XLSXContent::PersonalTable::SheetPersonal::Object* data, int32_t paramId) {
            return external<uint32_t>(game::Address::Pml_Personal_PersonalTableExtensions_GetParam, data, paramId);
        }
    };
}
