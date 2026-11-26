#pragma once

#include "externals/il2cpp-api.h"

#include "externals/DPData/MYSTATUS.h"
#include "externals/System/String.h"

namespace Pml::PokePara {
    struct OwnerInfo : ILClass<OwnerInfo, game::Address::Pml_PokePara_OwnerInfo_TypeInfo> {
        struct Fields {
            uint32_t trainerId;
            uint8_t sex;
            uint8_t langID;
            System::String::Object* name;
        };

        inline void ctor(DPData::MYSTATUS::Object* ownerStatus) {
            external<void>(game::Address::Pml_PokePara_OwnerInfo_ctor, this, ownerStatus);
        }
    };
}