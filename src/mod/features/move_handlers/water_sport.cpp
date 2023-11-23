#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/Handler/Waza.h"
#include "externals/Dpr/Battle/Logic/Section_FieldEffect_End.h"
#include "externals/Dpr/Battle/Logic/ServerCommandPutter.h"
#include "data/field_effects.h"
#include "data/moves.h"
#include "data/utils.h"
#include "move_handlers.h"
#include "utils/utils.h"

using namespace Dpr::Battle::Logic;

void HandlerWaterSportFieldEffectCall(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    if (!HandlerAddFieldEffect(args, pokeID, array_index(FIELD_EFFECTS, "Water Sport"), 5, BtlStrType::BTL_STRTYPE_STD, 129))
        return;

    Common::RewriteEventVar(args, EventVar::Label::SUCCESS_FLAG, 1);
}

EventFactor::EventHandlerTable::Array* ADD_WaterSport() {
    EventFactor::EventHandlerTable::Array* table = getExtraMoveHandlers()->HandlerTable_WaterSport;
    if (table == nullptr) {
        table = CreateMoveEventHandlerTable(1);
        table->m_Items[0] = CreateMoveEventHandler(EventID::FIELD_EFFECT_CALL, (Il2CppMethodPointer)&HandlerWaterSportFieldEffectCall);
        getExtraMoveHandlers()->HandlerTable_WaterSport = table;
    }
    return table;
}

void Handlers_Move_WaterSport(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Water Sport"), (Il2CppMethodPointer)&ADD_WaterSport);
    getExtraMoveHandlers()->currentIndex++;
}