#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventLauncher.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/Handler/Waza.h"
#include "externals/Dpr/Battle/Logic/Section_FromEvent_FreeFallStart.h"
#include "externals/Dpr/Battle/Logic/SectionContainer.h"
#include "data/moves.h"
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

}

void HandlerSkyDropWazaExeStart(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {

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