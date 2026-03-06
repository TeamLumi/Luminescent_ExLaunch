#include "exlaunch.hpp"

#include "features/ability_handlers/ability_handlers.h"

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

bool IsMeteorMiniorForm(BTL_POKEPARAM::Object* bpp) {
    if (bpp->GetMonsNo() != array_index(SPECIES, "Minior") || bpp->HENSIN_Check())
        return false;

    auto currentForm = (MiniorForm)bpp->GetFormNo();
    switch (currentForm)
    {
        case MiniorForm::RED_CORE:
        case MiniorForm::ORANGE_CORE:
        case MiniorForm::YELLOW_CORE:
        case MiniorForm::GREEN_CORE:
        case MiniorForm::BLUE_CORE:
        case MiniorForm::INDIGO_CORE:
        case MiniorForm::VIOLET_CORE:
        default:
            return false;

        case MiniorForm::RED_METEOR:
        case MiniorForm::ORANGE_METEOR:
        case MiniorForm::YELLOW_METEOR:
        case MiniorForm::GREEN_METEOR:
        case MiniorForm::BLUE_METEOR:
        case MiniorForm::INDIGO_METEOR:
        case MiniorForm::VIOLET_METEOR:
            return true;
    }
}

MiniorForm FindNextMiniorForm(BTL_POKEPARAM::Object* bpp) {
    auto currentForm = (MiniorForm)bpp->GetFormNo();
    bool isCore = (uint32_t)bpp->GetValue(BTL_POKEPARAM::ValueID::BPP_HP) <= calc::QuotMaxHP_Zero(bpp, 2, false);

    MiniorForm nextForm;
    switch (currentForm)
    {
        case MiniorForm::RED_METEOR:
        case MiniorForm::RED_CORE:
            nextForm = isCore ? MiniorForm::RED_CORE : MiniorForm::RED_METEOR;
            break;

        case MiniorForm::ORANGE_METEOR:
        case MiniorForm::ORANGE_CORE:
            nextForm = isCore ? MiniorForm::ORANGE_CORE : MiniorForm::ORANGE_METEOR;
            break;

        case MiniorForm::YELLOW_METEOR:
        case MiniorForm::YELLOW_CORE:
            nextForm = isCore ? MiniorForm::YELLOW_CORE : MiniorForm::YELLOW_METEOR;
            break;

        case MiniorForm::GREEN_METEOR:
        case MiniorForm::GREEN_CORE:
            nextForm = isCore ? MiniorForm::GREEN_CORE : MiniorForm::GREEN_METEOR;
            break;

        case MiniorForm::BLUE_METEOR:
        case MiniorForm::BLUE_CORE:
            nextForm = isCore ? MiniorForm::BLUE_CORE : MiniorForm::BLUE_METEOR;
            break;

        case MiniorForm::INDIGO_METEOR:
        case MiniorForm::INDIGO_CORE:
            nextForm = isCore ? MiniorForm::INDIGO_CORE : MiniorForm::INDIGO_METEOR;
            break;

        case MiniorForm::VIOLET_METEOR:
        case MiniorForm::VIOLET_CORE:
            nextForm = isCore ? MiniorForm::VIOLET_CORE : MiniorForm::VIOLET_METEOR;
            break;

        default:
            nextForm = currentForm;
            break;
    }

    return nextForm;
}

int32_t MiniorFormToShieldsDownStringID(MiniorForm formno) {
    switch (formno)
    {
        case MiniorForm::RED_CORE:
        case MiniorForm::ORANGE_CORE:
        case MiniorForm::YELLOW_CORE:
        case MiniorForm::GREEN_CORE:
        case MiniorForm::BLUE_CORE:
        case MiniorForm::INDIGO_CORE:
        case MiniorForm::VIOLET_CORE:
            return BTL_STRID_STD::LimitShieldOff;

        case MiniorForm::RED_METEOR:
        case MiniorForm::ORANGE_METEOR:
        case MiniorForm::YELLOW_METEOR:
        case MiniorForm::GREEN_METEOR:
        case MiniorForm::BLUE_METEOR:
        case MiniorForm::INDIGO_METEOR:
        case MiniorForm::VIOLET_METEOR:
        default:
            return BTL_STRID_STD::LimitShieldOn;
    }
}

bool ShieldsDownCanChangeForm(BTL_POKEPARAM::Object* bpp, uint8_t nextForm) {
    return bpp->GetMonsNo() == array_index(SPECIES, "Minior") && !bpp->HENSIN_Check() && bpp->GetFormNo() != (uint8_t)nextForm;
}

bool AdjustShieldsDownForm(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID, bool displayAbiltiy) {
    system_load_typeinfo(0x890f);
    Common::getClass()->initIfNeeded();

    BTL_POKEPARAM::Object* bpp = Common::GetPokeParam(args, pokeID);
    MiniorForm nextForm = FindNextMiniorForm(bpp);
    if (ShieldsDownCanChangeForm(bpp, (uint8_t)nextForm)) {
        HandlerFormChange(args, pokeID, (uint8_t)nextForm, false, displayAbiltiy, true, BtlStrType::BTL_STRTYPE_STD, MiniorFormToShieldsDownStringID(nextForm));
        return true;
    } else {
        return false;
    }
}

void HandlerShieldsDownMemberInEvo(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    system_load_typeinfo(0x8a7a);
    Common::getClass()->initIfNeeded();

    if (Common::GetEventVar(args, EventVar::Label::POKEID) != pokeID)
        return;

    AdjustShieldsDownForm(args, pokeID, true);
}

void HandlerShieldsDownTurnCheckDone(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    AdjustShieldsDownForm(args, pokeID, true);
}

void HandlerShieldsDownAddSickCheckFail(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    BTL_POKEPARAM::Object* bpp = Common::GetPokeParam(args, pokeID);
    if (!IsMeteorMiniorForm(bpp))
        return;

    nn::vector<Pml::WazaData::WazaSick> sicks = {
        Pml::WazaData::WazaSick::WAZASICK_MAHI,
        Pml::WazaData::WazaSick::WAZASICK_NEMURI,
        Pml::WazaData::WazaSick::WAZASICK_KOORI,
        Pml::WazaData::WazaSick::WAZASICK_YAKEDO,
        Pml::WazaData::WazaSick::WAZASICK_DOKU,
        Pml::WazaData::WazaSick::WAZASICK_AKUBI,
    };
    for (auto & sick : sicks) {
        bool guarded = Common::GuardWazaSick(args, pokeID, sick);
        Common::SetWorkValue(args, WorkValue::A, guarded);
        if (guarded)
            return;
    }
}

void HandlerShieldsDownAddSickFailed(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    BTL_POKEPARAM::Object* bpp = Common::GetPokeParam(args, pokeID);
    if (!IsMeteorMiniorForm(bpp))
        return;

    Handler::Tokusei::handler_AddSickFailCommon(args, pokeID);
}

void HandlerShieldsDownCheckInemuri(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    BTL_POKEPARAM::Object* bpp = Common::GetPokeParam(args, pokeID);
    if (!IsMeteorMiniorForm(bpp))
        return;

    Handler::Tokusei::handler_Fumin_InemuriCheck(args, pokeID);
}

EventFactor::EventHandlerTable::Array* ADD_ShieldsDown(uint8_t* prio) {
    EventFactor::EventHandlerTable::Array* table = getExtraAbilityHandlers()->HandlerTable_ShieldsDown;
    if (table == nullptr) {
        table = CreateAbilityEventHandlerTable(5);
        table->m_Items[0] = CreateAbilityEventHandler(EventID::MEMBER_IN_EVO, (Il2CppMethodPointer)&HandlerShieldsDownMemberInEvo);
        table->m_Items[1] = CreateAbilityEventHandler(EventID::TURNCHECK_DONE, (Il2CppMethodPointer)&HandlerShieldsDownTurnCheckDone);
        table->m_Items[2] = CreateAbilityEventHandler(EventID::ADDSICK_CHECKFAIL, (Il2CppMethodPointer)&HandlerShieldsDownAddSickCheckFail);
        table->m_Items[3] = CreateAbilityEventHandler(EventID::ADDSICK_FAILED, (Il2CppMethodPointer)&HandlerShieldsDownAddSickFailed);
        table->m_Items[4] = CreateAbilityEventHandler(EventID::CHECK_INEMURI, (Il2CppMethodPointer)&HandlerShieldsDownCheckInemuri);
        getExtraAbilityHandlers()->HandlerTable_ShieldsDown = table;
    }
    return table;
}

void Handlers_Ability_ShieldsDown(Handler::Tokusei::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetAbilityFunctionTable(getFuncTable, getExtraAbilityHandlers()->currentIndex, array_index(ABILITIES, "Shields Down"), (Il2CppMethodPointer)&ADD_ShieldsDown);
    getExtraAbilityHandlers()->currentIndex++;
}