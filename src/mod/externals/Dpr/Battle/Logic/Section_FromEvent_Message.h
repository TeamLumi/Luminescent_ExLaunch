#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/Section.h"
#include "externals/Dpr/Battle/Logic/StrParam.h"

namespace Dpr::Battle::Logic {
    struct Section_FromEvent_Message : ILClass<Section_FromEvent_Message, 0x04c5b998> {
        struct Description : ILClass<Description, 0x04c5afe8> {
            struct Fields {
                uint8_t pokeID;
                bool isDisplayTokuseiWindow;
                Dpr::Battle::Logic::StrParam::Object* message;
            };

            inline void ctor() {
                external<void>(0x01fb6020, this);
            }
        };

        struct Result : ILClass<Result, 0x04c5ae08> {
            struct Fields {};

            inline void ctor() {
                external<void>(0x01fb60a0, this);
            }
        };

        struct Fields : Section::Fields {};

        inline void Execute(Result::Object* result, Description::Object** description) {
            external<void>(0x01fb5f40, this, result, description);
        }
    };
}
