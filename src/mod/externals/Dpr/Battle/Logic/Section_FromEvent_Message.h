#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/Section.h"
#include "externals/Dpr/Battle/Logic/StrParam.h"

namespace Dpr::Battle::Logic {
    struct Section_FromEvent_Message : ILClass<Section_FromEvent_Message, game::Address::Dpr_Battle_Logic_Section_FromEvent_Message_TypeInfo> {
        struct Description : ILClass<Description, game::Address::Dpr_Battle_Logic_Section_FromEvent_Message_Description_TypeInfo> {
            struct Fields {
                uint8_t pokeID;
                bool isDisplayTokuseiWindow;
                Dpr::Battle::Logic::StrParam::Object* message;
            };

            inline void ctor() {
                external<void>(game::Address::Dpr_Battle_Logic_Section_FromEvent_Message_Description_ctor, this);
            }
        };

        struct Result : ILClass<Result, game::Address::Dpr_Battle_Logic_Section_FromEvent_Message_Result_TypeInfo> {
            struct Fields {};

            inline void ctor() {
                external<void>(game::Address::Dpr_Battle_Logic_Section_FromEvent_Message_Result_ctor, this);
            }
        };

        struct Fields : Section::Fields {};

        inline void Execute(Result::Object* result, Description::Object** description) {
            external<void>(game::Address::Dpr_Battle_Logic_Section_FromEvent_Message_Execute, this, result, description);
        }
    };
}
