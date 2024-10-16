#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Pml/PokePara/InitialSpec.h"
#include "externals/Pml/WazaNo.h"

namespace Pml::PokePara {
    struct EggParam : ILClass<EggParam> {
        struct Fields {
            Pml::PokePara::InitialSpec::Object* spec;
            uint32_t wazaCount;
            Pml::WazaNo_array* wazano;
            uint32_t ballID;
            System::Boolean_array* isTalentDerivedFromFather;
            System::Boolean_array* isTalentDerivedFromMother;
            uint32_t derivedTalentCount;
            uint32_t totalDeriveTalentCount;
        };

        static_assert(offsetof(Fields, totalDeriveTalentCount) == 0x34);
    };
}
