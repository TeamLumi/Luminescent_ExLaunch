#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "externals/Dpr/Battle/Logic/BTL_SICKCONT.h"
#include "externals/Pml/WazaData/WazaSick.h"

namespace Dpr::Battle::Logic {
    struct calc : ILClass<calc, game::Address::Dpr_Battle_Logic_calc_TypeInfo> {
        static inline uint32_t PERSONAL_GetParam(int32_t monsno, int32_t formno, int32_t paramID) {
            return external<uint32_t>(game::Address::Dpr_Battle_Logic_calc_PERSONAL_GetParam, monsno, formno, paramID);
        }

        static inline bool PERSONAL_IsEvoCancelPokemon(int32_t monsno, int32_t formno, uint8_t level) {
            return external<bool>(game::Address::Dpr_Battle_Logic_calc_PERSONAL_IsEvoCancelPokemon, monsno, formno, level);
        }

        static inline uint32_t RandRange(uint32_t min, uint32_t max) {
            return external<uint32_t>(game::Address::Dpr_Battle_Logic_calc_RandRange, min, max);
        }

        static inline uint32_t GetRand(uint32_t range) {
            return external<uint32_t>(game::Address::Dpr_Battle_Logic_calc_GetRand, range);
        }

        static inline BTL_SICKCONT::Object MakeDefaultPokeSickCont(Pml::WazaData::WazaSick sick, uint8_t causePokeID, bool isCantUseRand) {
            return external<BTL_SICKCONT::Object>(game::Address::Dpr_Battle_Logic_calc_MakeDefaultPokeSickCont, sick, causePokeID, isCantUseRand);
        }

        static inline bool IsOccurPer(uint32_t per) {
            return external<bool>(game::Address::Dpr_Battle_Logic_calc_IsOccurPer, per);
        }

        static inline uint32_t QuotMaxHP(BTL_POKEPARAM::Object* bpp, uint32_t denom, bool useBeforeGParam) {
            return external<uint32_t>(game::Address::Dpr_Battle_Logic_calc_QuotMaxHP, bpp, denom, useBeforeGParam);
        }

        static inline bool ITEM_IsMail(uint16_t item) {
            return external<bool>(game::Address::Dpr_Battle_Logic_calc_ITEM_IsMail, item);
        }

        static inline uint32_t QuotMaxHP_Zero(BTL_POKEPARAM::Object* bpp, uint32_t denom, bool useBeforeGParam) {
            return external<uint32_t>(game::Address::Dpr_Battle_Logic_calc_QuotMaxHP_Zero, bpp, denom, useBeforeGParam);
        }
    };
}
