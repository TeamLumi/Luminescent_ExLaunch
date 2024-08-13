#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/Handler/Waza.h"
#include "externals/ItemWork.h"
#include "data/items.h"
#include "data/moves.h"
#include "data/utils.h"
#include "move_handlers.h"
#include "utils/utils.h"

using namespace Dpr::Battle::Logic;

bool IsHoldingNaturalGiftItem(BTL_POKEPARAM::Object* bpp) {
    XLSXContent::ItemTable::SheetItem::Object* data = ItemWork::GetItemData(bpp->GetItem());
    if (data->fields.sizen_type == 31)
        return false;

    return true;
}

void HandlerNaturalGiftWazaExecuteCheck3rd(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID) != pokeID)
        return;

    BTL_POKEPARAM::Object* bpp = Common::GetPokeParam(args, pokeID);
    if (!IsHoldingNaturalGiftItem(bpp) || !Common::CheckItemUsable(args, pokeID))
        Common::RewriteEventVar_FAIL_CAUSE(args, WazaFailCause::OTHER);
}

void HandlerNaturalGiftWazaExecuteDone(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID) != pokeID)
        return;

    if (Common::GetPokeParam(args, pokeID)->GetItem() == array_index(ITEMS, "-ITEM ZERO-"))
        return;

    HandlerConsumeItem(args, pokeID, true, true);
}

void HandlerNaturalGiftWazaParam(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID) != pokeID)
        return;

    if (!Common::CheckItemUsable(args, pokeID))
        return;

    XLSXContent::ItemTable::SheetItem::Object* data = ItemWork::GetItemData(Common::GetPokeParam(args, pokeID)->GetItem());
    if (data->fields.sizen_type == 31)
        return;

    Common::RewriteEventVar(args, EventVar::Label::WAZA_TYPE, data->fields.sizen_type);
}

void HandlerNaturalGiftWazaPowerBase(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    XLSXContent::ItemTable::SheetItem::Object* data = ItemWork::GetItemData(Common::GetPokeParam(args, pokeID)->GetItem());
    Common::RewriteEventVar(args, EventVar::Label::WAZA_POWER, data->fields.sizen_atc);
}

EventFactor::EventHandlerTable::Array* ADD_NaturalGift() {
    EventFactor::EventHandlerTable::Array* table = getExtraMoveHandlers()->HandlerTable_NaturalGift;
    if (table == nullptr) {
        table = CreateMoveEventHandlerTable(4);
        table->m_Items[0] = CreateMoveEventHandler(EventID::WAZA_EXECUTE_CHECK_3RD, (Il2CppMethodPointer)&HandlerNaturalGiftWazaExecuteCheck3rd);
        table->m_Items[1] = CreateMoveEventHandler(EventID::WAZA_EXECUTE_DONE, (Il2CppMethodPointer)&HandlerNaturalGiftWazaExecuteDone);
        table->m_Items[2] = CreateMoveEventHandler(EventID::WAZA_PARAM, (Il2CppMethodPointer)&HandlerNaturalGiftWazaParam);
        table->m_Items[3] = CreateMoveEventHandler(EventID::WAZA_POWER_BASE, (Il2CppMethodPointer)&HandlerNaturalGiftWazaPowerBase);
        getExtraMoveHandlers()->HandlerTable_NaturalGift = table;
    }
    return table;
}

void Handlers_Move_NaturalGift(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Natural Gift"), (Il2CppMethodPointer)&ADD_NaturalGift);
    getExtraMoveHandlers()->currentIndex++;
}