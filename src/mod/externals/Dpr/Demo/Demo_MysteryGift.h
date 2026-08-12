#pragma once

#include <externals/Dpr/SubContents/TimeLineBinder.h>

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Demo/DemoBase.h"

namespace Dpr::Demo {
    struct Demo_MysteryGift : ILClass<Demo_MysteryGift, 0x04c61c98> {
        struct Fields : Dpr::Demo::DemoBase::Fields {
            Dpr::SubContents::TimeLineBinder::Object* timeLine;
            Pml::PokePara::PokemonParam::Object* gift_Pokemon;
            bool isGetMons;
        };

        inline void ctor() {
            external<void>(0x01ada560, this);
        }
    };
}
