#include "exlaunch.hpp"

#include "features/ability_handlers/ability_handlers.h"

#include "externals/BTL_STRID_STD.h"
#include "externals/DPData/Form_Enums.h"
#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/Handler/Tokusei.h"
#include "externals/Dpr/Battle/Logic/WAZADATA.h"

#include "data/abilities.h"
#include "data/moves.h"
#include "data/species.h"
#include "data/utils.h"

#include "utils/utils.h"

using namespace Dpr::Battle::Logic;

void HandlerStanceChangeWazaCallDecide(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    system_load_typeinfo(0x8a7a);
    Common::getClass()->initIfNeeded();

    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    BTL_POKEPARAM::Object* bpp = Common::GetPokeParam(args, pokeID);
    if (bpp->GetMonsNo() != array_index(SPECIES, "Aegislash"))
        return;

    auto waza = Common::GetEventVar(args, EventVar::Label::WAZAID);
    AegislashForm nextForm;
    if (WAZADATA::IsDamage(waza))
        nextForm = AegislashForm::BLADE;
    else if (waza == array_index(MOVES, "King's Shield"))
        nextForm = AegislashForm::SHIELD;
    else
        return;

    if (bpp->GetFormNo() != (uint8_t)nextForm) {
        auto strID = nextForm == AegislashForm::SHIELD ? BTL_STRID_STD::BattleSwitch_Shield : BTL_STRID_STD::BattleSwitch_Sord;
        HandlerFormChange(args, pokeID, (uint8_t)nextForm, false, true, true, BtlStrType::BTL_STRTYPE_STD, strID);
    }
}

EventFactor::EventHandlerTable::Array* ADD_StanceChange(uint8_t* prio) {
    EventFactor::EventHandlerTable::Array* table = getExtraAbilityHandlers()->HandlerTable_StanceChange;
    if (table == nullptr) {
        table = CreateAbilityEventHandlerTable(1);
        table->m_Items[0] = CreateAbilityEventHandler(EventID::WAZA_CALL_DECIDE, (Il2CppMethodPointer)&HandlerStanceChangeWazaCallDecide);
        getExtraAbilityHandlers()->HandlerTable_StanceChange = table;
    }
    return table;
}

void Handlers_Ability_StanceChange(Handler::Tokusei::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetAbilityFunctionTable(getFuncTable, getExtraAbilityHandlers()->currentIndex, array_index(ABILITIES, "Stance Change"), (Il2CppMethodPointer)&ADD_StanceChange);
    getExtraAbilityHandlers()->currentIndex++;
}