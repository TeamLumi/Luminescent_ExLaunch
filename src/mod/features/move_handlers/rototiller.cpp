#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/Handler/Field.h"
#include "externals/Dpr/Battle/Logic/Handler/Waza.h"
#include "data/moves.h"
#include "data/types.h"
#include "data/utils.h"
#include "move_handlers.h"

using namespace Dpr::Battle::Logic;

void HandlerRototillerNoeffectCheckL2(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    uint8_t targetPokeID = Common::GetEventVar(args, EventVar::Label::POKEID_DEF);
    if (Handler::Field::common_isGroundEffective(args, targetPokeID) &&
        Common::GetPokeParam(args, targetPokeID)->IsMatchType(array_index(TYPES, "Grass")))
        return;

    Common::RewriteEventVar(args, EventVar::Label::NOEFFECT_FLAG, 1);
}

EventFactor::EventHandlerTable::Array* ADD_Rototiller() {
    EventFactor::EventHandlerTable::Array* table = getExtraMoveHandlers()->HandlerTable_Rototiller;
    if (table == nullptr) {
        table = CreateMoveEventHandlerTable(1);
        table->m_Items[0] = CreateMoveEventHandler(EventID::NOEFFECT_CHECK_L2, (Il2CppMethodPointer) &HandlerRototillerNoeffectCheckL2);
        table->m_Items[1] = CreateMoveEventHandler(EventID::CHECK_POKE_HIDE, (Il2CppMethodPointer) *Handler::Waza::PTR_Method$$handler_Tagayasu_CheckHide);
        getExtraMoveHandlers()->HandlerTable_Rototiller = table;
    }
    return table;
}

void Handlers_Move_Rototiller(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Rototiller"), (Il2CppMethodPointer)&ADD_Rototiller);
    getExtraMoveHandlers()->currentIndex++;
}