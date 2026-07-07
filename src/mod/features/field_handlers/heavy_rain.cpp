#include "exlaunch.hpp"

#include "externals/BTL_STRID_STD.h"
#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/FX32.h"
#include "externals/Dpr/Battle/Logic/Handler/Field.h"
#include "externals/Dpr/Battle/Logic/Section_TurnCheck_Weather.h"
#include "externals/Dpr/Battle/Logic/ServerCommandPutter.h"

#include "data/types.h"
#include "data/utils.h"
#include "field_handlers.h"

using namespace Dpr::Battle::Logic;

void HandlerFieldEffectHeavyRainWazaDmgProc3(EventFactor::EventHandlerArgs::Object** args, uint8_t subParam) {
    if (Common::GetEventVar(args, EventVar::Label::WAZA_TYPE) == array_index(TYPES, "Water"))
        Common::MulEventVar(args, EventVar::Label::RATIO, FX32::CONST_1_5);
}

EventFactor::EventHandlerTable::Array* ADD_FieldEffect_Weather_HeavyRain() {
    EventFactor::EventHandlerTable::Array* table = getExtraFieldEffectWeatherHandlers()->HandlerTable_HeavyRain;
    if (table == nullptr) {
        table = CreateFieldEffectEventHandlerTable(2);
        table->m_Items[0] = CreateFieldEffectEventHandler(EventID::WAZA_DMG_PROC3, (Il2CppMethodPointer)&HandlerFieldEffectHeavyRainWazaDmgProc3);
        table->m_Items[1] = CreateFieldEffectEventHandler(EventID::WAZA_EXECUTE_CHECK_2ND, *Handler::Field::PTR_Method$$handler_ooame_CheckExe);
        getExtraFieldEffectWeatherHandlers()->HandlerTable_HeavyRain = table;
    }
    return table;
}

uint16_t FieldEffect_Weather_HeavyRain_Damage(BTL_POKEPARAM::Object* bpp) {
    return 0;
}

void FieldEffect_Weather_HeavyRain_DamageStrParam(StrParam::Object* strParam, Section_TurnCheck_Weather::Object* section, uint8_t pokeID) {
    strParam->Clear();
}

int32_t FieldEffect_Weather_HeavyRain_StartStrID(BTL_CLIENT::Object* client, ChangeWeatherCause cause) {
    if (cause == ChangeWeatherCause::ON_BATTLE_START)
        return -1;
    else
        return BTL_STRID_STD::RainStormStart;
}

int32_t FieldEffect_Weather_HeavyRain_EndStrID(BTL_CLIENT::Object* client) {
    return BTL_STRID_STD::RainStormEnd;
}