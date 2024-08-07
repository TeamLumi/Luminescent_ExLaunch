#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/Section.h"
#include "externals/Dpr/Battle/Logic/PokeAction.h"
#include "externals/Dpr/Battle/Logic/StrParam.h"

namespace Dpr::Battle::Logic {
    struct Section_FromEvent_FormChange : ILClass<Section_FromEvent_FormChange> {
        struct Description : ILClass<Description, 0x04c5b2b8> {
            struct Fields {
                uint8_t pokeID;
                uint8_t formNo;
                bool isDontResetFormByOut;
                bool isEnableInCaseOfDead;
                bool isDisplayTokuseiWindow;
                bool isDisplayChangeEffect;
                Dpr::Battle::Logic::StrParam::Object* successMessage;
            };

            inline void ctor() {
                external<void>(0x01fb4570, this);
            }
        };

        struct Result : ILClass<Result> {
            struct Fields {
                bool isChanged;
            };
        };

        struct Fields : Dpr::Battle::Logic::Section::Fields {

        };
    };
}
