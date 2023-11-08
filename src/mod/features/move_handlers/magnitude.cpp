#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/calc.h"
#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/Handler/Waza.h"
#include "data/moves.h"
#include "data/utils.h"
#include "move_handlers.h"

using namespace Dpr::Battle::Logic;

void HandlerMagnitudeWazaPower(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    uint32_t r = calc::GetRand(20);
    int32_t power;
    if (r < 1) power = 10;
    else if (r < 3) power = 30;
    else if (r < 7) power = 50;
    else if (r < 13) power = 70;
    else if (r < 17) power = 90;
    else if (r < 19) power = 110;
    else power = 150;

    Common::RewriteEventVar(args, EventVar::Label::WAZA_POWER, power);
}

EventFactor::EventHandlerTable::Array* ADD_Magnitude() {
    EventFactor::EventHandlerTable::Array* table = getExtraMoveHandlers()->HandlerTable_Magnitude;
    if (table == nullptr) {
        table = CreateEventHandlerTable(3);
        table->m_Items[0] = CreateMoveEventHandler(EventID::WAZA_POWER, (Il2CppMethodPointer) &HandlerMagnitudeWazaPower);
        table->m_Items[1] = CreateMoveEventHandler(EventID::WAZA_DMG_PROC3, *Handler::Waza::PTR_Method$$handler_Jisin_damage);
        table->m_Items[2] = CreateMoveEventHandler(EventID::CHECK_POKE_HIDE, *Handler::Waza::PTR_Method$$handler_Jisin_checkHide);
        getExtraMoveHandlers()->HandlerTable_Magnitude = table;
    }
    return table;
}

void Handlers_Magnitude(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Magnitude"), (Il2CppMethodPointer)&ADD_Magnitude);
    getExtraMoveHandlers()->currentIndex++;
}