#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "externals/Dpr/Battle/Logic/Section.h"

namespace Dpr::Battle::Logic {
    struct Section_FieldEffect_End : ILClass<Section_FieldEffect_End, game::Address::Dpr_Battle_Logic_Section_FieldEffect_End_TypeInfo> {
        struct Description : ILClass<Description, game::Address::Dpr_Battle_Logic_Section_FieldEffect_End_Description_TypeInfo> {
            struct Fields {
                int32_t effect;
                BTL_POKEPARAM::Object* pDependPoke;
            };

            inline void ctor() {
                external<void>(game::Address::Dpr_Battle_Logic_Section_FieldEffect_End_Description_ctor, this);
            }
        };

        struct Result : ILClass<Result, game::Address::Dpr_Battle_Logic_Section_FieldEffect_End_Result_TypeInfo> {
            struct Fields {
                bool isRemoved;
            };

            inline void ctor() {
                external<void>(game::Address::Dpr_Battle_Logic_Section_FieldEffect_End_Result_ctor, this);
            }
        };

        struct Fields : Section::Fields {};

        inline void checkItemReaction_All() {
            external<void>(game::Address::Dpr_Battle_Logic_Section_FieldEffect_End_checkItemReaction_All, this);
        }

        inline void resetGround() {
            external<void>(game::Address::Dpr_Battle_Logic_Section_FieldEffect_End_resetGround, this);
        }

        inline void resetKagakuhenkaGas() {
            external<void>(game::Address::Dpr_Battle_Logic_Section_FieldEffect_End_resetKagakuhenkaGas, this);
        }
    };
}
