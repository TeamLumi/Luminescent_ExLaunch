#include "exlaunch.hpp"

#include "externals/BTL_STRID_STD.h"
#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "data/field_effects.h"
#include "data/moves.h"
#include "data/utils.h"
#include "move_handlers.h"
#include "utils/utils.h"

using namespace Dpr::Battle::Logic;

void HandlerIonDelugeFieldEffectCall(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    if (!HandlerAddFieldEffect(args, pokeID, (EffectType)array_index(FIELD_EFFECTS, "Ion Deluge"), 1,
        BtlStrType::BTL_STRTYPE_STD, BTL_STRID_STD::PlasmaShower))
        return;

    Common::RewriteEventVar(args, EventVar::Label::SUCCESS_FLAG, 1);
}

EventFactor::EventHandlerTable::Array* ADD_IonDeluge() {
    EventFactor::EventHandlerTable::Array* table = getExtraMoveHandlers()->HandlerTable_IonDeluge;
    if (table == nullptr) {
        table = CreateMoveEventHandlerTable(1);
        table->m_Items[0] = CreateMoveEventHandler(EventID::FIELD_EFFECT_CALL, (Il2CppMethodPointer) &HandlerIonDelugeFieldEffectCall);
        getExtraMoveHandlers()->HandlerTable_IonDeluge = table;
    }
    return table;
}

void Handlers_Move_IonDeluge(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Ion Deluge"), (Il2CppMethodPointer)&ADD_IonDeluge);
    getExtraMoveHandlers()->currentIndex++;
}