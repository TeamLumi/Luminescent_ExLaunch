#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/Handler/Waza.h"
#include "data/moves.h"
#include "data/utils.h"
#include "move_handlers.h"

using namespace Dpr::Battle::Logic;

void HandlerFrustrationWazaPower(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    int32_t power = (int32_t) ((255 - Common::GetPokeParam(args, pokeID)->GetFriendship()) / 2.5);
    if (power < 1)
        power = 1;

    Common::RewriteEventVar(args, EventVar::Label::WAZA_POWER, power);
}

EventFactor::EventHandlerTable::Array* ADD_Frustration() {
    EventFactor::EventHandlerTable::Array* table = getExtraMoveHandlers()->HandlerTable_Frustration;
    if (table == nullptr) {
        table = CreateEventHandlerTable(1);
        table->m_Items[0] = CreateMoveEventHandler(EventID::WAZA_POWER, (Il2CppMethodPointer)&HandlerFrustrationWazaPower);
        getExtraMoveHandlers()->HandlerTable_Frustration = table;
    }
    return table;
}

void Handlers_Frustration(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Frustration"), (Il2CppMethodPointer)&ADD_Frustration);
    getExtraMoveHandlers()->currentIndex++;
}