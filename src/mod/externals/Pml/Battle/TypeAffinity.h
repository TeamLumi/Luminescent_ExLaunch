#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "externals/Pml/Battle/TypeAffinity.h"

namespace Pml::Battle {
    struct TypeAffinity : ILClass<TypeAffinity> {
        struct Fields {};

        enum class AffinityID : int32_t {
            TYPEAFF_0 = 0,
            TYPEAFF_1_64 = 1,
            TYPEAFF_1_32 = 2,
            TYPEAFF_1_16 = 3,
            TYPEAFF_1_8 = 4,
            TYPEAFF_1_4 = 5,
            TYPEAFF_1_2 = 6,
            TYPEAFF_1 = 7,
            TYPEAFF_2 = 8,
            TYPEAFF_4 = 9,
            TYPEAFF_8 = 10,
            TYPEAFF_16 = 11,
            TYPEAFF_32 = 12,
            TYPEAFF_64 = 13,
            TYPEAFF_MAX = 14,
            TYPEAFF_NULL = 14,
        };

        enum class AboutAffinityID : int32_t {
            NONE = 0,
            NORMAL = 1,
            ADVANTAGE = 2,
            DISADVANTAGE = 3,
        };
    };
}

