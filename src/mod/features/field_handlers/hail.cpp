#include "exlaunch.hpp"

#include "externals/BTL_STRID_SET.h"
#include "externals/BTL_STRID_STD.h"
#include "externals/Dpr/Battle/Logic/Handler/Field.h"
#include "externals/Dpr/Battle/Logic/Section_TurnCheck_Weather.h"
#include "externals/Pml/WazaData/WazaDamageType.h"

#include "data/types.h"
#include "data/utils.h"
#include "field_handlers.h"

using namespace Dpr::Battle::Logic;

EventFactor::EventHandlerTable::Array* ADD_FieldEffect_Weather_Hail() {
    EventFactor::EventHandlerTable::Array* table = getExtraFieldEffectWeatherHandlers()->HandlerTable_Hail;
    if (table == nullptr) {
        table = CreateFieldEffectEventHandlerTable(1);
        table->m_Items[0] = CreateFieldEffectEventHandler(EventID::INVALID, *Handler::Field::PTR_Method$$handler_fld_dummy);
        getExtraFieldEffectWeatherHandlers()->HandlerTable_Hail = table;
    }
    return table;
}

uint16_t FieldEffect_Weather_Hail_Damage(BTL_POKEPARAM::Object* bpp) {
    if (bpp->IsMatchType(array_index(TYPES, "Ice")))
        return 0;

    auto hp = bpp->GetValue(BTL_POKEPARAM::ValueID::BPP_MAX_HP_BEFORE_G);
    auto dmg = hp / 16;

    if (dmg == 0)
        dmg = 1;

    return dmg;
}

void FieldEffect_Weather_Hail_DamageStrParam(StrParam::Object* strParam, Section_TurnCheck_Weather::Object* section, uint8_t pokeID) {
    strParam->Setup(BtlStrType::BTL_STRTYPE_SET, BTL_STRID_SET::WeatherDmg_Snow);
    strParam->AddArg(pokeID);
}

int32_t FieldEffect_Weather_Hail_StartStrID(BTL_CLIENT::Object* client, ChangeWeatherCause cause) {
    if (cause == ChangeWeatherCause::ON_BATTLE_START)
        return BTL_STRID_STD::SnowStart_OnBattleStart;
    else
        return BTL_STRID_STD::SnowStart;
}

int32_t FieldEffect_Weather_Hail_EndStrID(BTL_CLIENT::Object* client) {
    return BTL_STRID_STD::SnowEnd;
}