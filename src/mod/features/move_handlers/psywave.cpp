#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/calc.h"
#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/Handler/Waza.h"
#include "data/moves.h"
#include "data/utils.h"
#include "move_handlers.h"

using namespace Dpr::Battle::Logic;

void HandlerPsywaveWazaDmgProc1(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    Common::RewriteEventVar(args, EventVar::Label::FIX_DAMAGE_FLAG, true);

    int32_t randRange = calc::RandRange(50, 150);
    int32_t level = Common::GetPokeParam(args, pokeID)->GetValue(BTL_POKEPARAM::ValueID::BPP_LEVEL);
    Common::RewriteEventVar(args, EventVar::Label::FIX_DAMAGE, randRange * level / 100);
}

EventFactor::EventHandlerTable::Array* ADD_Psywave() {
    EventFactor::EventHandlerTable::Array* table = getExtraMoveHandlers()->HandlerTable_Psywave;
    if (table == nullptr) {
        table = CreateEventHandlerTable(1);
        table->m_Items[0] = CreateMoveEventHandler(EventID::WAZA_DMG_PROC1, (Il2CppMethodPointer)&HandlerPsywaveWazaDmgProc1);
        getExtraMoveHandlers()->HandlerTable_Psywave = table;
    }
    return table;
}

void Handlers_Psywave(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Psywave"), (Il2CppMethodPointer)&ADD_Psywave);
    getExtraMoveHandlers()->currentIndex++;
}