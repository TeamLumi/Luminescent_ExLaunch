#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Battle::Logic {
    struct Exp : ILClass<Exp> {
        struct CalcParam : ILClass<CalcParam, game::Address::Dpr_Battle_Logic_Exp_CalcParam_TypeInfo> {
            struct Fields {
                int32_t competitor;
                uint32_t playerLanguageId;
                uint32_t levelCap;
                uint32_t deadPokeExp;
                uint32_t deadPokeLevel;
                uint32_t getPokeLevel;
                uint32_t getPokeFriendship;
                uint32_t getPokeLanguageId;
                bool isMatch;
                bool isGakusyuSoutiOn;
                bool haveSiawasetamago;
                bool isEvoCanceledPoke;
                bool isMyPoke;
            };

            inline void ctor() {
                external<void>(game::Address::System_Object_ctor, this);
            }
        };

        struct CalcResult : ILClass<CalcResult, game::Address::Dpr_Battle_Logic_Exp_CalcResult_TypeInfo> {
            struct Fields {
                uint32_t exp;
                bool byGakusyuSouti;
                bool isContainBonus;
            };

            inline void ctor() {
                external<void>(game::Address::System_Object_ctor, this);
            }
        };

        static inline void CalcExp(Dpr::Battle::Logic::Exp::CalcParam** param, Dpr::Battle::Logic::Exp::CalcResult* result) {
            external<void>(game::Address::Dpr_Battle_Logic_Exp_CalcExp, param, result);
        }
    };
}
