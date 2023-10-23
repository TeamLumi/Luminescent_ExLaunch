#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BTL_SICKCONT.h"
#include "externals/Dpr/Battle/Logic/Section.h"
#include "externals/Dpr/Battle/Logic/SickCause.h"
#include "externals/Dpr/Battle/Logic/SickOverWriteMode.h"
#include "externals/Pml/WazaData/WazaSick.h"

namespace Dpr::Battle::Logic {
    struct Section_AddSick : ILClass<Section_AddSick, 0x04c5b890> {
        struct Description : ILClass<Description, 0x04c5adb8> {
            struct Fields {
                uint8_t pokeID;
                uint8_t targetPokeID;
                Pml::WazaData::WazaSick sickID;
                BTL_SICKCONT::Object sickCont;
                SickCause sickCause;
                SickOverWriteMode overWriteMode;
                bool isDisplayTokuseiWindow;
                bool isFailResultDisplay;
                bool isEffectDisplay;
                bool isStandardMessageDisable;
                bool isItemReactionDisable;
                bool isEffectiveToDeadPoke;
                void* specialMessage; // Dpr_Battle_Logic_StrParam_o*
            };

            inline void ctor() {
                external<void>(0x020dbd00, this);
            }
        };

        struct Fields : Section::Fields {};
    };
}
