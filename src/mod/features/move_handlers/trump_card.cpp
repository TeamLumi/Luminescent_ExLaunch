#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/Handler/Waza.h"
#include "data/moves.h"
#include "data/utils.h"
#include "move_handlers.h"

using namespace Dpr::Battle::Logic;

void HandlerTrumpCardWazaPowerBase(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    BTL_POKEPARAM::Object* bpp = Common::GetPokeParam(args, pokeID);
    int32_t wazaId = Common::GetEventVar(args, EventVar::Label::WAZAID);
    uint8_t wazaIdx = bpp->WAZA_SearchIdx(wazaId);
    if (wazaIdx == 4)
        return;

    int32_t power;
    uint16_t pp = bpp->WAZA_GetPP(wazaIdx);
    switch (pp) {
        case 0:
            power = 200;
            break;
        case 1:
            power = 80;
            break;
        case 2:
            power = 60;
            break;
        case 3:
            power = 50;
            break;
        default:
            power = 40;
            break;
    }

    Common::RewriteEventVar(args, EventVar::Label::WAZA_POWER, power);
}

EventFactor::EventHandlerTable::Array* ADD_TrumpCard() {
    EventFactor::EventHandlerTable::Array* table = getExtraMoveHandlers()->HandlerTable_TrumpCard;
    if (table == nullptr) {
        table = CreateMoveEventHandlerTable(1);
        table->m_Items[0] = CreateMoveEventHandler(EventID::WAZA_POWER_BASE, (Il2CppMethodPointer)&HandlerTrumpCardWazaPowerBase);
        getExtraMoveHandlers()->HandlerTable_TrumpCard = table;
    }
    return table;
}

void Handlers_Move_TrumpCard(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Trump Card"), (Il2CppMethodPointer)&ADD_TrumpCard);
    getExtraMoveHandlers()->currentIndex++;
}