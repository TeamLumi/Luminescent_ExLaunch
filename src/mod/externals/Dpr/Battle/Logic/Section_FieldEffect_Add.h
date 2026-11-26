#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BTL_SICKCONT.h"
#include "externals/Dpr/Battle/Logic/Section.h"
#include "externals/Dpr/Battle/Logic/StrParam.h"

namespace Dpr::Battle::Logic {
    struct Section_FieldEffect_Add : ILClass<Section_FieldEffect_Add, game::Address::Dpr_Battle_Logic_Section_FieldEffect_Add_TypeInfo> {
        struct Description : ILClass<Description, game::Address::Dpr_Battle_Logic_Section_FieldEffect_Add_Description_TypeInfo> {
            struct Fields {
                uint8_t pokeID;
                int32_t effect;
                uint8_t ground;
                uint8_t ground_cause;
                BTL_SICKCONT::Object cont;
                uint16_t successEffectNo;
                StrParam::Object* successMessage;
                bool isAddDependPoke;
                bool isDisplayTokuseiWindow;
            };

            inline void ctor() {
                external<void>(game::Address::Dpr_Battle_Logic_Section_FieldEffect_Add_Description_ctor, this);
            }
        };

        struct Result : ILClass<Result, game::Address::Dpr_Battle_Logic_Section_FieldEffect_Add_Result_TypeInfo> {
            struct Fields {
                bool isSuccessed;
            };

            inline void ctor() {
                external<void>(game::Address::Dpr_Battle_Logic_Section_FieldEffect_Add_Result_ctor, this);
            }
        };

        struct Fields : Section::Fields {};
    };
}
