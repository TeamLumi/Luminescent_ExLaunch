#include "exlaunch.hpp"

#include "externals/BTL_STRID_SET.h"
#include "externals/Dpr/Battle/Logic/calc.h"
#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/Handler/Waza.h"
#include "data/items.h"
#include "data/moves.h"
#include "data/utils.h"
#include "move_handlers.h"

using namespace Dpr::Battle::Logic;

void HandlerBestowUncategorizeWaza(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    BTL_POKEPARAM::Object* bpp = Common::GetPokeParam(args, pokeID);
    uint16_t itemID = bpp->GetItem();
    if (itemID == array_index(ITEMS, "-ITEM ZERO-"))
        return;

    uint8_t targetPokeID = Common::GetEventVar(args, EventVar::Label::POKEID_TARGET1);
    BTL_POKEPARAM::Object* targetBPP = Common::GetPokeParam(args, targetPokeID);
    if (targetBPP->GetItem() != array_index(ITEMS, "-ITEM ZERO-"))
        return;

    if (calc::ITEM_IsMail(itemID))
        return;

    if (Common::CheckUnbreakablePokeItem(targetBPP->GetMonsNo(), itemID))
        return;

    if (Common::CheckUnbreakablePokeItem(bpp->GetMonsNo(), itemID))
        return;

    system_load_typeinfo(0xaa76);
    auto desc = Section_FromEvent_SwapItem::Description::newInstance();
    desc->fields.userPokeID = pokeID;
    desc->fields.targetPokeID = targetPokeID;
    desc->fields.successMessage1->Setup(BtlStrType::BTL_STRTYPE_SET, BTL_STRID_SET::GiftPass);
    desc->fields.successMessage1->AddArg(pokeID);
    desc->fields.successMessage1->AddArg(targetPokeID);
    desc->fields.successMessage1->AddArg(itemID);

    if (!Common::SwapItem(args, &desc))
        return;

    Common::RewriteEventVar(args, EventVar::Label::SUCCESS_FLAG, 1);
}

EventFactor::EventHandlerTable::Array* ADD_Bestow() {
    EventFactor::EventHandlerTable::Array* table = getExtraMoveHandlers()->HandlerTable_Bestow;
    if (table == nullptr) {
        table = CreateMoveEventHandlerTable(1);
        table->m_Items[0] = CreateMoveEventHandler(EventID::UNCATEGORIZE_WAZA, (Il2CppMethodPointer)&HandlerBestowUncategorizeWaza);
        getExtraMoveHandlers()->HandlerTable_Bestow = table;
    }
    return table;
}

void Handlers_Move_Bestow(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Bestow"), (Il2CppMethodPointer)&ADD_Bestow);
    getExtraMoveHandlers()->currentIndex++;
}