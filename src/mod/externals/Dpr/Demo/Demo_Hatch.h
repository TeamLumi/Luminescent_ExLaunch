#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Demo/DemoBase.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/System/Primitives.h"
#include "externals/XLSXContent/PokemonInfo.h"

namespace Dpr::Demo {
    struct Demo_Hatch : ILClass<Demo_Hatch> {
        struct Fields : Dpr::Demo::DemoBase::Fields {
            void* timeLine; // Dpr_SubContents_TimeLineBinder_o*
            Pml::PokePara::PokemonParam::Object* param;
            float waitTime;
            float pokeRoarAnimTime;
            bool isCloseHatchMsg;
            bool isManafy;
            void* receiver; // Dpr_SubContents_MarkerReceiver_o*
        };

        struct DisplayClass9_0 : ILClass<DisplayClass9_0> {
            struct Fields {
                Demo_Hatch::Object* __4__this;
                XLSXContent::PokemonInfo::SheetCatalog::Object* tamagoCatalog;
            };

            static_assert(offsetof(Fields, tamagoCatalog) == 0x8);
            static_assert(sizeof(Fields) == 0x10);
        };

        static_assert(offsetof(Fields, receiver) == 0xD0);
        static_assert(sizeof(Fields) == 0xD8);
    };
}
