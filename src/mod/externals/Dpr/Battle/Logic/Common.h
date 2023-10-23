#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "externals/Dpr/Battle/Logic/EventFactor.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/Section_FromEvent_RankEffect.h"
#include "externals/Dpr/Battle/Logic/WorkValue.h"

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

        static inline int32_t GetWorkValue(EventFactor::EventHandlerArgs::Object** args, WorkValue workIdx) {
            return external<int32_t>(0x01d09d20, args, workIdx);
        }

        static inline void SetWorkValue(EventFactor::EventHandlerArgs::Object** args, WorkValue workIdx, int32_t value) {
            external<void>(0x01d09da0, args, workIdx, value);
        }

        static inline bool CheckShowDown(EventFactor::EventHandlerArgs::Object** args) {
            return external<bool>(0x01d0efc0, args);
        }

        static inline bool RankEffect(EventFactor::EventHandlerArgs::Object** args, Section_FromEvent_RankEffect::Description::Object** desc) {
            return external<bool>(0x01d0ac60, args, desc);
        }
    };
}