#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "externals/Dpr/Battle/Logic/EventFactor.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/Section_FromEvent_SetItem.h"
#include "externals/Dpr/Battle/Logic/Section_FromEvent_SwapItem.h"
#include "externals/Dpr/Battle/Logic/Section_FromEvent_SetWazaEffectIndex.h"
#include "externals/Dpr/Battle/Logic/Section_FromEvent_FormChange.h"

namespace Dpr::Battle::Logic {
    struct Common : ILClass<Common, 0x04c5ad48> {
        static inline int32_t GetEventVar(EventFactor::EventHandlerArgs::Object** args, EventVar::Label label) {
            return external<int32_t>(0x01d09be0, args, label);
        }

        static inline BTL_POKEPARAM::Object* GetPokeParam(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
            return external<BTL_POKEPARAM::Object*>(0x01d09e40, args, pokeID);
        }

        static inline bool RewriteEventVar(EventFactor::EventHandlerArgs::Object** args, EventVar::Label label, int32_t value) {
            return external<bool>(0x01d09c90, args, label, value);
        }

        static inline int32_t GetCompetitor(EventFactor::EventHandlerArgs::Object** args) {
            return external<int32_t>(0x01d0a1f0, args);
        }

        static inline bool Dorobou_CheckEnable(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
            return external<bool>(0x01d0e720, args, pokeID);
        }

        static inline bool CheckCantStealPoke(EventFactor::EventHandlerArgs::Object** args, uint8_t attackPokeID, uint8_t targetPokeID) {
            return external<bool>(0x01d0da30, args, attackPokeID, targetPokeID);
        }

        static inline bool SetItem(EventFactor::EventHandlerArgs::Object** args, Section_FromEvent_SetItem::Description::Object** desc) {
            return external<bool>(0x01d0bd20, args, desc);
        }

        static inline bool SwapItem(EventFactor::EventHandlerArgs::Object** args, Section_FromEvent_SwapItem::Description::Object** desc) {
            return external<bool>(0x01d0bdb0, args, desc);
        }

        static inline void SetWazaEffectIndex(EventFactor::EventHandlerArgs::Object** args, Section_FromEvent_SetWazaEffectIndex::Description::Object** desc) {
            external<void>(0x01d0cb40, args, desc);
        }

        static inline void FormChange(EventFactor::EventHandlerArgs::Object** args, Section_FromEvent_FormChange::Description::Object** desc) {
            external<void>(0x01d0cab0, args, desc);
        }
    };
}