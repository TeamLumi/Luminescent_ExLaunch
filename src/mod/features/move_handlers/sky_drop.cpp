#include "exlaunch.hpp"

#include "externals/BTL_STRID_SET.h"
#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventLauncher.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/Handler/Waza.h"
#include "externals/Dpr/Battle/Logic/Section_FreeFall_Release.h"
#include "externals/Dpr/Battle/Logic/Section_FromEvent_FreeFallStart.h"
#include "externals/Dpr/Battle/Logic/SectionContainer.h"
#include "externals/Dpr/Battle/Logic/ServerCommandPutter.h"
#include "data/moves.h"
#include "data/types.h"
#include "data/utils.h"
#include "move_handlers.h"

using namespace Dpr::Battle::Logic;

void HandlerSkyDropCheckTameturnFail(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    auto freeFallStartDesc = Section_FromEvent_FreeFallStart::Description::newInstanceMAlloc();
    freeFallStartDesc->fields.attackerID = pokeID;
    freeFallStartDesc->fields.targetID = Common::GetEventVar(args, EventVar::Label::POKEID_DEF);

    auto wp = WazaParam::newInstance();
    int32_t wazaID = Common::GetSubID(args);
    SectionContainer::Object* sc = (*args)->fields.pSectionContainer->instance();
    EventLauncher::Object* el = sc->fields.m_section_FromEvent_FreeFallStart->fields.m_pEventLauncher->instance();
    el->Event_GetWazaParam(wazaID, wazaID, wazaID, 0, Common::GetPokeParam(args, pokeID), wp);
    freeFallStartDesc->fields.wazaParam = wp;

    if (!Common::FreeFallStart(args, &freeFallStartDesc))
        Common::RewriteEventVar(args, EventVar::Label::FAIL_FLAG, 1);

    nn_free(freeFallStartDesc);
}

void HandlerSkyDropNoeffectCheckL2(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    if (!Common::GetPokeParam(args, Common::GetEventVar(args, EventVar::Label::POKEID_DEF))->IsMatchType(array_index(TYPES, "Flying")))
        return;

    Common::RewriteEventVar(args, EventVar::Label::NOEFFECT_FLAG, true);
}

void HandlerSkyDropWazaExeStart(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    system_load_typeinfo(0x77f7);

    auto desc = Section_FreeFall_Release::Description::newInstance();
    desc->fields.attacker = Common::GetPokeParam(args, pokeID);
    desc->fields.canAppearSelf = true;
    desc->fields.canAppearTarget = true;

    Common::FreeFallRelease(args, &desc);
}

EventFactor::EventHandlerTable::Array* ADD_SkyDrop() {
    EventFactor::EventHandlerTable::Array* table = getExtraMoveHandlers()->HandlerTable_SkyDrop;
    if (table == nullptr) {
        table = CreateMoveEventHandlerTable(3);
        table->m_Items[0] = CreateMoveEventHandler(EventID::CHECK_TAMETURN_FAIL, (Il2CppMethodPointer)&HandlerSkyDropCheckTameturnFail);
        table->m_Items[1] = CreateMoveEventHandler(EventID::NOEFFECT_CHECK_L2, (Il2CppMethodPointer)&HandlerSkyDropNoeffectCheckL2);
        table->m_Items[2] = CreateMoveEventHandler(EventID::WAZA_EXE_START, (Il2CppMethodPointer)&HandlerSkyDropWazaExeStart);
        getExtraMoveHandlers()->HandlerTable_SkyDrop = table;
    }
    return table;
}

void Handlers_Move_SkyDrop(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Sky Drop"), (Il2CppMethodPointer)&ADD_SkyDrop);
    getExtraMoveHandlers()->currentIndex++;
}

HOOK_DEFINE_REPLACE(Dpr_Battle_Logic_Section_FromEvent_FreeFallStart_Execute) {
    static void Callback(Section_FromEvent_FreeFallStart::Object* __this,
                            Section_FromEvent_FreeFallStart::Result::Object* result,
                            Section_FromEvent_FreeFallStart::Description::Object** description) {
        result->fields.isSucceeded = false;
        result->fields.isFailMessageDisplayed = false;

        auto attacker = ((Section::Object*)__this)->GetPokeParam((*description)->fields.attackerID);
        auto target = ((Section::Object*)__this)->GetPokeParam((*description)->fields.targetID);
        if (!target->IsDead() && !target->MIGAWARI_IsExist() && !target->IsWazaHide())
        {
            if (__this->getWeight(target) < 2000)
            {
                if (!target->TURNFLAG_Get(BTL_POKEPARAM::TurnFlag::TURNFLG_MAMORU))
                {
                    auto strParam = StrParam::newInstance();
                    if (!__this->fields.m_pEventLauncher->instance()->Event_Check_FreeFallStart_Guard(attacker, target, (*description)->fields.wazaParam, strParam))
                    {
                        auto freeFallCounter = BTL_POKEPARAM::PokeIDtoFreeFallCounter(target->GetID());
                        __this->fields.m_pServerCmdPutter->SetContFlag(attacker, ContFlag::CONTFLG_SORAWOTOBU);
                        __this->fields.m_pServerCmdPutter->SetContFlag(target, ContFlag::CONTFLG_SORAWOTOBU);
                        __this->fields.m_pServerCmdPutter->SetBppCounter(attacker, BTL_POKEPARAM::Counter::COUNTER_FREEFALL, freeFallCounter);
                        __this->setFreeFallSick(attacker, target);
                        result->fields.isSucceeded = true;
                    }
                    else
                    {
                        __this->fields.m_pServerCmdPutter->instance()->Message(&strParam);
                        result->fields.isFailMessageDisplayed = true;
                    }
                }
                else
                {
                    __this->onMamoruSuccess(attacker, target, (*description)->fields.wazaParam);
                    result->fields.isFailMessageDisplayed = true;
                }
            }
            else
            {
                __this->fields.m_pServerCmdPutter->instance()->Message_Set(target, BTL_STRID_SET::FreeFallFail);
                result->fields.isFailMessageDisplayed = true;
            }
        }
    }
};

void InstallHooks_Move_SkyDrop() {
    Dpr_Battle_Logic_Section_FromEvent_FreeFallStart_Execute::InstallAtOffset(0x01fb4620);
}