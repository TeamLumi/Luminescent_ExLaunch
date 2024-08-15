#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/Handler/Waza.h"
#include "data/moves.h"
#include "data/utils.h"
#include "move_handlers.h"

using namespace Dpr::Battle::Logic;

void HandlerSonicBoomWazaDmgProc1(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;
    Common::RewriteEventVar(args, EventVar::Label::FIX_DAMAGE_FLAG, true);
    Common::RewriteEventVar(args, EventVar::Label::FIX_DAMAGE, 20);
}

EventFactor::EventHandlerTable::Array* ADD_SonicBoom() {
    EventFactor::EventHandlerTable::Array* table = getExtraMoveHandlers()->HandlerTable_SonicBoom;
    if (table == nullptr) {
        table = CreateMoveEventHandlerTable(1);
        table->m_Items[0] = CreateMoveEventHandler(EventID::WAZA_DMG_PROC1, (Il2CppMethodPointer)&HandlerSonicBoomWazaDmgProc1);
        getExtraMoveHandlers()->HandlerTable_SonicBoom = table;
    }
    return table;
}

void Handlers_Move_SonicBoom(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Sonic Boom"), (Il2CppMethodPointer)&ADD_SonicBoom);
    getExtraMoveHandlers()->currentIndex++;
}