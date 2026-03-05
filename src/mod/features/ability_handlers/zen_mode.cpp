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

DarmanitanForm FindNextDarmanitanForm(BTL_POKEPARAM::Object* bpp) {
    DarmanitanForm currentForm = (DarmanitanForm)bpp->GetFormNo();
    bool isZen = (uint32_t)bpp->GetValue(BTL_POKEPARAM::ValueID::BPP_HP) <= calc::QuotMaxHP_Zero(bpp, 2, false);

    DarmanitanForm nextForm;
    if (currentForm == DarmanitanForm::UNOVAN_STANDARD || currentForm == DarmanitanForm::UNOVAN_ZEN)
        nextForm = isZen ? DarmanitanForm::UNOVAN_ZEN : DarmanitanForm::UNOVAN_STANDARD;
    else if (currentForm == DarmanitanForm::GALARIAN_STANDARD || currentForm == DarmanitanForm::GALARIAN_ZEN)
        nextForm = isZen ? DarmanitanForm::GALARIAN_ZEN : DarmanitanForm::GALARIAN_STANDARD;
    else
        nextForm = currentForm;

    return nextForm;
}

DarmanitanForm FindStandardDarmanitanForm(BTL_POKEPARAM::Object* bpp) {
    DarmanitanForm currentForm = (DarmanitanForm)bpp->GetFormNo();

    DarmanitanForm nextForm;
    if (currentForm == DarmanitanForm::UNOVAN_STANDARD || currentForm == DarmanitanForm::UNOVAN_ZEN)
        nextForm = DarmanitanForm::UNOVAN_STANDARD;
    else if (currentForm == DarmanitanForm::GALARIAN_STANDARD || currentForm == DarmanitanForm::GALARIAN_ZEN)
        nextForm = DarmanitanForm::GALARIAN_STANDARD;
    else
        nextForm = currentForm;

    return nextForm;
}

int32_t DarmanitanFormToZenModeStringID(DarmanitanForm formno) {
    switch (formno)
    {
        case DarmanitanForm::UNOVAN_STANDARD:
        case DarmanitanForm::GALARIAN_STANDARD:
            return BTL_STRID_STD::DarumaOff;

        case DarmanitanForm::UNOVAN_ZEN:
        case DarmanitanForm::GALARIAN_ZEN:
        default:
            return BTL_STRID_STD::DarumaOn;
    }
}

void DisableZenMode(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    BTL_POKEPARAM::Object* bpp = Common::GetPokeParam(args, pokeID);
    DarmanitanForm nextForm = FindStandardDarmanitanForm(bpp);
    if (bpp->GetMonsNo() == array_index(SPECIES, "Darmanitan") && !bpp->HENSIN_Check() && bpp->GetFormNo() != (uint8_t)nextForm)
        HandlerFormChange(args, pokeID, (uint8_t)nextForm, false, false, true, BtlStrType::BTL_STRTYPE_STD, BTL_STRID_STD::DarumaOff);
}

void HandlerZenModeTurnCheckDone(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    system_load_typeinfo(0x890f);
    Common::getClass()->initIfNeeded();

    BTL_POKEPARAM::Object* bpp = Common::GetPokeParam(args, pokeID);
    DarmanitanForm nextForm = FindNextDarmanitanForm(bpp);
    if (bpp->GetMonsNo() == array_index(SPECIES, "Darmanitan") && !bpp->HENSIN_Check() && bpp->GetFormNo() != (uint8_t)nextForm)
        HandlerFormChange(args, pokeID, (uint8_t)nextForm, false, true, true, BtlStrType::BTL_STRTYPE_STD, DarmanitanFormToZenModeStringID(nextForm));
}

void HandlerZenModeTokuseiDisable(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    system_load_typeinfo(0x8a73);
    Common::getClass()->initIfNeeded();

    if (Common::GetEventVar(args, EventVar::Label::POKEID) != pokeID)
        return;

    DisableZenMode(args, pokeID);
}

void HandlerZenModeChangeTokuseiBefore(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    system_load_typeinfo(0x8a70);
    Common::getClass()->initIfNeeded();

    if (Common::GetEventVar(args, EventVar::Label::POKEID) != pokeID)
        return;

    if (Common::GetEventVar(args, EventVar::Label::TOKUSEI_NEXT) == Common::GetSubID(args))
        return;

    DisableZenMode(args, pokeID);
}

EventFactor::EventHandlerTable::Array* ADD_ZenMode(uint8_t* prio) {
    EventFactor::EventHandlerTable::Array* table = getExtraAbilityHandlers()->HandlerTable_ZenMode;
    if (table == nullptr) {
        table = CreateAbilityEventHandlerTable(3);
        table->m_Items[0] = CreateAbilityEventHandler(EventID::TURNCHECK_DONE, (Il2CppMethodPointer)&HandlerZenModeTurnCheckDone);
        table->m_Items[1] = CreateAbilityEventHandler(EventID::TOKUSEI_DISABLE, (Il2CppMethodPointer)&HandlerZenModeTokuseiDisable);
        table->m_Items[2] = CreateAbilityEventHandler(EventID::CHANGE_TOKUSEI_BEFORE, (Il2CppMethodPointer)&HandlerZenModeChangeTokuseiBefore);
        getExtraAbilityHandlers()->HandlerTable_ZenMode = table;
    }
    return table;
}

void Handlers_Move_ZenMode(Handler::Tokusei::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetAbilityFunctionTable(getFuncTable, getExtraAbilityHandlers()->currentIndex, array_index(ABILITIES, "Zen Mode"), (Il2CppMethodPointer)&ADD_ZenMode);
    getExtraAbilityHandlers()->currentIndex++;
}