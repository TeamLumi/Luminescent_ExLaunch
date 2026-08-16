#include "exlaunch.hpp"

#include "externals/BTL_STRID_STD.h"
#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "data/moves.h"
#include "data/side_effects.h"
#include "data/utils.h"
#include "move_handlers.h"
#include "utils/utils.h"

using namespace Dpr::Battle::Logic;

void HandlerBaddyBadDamageprocEndHitReal(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    HandlerAddSideEffect(args, pokeID, (BtlSideEffect)array_index(SIDE_EFFECTS, "Reflect"),
                         Common::PokeIDtoSide(args, &pokeID), 5, BtlStrType::BTL_STRTYPE_STD, BTL_STRID_STD::Reflector);
}

EventFactor::EventHandlerTable::Array* ADD_BaddyBad() {
    EventFactor::EventHandlerTable::Array* table = getExtraMoveHandlers()->HandlerTable_BaddyBad;
    if (table == nullptr) {
        table = CreateMoveEventHandlerTable(1);
        table->m_Items[0] = CreateMoveEventHandler(EventID::DAMAGEPROC_END_HIT_REAL, (Il2CppMethodPointer) &HandlerBaddyBadDamageprocEndHitReal);
        getExtraMoveHandlers()->HandlerTable_BaddyBad = table;
    }
    return table;
}

void Handlers_Move_BaddyBad(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Baddy Bad"), (Il2CppMethodPointer)&ADD_BaddyBad);
    getExtraMoveHandlers()->currentIndex++;
}