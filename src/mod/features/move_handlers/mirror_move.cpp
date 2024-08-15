#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/Handler/Waza.h"
#include "externals/Dpr/Battle/Logic/PokeID.h"
#include "externals/Dpr/Battle/Logic/WAZADATA.h"
#include "data/moves.h"
#include "data/utils.h"
#include "move_handlers.h"

using namespace Dpr::Battle::Logic;

void HandlerMirrorMoveReqwazaParam(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID) != pokeID)
        return;

    uint8_t targetPokePos = Common::GetEventVar(args, EventVar::Label::POKEPOS_ORG);
    uint8_t targetPokeID = Common::GetFrontPokeID(args, &targetPokePos);

    if (targetPokeID == PokeID::INVALID)
        return;

    int32_t targetWazaID = Common::GetPokeParam(args, targetPokeID)->fields.m_prevActWazaID;

    if (targetWazaID == array_index(MOVES, "-MOVE ZERO-"))
        return;

    if (!WAZADATA::GetFlag(targetWazaID, Pml::WazaData::WazaFlag::OUMU))
        return;

    Common::RewriteEventVar(args, EventVar::Label::WAZAID, targetWazaID);
    Common::RewriteEventVar(args, EventVar::Label::POKEPOS, targetPokePos);
}

EventFactor::EventHandlerTable::Array* ADD_MirrorMove() {
    EventFactor::EventHandlerTable::Array* table = getExtraMoveHandlers()->HandlerTable_MirrorMove;
    if (table == nullptr) {
        table = CreateMoveEventHandlerTable(1);
        table->m_Items[0] = CreateMoveEventHandler(EventID::REQWAZA_PARAM, (Il2CppMethodPointer)&HandlerMirrorMoveReqwazaParam);
        getExtraMoveHandlers()->HandlerTable_MirrorMove = table;
    }
    return table;
}

void Handlers_Move_MirrorMove(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Mirror Move"), (Il2CppMethodPointer)&ADD_MirrorMove);
    getExtraMoveHandlers()->currentIndex++;
}