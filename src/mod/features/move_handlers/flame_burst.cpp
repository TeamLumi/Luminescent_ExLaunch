#include "exlaunch.hpp"

#include "externals/BTL_STRID_SET.h"
#include "externals/Dpr/Battle/Logic/calc.h"
#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/Handler/Waza.h"
#include "externals/System/Primitives.h"
#include "data/moves.h"
#include "data/utils.h"
#include "move_handlers.h"
#include "utils/utils.h"

using namespace Dpr::Battle::Logic;

void HandlerFlameBurstDamageprocEndHitReal(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    uint8_t targetCount = Common::GetEventVar(args, EventVar::Label::TARGET_POKECNT);
    for (uint8_t i=0; i<targetCount; i++)
    {
        auto target = (EventVar::Label)(i + (uint16_t)EventVar::Label::POKEID_TARGET1);
        uint8_t targetPokeID = Common::GetEventVar(args, target);

        auto opponents = (System::Byte_array*)system_array_new(System::Byte_array_TypeInfo(), 5);
        uint8_t opponentCount = GetAllOtherOutPokeID(args, targetPokeID, opponents);
        for (int32_t j=0; j<opponentCount; j++)
        {
            uint8_t opponentPokeID = opponents->m_Items[j];
            if (targetPokeID != opponentPokeID && Common::IsFriendPokeID(args, targetPokeID, opponentPokeID))
            {
                auto damage = calc::QuotMaxHP(Common::GetPokeParam(args, opponentPokeID), 16, true);
                HandlerDamage(args, pokeID, opponentPokeID, damage, false, false, BtlStrType::BTL_STRTYPE_SET, BTL_STRID_SET::HajikeruHonoo_Side);
            }
        }
    }
}

EventFactor::EventHandlerTable::Array* ADD_FlameBurst() {
    EventFactor::EventHandlerTable::Array* table = getExtraMoveHandlers()->HandlerTable_FlameBurst;
    if (table == nullptr) {
        table = CreateMoveEventHandlerTable(1);
        table->m_Items[0] = CreateMoveEventHandler(EventID::DAMAGEPROC_END_HIT_REAL, (Il2CppMethodPointer)&HandlerFlameBurstDamageprocEndHitReal);
        getExtraMoveHandlers()->HandlerTable_FlameBurst = table;
    }
    return table;
}

void Handlers_Move_FlameBurst(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Flame Burst"), (Il2CppMethodPointer)&ADD_FlameBurst);
    getExtraMoveHandlers()->currentIndex++;
}