#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/Section.h"
#include "externals/Dpr/Battle/Logic/StrParam.h"

namespace Dpr::Battle::Logic {
    struct Section_FromEvent_FormChange : ILClass<Section_FromEvent_FormChange> {
        struct Description : ILClass<Description, game::Address::Dpr_Battle_Logic_Section_FromEvent_FormChange_Description_TypeInfo> {
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
                external<void>(game::Address::Dpr_Battle_Logic_Section_FromEvent_FormChange_Description_ctor, this);
            }
        };

        struct Result : ILClass<Result, game::Address::Dpr_Battle_Logic_Section_FromEvent_FormChange_Result_TypeInfo> {
            struct Fields {
                bool isChanged;
            };

            inline void ctor() {
                external<void>(game::Address::Dpr_Battle_Logic_Section_FromEvent_FormChange_Result_ctor, this);
            }
        };

        inline void Execute(Result::Object* result, Description::Object** description) {
            external<void>(game::Address::Dpr_Battle_Logic_Section_FromEvent_FormChange_Execute, this, result, description);
        }
    };
}
