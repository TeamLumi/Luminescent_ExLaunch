#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Pml/PokePara/CoreParam.h"
#include "externals/Pml/PokePara/EggParam.h"
#include "externals/Pml/PokePara/Parents.h"
#include "externals/System/MulticastDelegate.h"

namespace Pml::PokePara {
    struct EggGenerator : ILClass<EggGenerator> {
        struct SavedRandomFunc : ILClass<SavedRandomFunc> {
            struct Fields : System::MulticastDelegate::Fields {};
        };

        struct Setting : ILClass<Setting> {
            struct Fields {
                uint32_t IDNo;
                System::String::Object* parentName;
                bool haveItem_HIKARUOMAMORI;
                SavedRandomFunc::Object* randFunc;
                uint8_t realRegionFormNo;
            };

            static_assert(offsetof(Fields, realRegionFormNo) == 0x20);
        };

        struct Fields {};

        static inline void SetupEggParam(CoreParam::Object* egg, EggParam::Object* egg_param, Parents::Object* parents, Setting::Object* setting) {
            external<void>(0x0204e970, egg, egg_param, parents, setting);
        }
    };
}
