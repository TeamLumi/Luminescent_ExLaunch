#pragma once

#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/BTL_SICKCONT.h"
#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventFactor.h"
#include "externals/Dpr/Battle/Logic/PokeTypePair.h"

using namespace Dpr::Battle::Logic;
bool HandlerAddFieldEffect(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID, int32_t effect, uint8_t turns, BtlStrType strType, uint16_t strID);
//void HandlerAddPosEffect(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID);
bool HandlerAddSick(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID, uint8_t targetPokeID, Pml::WazaData::WazaSick sickID, int64_t sickCont);
bool HandlerChangeType(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID, PokeTypePair::Object type, BTL_POKEPARAM::ExTypeCause exTypeCause, bool isStandardMessageDisable, bool isFailMessageEnable, bool isDisplayTokuseiWindow);
void HandlerConsumeItem(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID, bool isUseActionDisable, bool isKinomiCheckDisable);
bool HandlerCureSick(EventFactor::EventHandlerArgs::Object** args, uint8_t causePokeID, Pml::WazaData::WazaSick sickID, uint8_t targetPokeID);
//void HandlerDamage(EventFactor::EventHandlerArgs::Object** args, uint8_t causePokeID, uint8_t targetPokeID, uint16_t damage, bool disableDeadProcess, bool displayAbility);
//void HandlerFormChange(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID, uint8_t nextForm, bool persistOnSwitch, bool displayAbility, bool animationEnabled);
//void HandlerMessage(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID, bool displayAbility);
void HandlerPlayWazaEffect(EventFactor::EventHandlerArgs::Object** args, uint8_t atkPos, uint8_t defPos, int32_t waza, uint8_t wazaType);
bool HandlerRankEffect(EventFactor::EventHandlerArgs::Object** args, uint8_t causePokeID, uint8_t targetPokeID, Pml::WazaData::WazaRankEffect rankType, int8_t rankVolume, bool displayAbility, bool ignoreSubstitute, bool messageOnFail, bool byWazaEffect);
//void HandlerRankReset(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID);
//void HandlerRankResetAll(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID);
void HandlerSetEffectEnable(EventFactor::EventHandlerArgs::Object** args);
void HandlerSetEffectIndex(EventFactor::EventHandlerArgs::Object** args, uint8_t effectIndex);
bool HandlerShrink(EventFactor::EventHandlerArgs::Object** args, uint8_t targetPokeID, uint8_t percentage);
