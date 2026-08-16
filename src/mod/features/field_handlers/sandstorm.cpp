#include "exlaunch.hpp"

#include "externals/BTL_STRID_SET.h"
#include "externals/BTL_STRID_STD.h"
#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/FX32.h"
#include "externals/Dpr/Battle/Logic/Handler/Field.h"
#include "externals/Dpr/Battle/Logic/Section_TurnCheck_Weather.h"
#include "externals/Dpr/Battle/Logic/ServerCommandPutter.h"
#include "externals/Pml/WazaData/WazaDamageType.h"

#include "data/types.h"
#include "data/utils.h"
#include "field_handlers.h"

using namespace Dpr::Battle::Logic;

void HandlerFieldEffectSandstormDefenderGuard(EventFactor::EventHandlerArgs::Object** args, uint8_t subParam) {
    auto defID = Common::GetEventVar(args, EventVar::Label::POKEID_DEF);
    BTL_POKEPARAM::Object* bpp = Common::GetPokeParam(args, defID);

    if (bpp->IsMatchType(array_index(TYPES, "Rock")) &&
        Common::GetEventVar(args, EventVar::Label::DAMAGE_TYPE) == (int32_t)Pml::WazaData::WazaDamageType::SPECIAL)
    {
        Common::MulEventVar(args, EventVar::Label::RATIO, FX32::CONST_1_5);
    }
}

EventFactor::EventHandlerTable::Array* ADD_FieldEffect_Weather_Sandstorm() {
    EventFactor::EventHandlerTable::Array* table = getExtraFieldEffectWeatherHandlers()->HandlerTable_Sandstorm;
    if (table == nullptr) {
        table = CreateFieldEffectEventHandlerTable(1);
        table->m_Items[0] = CreateFieldEffectEventHandler(EventID::DEFENDER_GUARD, (Il2CppMethodPointer)&HandlerFieldEffectSandstormDefenderGuard);
        getExtraFieldEffectWeatherHandlers()->HandlerTable_Sandstorm = table;
    }
    return table;
}

uint16_t FieldEffect_Weather_Sandstorm_Damage(BTL_POKEPARAM::Object* bpp) {
    if (bpp->IsMatchType(array_index(TYPES, "Ground")) ||
        bpp->IsMatchType(array_index(TYPES, "Rock")) ||
        bpp->IsMatchType(array_index(TYPES, "Steel")))
        return 0;

    auto hp = bpp->GetValue(BTL_POKEPARAM::ValueID::BPP_MAX_HP_BEFORE_G);
    auto dmg = hp / 16;

    if (dmg == 0)
        dmg = 1;

    return dmg;
}

void FieldEffect_Weather_Sandstorm_DamageStrParam(StrParam::Object* strParam, Section_TurnCheck_Weather::Object* section, uint8_t pokeID) {
    strParam->Setup(BtlStrType::BTL_STRTYPE_SET, BTL_STRID_SET::WeatherDmg_Sand);
    strParam->AddArg(pokeID);
}

int32_t FieldEffect_Weather_Sandstorm_StartStrID(BTL_CLIENT::Object* client, ChangeWeatherCause cause) {
    if (cause == ChangeWeatherCause::ON_BATTLE_START)
        return BTL_STRID_STD::StormStart_OnBattleStart;
    else
        return BTL_STRID_STD::StormStart;
}

int32_t FieldEffect_Weather_Sandstorm_EndStrID(BTL_CLIENT::Object* client) {
    return BTL_STRID_STD::StormEnd;
}