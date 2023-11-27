#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/Handler/Waza.h"
#include "externals/Dpr/Battle/Logic/PokeTypePair.h"
#include "data/moves.h"
#include "data/types.h"
#include "data/utils.h"
#include "move_handlers.h"

using namespace Dpr::Battle::Logic;

void HandlerSynchronoiseNoeffectCheckL2(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    uint8_t typing[3] = { TYPE_COUNT, TYPE_COUNT, TYPE_COUNT };
    PokeTypePair::Split(Common::GetPokeParam(args, pokeID)->GetPokeType(), &typing[0], &typing[1], &typing[2]);
    BTL_POKEPARAM::Object* defBPP = Common::GetPokeParam(args,Common::GetEventVar(args, EventVar::Label::POKEID_DEF));
    for (uint8_t type : typing)
        if (type != TYPE_COUNT && defBPP->IsMatchType(type))
            return;

    Common::RewriteEventVar(args, EventVar::Label::NOEFFECT_FLAG, 1);
}

EventFactor::EventHandlerTable::Array* ADD_Synchronoise() {
    EventFactor::EventHandlerTable::Array* table = getExtraMoveHandlers()->HandlerTable_Synchronoise;
    if (table == nullptr) {
        table = CreateMoveEventHandlerTable(1);
        table->m_Items[0] = CreateMoveEventHandler(EventID::NOEFFECT_CHECK_L2, (Il2CppMethodPointer)&HandlerSynchronoiseNoeffectCheckL2);
        getExtraMoveHandlers()->HandlerTable_Synchronoise = table;
    }
    return table;
}

void Handlers_Move_Synchronoise(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Synchronoise"), (Il2CppMethodPointer)&ADD_Synchronoise);
    getExtraMoveHandlers()->currentIndex++;
}