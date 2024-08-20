#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "data/moves.h"
#include "data/utils.h"
#include "move_handlers.h"

using namespace Dpr::Battle::Logic;

void HandlerGenesisSupernovaDamageprocEndHitReal(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    Handler::Waza::handler_common_GroundSet(args, pokeID, BtlGround::BTL_GROUND_PHYCHO);
}

EventFactor::EventHandlerTable::Array* ADD_GenesisSupernova() {
    EventFactor::EventHandlerTable::Array* table = getExtraMoveHandlers()->HandlerTable_GenesisSupernova;
    if (table == nullptr) {
        table = CreateMoveEventHandlerTable(1);
        table->m_Items[0] = CreateMoveEventHandler(EventID::DAMAGEPROC_END_HIT_REAL, (Il2CppMethodPointer) &HandlerGenesisSupernovaDamageprocEndHitReal);
        getExtraMoveHandlers()->HandlerTable_GenesisSupernova = table;
    }
    return table;
}

void Handlers_Move_GenesisSupernova(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Genesis Supernova"), (Il2CppMethodPointer)&ADD_GenesisSupernova);
    getExtraMoveHandlers()->currentIndex++;
}