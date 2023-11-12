#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/Handler/Waza.h"
#include "externals/Dpr/Battle/Logic/PokeID.h"
#include "externals/Dpr/Battle/Logic/WAZADATA.h"
#include "data/moves.h"
#include "data/utils.h"
#include "move_handlers.h"

using namespace Dpr::Battle::Logic;

void HandlerSnatchCheckWazaRob(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) == pokeID)
        return;

    if (Common::GetEventVar(args, EventVar::Label::POKEID) != PokeID::INVALID)
        return;

    if (!WAZADATA::GetFlag(Common::GetEventVar(args, EventVar::Label::WAZAID), Pml::WazaData::WazaFlag::YOKODORI))
        return;

    Common::RewriteEventVar(args, EventVar::Label::POKEID, pokeID);
    Common::RewriteEventVar(args, EventVar::Label::POKEID_DEF, pokeID);
    Common::SetWorkValue(args, WorkValue::PERSIST, 0);
    Handler::Waza::removeHandlerForce((*args)->fields.pEventSystem->instance(), pokeID, Common::GetSubID(args));
}

void HandlerSnatchTurncheckEnd(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID) != pokeID)
        return;

    Common::SetWorkValue(args, WorkValue::PERSIST, 0);
    Handler::Waza::removeHandlerForce((*args)->fields.pEventSystem->instance(), pokeID, Common::GetSubID(args));
}

void HandlerSnatchUncategorizeWaza(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    Common::SetWorkValue(args, WorkValue::PERSIST, 1);
    Common::RewriteEventVar(args, EventVar::Label::SUCCESS_FLAG, true);
}

EventFactor::EventHandlerTable::Array* ADD_Snatch() {
    EventFactor::EventHandlerTable::Array* table = getExtraMoveHandlers()->HandlerTable_Snatch;
    if (table == nullptr) {
        table = CreateEventHandlerTable(3);
        table->m_Items[0] = CreateMoveEventHandler(EventID::CHECK_WAZA_ROB, (Il2CppMethodPointer)&HandlerSnatchCheckWazaRob);
        table->m_Items[1] = CreateMoveEventHandler(EventID::TURNCHECK_END, (Il2CppMethodPointer)&HandlerSnatchTurncheckEnd);
        table->m_Items[2] = CreateMoveEventHandler(EventID::UNCATEGORIZE_WAZA, (Il2CppMethodPointer)&HandlerSnatchUncategorizeWaza);
        getExtraMoveHandlers()->HandlerTable_Snatch = table;
    }
    return table;
}

void Handlers_Snatch(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Snatch"), (Il2CppMethodPointer)&ADD_Snatch);
    getExtraMoveHandlers()->currentIndex++;
}