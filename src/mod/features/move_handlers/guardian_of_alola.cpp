#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "data/moves.h"
#include "data/utils.h"
#include "move_handlers.h"

using namespace Dpr::Battle::Logic;

void HandlerGuardianOfAlolaWazaDmgProc1(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    auto bpp = Common::GetPokeParam(args, Common::GetEventVar(args, EventVar::Label::POKEID_DEF));
    auto damage = (int32_t)(bpp->GetValue(BTL_POKEPARAM::ValueID::BPP_HP) * 0.75);
    if (damage < 1)
        damage = 1;

    Common::RewriteEventVar(args, EventVar::Label::FIX_DAMAGE, damage);
    Common::RewriteEventVar(args, EventVar::Label::FIX_DAMAGE_FLAG, true);
}

EventFactor::EventHandlerTable::Array* ADD_GuardianOfAlola() {
    EventFactor::EventHandlerTable::Array* table = getExtraMoveHandlers()->HandlerTable_GuardianOfAlola;
    if (table == nullptr) {
        table = CreateMoveEventHandlerTable(1);
        table->m_Items[0] = CreateMoveEventHandler(EventID::WAZA_DMG_PROC1, (Il2CppMethodPointer) &HandlerGuardianOfAlolaWazaDmgProc1);
        getExtraMoveHandlers()->HandlerTable_GuardianOfAlola = table;
    }
    return table;
}

void Handlers_Move_GuardianOfAlola(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Guardian of Alola"), (Il2CppMethodPointer)&ADD_GuardianOfAlola);
    getExtraMoveHandlers()->currentIndex++;
}