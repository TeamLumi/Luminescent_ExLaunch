#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/DamageCause.h"
#include "externals/Dpr/Battle/Logic/Section.h"
#include "externals/Dpr/Battle/Logic/StrParam.h"

namespace Dpr::Battle::Logic {
    struct Section_FromEvent_Damage : ILClass<Section_FromEvent_Damage, game::Address::Dpr_Battle_Logic_Section_FromEvent_Damage_TypeInfo> {
        enum class EffectPlayMode : uint8_t {
            DISABLE = 0,
            ENABLE = 1,
            FORCE = 2,
        };

        struct Description : ILClass<Description, game::Address::Dpr_Battle_Logic_Section_FromEvent_Damage_Description_TypeInfo> {
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
                external<void>(game::Address::Dpr_Battle_Logic_Section_FromEvent_Damage_Description_ctor, this);
            }
        };

        struct Result : ILClass<Result, game::Address::Dpr_Battle_Logic_Section_FromEvent_Damage_Result_TypeInfo> {
            struct Fields {
                bool isSuccessed;
            };

            inline void ctor() {
                external<void>(game::Address::Dpr_Battle_Logic_Section_FromEvent_Damage_Result_ctor, this);
            }
        };

        struct Fields : Section::Fields {};
    };
}
