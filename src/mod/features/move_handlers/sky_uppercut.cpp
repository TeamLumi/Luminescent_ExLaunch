#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/Handler/Waza.h"
#include "data/moves.h"
#include "data/utils.h"
#include "move_handlers.h"

using namespace Dpr::Battle::Logic;

EventFactor::EventHandlerTable::Array* ADD_SkyUppercut() {
    EventFactor::EventHandlerTable::Array* table = getExtraMoveHandlers()->HandlerTable_SkyUppercut;
    if (table == nullptr) {
        table = CreateMoveEventHandlerTable(1);
        table->m_Items[0] = CreateMoveEventHandler(EventID::CHECK_POKE_HIDE, *Handler::Waza::PTR_Method$$handler_Tatumaki_checkHide);
        getExtraMoveHandlers()->HandlerTable_SkyUppercut = table;
    }
    return table;
}

void Handlers_Move_SkyUppercut(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Sky Uppercut"), (Il2CppMethodPointer)&ADD_SkyUppercut);
    getExtraMoveHandlers()->currentIndex++;
}