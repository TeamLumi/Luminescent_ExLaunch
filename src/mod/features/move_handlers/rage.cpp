#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/Handler/Waza.h"
#include "data/moves.h"
#include "data/utils.h"
#include "move_handlers.h"
#include "utils/utils.h"

using namespace Dpr::Battle::Logic;

void HandlerRageWazaCallDecide(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetWorkValue(args, WorkValue::PERSIST) == 0)
        return;

    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    Common::SetWorkValue(args, WorkValue::PERSIST, 0);
}

void HandlerRageWazaExecuteEffective(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID) != pokeID)
        return;

    if (Common::GetEventVar(args, EventVar::Label::WAZAID) != array_index(MOVES, "Rage"))
        return;

    Common::SetWorkValue(args, WorkValue::PERSIST, 1);
}

void HandlerRageWazaDmgReaction(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetWorkValue(args, WorkValue::PERSIST) == 0)
        return;

    if (Common::CheckShowDown(args))
        return;

    if (Common::GetEventVar(args, EventVar::Label::POKEID_DEF) != pokeID)
        return;

    if (Common::GetEventVar(args, EventVar::Label::MIGAWARI_FLAG))
        return;

    HandlerRankEffect(args, pokeID, pokeID, Pml::WazaData::WazaRankEffect::ATTACK, 1, false, false, true, false);
}

void HandlerRageMemberOutFixed(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetWorkValue(args, WorkValue::PERSIST) == 0)
        return;

    if (Common::GetEventVar(args, EventVar::Label::POKEID) != pokeID)
        return;

    Common::SetWorkValue(args, WorkValue::PERSIST, false);
}

EventFactor::EventHandlerTable::Array* ADD_Rage() {
    EventFactor::EventHandlerTable::Array* table = getExtraMoveHandlers()->HandlerTable_Rage;
    if (table == nullptr) {
        table = CreateMoveEventHandlerTable(4);
        table->m_Items[0] = CreateMoveEventHandler(EventID::WAZA_CALL_DECIDE, (Il2CppMethodPointer)&HandlerRageWazaCallDecide);
        table->m_Items[1] = CreateMoveEventHandler(EventID::WAZA_EXECUTE_EFFECTIVE, (Il2CppMethodPointer)&HandlerRageWazaExecuteEffective);
        table->m_Items[2] = CreateMoveEventHandler(EventID::WAZA_DMG_REACTION, (Il2CppMethodPointer)&HandlerRageWazaDmgReaction);
        table->m_Items[3] = CreateMoveEventHandler(EventID::MEMBER_OUT_FIXED, (Il2CppMethodPointer)&HandlerRageMemberOutFixed);
        getExtraMoveHandlers()->HandlerTable_Rage = table;
    }
    return table;
}

void Handlers_Move_Rage(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Rage"), (Il2CppMethodPointer)&ADD_Rage);
    getExtraMoveHandlers()->currentIndex++;
}