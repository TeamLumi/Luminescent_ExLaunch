#pragma once

#include "externals/il2cpp-api.h"
#include "externals/XLSXContent/ContestMasterDatas.h"

namespace Dpr::Contest {
    struct ContestDataModel : ILClass<ContestDataModel> {
        struct Fields {
            // TODO
        };

        struct DisplayClass72_0 : ILClass<DisplayClass72_0> {
            struct Fields {
                XLSXContent::ContestMasterDatas::SheetNPCData::Object* npcMDDatas;
            };
        };
    };
}
