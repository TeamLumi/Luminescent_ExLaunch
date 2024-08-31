#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/BattleEnv.h"
#include "externals/Dpr/Battle/Logic/BtlEscapeMode.h"
#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/ContFlag.h"
#include "externals/Dpr/Battle/Logic/EventLauncher.h"
#include "externals/Dpr/Battle/Logic/EventSystem.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/Handler/Waza.h"
#include "externals/Dpr/Battle/Logic/InterruptCode.h"
#include "externals/Dpr/Battle/Logic/MainModule.h"
#include "externals/Dpr/Battle/Logic/PokeActionCategory.h"
#include "externals/Dpr/Battle/Logic/PokeActionContainer.h"
#include "externals/Dpr/Battle/Logic/PokeSet.h"
#include "externals/Dpr/Battle/Logic/Section_ProcessActionCore.h"
#include "externals/Dpr/Battle/Logic/SectionContainer.h"
#include "externals/Dpr/Battle/Logic/SectionSharedData.h"
#include "externals/Dpr/Battle/Logic/ServerCommandPutter.h"
#include "externals/Dpr/Battle/Logic/WazaEffectParams.h"
#include "externals/Dpr/Battle/Logic/WazaParam.h"
#include "data/moves.h"
#include "data/utils.h"
#include "move_handlers.h"

using namespace Dpr::Battle::Logic;

void HandlerPursuitWazaExeStart(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID) != pokeID)
        return;

    if (!Common::GetEventVar(args, EventVar::Label::OIUTI_INTERRUPT_FLAG))
        return;

    Common::SetWorkValue(args, WorkValue::A, true);
}

void HandlerPursuitCalcHitCancel(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (!Common::GetWorkValue(args, WorkValue::A))
        return;

    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    Common::RewriteEventVar(args, EventVar::Label::GEN_FLAG, true);
}

void HandlerPursuitWazaPower(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (!Common::GetWorkValue(args, WorkValue::A))
        return;

    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    Common::RewriteEventVar(args, EventVar::Label::WAZA_POWER, Common::GetEventVar(args, EventVar::Label::WAZA_POWER) * 2);
}

EventFactor::EventHandlerTable::Array* ADD_Pursuit() {
    EventFactor::EventHandlerTable::Array* table = getExtraMoveHandlers()->HandlerTable_Pursuit;
    if (table == nullptr) {
        table = CreateMoveEventHandlerTable(3);
        table->m_Items[0] = CreateMoveEventHandler(EventID::WAZA_EXE_START, (Il2CppMethodPointer)&HandlerPursuitWazaExeStart);
        table->m_Items[1] = CreateMoveEventHandler(EventID::CALC_HIT_CANCEL, (Il2CppMethodPointer)&HandlerPursuitCalcHitCancel);
        table->m_Items[2] = CreateMoveEventHandler(EventID::WAZA_POWER, (Il2CppMethodPointer)&HandlerPursuitWazaPower);
        getExtraMoveHandlers()->HandlerTable_Pursuit = table;
    }
    return table;
}

void Handlers_Move_Pursuit(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Pursuit"), (Il2CppMethodPointer)&ADD_Pursuit);
    getExtraMoveHandlers()->currentIndex++;
}

void PursuitInterrupt(Section_ProcessActionCore::Object* section, PokeAction::Object* action, uint8_t targetPokeID) {
    if (((Section::Object*)section)->CheckEncoreWazaChange(action) != array_index(MOVES, "-MOVE ZERO-"))
        return;

    action->fields.actionDesc->fields.isOiutiInterruptAction = true;
    action->fields.actionParam_Fight->fields.targetPos = ((Section::Object*)section)->GetPokePos(targetPokeID);
    section->fields.m_pSectionContainer->instance()->fields.m_section_InterruptAction->processAction(action);
}

void PursuitProcess(Section_ProcessActionCore::Object* section, PokeActionContainer::Object* pokeActionContainer, PokeAction::Object* thisAction) {
    bool isPursuitTarget = thisAction->fields.actionCategory == PokeActionCategory::PokeChange;
    bool isFight = thisAction->fields.actionCategory == PokeActionCategory::Fight;
    if (!isPursuitTarget && isFight) {
        int32_t wazaID = thisAction->fields.actionParam_Fight->fields.waza;
        isPursuitTarget = wazaID == array_index(MOVES, "U-turn") ||
                          wazaID == array_index(MOVES, "Volt Switch") ||
                          wazaID == array_index(MOVES, "Parting Shot") ||
                          wazaID == array_index(MOVES, "Flip Turn");
    }

    if (!isPursuitTarget)
        return;

    uint8_t thisPokeID = thisAction->fields.bpp->GetID();
    for (int i = 0; i < pokeActionContainer->fields.m_count; i++) {
        PokeAction::Object* action = pokeActionContainer->fields.m_actions->m_Items[i];
        if (action->fields.fDone || action->fields.actionCategory != PokeActionCategory::Fight)
            continue;

        PokeActionParam_Fight::Object* papf = action->fields.actionParam_Fight;
        if (papf->fields.waza != array_index(MOVES, "Pursuit"))
            continue;

        uint8_t pokeID = action->fields.bpp->GetID();
        if (!isFight && section->fields.m_pMainModule->IsFriendPokeID(thisPokeID, pokeID))
            continue;

        if (isFight && papf->fields.aimTargetID != thisPokeID)
            continue;

        PursuitInterrupt(section, action, thisPokeID);
    }
}

HOOK_DEFINE_REPLACE(Dpr_Battle_Logic_Section_ProcessActionCore_Execute) {
    static void Callback(Section_ProcessActionCore::Object* __this, Section_ProcessActionCore::Result::Object* pResult, Section_ProcessActionCore::Description::Object** description) {
        PokeAction::Object* pa = (*description)->fields.pokeAction;
        if (pa->fields.fDone)
            return;

        pa->fields.fDone = true;
        PursuitProcess(__this, __this->fields.m_pPokemonActionContainer->instance(), pa);
        PokeActionCategory pac = pa->fields.actionCategory;
        if (pac == PokeActionCategory::Cheer) {
            __this->action_Cheer(pa);
            return;
        }

        BTL_POKEPARAM::Object* bpp = pa->fields.bpp;
        if (bpp == nullptr || bpp->IsDead())
            return;

        uint8_t pokeID = bpp->GetID();
        if (!__this->fields.m_pBattleEnv->instance()->fields.m_posPoke->IsExist(pokeID))
            return;

        ServerCommandPutter::Object* scp = __this->fields.m_pServerCmdPutter->instance();
        EventLauncher::Object* el = __this->fields.m_pEventLauncher->instance();
        if (bpp->CheckSick(Pml::WazaData::WazaSick::WAZASICK_FREEFALL) &&
                    (pac != PokeActionCategory::Escape ||
                        __this->fields.m_pMainModule->instance()->GetEscapeMode() != BtlEscapeMode::BTL_ESCAPE_MODE_CONFIRM)) {
            if (bpp->COUNTER_Get(BTL_POKEPARAM::Counter::COUNTER_MAMORU) != 0)
                scp->SetBppCounter(bpp,BTL_POKEPARAM::Counter::COUNTER_MAMORU, 0);

            el->Event_ActProcCanceled(pa);
            return;
        }

        __this->reinforceRaidBoss(pa);
        ActionDesc::Object* ad = pa->fields.actionDesc;
        if (ad->fields.isOdorikoReaction ||
            ad->fields.isSaihaiReaction) {
            InsertActionInfo::Object *iaf = (ad->fields).insertInfo;
            scp->InsertWazaInfo(pokeID, iaf->fields.isTokuseiWindowDisplay, &iaf->fields.prevActionMessage);
        }
        el->Event_ActProcStart(pa);
        bpp->TURNFLAG_Set(BTL_POKEPARAM::TurnFlag::TURNFLG_ACTION_START);
        __this->action(pa);
        if (pac < PokeActionCategory::PokeChange) {
            bpp->TURNFLAG_Set(BTL_POKEPARAM::TurnFlag::TURNFLG_ACTION_DONE);
            scp->SetContFlag(bpp, ContFlag::CONTFLG_ACTION_DONE);
        }

        if (__this->fields.m_pSharedData->instance()->fields.m_interruptAccessor->IsRequested(InterruptCode::QUIT_ESCAPE))
            return;

        el->Event_ActProcEnd(bpp, pac);
    }
};

HOOK_DEFINE_REPLACE(Dpr_Battle_Logic_EventLauncher_Event_WazaExecuteStart) {
    static uint8_t Callback(EventLauncher::Object* __this, ActionDesc::Object* actionDesc, BTL_POKEPARAM::Object* attacker,
                         WazaParam::Object* wazaParam, PokeSet::Object* rec, WazaEffectParams::Object* pWazaEffectParams) {
        uint8_t pokeCount = rec->fields.m_count;
        EventSystem::Object* es = __this->fields.m_pEventSystem->instance();

        es->EVENTVAR_Push();
        uint8_t pokeID = attacker->GetID();
        es->EVENTVAR_SetConstValue(EventVar::Label::POKEID, pokeID);
        es->EVENTVAR_SetConstValue(EventVar::Label::POKEID_ATK, pokeID);
        es->EVENTVAR_SetConstValue(EventVar::Label::WAZAID, wazaParam->fields.wazaID);
        es->EVENTVAR_SetConstValue(EventVar::Label::WAZA_TYPE, wazaParam->fields.wazaType);
        es->EVENTVAR_SetConstValue(EventVar::Label::OIUTI_INTERRUPT_FLAG, actionDesc->fields.isOiutiInterruptAction);
        es->EVENTVAR_SetConstValue(EventVar::Label::ACTION_DESC_IS_YOKODORI_ROB_ACTION, actionDesc->fields.isYokodoriRobAction);
        es->EVENTVAR_SetConstValue(EventVar::Label::ACTION_DESC_IS_ODORIKO_REACTION, actionDesc->fields.isOdorikoReaction);
        es->EVENTVAR_SetConstValue(EventVar::Label::TARGET_POKECNT, pokeCount);

        for (uint8_t i=0; i<pokeCount; i++) {
            auto target = (EventVar::Label)(i + (uint16_t)EventVar::Label::POKEID_TARGET1);
            es->EVENTVAR_SetConstValue(target, rec->Get(i)->GetID());
        }

        es->EVENTVAR_SetRewriteOnceValue(EventVar::Label::ENABLE_MODE, 0);
        es->EVENTVAR_SetRewriteOnceValue(EventVar::Label::EFFECT_WAZAID, array_index(MOVES, "-MOVE ZERO-"));
        es->CallEvent(EventID::WAZA_EXE_START);
        int32_t enableMode = es->EVENTVAR_GetValue(EventVar::Label::ENABLE_MODE);
        int32_t effectWazaID = es->EVENTVAR_GetValue(EventVar::Label::EFFECT_WAZAID);
        es->EVENTVAR_Pop();

        if (effectWazaID != array_index(MOVES, "-MOVE ZERO-"))
            pWazaEffectParams->fields.effectWazaID = effectWazaID;

        return (uint8_t)enableMode;
    }
};

void InstallHooks_Move_Pursuit() {
    Dpr_Battle_Logic_Section_ProcessActionCore_Execute::InstallAtOffset(0x021c0460);
    Dpr_Battle_Logic_EventLauncher_Event_WazaExecuteStart::InstallAtOffset(0x01d1cc90);
}