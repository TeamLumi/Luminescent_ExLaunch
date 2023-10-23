#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/Handler/Waza.h"
#include "data/moves.h"
#include "data/utils.h"
#include "move_handlers.h"

using namespace Dpr::Battle::Logic;

EventFactor::EventHandlerTable::Array* ADD_JumpKick() {
    EventFactor::EventHandlerTable::Array* table = getExtraMoveHandlers()->HandlerTable_JumpKick;
    if (table == nullptr) {
        table = CreateEventHandlerTable(1);
        table->m_Items[0] = CreateMoveEventHandler(EventID::WAZA_EXECUTE_NO_EFFECT, *Handler::Waza::PTR_Method$$handler_Tobigeri_NoEffect);
    }
    return table;
}

void Handlers_JumpKick(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Jump Kick"), (Il2CppMethodPointer)&ADD_JumpKick);
    getExtraMoveHandlers()->currentIndex++;
}