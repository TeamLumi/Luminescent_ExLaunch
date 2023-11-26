#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BTL_FIELD_SITUATION.h"
#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "externals/Dpr/Battle/Logic/BtlGround.h"
#include "externals/Dpr/Battle/Logic/EventFactor.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/Section_AddSick.h"
#include "externals/Dpr/Battle/Logic/Section_CureSick.h"
#include "externals/Dpr/Battle/Logic/Section_FieldEffect_Add.h"
#include "externals/Dpr/Battle/Logic/Section_FromEvent_ChangePokeType.h"
#include "externals/Dpr/Battle/Logic/Section_FromEvent_ConsumeItem.h"
#include "externals/Dpr/Battle/Logic/Section_FromEvent_PlayWazaEffect.h"
#include "externals/Dpr/Battle/Logic/Section_FromEvent_RankEffect.h"
#include "externals/Dpr/Battle/Logic/Section_FromEvent_SetWazaEffectEnable.h"
#include "externals/Dpr/Battle/Logic/Section_FromEvent_SetWazaEffectIndex.h"
#include "externals/Dpr/Battle/Logic/Section_FromEvent_Shrink.h"
#include "externals/Dpr/Battle/Logic/Section_SideEffect_Add.h"
#include "externals/Dpr/Battle/Logic/WazaFailCause.h"
#include "externals/Dpr/Battle/Logic/WorkValue.h"
#include "externals/Pml/PokePara/Sick.h"

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

        static inline bool AddSick(EventFactor::EventHandlerArgs::Object** args, Section_AddSick::Description::Object** desc) {
            return external<bool>(0x01d0aa80, args, desc);
        }

        static inline uint16_t GetSubID(EventFactor::EventHandlerArgs::Object** args) {
            return external<uint16_t>(0x01d09cf0, args);
        }

        static inline uint8_t GetFrontPokeID(EventFactor::EventHandlerArgs::Object** args, uint8_t* pos) {
            return external<uint8_t>(0x01d0a110, args, pos);
        }

        static inline bool IsFriendPokeID(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID1, uint8_t pokeID2) {
            return external<bool>(0x01d0a040, args, pokeID1, pokeID2);
        }

        static inline bool CureSick(EventFactor::EventHandlerArgs::Object** args, Section_CureSick::Description::Object** desc) {
            return external<bool>(0x01d0a960, args, desc);
        }

        static inline BTL_PARTY::Object* GetPartyData(EventFactor::EventHandlerArgs::Object** args, int32_t clientID) {
            return external<BTL_PARTY::Object*>(0x01d0f030, args, clientID);
        }

        static inline BTL_PARTY::Object* GetPartyData(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
            return external<BTL_PARTY::Object*>(0x01d0f050, args, pokeID);
        }

        static inline uint8_t DecideWazaTargetAuto(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID, int32_t wazano) {
            return external<uint8_t>(0x01d0d090, args, pokeID, wazano);
        }

        static inline Pml::PokePara::Sick CheckPokeSick(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
            return external<Pml::PokePara::Sick>(0x01d0eb40, args, pokeID);
        }

        static inline BtlGround GetGround(EventFactor::EventHandlerArgs::Object** args) {
            return external<BtlGround>(0x01d0a060, args);
        }

        static inline BTL_FIELD_SITUATION::Object* GetFieldSituation(EventFactor::EventHandlerArgs::Object** args) {
            return external<BTL_FIELD_SITUATION::Object*>(0x01d0a080, args);
        }

        static inline bool Shrink(EventFactor::EventHandlerArgs::Object** args, Section_FromEvent_Shrink::Description::Object** desc) {
            return external<bool>(0x01d0c3a0, args, desc);
        }

        static inline void PlayWazaEffect(EventFactor::EventHandlerArgs::Object** args, Section_FromEvent_PlayWazaEffect::Description::Object** desc) {
            external<void>(0x01d0d300, args, desc);
        }

        static inline uint8_t GetExistFrontPokePos(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
            return external<uint8_t>(0x01d0f1e0, args, pokeID);
        }

        static inline void SetWazaEffectIndex(EventFactor::EventHandlerArgs::Object** args, Section_FromEvent_SetWazaEffectIndex::Description::Object** desc) {
            external<void>(0x01d0cb40, args, desc);
        }

        static inline void SetWazaEffectEnable(EventFactor::EventHandlerArgs::Object** args, Section_FromEvent_SetWazaEffectEnable::Description::Object** desc) {
            external<void>(0x01d0cbc0, args, desc);
        }

        static inline bool ChangeType(EventFactor::EventHandlerArgs::Object** args, Section_FromEvent_ChangePokeType::Description::Object** desc) {
            return external<bool>(0x01d0b050, args, desc);
        }

        static inline bool AddFieldEffect(EventFactor::EventHandlerArgs::Object** args, Section_FieldEffect_Add::Description::Object** desc) {
            return external<bool>(0x01d0b730, args, desc);
        }

        static inline void MulEventVar(EventFactor::EventHandlerArgs::Object** args, EventVar::Label label, int32_t value) {
            external<void>(0x01d09cc0, args, label, value);
        }

        static inline bool CheckItemUsable(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
            return external<bool>(0x01d0f270, args, pokeID);
        }

        static inline bool RewriteEventVar_FAIL_CAUSE(EventFactor::EventHandlerArgs::Object** args, WazaFailCause value) {
            return external<bool>(0x01d09ca0, args, value);
        }

        static inline void ConsumeItem(EventFactor::EventHandlerArgs::Object** args, Section_FromEvent_ConsumeItem::Description::Object** desc) {
            external<void>(0x01d0bfe0, args, desc);
        }

        static inline bool AddSideEffect(EventFactor::EventHandlerArgs::Object** args, Section_SideEffect_Add::Description::Object** desc) {
            return external<bool>(0x01d0b4b0, args, desc);
        }

        static inline int32_t PokeIDtoSide(EventFactor::EventHandlerArgs::Object** args, uint8_t* pokeID) {
            return external<int32_t>(0x01d09f50, args, pokeID);
        }
    };
}