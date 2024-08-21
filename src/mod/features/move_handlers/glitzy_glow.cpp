#include "exlaunch.hpp"

#include "externals/BTL_STRID_STD.h"
#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "data/moves.h"
#include "data/side_effects.h"
#include "data/utils.h"
#include "move_handlers.h"
#include "utils/utils.h"

using namespace Dpr::Battle::Logic;

void HandlerGlitzyGlowDamageprocEndHitReal(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    HandlerAddSideEffect(args, pokeID, (BtlSideEffect)array_index(SIDE_EFFECTS, "Light Screen"),
                         Common::PokeIDtoSide(args, &pokeID), 5, BtlStrType::BTL_STRTYPE_STD, BTL_STRID_STD::HikariNoKabe);
}

EventFactor::EventHandlerTable::Array* ADD_GlitzyGlow() {
    EventFactor::EventHandlerTable::Array* table = getExtraMoveHandlers()->HandlerTable_GlitzyGlow;
    if (table == nullptr) {
        table = CreateMoveEventHandlerTable(1);
        table->m_Items[0] = CreateMoveEventHandler(EventID::DAMAGEPROC_END_HIT_REAL, (Il2CppMethodPointer) &HandlerGlitzyGlowDamageprocEndHitReal);
        getExtraMoveHandlers()->HandlerTable_GlitzyGlow = table;
    }
    return table;
}

void Handlers_Move_GlitzyGlow(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Glitzy Glow"), (Il2CppMethodPointer)&ADD_GlitzyGlow);
    getExtraMoveHandlers()->currentIndex++;
}