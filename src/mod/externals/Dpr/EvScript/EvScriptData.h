#pragma once

#include "externals/il2cpp-api.h"

#include "externals/EvData.h"

namespace Dpr::EvScript {
    struct EvScriptData : ILClass<EvScriptData> {
        struct Fields {
            EvData::Object* _EvData;
            int32_t LabelIndex;
            int32_t CommandIndex;
            int32_t RetIndex;
        };

        inline EvData::Script::Object* get_GetScript() {
            return external<EvData::Script::Object*>(game::Address::Dpr_EvScript_EvScriptData_get_GetScript, this);
        }
    };
}
