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

void HandlerFieldEffectRainWazaDmgProc3(EventFactor::EventHandlerArgs::Object** args, uint8_t subParam) {
    auto type = Common::GetEventVar(args, EventVar::Label::WAZA_TYPE);
    switch (type)
    {
        case array_index(TYPES, "Fire"):
            Common::MulEventVar(args, EventVar::Label::RATIO, FX32::CONST_0_5);
            break;

        case array_index(TYPES, "Water"):
            Common::MulEventVar(args, EventVar::Label::RATIO, FX32::CONST_1_5);
            break;
    }
}

EventFactor::EventHandlerTable::Array* ADD_FieldEffect_Weather_Rain() {
    EventFactor::EventHandlerTable::Array* table = getExtraFieldEffectWeatherHandlers()->HandlerTable_Rain;
    if (table == nullptr) {
        table = CreateFieldEffectEventHandlerTable(1);
        table->m_Items[0] = CreateFieldEffectEventHandler(EventID::WAZA_DMG_PROC3, (Il2CppMethodPointer)&HandlerFieldEffectRainWazaDmgProc3);
        getExtraFieldEffectWeatherHandlers()->HandlerTable_Rain = table;
    }
    return table;
}

uint16_t FieldEffect_Weather_Rain_Damage(BTL_POKEPARAM::Object* bpp) {
    return 0;
}

void FieldEffect_Weather_Rain_DamageStrParam(StrParam::Object* strParam, Section_TurnCheck_Weather::Object* section, uint8_t pokeID) {
    strParam->Clear();
}

int32_t FieldEffect_Weather_Rain_StartStrID(BTL_CLIENT::Object* client, ChangeWeatherCause cause) {
    if (cause == ChangeWeatherCause::ON_BATTLE_START)
        return BTL_STRID_STD::RainStart_OnBattleStart;
    else
        return BTL_STRID_STD::RainStart;
}

int32_t FieldEffect_Weather_Rain_EndStrID(BTL_CLIENT::Object* client) {
    return BTL_STRID_STD::RainEnd;
}