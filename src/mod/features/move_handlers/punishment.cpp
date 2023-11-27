#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/Handler/Waza.h"
#include "data/moves.h"
#include "data/utils.h"
#include "move_handlers.h"

using namespace Dpr::Battle::Logic;

void HandlerPunishmentWazaPowerBase(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    BTL_POKEPARAM::Object* bpp = Common::GetPokeParam(args, Common::GetEventVar(args, EventVar::Label::POKEID_DEF));
    int32_t ranks = 0;
    for (int i = 1; i <= 7; i++) {
        int32_t rank = bpp->GetValue((BTL_POKEPARAM::ValueID)i) - 6;
        if (rank > 0)
            ranks += rank;
    }

    int32_t power = 60 + ranks * 20;
    if (power > 200)
        power = 200;

    Common::RewriteEventVar(args, EventVar::Label::WAZA_POWER, power);
}

EventFactor::EventHandlerTable::Array* ADD_Punishment() {
    EventFactor::EventHandlerTable::Array* table = getExtraMoveHandlers()->HandlerTable_Refresh;
    if (table == nullptr) {
        table = CreateMoveEventHandlerTable(1);
        table->m_Items[0] = CreateMoveEventHandler(EventID::WAZA_POWER_BASE, (Il2CppMethodPointer)&HandlerPunishmentWazaPowerBase);
        getExtraMoveHandlers()->HandlerTable_Refresh = table;
    }
    return table;
}

void Handlers_Move_Punishment(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Punishment"), (Il2CppMethodPointer)&ADD_Punishment);
    getExtraMoveHandlers()->currentIndex++;
}