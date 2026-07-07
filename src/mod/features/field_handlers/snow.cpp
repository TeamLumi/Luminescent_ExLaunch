#include "exlaunch.hpp"

#include "externals/BTL_STRID_STD.h"
#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/FX32.h"
#include "externals/Dpr/Battle/Logic/Handler/Field.h"
#include "externals/Dpr/Battle/Logic/Section_TurnCheck_Weather.h"
#include "externals/Pml/WazaData/WazaDamageType.h"

#include "data/types.h"
#include "data/utils.h"
#include "field_handlers.h"

using namespace Dpr::Battle::Logic;

void HandlerFieldEffectSnowDefenderGuard(EventFactor::EventHandlerArgs::Object** args, uint8_t subParam) {
    auto defID = Common::GetEventVar(args, EventVar::Label::POKEID_DEF);
    BTL_POKEPARAM::Object* bpp = Common::GetPokeParam(args, defID);

    if (bpp->IsMatchType(array_index(TYPES, "Ice")) &&
        Common::GetEventVar(args, EventVar::Label::DAMAGE_TYPE) == (int32_t)Pml::WazaData::WazaDamageType::PHYSIC)
    {
        Common::MulEventVar(args, EventVar::Label::RATIO, FX32::CONST_1_5);
    }
}

EventFactor::EventHandlerTable::Array* ADD_FieldEffect_Weather_Snow() {
    EventFactor::EventHandlerTable::Array* table = getExtraFieldEffectWeatherHandlers()->HandlerTable_Snow;
    if (table == nullptr) {
        table = CreateFieldEffectEventHandlerTable(1);
        table->m_Items[0] = CreateFieldEffectEventHandler(EventID::DEFENDER_GUARD, (Il2CppMethodPointer)&HandlerFieldEffectSnowDefenderGuard);
        getExtraFieldEffectWeatherHandlers()->HandlerTable_Snow = table;
    }
    return table;
}

uint16_t FieldEffect_Weather_Snow_Damage(BTL_POKEPARAM::Object* bpp) {
    return 0;
}

void FieldEffect_Weather_Snow_DamageStrParam(StrParam::Object* strParam, Section_TurnCheck_Weather::Object* section, uint8_t pokeID) {
    strParam->Clear();
}

int32_t FieldEffect_Weather_Snow_StartStrID(BTL_CLIENT::Object* client, ChangeWeatherCause cause) {
    if (cause == ChangeWeatherCause::ON_BATTLE_START)
        return BTL_STRID_STD::SnowStart_OnBattleStart;
    else
        return BTL_STRID_STD::SnowStart;
}

int32_t FieldEffect_Weather_Snow_EndStrID(BTL_CLIENT::Object* client) {
    return BTL_STRID_STD::SnowEnd;
}