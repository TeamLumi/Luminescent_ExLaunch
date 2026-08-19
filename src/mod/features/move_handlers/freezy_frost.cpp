#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/ExPokePos.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/BTL_STRID_STD.h"
#include "externals/Dpr/Battle/Logic/Section_FromEvent_Message.h"
#include "data/moves.h"
#include "data/utils.h"
#include "move_handlers.h"
#include "utils/utils.h"

using namespace Dpr::Battle::Logic;

void HandlerFreezyFrostDamageprocEndHitReal(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    auto basePos = Common::GetExistFrontPokePos(args, pokeID);

    auto exPos = ExPokePos::newInstance(9, basePos);

    auto pokeIDs = System::Byte_array::newArray(5);

    uint8_t pokeCount = Common::ExpandExistPokeID(args, &exPos, pokeIDs);

    HandlerRankReset(args, pokeIDs, pokeCount);
    
    auto messageDesc = Section_FromEvent_Message::Description::newInstance();
    messageDesc->ctor();

    messageDesc->fields.message->Setup(
        BtlStrType::BTL_STRTYPE_STD,
        BTL_STRID_STD::RankReset
    );

    Common::Message(args, &messageDesc);
}

EventFactor::EventHandlerTable::Array* ADD_FreezyFrost() {
    EventFactor::EventHandlerTable::Array* table = getExtraMoveHandlers()->HandlerTable_FreezyFrost;
    if (table == nullptr) {
        table = CreateMoveEventHandlerTable(1);
        table->m_Items[0] = CreateMoveEventHandler(EventID::DAMAGEPROC_END_HIT_REAL, (Il2CppMethodPointer) &HandlerFreezyFrostDamageprocEndHitReal);
        getExtraMoveHandlers()->HandlerTable_FreezyFrost = table;
    }
    return table;
}

void Handlers_Move_FreezyFrost(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Freezy Frost"), (Il2CppMethodPointer)&ADD_FreezyFrost);
    getExtraMoveHandlers()->currentIndex++;
}