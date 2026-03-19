#include "exlaunch.hpp"

#include "features/ability_handlers/ability_handlers.h"

#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/FX32.h"
#include "externals/Dpr/Battle/Logic/Handler/Tokusei.h"

#include "data/abilities.h"
#include "data/moves.h"
#include "data/utils.h"

using namespace Dpr::Battle::Logic;

void HandlerSharpnessWazaPower(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    system_load_typeinfo(0x8a7a);

    Common::getClass()->initIfNeeded();

    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    auto waza = Common::GetEventVar(args, EventVar::Label::WAZAID);
    switch (waza)
    {
        case array_index(MOVES, "Cut"):
        case array_index(MOVES, "Razor Leaf"):
        case array_index(MOVES, "Slash"):
        case array_index(MOVES, "Fury Cutter"):
        case array_index(MOVES, "Metal Claw"):
        case array_index(MOVES, "Crush Claw"):
        case array_index(MOVES, "Air Cutter"):
        case array_index(MOVES, "Aerial Ace"):
        case array_index(MOVES, "Dragon Claw"):
        case array_index(MOVES, "Leaf Blade"):
        case array_index(MOVES, "Night Slash"):
        case array_index(MOVES, "Air Slash"):
        case array_index(MOVES, "X-Scissor"):
        case array_index(MOVES, "Shadow Claw"):
        case array_index(MOVES, "Psycho Cut"):
        case array_index(MOVES, "Cross Poison"):
        case array_index(MOVES, "Sacred Sword"):
        case array_index(MOVES, "Razor Shell"):
        case array_index(MOVES, "Secret Sword"):
        case array_index(MOVES, "Solar Blade"):
        case array_index(MOVES, "Smart Strike"):
        case array_index(MOVES, "Behemoth Blade"):
        case array_index(MOVES, "Stone Axe"):
        case array_index(MOVES, "Ceaseless Edge"):
        case array_index(MOVES, "Population Bomb"):
        case array_index(MOVES, "Kowtow Cleave"):
        case array_index(MOVES, "Psyblade"):
        case array_index(MOVES, "Bitter Blade"):
        case array_index(MOVES, "Aqua Cutter"):
        case array_index(MOVES, "Mighty Cleave"):
        case array_index(MOVES, "Tachyon Cutter"):
            Common::MulEventVar(args, EventVar::Label::WAZA_POWER_RATIO, FX32::CONST_1_5);
            break;
    }
}

EventFactor::EventHandlerTable::Array* ADD_Sharpness(uint8_t* prio) {
    EventFactor::EventHandlerTable::Array* table = getExtraAbilityHandlers()->HandlerTable_Sharpness;
    if (table == nullptr) {
        table = CreateAbilityEventHandlerTable(1);
        table->m_Items[0] = CreateAbilityEventHandler(EventID::WAZA_POWER, (Il2CppMethodPointer)&HandlerSharpnessWazaPower);
        getExtraAbilityHandlers()->HandlerTable_Sharpness = table;
    }
    return table;
}

void Handlers_Ability_Sharpness(Handler::Tokusei::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetAbilityFunctionTable(getFuncTable, getExtraAbilityHandlers()->currentIndex, array_index(ABILITIES, "Sharpness"), (Il2CppMethodPointer)&ADD_Sharpness);
    getExtraAbilityHandlers()->currentIndex++;
}