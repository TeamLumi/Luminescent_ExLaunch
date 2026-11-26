#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/Section.h"
#include "externals/Dpr/Battle/Logic/StrParam.h"
#include "externals/Dpr/Battle/Logic/WazaSickEx.h"
#include "externals/System/Primitives.h"

namespace Dpr::Battle::Logic {
    struct Section_CureSick : ILClass<Section_CureSick, game::Address::Dpr_Battle_Logic_Section_CureSick_TypeInfo> {
        struct Description : ILClass<Description, game::Address::Dpr_Battle_Logic_Section_CureSick_Description_TypeInfo> {
            struct Fields {
                uint8_t pokeID;
                WazaSickEx sick;
                System::Byte_array* targetPokeID;
                uint8_t targetPokeCount;
                uint16_t itemID;
                bool isDisplayTokuseiWindow;
                bool isStandardMessageDisable;
                StrParam::Object* successSpMessage;
            };

            inline void ctor() {
                external<void>(game::Address::Dpr_Battle_Logic_Section_CureSick_Description_ctor, this);
            }
        };

        struct Fields : Section::Fields {};
    };
}
