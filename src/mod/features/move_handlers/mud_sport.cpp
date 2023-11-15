#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/calc.h"
#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/Handler/Waza.h"
#include "data/field_effects.h"
#include "data/moves.h"
#include "data/utils.h"
#include "move_handlers.h"
#include "utils/utils.h"

using namespace Dpr::Battle::Logic;

void HandlerMudSportFieldEffectCall(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    if (!HandlerAddFieldEffect(args, pokeID, array_index(FIELD_EFFECTS, "Mud Sport"), 5))
        return;

    Common::RewriteEventVar(args, EventVar::Label::SUCCESS_FLAG, 1);
}

EventFactor::EventHandlerTable::Array* ADD_MudSport() {
    EventFactor::EventHandlerTable::Array* table = getExtraMoveHandlers()->HandlerTable_MudSport;
    if (table == nullptr) {
        table = CreateMoveEventHandlerTable(1);
        table->m_Items[0] = CreateMoveEventHandler(EventID::FIELD_EFFECT_CALL, (Il2CppMethodPointer)&HandlerMudSportFieldEffectCall);
        getExtraMoveHandlers()->HandlerTable_MudSport = table;
    }
    return table;
}

void Handlers_Move_MudSport(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Mud Sport"), (Il2CppMethodPointer)&ADD_MudSport);
    getExtraMoveHandlers()->currentIndex++;
}