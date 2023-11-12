#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/calc.h"
#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/Handler/Waza.h"
#include "data/moves.h"
#include "data/utils.h"
#include "move_handlers.h"

using namespace Dpr::Battle::Logic;

// Possibly just move this to a move flag?
static constexpr int32_t ASSIST_UNCALLABLE[] = {
    array_index(MOVES, "Baneful Bunker"),
    array_index(MOVES, "Beak Blast"),
    array_index(MOVES, "Belch"),
    array_index(MOVES, "Bestow"),
    array_index(MOVES, "Bounce"),
    array_index(MOVES, "Celebrate"),
    array_index(MOVES, "Chatter"),
    array_index(MOVES, "Circle Throw"),
    array_index(MOVES, "Copycat"),
    array_index(MOVES, "Counter"),
    array_index(MOVES, "Covet"),
    array_index(MOVES, "Destiny Bond"),
    array_index(MOVES, "Detect"),
    array_index(MOVES, "Dig"),
    array_index(MOVES, "Dive"),
    array_index(MOVES, "Dragon Tail"),
    array_index(MOVES, "Endure"),
    array_index(MOVES, "Feint"),
    array_index(MOVES, "Fly"),
    array_index(MOVES, "Focus Punch"),
    array_index(MOVES, "Follow Me"),
    array_index(MOVES, "Helping Hand"),
    array_index(MOVES, "Hold Hands"),
    array_index(MOVES, "King's Shield"),
    array_index(MOVES, "Mat Block"),
    array_index(MOVES, "Me First"),
    array_index(MOVES, "Metronome"),
    array_index(MOVES, "Mimic"),
    array_index(MOVES, "Mirror Coat"),
    array_index(MOVES, "Mirror Move"),
    array_index(MOVES, "Nature Power"),
    array_index(MOVES, "Phantom Force"),
    array_index(MOVES, "Protect"),
    array_index(MOVES, "Rage Powder"),
    array_index(MOVES, "Roar"),
    array_index(MOVES, "Shadow Force"),
    array_index(MOVES, "Shell Trap"),
    array_index(MOVES, "Sketch"),
    array_index(MOVES, "Sky Drop"),
    array_index(MOVES, "Sleep Talk"),
    array_index(MOVES, "Snatch"),
    array_index(MOVES, "Spiky Shield"),
    array_index(MOVES, "Spotlight"),
    array_index(MOVES, "Struggle"),
    array_index(MOVES, "Switcheroo"),
    array_index(MOVES, "Thief"),
    array_index(MOVES, "Transform"),
    array_index(MOVES, "Trick"),
    array_index(MOVES, "Whirlwind"),
};

bool isCallableByAssist(int32_t move) {
    for (int i: ASSIST_UNCALLABLE)
        if (i == move)
            return false;

    return true;
}

void HandlerAssistReqwazaParam(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::GetEventVar(args, EventVar::Label::POKEID) != pokeID)
        return;

    const int32_t maxCandidateWazaIDCount = 20;
    int32_t candidateWazaIDs[maxCandidateWazaIDCount] = {};
    int32_t candidateWazaIDCount = 0;

    BTL_PARTY::Object* party = Common::GetPartyData(args, pokeID);
    uint8_t memberCount = party->fields.m_memberCount;
    for (int i=0; i<memberCount; i++) {
        BTL_POKEPARAM::Object* memberBPP = party->GetMemberDataConst(i);
        if (memberBPP->GetID() == pokeID)
            continue;

        uint8_t wazaCount = memberBPP->fields.m_wazaCnt;
        for (int j=0; j<wazaCount; j++) {
            int32_t candidateWazaID = memberBPP->WAZA_GetID(j);
            if (isCallableByAssist(candidateWazaID) && candidateWazaIDCount < maxCandidateWazaIDCount) {
                candidateWazaIDs[candidateWazaIDCount] = candidateWazaID;
                candidateWazaIDCount++;
            }
        }
    }

    if (candidateWazaIDCount == 0)
        return;

    int32_t targetWazaID = candidateWazaIDs[calc::GetRand(candidateWazaIDCount)];
    Common::RewriteEventVar(args, EventVar::Label::WAZAID, targetWazaID);
    Common::RewriteEventVar(args, EventVar::Label::POKEPOS, Common::DecideWazaTargetAuto(args, pokeID, targetWazaID));
}

EventFactor::EventHandlerTable::Array* ADD_Assist() {
    EventFactor::EventHandlerTable::Array* table = getExtraMoveHandlers()->HandlerTable_Assist;
    if (table == nullptr) {
        table = CreateEventHandlerTable(1);
        table->m_Items[0] = CreateMoveEventHandler(EventID::REQWAZA_PARAM, (Il2CppMethodPointer)&HandlerAssistReqwazaParam);
        getExtraMoveHandlers()->HandlerTable_Assist = table;
    }
    return table;
}

void Handlers_Assist(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Assist"), (Il2CppMethodPointer)&ADD_Assist);
    getExtraMoveHandlers()->currentIndex++;
}