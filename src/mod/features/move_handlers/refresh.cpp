#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/Handler/Waza.h"
#include "data/moves.h"
#include "data/utils.h"
#include "move_handlers.h"
#include "utils/utils.h"

using namespace Dpr::Battle::Logic;

void HandlerRefreshUncategorizeWaza(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    Pml::PokePara::Sick sickID = Common::CheckPokeSick(args, pokeID);

    switch (sickID) {
        case Pml::PokePara::Sick::MAHI:
        case Pml::PokePara::Sick::YAKEDO:
        case Pml::PokePara::Sick::DOKU:
            HandlerCureSick(args, pokeID, Pml::WazaData::WazaSick::WAZASICK_MAX, pokeID);
            Common::RewriteEventVar(args, EventVar::Label::SUCCESS_FLAG, true);
            break;

        default:
            Common::RewriteEventVar(args, EventVar::Label::GEN_FLAG, true);
            break;
    }
}

EventFactor::EventHandlerTable::Array* ADD_Refresh() {
    EventFactor::EventHandlerTable::Array* table = getExtraMoveHandlers()->HandlerTable_Refresh;
    if (table == nullptr) {
        table = CreateEventHandlerTable(1);
        table->m_Items[0] = CreateMoveEventHandler(EventID::UNCATEGORIZE_WAZA, (Il2CppMethodPointer)&HandlerRefreshUncategorizeWaza);
        getExtraMoveHandlers()->HandlerTable_Refresh = table;
    }
    return table;
}

void Handlers_Refresh(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Refresh"), (Il2CppMethodPointer)&ADD_Refresh);
    getExtraMoveHandlers()->currentIndex++;
}