#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/Handler/Waza.h"
#include "data/moves.h"
#include "data/utils.h"
#include "move_handlers.h"

using namespace Dpr::Battle::Logic;

void HandlerDragonRageWazaDmgProc1(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;
    Common::RewriteEventVar(args, EventVar::Label::FIX_DAMAGE_FLAG, true);
    Common::RewriteEventVar(args, EventVar::Label::FIX_DAMAGE, 40);
}

EventFactor::EventHandlerTable::Array* ADD_DragonRage() {
    EventFactor::EventHandlerTable::Array* table = getExtraMoveHandlers()->HandlerTable_DragonRage;
    if (table == nullptr) {
        table = CreateEventHandlerTable(1);
        table->m_Items[0] = CreateMoveEventHandler(EventID::WAZA_DMG_PROC1, (Il2CppMethodPointer)&HandlerDragonRageWazaDmgProc1);
    }
    return table;
}

void Handlers_DragonRage(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Dragon Rage"), (Il2CppMethodPointer)&ADD_DragonRage);
    getExtraMoveHandlers()->currentIndex++;
}