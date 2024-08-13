#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/Handler/Waza.h"
#include "data/moves.h"
#include "data/types.h"
#include "data/utils.h"
#include "move_handlers.h"
#include "romdata/romdata.h"
#include "utils/utils.h"

using namespace Dpr::Battle::Logic;

uint8_t GetCamouflageType(EventFactor::EventHandlerArgs::Object** args) {
    switch (Common::GetGround(args))
    {
        case BtlGround::BTL_GROUND_GRASS:
            return array_index(TYPES, "Grass");
        case BtlGround::BTL_GROUND_MIST:
            return array_index(TYPES, "Fairy");
        case BtlGround::BTL_GROUND_ELEKI:
            return array_index(TYPES, "Electric");
        case BtlGround::BTL_GROUND_PHYCHO:
            return array_index(TYPES, "Psychic");
        default:
            break;
    }

    int32_t arenaId = Common::GetFieldSituation(args)->fields.bgComponent->fields.arenaIndex;
    return GetExtraArenaData(arenaId).camouflageType;
}

void HandlerCamouflageUncategorizeWaza(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    uint8_t type = GetCamouflageType(args);
    if (Common::GetPokeParam(args, pokeID)->IsMatchType(type))
        return;

    if (!HandlerChangeType(args, pokeID, PokeTypePair::MakePure(type), BTL_POKEPARAM::ExTypeCause::EXTYPE_CAUSE_NONE, false, false, false))
        return;

    Common::RewriteEventVar(args, EventVar::Label::SUCCESS_FLAG, 1);
}

EventFactor::EventHandlerTable::Array* ADD_Camouflage() {
    EventFactor::EventHandlerTable::Array* table = getExtraMoveHandlers()->HandlerTable_Camouflage;
    if (table == nullptr) {
        table = CreateMoveEventHandlerTable(1);
        table->m_Items[0] = CreateMoveEventHandler(EventID::UNCATEGORIZE_WAZA, (Il2CppMethodPointer)&HandlerCamouflageUncategorizeWaza);
        getExtraMoveHandlers()->HandlerTable_Camouflage = table;
    }
    return table;
}

void Handlers_Move_Camouflage(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Camouflage"), (Il2CppMethodPointer)&ADD_Camouflage);
    getExtraMoveHandlers()->currentIndex++;
}