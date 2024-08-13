#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/DamageCause.h"
#include "externals/Dpr/Battle/Logic/Section.h"
#include "externals/Dpr/Battle/Logic/StrParam.h"

namespace Dpr::Battle::Logic {
    struct Section_FromEvent_Damage : ILClass<Section_FromEvent_Damage, 0x04c5b8f8> {
        enum class EffectPlayMode : uint8_t {
            DISABLE = 0,
            ENABLE = 1,
            FORCE = 2,
        };

        struct Description : ILClass<Description, 0x04c5b1d8> {
            struct Fields {
                uint8_t pokeID;
                uint8_t targetPokeID;
                uint16_t damage;
                DamageCause damageCause;
                uint8_t damageCausePokeID;
                bool canHidePokeAvoid;
                bool disableDeadProcess;
                bool isDisplayTokuseiWindow;
                EffectPlayMode exEffectPlayMode;
                uint16_t exEffectNo;
                uint8_t exEffectPos_from;
                uint8_t exEffectPos_to;
                StrParam::Object* successMessage;
            };

            inline void ctor() {
                external<void>(0x01fb32c0, this);
            }
        };

        struct Result : ILClass<Result, 0x04c5ad80> {
            struct Fields {
                bool isSuccessed;
            };

            inline void ctor() {
                external<void>(0x01fb3350, this);
            }
        };

        struct Fields : Section::Fields {};
    };
}
