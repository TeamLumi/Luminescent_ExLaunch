#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/Section.h"
#include "externals/Dpr/Battle/Logic/WazaParam.h"

namespace Dpr::Battle::Logic {
    struct Section_FromEvent_FreeFallStart : ILClass<Section_FromEvent_FreeFallStart, 0x04c5b948> {
        struct Description : ILClass<Description> {
            struct Fields {
                uint8_t attackerID;
                uint8_t targetID;
                WazaParam::Object* wazaParam;
            };

            inline void ctor() {
                external<void>(0x01fb4d50, this);
            }
        };

        struct Result : ILClass<Result, 0x04c5af90> {
            struct Fields {
                bool isSucceeded;
                bool isFailMessageDisplayed;
            };

            inline void ctor() {
                external<void>(0x01fb4d90, this);
            }
        };

        struct Fields : Section::Fields {};

        inline uint32_t getWeight(BTL_POKEPARAM::Object* poke) {
            return external<uint32_t>(0x01fb4860, this, poke);
        }

        inline void onMamoruSuccess(BTL_POKEPARAM::Object* attacker, BTL_POKEPARAM::Object* target, WazaParam::Object* wazaParam) {
            external<void>(0x01fb4990, this, attacker, target, wazaParam);
        }

        inline void setFreeFallSick(BTL_POKEPARAM::Object* attacker, BTL_POKEPARAM::Object* target) {
            external<void>(0x01fb4b30, this, attacker, target);
        }
    };
}
