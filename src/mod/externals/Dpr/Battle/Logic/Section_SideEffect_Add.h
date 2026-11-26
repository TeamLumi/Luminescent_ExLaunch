#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BTL_SICKCONT.h"
#include "externals/Dpr/Battle/Logic/BtlSideEffect.h"
#include "externals/Dpr/Battle/Logic/Section.h"
#include "externals/Dpr/Battle/Logic/StrParam.h"

namespace Dpr::Battle::Logic {
    struct Section_SideEffect_Add : ILClass<Section_SideEffect_Add, game::Address::Dpr_Battle_Logic_Section_SideEffect_Add_TypeInfo> {
        struct Description : ILClass<Description, game::Address::Dpr_Battle_Logic_Section_SideEffect_Add_Description_TypeInfo> {
            struct Fields {
                uint8_t pokeID;
                BtlSideEffect effect;
                BTL_SICKCONT::Object cont;
                int32_t side;
                uint16_t successEffectNo;
                StrParam::Object* successMessage;
                bool isReplaceSuccessMessageArgs0ByExpandSide;
            };

            inline void ctor() {
                external<void>(game::Address::Dpr_Battle_Logic_Section_SideEffect_Add_Description_ctor, this);
            }
        };

        struct Result : ILClass<Result, game::Address::Dpr_Battle_Logic_Section_SideEffect_Add_Result_TypeInfo> {
            struct Fields {
                bool isSuccessed;
            };

            inline void ctor() {
                external<void>(game::Address::Dpr_Battle_Logic_Section_SideEffect_Add_Result_ctor, this);
            }
        };

        struct Fields : Section::Fields {};
    };
}
