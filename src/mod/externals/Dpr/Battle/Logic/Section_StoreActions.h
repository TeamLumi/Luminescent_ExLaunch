#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/Section.h"
#include "externals/Dpr/Battle/Logic/Section_CommonParam.h"
#include "externals/Dpr/Battle/Logic/BTL_ACTION.h"

namespace Dpr::Battle::Logic {
    struct Section_StoreActions : ILClass<Section_StoreActions, 0x04c5bc70> {
        struct Fields : Dpr::Battle::Logic::Section::Fields {

        };
        inline void ctor(Section_CommonParam::Object** commonParam) {
            external<void>(0x021cd8b0, this, commonParam);
        }


        inline BTL_POKEPARAM::Object* getActionPoke(BTL_ACTION::PARAM::Object* clientInstruction, uint8_t clientID) {
            return external<BTL_POKEPARAM::Object*>(0x021cdea0, this, clientInstruction, clientID);
        }
    };
}
