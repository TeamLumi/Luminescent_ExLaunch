#pragma once

#include "externals/il2cpp-api.h"

#include "externals/DPData/MYSTATUS.h"
#include "externals/System/String.h"

namespace Pml::PokePara {
    struct OwnerInfo : ILClass<OwnerInfo, 0x04c5fbe8> {
        struct Fields {
            uint32_t trainerId;
            uint8_t sex;
            uint8_t langID;
            System::String::Object* name;
        };

        inline void ctor(DPData::MYSTATUS::Object* ownerStatus) {
            external<void>(0x02054ea0, this, ownerStatus);
        }
    };
}