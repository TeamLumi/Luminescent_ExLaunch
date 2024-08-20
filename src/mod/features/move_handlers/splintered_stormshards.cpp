#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "data/moves.h"
#include "data/field_effects.h"
#include "data/utils.h"
#include "move_handlers.h"

using namespace Dpr::Battle::Logic;

void HandlerSplinteredStormshardsDamageprocEndHitReal(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    int32_t effType = (int32_t)array_index(FIELD_EFFECTS, "Terrain");
    if (!Common::CheckFieldEffect(args, &effType))
        return;

    system_load_typeinfo(0x89b9);
    auto desc = Section_FromEvent_FieldEffect_Remove::Description::newInstance();
    desc->fields.effect = effType;
    Common::RemoveFieldEffect(args, &desc);
}

EventFactor::EventHandlerTable::Array* ADD_SplinteredStormshards() {
    EventFactor::EventHandlerTable::Array* table = getExtraMoveHandlers()->HandlerTable_SplinteredStormshards;
    if (table == nullptr) {
        table = CreateMoveEventHandlerTable(1);
        table->m_Items[0] = CreateMoveEventHandler(EventID::DAMAGEPROC_END_HIT_REAL, (Il2CppMethodPointer) &HandlerSplinteredStormshardsDamageprocEndHitReal);
        getExtraMoveHandlers()->HandlerTable_SplinteredStormshards = table;
    }
    return table;
}

void Handlers_Move_SplinteredStormshards(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Splintered Stormshards"), (Il2CppMethodPointer)&ADD_SplinteredStormshards);
    getExtraMoveHandlers()->currentIndex++;
}