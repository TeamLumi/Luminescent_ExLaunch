#include "exlaunch.hpp"

#include "features/ability_handlers/ability_handlers.h"

#include "externals/BTL_STRID_SET.h"
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

WishiwashiForm FindNextWishiwashiForm(BTL_POKEPARAM::Object* bpp) {
    bool isSchool = bpp->fields.m_coreParam->fields.level >= 20 && (uint32_t)bpp->GetValue(BTL_POKEPARAM::ValueID::BPP_HP) > calc::QuotMaxHP_Zero(bpp, 4, false);
    return isSchool ? WishiwashiForm::SCHOOL : WishiwashiForm::SOLO;
}

int32_t WishiwashiFormToSchoolingStringID(WishiwashiForm formno) {
    switch (formno)
    {
        case WishiwashiForm::SOLO:
            return BTL_STRID_SET::Gyogun_Off;

        case WishiwashiForm::SCHOOL:
        default:
            return BTL_STRID_SET::Gyogun_On;
    }
}

bool SchoolingCanChangeForm(BTL_POKEPARAM::Object* bpp, uint8_t nextForm) {
    return bpp->GetMonsNo() == array_index(SPECIES, "Wishiwashi") && !bpp->HENSIN_Check() && bpp->GetFormNo() != (uint8_t)nextForm;
}

bool AdjustSchoolingForm(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID, bool displayAbility) {
    system_load_typeinfo(0x890f);
    Common::getClass()->initIfNeeded();

    BTL_POKEPARAM::Object* bpp = Common::GetPokeParam(args, pokeID);
    WishiwashiForm nextForm = FindNextWishiwashiForm(bpp);
    if (SchoolingCanChangeForm(bpp, (uint8_t)nextForm)) {
        HandlerFormChange(args, pokeID, (uint8_t)nextForm, false, displayAbility, true, BtlStrType::BTL_STRTYPE_SET, WishiwashiFormToSchoolingStringID(nextForm));
        return true;
    } else {
        return false;
    }
}

void HandlerSchoolingMemberInEvo(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    system_load_typeinfo(0x8a7a);
    Common::getClass()->initIfNeeded();

    if (Common::GetEventVar(args, EventVar::Label::POKEID) != pokeID)
        return;

    AdjustSchoolingForm(args, pokeID, true);
}

void HandlerSchoolingTurnCheckDone(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    AdjustSchoolingForm(args, pokeID, true);
}

EventFactor::EventHandlerTable::Array* ADD_Schooling(uint8_t* prio) {
    EventFactor::EventHandlerTable::Array* table = getExtraAbilityHandlers()->HandlerTable_Schooling;
    if (table == nullptr) {
        table = CreateAbilityEventHandlerTable(2);
        table->m_Items[0] = CreateAbilityEventHandler(EventID::MEMBER_IN_EVO, (Il2CppMethodPointer)&HandlerSchoolingMemberInEvo);
        table->m_Items[1] = CreateAbilityEventHandler(EventID::TURNCHECK_DONE, (Il2CppMethodPointer)&HandlerSchoolingTurnCheckDone);
        getExtraAbilityHandlers()->HandlerTable_Schooling = table;
    }
    return table;
}

void Handlers_Ability_Schooling(Handler::Tokusei::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetAbilityFunctionTable(getFuncTable, getExtraAbilityHandlers()->currentIndex, array_index(ABILITIES, "Schooling"), (Il2CppMethodPointer)&ADD_Schooling);
    getExtraAbilityHandlers()->currentIndex++;
}