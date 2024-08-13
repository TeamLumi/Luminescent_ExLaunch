#include "exlaunch.hpp"

#include "externals/BTL_STRID_STD.h"
#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/Handler/Waza.h"
#include "externals/Dpr/Battle/Logic/SICKCONT.h"
#include "data/moves.h"
#include "data/side_effects.h"
#include "data/utils.h"
#include "move_handlers.h"

using namespace Dpr::Battle::Logic;

void HandlerLuckyChantUncategorizeWazaNoTarget(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    auto cont = SICKCONT::MakeTurn(pokeID, 5);
    if (!Handler::Waza::common_SideEffectStdMsg(args, &pokeID, Common::PokeIDtoSide(args, &pokeID), (BtlSideEffect)array_index(SIDE_EFFECTS, "Lucky Chant"), &cont, BTL_STRID_STD::Omajinai))
        return;

    Common::RewriteEventVar(args, EventVar::Label::SUCCESS_FLAG, 1);
}

EventFactor::EventHandlerTable::Array* ADD_LuckyChant() {
    EventFactor::EventHandlerTable::Array* table = getExtraMoveHandlers()->HandlerTable_LuckyChant;
    if (table == nullptr) {
        table = CreateMoveEventHandlerTable(1);
        table->m_Items[0] = CreateMoveEventHandler(EventID::UNCATEGORIZE_WAZA_NO_TARGET, (Il2CppMethodPointer)&HandlerLuckyChantUncategorizeWazaNoTarget);
        getExtraMoveHandlers()->HandlerTable_LuckyChant = table;
    }
    return table;
}

void Handlers_Move_LuckyChant(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Lucky Chant"), (Il2CppMethodPointer)&ADD_LuckyChant);
    getExtraMoveHandlers()->currentIndex++;
}