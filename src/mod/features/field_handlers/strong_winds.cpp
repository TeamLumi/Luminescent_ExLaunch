#include "exlaunch.hpp"

#include "externals/BTL_STRID_STD.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/Handler/Field.h"
#include "externals/Dpr/Battle/Logic/Section_TurnCheck_Weather.h"

#include "field_handlers.h"

using namespace Dpr::Battle::Logic;

EventFactor::EventHandlerTable::Array* ADD_FieldEffect_Weather_StrongWinds() {
    return Handler::Field::getClass()->static_fields->HandlerTable_Rankiryuu;
}

uint16_t FieldEffect_Weather_StrongWinds_Damage(BTL_POKEPARAM::Object* bpp) {
    return 0;
}

void FieldEffect_Weather_StrongWinds_DamageStrParam(StrParam::Object* strParam, Section_TurnCheck_Weather::Object* section, uint8_t pokeID) {
    strParam->Clear();
}

int32_t FieldEffect_Weather_StrongWinds_StartStrID(BTL_CLIENT::Object* client, ChangeWeatherCause cause) {
    if (cause == ChangeWeatherCause::ON_BATTLE_START)
        return -1;
    else
        return BTL_STRID_STD::TurbulenceStart;
}

int32_t FieldEffect_Weather_StrongWinds_EndStrID(BTL_CLIENT::Object* client) {
    return BTL_STRID_STD::TurbulenceEnd;
}