#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/Section.h"
#include "externals/Dpr/Battle/Logic/StrParam.h"

namespace Dpr::Battle::Logic {
    struct Section_FromEvent_FormChange : ILClass<Section_FromEvent_FormChange, 0x04c5b940> {
        struct Description : ILClass<Description, 0x04c5b2b8> {
            struct Fields {
                uint8_t pokeID;
                uint8_t formNo;
                bool isDontResetFormByOut;
                bool isEnableInCaseOfDead;
                bool isDisplayTokuseiWindow;
                bool isDisplayChangeEffect;
                StrParam::Object* successMessage;
            };

            inline void ctor() {
                external<void>(0x01fb4570, this);
            }
        };

        struct Result : ILClass<Result, 0x04c5af68> {
            struct Fields {
                bool isChanged;
            };

            inline void ctor() {
                external<void>(0x01fb4600, this);
            }
        };

        struct Fields : Section::Fields {};
    };
}
