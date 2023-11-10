#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/Primitives.h"

namespace Dpr::Battle::Logic {
    struct StrParam : ILClass<StrParam, 0x04c5b010> {
        struct Param : ILClass<Param, 0x04c5c8e8> {
            struct Fields {
                uint16_t ID;
                uint16_t type;
                uint16_t argCnt;
                bool fSEAdd;
                bool fFailMsg;
                System::Int32_array* args;
            };
        };

        struct Fields {
            StrParam::Param::Object* m_param;
        };
    };
}
