#include "exlaunch.hpp"

#include "externals/BTL_STRID_SET.h"
#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "data/moves.h"
#include "data/side_effects.h"
#include "data/utils.h"
#include "move_handlers.h"
#include "utils/utils.h"

using namespace Dpr::Battle::Logic;

void HandlerSpotlightUncategorizeWaza(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    uint8_t targetPokeID = Common::GetEventVar(args, EventVar::Label::POKEID_TARGET1);
    if (!HandlerAddSideEffect(args, pokeID, (BtlSideEffect)array_index(SIDE_EFFECTS, "Spotlight"),
        Common::PokeIDtoOpponentSide(args, &targetPokeID), targetPokeID, 1, BtlStrType::BTL_STRTYPE_SET, BTL_STRID_SET::SpotLight))
        return;

    Common::RewriteEventVar(args, EventVar::Label::SUCCESS_FLAG, true);
}

EventFactor::EventHandlerTable::Array* ADD_Spotlight() {
    EventFactor::EventHandlerTable::Array* table = getExtraMoveHandlers()->HandlerTable_Spotlight;
    if (table == nullptr) {
        table = CreateMoveEventHandlerTable(1);
        table->m_Items[0] = CreateMoveEventHandler(EventID::UNCATEGORIZE_WAZA, (Il2CppMethodPointer) &HandlerSpotlightUncategorizeWaza);
        getExtraMoveHandlers()->HandlerTable_Spotlight = table;
    }
    return table;
}

void Handlers_Move_Spotlight(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Spotlight"), (Il2CppMethodPointer)&ADD_Spotlight);
    getExtraMoveHandlers()->currentIndex++;
}