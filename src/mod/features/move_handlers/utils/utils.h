#pragma once

#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/BTL_SICKCONT.h"
#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventFactor.h"

using namespace Dpr::Battle::Logic;
//bool HandlerAddFieldEffect(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID, int32_t effect, uint8_t turns);
//void HandlerAddPosEffect(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID);
void HandlerAddSick(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID, uint8_t targetPokeID, Pml::WazaData::WazaSick sickID, int64_t sickCont);
//bool HandlerAddSideEffect(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID, int32_t effect, int32_t side, BTL_SICKCONT::Object cont);
//void HandlerDamage(EventFactor::EventHandlerArgs::Object** args, uint8_t causePokeID, uint8_t targetPokeID, uint16_t damage, bool disableDeadProcess, bool displayAbility);
//void HandlerCureSick(EventFactor::EventHandlerArgs::Object** args, uint8_t causePokeID, int32_t sickID, uint8_t targetPokeID);
//void HandlerFormChange(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID, uint8_t nextForm, bool persistOnSwitch, bool displayAbility, bool animationEnabled);
//void HandlerMessage(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID, bool displayAbility);
void HandlerRankEffect(EventFactor::EventHandlerArgs::Object** args, uint8_t causePokeID, uint8_t targetPokeID, Pml::WazaData::WazaRankEffect rankType, int8_t rankVolume, bool displayAbility, bool ignoreSubstitute, bool messageOnFail);
//void HandlerRankReset(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID);
//void HandlerRankResetAll(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID);
//void HandlerShrink(EventFactor::EventHandlerArgs::Object** args, uint8_t targetPokeID, uint8_t percentage);