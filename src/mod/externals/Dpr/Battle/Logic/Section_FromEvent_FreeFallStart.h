#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/Section.h"
#include "externals/Dpr/Battle/Logic/WazaParam.h"

namespace Dpr::Battle::Logic {
    struct Section_FromEvent_FreeFallStart : ILClass<Section_FromEvent_FreeFallStart, game::Address::Dpr_Battle_Logic_Section_FromEvent_FreeFallStart_TypeInfo> {
        struct Description : ILClass<Description> {
            struct Fields {
                uint8_t attackerID;
                uint8_t targetID;
                WazaParam::Object* wazaParam;
            };

            inline void ctor() {
                external<void>(game::Address::Dpr_Battle_Logic_Section_FromEvent_FreeFallStart_Description_ctor, this);
            }
        };

        struct Result : ILClass<Result, game::Address::Dpr_Battle_Logic_Section_FromEvent_FreeFallStart_Result_TypeInfo> {
            struct Fields {
                bool isSucceeded;
                bool isFailMessageDisplayed;
            };

            inline void ctor() {
                external<void>(game::Address::Dpr_Battle_Logic_Section_FromEvent_FreeFallStart_Result_ctor, this);
            }
        };

        struct Fields : Section::Fields {};

        inline uint32_t getWeight(BTL_POKEPARAM::Object* poke) {
            return external<uint32_t>(game::Address::Dpr_Battle_Logic_Section_FromEvent_FreeFallStart_getWeight, this, poke);
        }

        inline void onMamoruSuccess(BTL_POKEPARAM::Object* attacker, BTL_POKEPARAM::Object* target, WazaParam::Object* wazaParam) {
            external<void>(game::Address::Dpr_Battle_Logic_Section_FromEvent_FreeFallStart_onMamoruSuccess, this, attacker, target, wazaParam);
        }

        inline void setFreeFallSick(BTL_POKEPARAM::Object* attacker, BTL_POKEPARAM::Object* target) {
            external<void>(game::Address::Dpr_Battle_Logic_Section_FromEvent_FreeFallStart_setFreeFallSick, this, attacker, target);
        }
    };
}
