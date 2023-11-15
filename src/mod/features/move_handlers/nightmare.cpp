#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/Handler/Waza.h"
#include "data/moves.h"
#include "data/utils.h"
#include "move_handlers.h"

using namespace Dpr::Battle::Logic;

void HandlerNightmareAddsickCheckfail(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    if (Common::GetEventVar(args, EventVar::Label::SICKID) != (int32_t)Pml::WazaData::WazaSick::WAZASICK_AKUMU)
        return;

    BTL_POKEPARAM::Object * bpp = Common::GetPokeParam(args, Common::GetEventVar(args, EventVar::Label::POKEID_DEF));

    if (bpp->CheckNemuri(BTL_POKEPARAM::NemuriCheckMode::NEMURI_CHECK_ONLY_SICK) && !bpp->CheckSick(Pml::WazaData::WazaSick::WAZASICK_AKUMU))
        return;

    Common::RewriteEventVar(args, EventVar::Label::FAIL_FLAG, 1);
}

EventFactor::EventHandlerTable::Array* ADD_Nightmare() {
    EventFactor::EventHandlerTable::Array* table = getExtraMoveHandlers()->HandlerTable_Nightmare;
    if (table == nullptr) {
        table = CreateMoveEventHandlerTable(1);
        table->m_Items[0] = CreateMoveEventHandler(EventID::ADDSICK_CHECKFAIL, (Il2CppMethodPointer)&HandlerNightmareAddsickCheckfail);
        getExtraMoveHandlers()->HandlerTable_Nightmare = table;
    }
    return table;
}

void Handlers_Move_Nightmare(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Nightmare"), (Il2CppMethodPointer)&ADD_Nightmare);
    getExtraMoveHandlers()->currentIndex++;
}