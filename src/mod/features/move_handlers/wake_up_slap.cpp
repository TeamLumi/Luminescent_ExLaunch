#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/FX32.h"
#include "externals/Dpr/Battle/Logic/Handler/Waza.h"
#include "data/moves.h"
#include "data/utils.h"
#include "move_handlers.h"
#include "utils/utils.h"

using namespace Dpr::Battle::Logic;

void HandlerWakeUpSlapWazaPower(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    BTL_POKEPARAM::Object* bpp = Common::GetPokeParam(args, Common::GetEventVar(args, EventVar::Label::POKEID_DEF));
    if (!bpp->CheckNemuri(Dpr::Battle::Logic::BTL_POKEPARAM::NemuriCheckMode::NEMURI_CHECK_INCLUDE_ZETTAINEMURI))
        return;

    Common::MulEventVar(args, EventVar::Label::WAZA_POWER_RATIO, FX32::CONST_2_0);
    Common::SetWorkValue(args, WorkValue::A, 1);
}

void HandlerWakeUpSlapDamageprocEndHitReal(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::CheckShowDown(args))
        return;

    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    if (Common::GetWorkValue(args, WorkValue::A) == 0)
        return;

    uint8_t targetCount = Common::GetEventVar(args, EventVar::Label::TARGET_POKECNT);
    for (uint8_t i=0; i<targetCount; i++) {
        auto target = (EventVar::Label)(i + (uint16_t)EventVar::Label::POKEID_TARGET1);
        HandlerCureSick(args, pokeID, Pml::WazaData::WazaSick::WAZASICK_NEMURI, Common::GetEventVar(args, target));
    }
}

EventFactor::EventHandlerTable::Array* ADD_WakeUpSlap() {
    EventFactor::EventHandlerTable::Array* table = getExtraMoveHandlers()->HandlerTable_WakeUpSlap;
    if (table == nullptr) {
        table = CreateMoveEventHandlerTable(2);
        table->m_Items[0] = CreateMoveEventHandler(EventID::WAZA_POWER, (Il2CppMethodPointer)&HandlerWakeUpSlapWazaPower);
        table->m_Items[1] = CreateMoveEventHandler(EventID::DAMAGEPROC_END_HIT_REAL, (Il2CppMethodPointer)&HandlerWakeUpSlapDamageprocEndHitReal);
        getExtraMoveHandlers()->HandlerTable_WakeUpSlap = table;
    }
    return table;
}

void Handlers_Move_WakeUpSlap(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Wake-Up Slap"), (Il2CppMethodPointer)&ADD_WakeUpSlap);
    getExtraMoveHandlers()->currentIndex++;
}