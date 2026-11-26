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
#include "externals/Dpr/Battle/Logic/Section_FreeFall_Release.h"
#include "externals/Dpr/Battle/Logic/Section_FromEvent_ChangePokeType.h"
#include "externals/Dpr/Battle/Logic/Section_FromEvent_ConsumeItem.h"
#include "externals/Dpr/Battle/Logic/Section_FromEvent_Damage.h"
#include "externals/Dpr/Battle/Logic/Section_FromEvent_FormChange.h"
#include "externals/Dpr/Battle/Logic/Section_FromEvent_FreeFallStart.h"
#include "externals/Dpr/Battle/Logic/Section_FromEvent_PlayWazaEffect.h"
#include "externals/Dpr/Battle/Logic/Section_FromEvent_RankEffect.h"
#include "externals/Dpr/Battle/Logic/Section_FromEvent_SetItem.h"
#include "externals/Dpr/Battle/Logic/Section_FromEvent_SetWazaEffectEnable.h"
#include "externals/Dpr/Battle/Logic/Section_FromEvent_SetWazaEffectIndex.h"
#include "externals/Dpr/Battle/Logic/Section_FromEvent_Shrink.h"
#include "externals/Dpr/Battle/Logic/Section_FromEvent_SwapItem.h"
#include "externals/Dpr/Battle/Logic/Section_SideEffect_Add.h"
#include "externals/Dpr/Battle/Logic/WazaFailCause.h"
#include "externals/Dpr/Battle/Logic/WorkValue.h"
#include "externals/Pml/PokePara/Sick.h"

namespace Dpr::Battle::Logic {
    struct Common : ILClass<Common, game::Address::Dpr_Battle_Logic_Common_TypeInfo> {
        static inline int32_t GetEventVar(EventFactor::EventHandlerArgs::Object** args, EventVar::Label label) {
            return external<int32_t>(game::Address::Dpr_Battle_Logic_Common_GetEventVar, args, label);
        }

        static inline BTL_POKEPARAM::Object* GetPokeParam(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
            return external<BTL_POKEPARAM::Object*>(game::Address::Dpr_Battle_Logic_Common_GetPokeParam, args, pokeID);
        }

        static inline bool RewriteEventVar(EventFactor::EventHandlerArgs::Object** args, EventVar::Label label, int32_t value) {
            return external<bool>(game::Address::Dpr_Battle_Logic_Common_RewriteEventVar, args, label, value);
        }

        static inline int32_t GetWorkValue(EventFactor::EventHandlerArgs::Object** args, WorkValue workIdx) {
            return external<int32_t>(game::Address::Dpr_Battle_Logic_Common_GetWorkValue, args, workIdx);
        }

        static inline void SetWorkValue(EventFactor::EventHandlerArgs::Object** args, WorkValue workIdx, int32_t value) {
            external<void>(game::Address::Dpr_Battle_Logic_Common_SetWorkValue, args, workIdx, value);
        }

        static inline bool CheckShowDown(EventFactor::EventHandlerArgs::Object** args) {
            return external<bool>(game::Address::Dpr_Battle_Logic_Common_CheckShowDown, args);
        }

        static inline bool RankEffect(EventFactor::EventHandlerArgs::Object** args, Section_FromEvent_RankEffect::Description::Object** desc) {
            return external<bool>(game::Address::Dpr_Battle_Logic_Common_RankEffect, args, desc);
        }

        static inline bool AddSick(EventFactor::EventHandlerArgs::Object** args, Section_AddSick::Description::Object** desc) {
            return external<bool>(game::Address::Dpr_Battle_Logic_Common_AddSick, args, desc);
        }

        static inline uint16_t GetSubID(EventFactor::EventHandlerArgs::Object** args) {
            return external<uint16_t>(game::Address::Dpr_Battle_Logic_Common_GetSubID, args);
        }

        static inline uint8_t GetFrontPokeID(EventFactor::EventHandlerArgs::Object** args, uint8_t* pos) {
            return external<uint8_t>(game::Address::Dpr_Battle_Logic_Common_GetFrontPokeID, args, pos);
        }

        static inline bool IsFriendPokeID(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID1, uint8_t pokeID2) {
            return external<bool>(game::Address::Dpr_Battle_Logic_Common_IsFriendPokeID, args, pokeID1, pokeID2);
        }

        static inline bool CureSick(EventFactor::EventHandlerArgs::Object** args, Section_CureSick::Description::Object** desc) {
            return external<bool>(game::Address::Dpr_Battle_Logic_Common_CureSick, args, desc);
        }

        static inline BTL_PARTY::Object* GetPartyData(EventFactor::EventHandlerArgs::Object** args, int32_t clientID) {
            return external<BTL_PARTY::Object*>(game::Address::Dpr_Battle_Logic_Common_GetPartyData_EventHandlerArgsPtrPtr_Int32, args, clientID);
        }

        static inline BTL_PARTY::Object* GetPartyData(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
            return external<BTL_PARTY::Object*>(game::Address::Dpr_Battle_Logic_Common_GetPartyData_EventHandlerArgsPtrPtr_UInt8, args, pokeID);
        }

        static inline uint8_t DecideWazaTargetAuto(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID, int32_t wazano) {
            return external<uint8_t>(game::Address::Dpr_Battle_Logic_Common_DecideWazaTargetAuto, args, pokeID, wazano);
        }

        static inline Pml::PokePara::Sick CheckPokeSick(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
            return external<Pml::PokePara::Sick>(game::Address::Dpr_Battle_Logic_Common_CheckPokeSick, args, pokeID);
        }

        static inline BtlGround GetGround(EventFactor::EventHandlerArgs::Object** args) {
            return external<BtlGround>(game::Address::Dpr_Battle_Logic_Common_GetGround, args);
        }

        static inline BTL_FIELD_SITUATION::Object* GetFieldSituation(EventFactor::EventHandlerArgs::Object** args) {
            return external<BTL_FIELD_SITUATION::Object*>(game::Address::Dpr_Battle_Logic_Common_GetFieldSituation, args);
        }

        static inline bool Shrink(EventFactor::EventHandlerArgs::Object** args, Section_FromEvent_Shrink::Description::Object** desc) {
            return external<bool>(game::Address::Dpr_Battle_Logic_Common_Shrink, args, desc);
        }

        static inline void PlayWazaEffect(EventFactor::EventHandlerArgs::Object** args, Section_FromEvent_PlayWazaEffect::Description::Object** desc) {
            external<void>(game::Address::Dpr_Battle_Logic_Common_PlayWazaEffect, args, desc);
        }

        static inline uint8_t GetExistFrontPokePos(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
            return external<uint8_t>(game::Address::Dpr_Battle_Logic_Common_GetExistFrontPokePos, args, pokeID);
        }

        static inline int32_t GetCompetitor(EventFactor::EventHandlerArgs::Object** args) {
            return external<int32_t>(game::Address::Dpr_Battle_Logic_Common_GetCompetitor, args);
        }

        static inline bool Dorobou_CheckEnable(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
            return external<bool>(game::Address::Dpr_Battle_Logic_Common_Dorobou_CheckEnable, args, pokeID);
        }

        static inline bool CheckCantStealPoke(EventFactor::EventHandlerArgs::Object** args, uint8_t attackPokeID, uint8_t targetPokeID) {
            return external<bool>(game::Address::Dpr_Battle_Logic_Common_CheckCantStealPoke, args, attackPokeID, targetPokeID);
        }

        static inline bool SetItem(EventFactor::EventHandlerArgs::Object** args, Section_FromEvent_SetItem::Description::Object** desc) {
            return external<bool>(game::Address::Dpr_Battle_Logic_Common_SetItem, args, desc);
        }

        static inline bool SwapItem(EventFactor::EventHandlerArgs::Object** args, Section_FromEvent_SwapItem::Description::Object** desc) {
            return external<bool>(game::Address::Dpr_Battle_Logic_Common_SwapItem, args, desc);
        }

        static inline void SetWazaEffectIndex(EventFactor::EventHandlerArgs::Object** args, Section_FromEvent_SetWazaEffectIndex::Description::Object** desc) {
            external<void>(game::Address::Dpr_Battle_Logic_Common_SetWazaEffectIndex, args, desc);
        }

        static inline void SetWazaEffectEnable(EventFactor::EventHandlerArgs::Object** args, Section_FromEvent_SetWazaEffectEnable::Description::Object** desc) {
            external<void>(game::Address::Dpr_Battle_Logic_Common_SetWazaEffectEnable, args, desc);
        }

        static inline bool ChangeType(EventFactor::EventHandlerArgs::Object** args, Section_FromEvent_ChangePokeType::Description::Object** desc) {
            return external<bool>(game::Address::Dpr_Battle_Logic_Common_ChangeType, args, desc);
        }

        static inline bool AddFieldEffect(EventFactor::EventHandlerArgs::Object** args, Section_FieldEffect_Add::Description::Object** desc) {
            return external<bool>(game::Address::Dpr_Battle_Logic_Common_AddFieldEffect, args, desc);
        }

        static inline void MulEventVar(EventFactor::EventHandlerArgs::Object** args, EventVar::Label label, int32_t value) {
            external<void>(game::Address::Dpr_Battle_Logic_Common_MulEventVar, args, label, value);
        }

        static inline bool CheckItemUsable(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
            return external<bool>(game::Address::Dpr_Battle_Logic_Common_CheckItemUsable, args, pokeID);
        }

        static inline bool RewriteEventVar_FAIL_CAUSE(EventFactor::EventHandlerArgs::Object** args, WazaFailCause value) {
            return external<bool>(game::Address::Dpr_Battle_Logic_Common_RewriteEventVar_FAIL_CAUSE, args, value);
        }

        static inline void ConsumeItem(EventFactor::EventHandlerArgs::Object** args, Section_FromEvent_ConsumeItem::Description::Object** desc) {
            external<void>(game::Address::Dpr_Battle_Logic_Common_ConsumeItem, args, desc);
        }

        static inline bool AddSideEffect(EventFactor::EventHandlerArgs::Object** args, Section_SideEffect_Add::Description::Object** desc) {
            return external<bool>(game::Address::Dpr_Battle_Logic_Common_AddSideEffect, args, desc);
        }

        static inline int32_t PokeIDtoSide(EventFactor::EventHandlerArgs::Object** args, uint8_t* pokeID) {
            return external<int32_t>(game::Address::Dpr_Battle_Logic_Common_PokeIDtoSide, args, pokeID);
        }

        static inline PokeAction::Object* SearchByPokeID(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID, bool isSkipGStart, bool isSkipNull) {
            return external<PokeAction::Object*>(game::Address::Dpr_Battle_Logic_Common_SearchByPokeID, args, pokeID, isSkipGStart, isSkipNull);
        }

        static inline bool Damage(EventFactor::EventHandlerArgs::Object** args, Section_FromEvent_Damage::Description::Object** desc) {
            return external<bool>(game::Address::Dpr_Battle_Logic_Common_Damage, args, desc);
        }

        static inline uint8_t GetAllOpponentFrontPokeID(EventFactor::EventHandlerArgs::Object** args, uint8_t basePokeID, System::Byte_array* dst) {
            return external<uint8_t>(game::Address::Dpr_Battle_Logic_Common_GetAllOpponentFrontPokeID, args, basePokeID, dst);
        }

        static inline bool FreeFallStart(EventFactor::EventHandlerArgs::Object** args, Section_FromEvent_FreeFallStart::Description::Object** desc) {
            return external<bool>(game::Address::Dpr_Battle_Logic_Common_FreeFallStart, args, desc);
        }

        static inline void FreeFallRelease(EventFactor::EventHandlerArgs::Object** args, Section_FreeFall_Release::Description::Object** desc) {
            external<void>(game::Address::Dpr_Battle_Logic_Common_FreeFallRelease, args, desc);
        }

        static inline bool CheckUnbreakablePokeItem(uint16_t monsno, uint16_t itemID) {
            return external<bool>(game::Address::Dpr_Battle_Logic_Common_CheckUnbreakablePokeItem, monsno, itemID);
        }

        static inline bool FormChange(EventFactor::EventHandlerArgs::Object** args, Section_FromEvent_FormChange::Description::Object** desc) {
            return external<bool>(game::Address::Dpr_Battle_Logic_Common_FormChange, args, desc);
        }

        static inline bool GuardWazaSick(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID, Pml::WazaData::WazaSick guardSick) {
            return external<bool>(game::Address::Dpr_Battle_Logic_Common_GuardWazaSick, args, pokeID, guardSick);
        }
    };
}