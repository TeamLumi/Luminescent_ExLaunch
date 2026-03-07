#include "exlaunch.hpp"

#include "features/ability_handlers/ability_handlers.h"

#include "externals/BTL_STRID_SET.h"
#include "externals/BTL_STRID_STD.h"
#include "externals/DPData/Form_Enums.h"
#include "externals/Dpr/Battle/Logic/calc.h"
#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/Handler/Tokusei.h"

#include "data/abilities.h"
#include "data/species.h"
#include "data/utils.h"

#include "utils/utils.h"

using namespace Dpr::Battle::Logic;

bool DisguiseCanChangeForm(BTL_POKEPARAM::Object* bpp, uint8_t nextForm) {
    return bpp->GetMonsNo() == array_index(SPECIES, "Mimikyu") && !bpp->HENSIN_Check() && bpp->GetFormNo() != (uint8_t)nextForm;
}

bool AdjustDisguiseForm(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID, bool displayAbiltiy) {
    system_load_typeinfo(0x890f);
    Common::getClass()->initIfNeeded();

    BTL_POKEPARAM::Object* bpp = Common::GetPokeParam(args, pokeID);
    if (DisguiseCanChangeForm(bpp, (uint8_t)MimikyuForm::BUSTED)) {
        HandlerFormChange(args, pokeID, (uint8_t)MimikyuForm::BUSTED, false, displayAbiltiy, true, BtlStrType::BTL_STRTYPE_SET, BTL_STRID_SET::Bakenokawa);
        return true;
    } else {
        return false;
    }
}

void HandlerDisguiseIchigekiGuard(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {

}

void HandlerDisguiseWazaDmgProc2(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {

}

void HandlerDisguiseWazaDmgReactionL3(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {

}

void HandlerDisguiseFixConfDamage(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {

}

void HandlerDisguiseConfDamageReaction(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {

}

EventFactor::EventHandlerTable::Array* ADD_Disguise(uint8_t* prio) {
    EventFactor::EventHandlerTable::Array* table = getExtraAbilityHandlers()->HandlerTable_Disguise;
    if (table == nullptr) {
        table = CreateAbilityEventHandlerTable(5);
        table->m_Items[0] = CreateAbilityEventHandler(EventID::ICHIGEKI_GUARD, (Il2CppMethodPointer)&HandlerDisguiseIchigekiGuard);
        table->m_Items[1] = CreateAbilityEventHandler(EventID::WAZA_DMG_PROC2, (Il2CppMethodPointer)&HandlerDisguiseWazaDmgProc2);
        table->m_Items[2] = CreateAbilityEventHandler(EventID::WAZA_DMG_REACTION_L3, (Il2CppMethodPointer)&HandlerDisguiseWazaDmgReactionL3);
        table->m_Items[3] = CreateAbilityEventHandler(EventID::FIX_CONF_DAMAGE, (Il2CppMethodPointer)&HandlerDisguiseFixConfDamage);
        table->m_Items[4] = CreateAbilityEventHandler(EventID::CONF_DAMAGE_REACTION, (Il2CppMethodPointer)&HandlerDisguiseConfDamageReaction);
        getExtraAbilityHandlers()->HandlerTable_Disguise = table;
    }
    return table;
}

void Handlers_Ability_Disguise(Handler::Tokusei::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetAbilityFunctionTable(getFuncTable, getExtraAbilityHandlers()->currentIndex, array_index(ABILITIES, "Disguise"), (Il2CppMethodPointer)&ADD_Disguise);
    getExtraAbilityHandlers()->currentIndex++;
}