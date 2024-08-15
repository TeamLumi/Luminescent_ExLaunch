#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/FX32.h"
#include "externals/Dpr/Battle/Logic/Handler/Waza.h"
#include "externals/Dpr/Battle/Logic/PokeID.h"
#include "externals/Dpr/Battle/Logic/tables.h"
#include "externals/Dpr/Battle/Logic/WAZADATA.h"
#include "data/moves.h"
#include "data/utils.h"
#include "move_handlers.h"

using namespace Dpr::Battle::Logic;

void HandlerMeFirstReqwazaParam(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID) != pokeID)
        return;

    uint8_t targetPokePos = Common::GetEventVar(args, EventVar::Label::POKEPOS_ORG);
    uint8_t targetPokeID = Common::GetFrontPokeID(args, &targetPokePos);
    if (targetPokeID != PokeID::INVALID && !Common::GetPokeParam(args, targetPokeID)->TURNFLAG_Get(BTL_POKEPARAM::TurnFlag::TURNFLG_WAZAPROC_DONE))
    {
        PokeAction::Object* action = Common::SearchByPokeID(args, targetPokeID, true, true);
        if (action != nullptr)
        {
            int32_t targetWazaID = PokeAction::GetWazaID(action);
            if (targetWazaID != array_index(MOVES, "-MOVE ZERO-") && WAZADATA::IsDamage(targetWazaID) && !tables::IsSakidoriFailWaza(targetWazaID))
            {
                Common::RewriteEventVar(args, EventVar::Label::WAZAID, targetWazaID);
                Common::RewriteEventVar(args, EventVar::Label::POKEPOS, targetPokePos);
                return;
            }
        }
    }

    Common::RewriteEventVar(args, EventVar::Label::FAIL_FLAG, true);
}

void HandlerMeFirstWazaPower(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    Common::RewriteEventVar(args, EventVar::Label::WAZA_POWER_RATIO, FX32::CONST_1_5);
}

EventFactor::EventHandlerTable::Array* ADD_MeFirst() {
    EventFactor::EventHandlerTable::Array* table = getExtraMoveHandlers()->HandlerTable_MeFirst;
    if (table == nullptr) {
        table = CreateMoveEventHandlerTable(2);
        table->m_Items[0] = CreateMoveEventHandler(EventID::REQWAZA_PARAM, (Il2CppMethodPointer)&HandlerMeFirstReqwazaParam);
        table->m_Items[1] = CreateMoveEventHandler(EventID::WAZA_POWER, (Il2CppMethodPointer)&HandlerMeFirstWazaPower);
        getExtraMoveHandlers()->HandlerTable_MeFirst = table;
    }
    return table;
}

void Handlers_Move_MeFirst(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Me First"), (Il2CppMethodPointer)&ADD_MeFirst);
    getExtraMoveHandlers()->currentIndex++;
}