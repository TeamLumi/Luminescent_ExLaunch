#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/Handler/Waza.h"
#include "data/moves.h"
#include "data/utils.h"
#include "move_handlers.h"
#include "utils/utils.h"

using namespace Dpr::Battle::Logic;

void HandlerSparklySwirlDamageprocEndHitReal(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;
    auto* party = Common::GetPartyData(args, pokeID);

    if (party != nullptr)
    {
        for (uint8_t i = 0; i < party->fields.m_memberCount; i++)
        {
            auto* poke = party->GetMemberDataConst(i);

            if (poke == nullptr)
                break;

            uint8_t targetID = poke->GetID();

            if (Common::CheckPokeSick(args, targetID) != Pml::PokePara::Sick::NONE)
            {
                HandlerCureSick(args, pokeID, Pml::WazaData::WazaSick::WAZASICK_MAX, targetID);
            }
        }
    }
    auto* otherOutPokeIDs = System::Byte_array::newArray(5);
    uint8_t otherOutCount = GetAllOtherOutPokeID(args, pokeID, otherOutPokeIDs);

    for (uint8_t i = 0; i < otherOutCount; i++)
    {
        uint8_t targetID = otherOutPokeIDs->m_Items[i];

        if (Common::IsFriendPokeID(args, pokeID, targetID))
        {
            if (Common::CheckPokeSick(args, targetID) != Pml::PokePara::Sick::NONE)
            {
                HandlerCureSick(args, pokeID, Pml::WazaData::WazaSick::WAZASICK_MAX, targetID);
            }
            break;
        }
    }    
}

EventFactor::EventHandlerTable::Array* ADD_SparklySwirl() {
    EventFactor::EventHandlerTable::Array* table = getExtraMoveHandlers()->HandlerTable_SparklySwirl;
    if (table == nullptr) {
        table = CreateMoveEventHandlerTable(1);
        table->m_Items[0] = CreateMoveEventHandler(EventID::DAMAGEPROC_END_HIT_REAL, (Il2CppMethodPointer) &HandlerSparklySwirlDamageprocEndHitReal);
        getExtraMoveHandlers()->HandlerTable_SparklySwirl = table;
    }
    return table;
}

void Handlers_Move_SparklySwirl(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Sparkly Swirl"), (Il2CppMethodPointer)&ADD_SparklySwirl);
    getExtraMoveHandlers()->currentIndex++;
}