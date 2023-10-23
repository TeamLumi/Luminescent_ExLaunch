#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/Handler/Waza.h"
#include "externals/Dpr/Battle/Logic/PokeID.h"
#include "externals/Dpr/Battle/Logic/SICKCONT.h"
#include "externals/Dpr/Battle/Logic/WazaFailCause.h"
#include "data/moves.h"
#include "data/utils.h"
#include "move_handlers.h"
#include "utils/utils.h"

using namespace Dpr::Battle::Logic;

void HandlerBideCheckDelayWaza(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    Logger::log("HandlerBideCheckDelayWaza\n");
    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;
    
    if (Common::GetWorkValue(args, WorkValue::A) >= 2)
        return;

    Common::RewriteEventVar(args, EventVar::Label::GEN_FLAG, 1);
    Common::RewriteEventVar(args, EventVar::Label::SUCCESS_FLAG, 1);
}

void HandlerBideWazaExecuteCheck3rd(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    Logger::log("HandlerBideWazaExecuteCheck3rd\n");
    if (Common::GetEventVar(args, EventVar::Label::POKEID) != pokeID)
        return;

    if (Common::GetWorkValue(args, WorkValue::A) < 2)
        return;

    if (Common::GetWorkValue(args, WorkValue::B) > 0)
        return;

    Common::RewriteEventVar(args, EventVar::Label::FAIL_CAUSE, (int32_t)WazaFailCause::OTHER);
}

void HandlerBideWazaCallDecide(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    Logger::log("HandlerBideWazaCallDecide\n");
    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    BTL_POKEPARAM::Object* bpp = Common::GetPokeParam(args, pokeID);

    if (bpp->CheckSick(Pml::WazaData::WazaSick::WAZASICK_WAZALOCK))
        return;

    if (bpp->CheckNemuri(BTL_POKEPARAM::NemuriCheckMode::NEMURI_CHECK_ONLY_SICK))
        return;

    if (Common::GetWorkValue(args, WorkValue::PERSIST))
        return;

    HandlerAddSick(args, pokeID, pokeID, Pml::WazaData::WazaSick::WAZASICK_WAZALOCK, SICKCONT::MakeTurnParam(pokeID, 3, Common::GetSubID(args)).fields.raw);

    Common::SetWorkValue(args, WorkValue::PERSIST, 1); //Active flag
    Common::SetWorkValue(args, WorkValue::A, 0); //Turn counter
    Common::SetWorkValue(args, WorkValue::B, 0); //Damage taken
    Common::SetWorkValue(args, WorkValue::C, PokeID::INVALID); //Target
}

void HandlerBideDecideTarget(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    Logger::log("HandlerBideDecideTarget\n");
    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    Common::RewriteEventVar(args, EventVar::Label::POKEID_DEF, Common::GetWorkValue(args, WorkValue::C));
}

void HandlerBideWazaDmgProc1(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    Logger::log("HandlerBideWazaDmgProc1\n");
    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    Common::RewriteEventVar(args, EventVar::Label::FIX_DAMAGE_FLAG, 1);
    Common::RewriteEventVar(args, EventVar::Label::FIX_DAMAGE, Common::GetWorkValue(args, WorkValue::B) * 2);

    Handler::Waza::common_Korogaru_Unlock(args, pokeID);
}

void HandlerBideWazaDmgReaction(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    Logger::log("HandlerBideWazaDmgReaction\n");
    if (Common::GetEventVar(args, EventVar::Label::POKEID_DEF) != pokeID)
        return;

    if (!Common::GetWorkValue(args, WorkValue::PERSIST))
        return;

    Common::SetWorkValue(args, WorkValue::B, Common::GetWorkValue(args, WorkValue::B) + Common::GetEventVar(args, EventVar::Label::DAMAGE));
    Common::SetWorkValue(args, WorkValue::C, Common::GetEventVar(args, EventVar::Label::POKEID_ATK));
}

void HandlerBideTurncheckEnd(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    Logger::log("HandlerBideTurncheckEnd\n");
    if (Common::GetEventVar(args, EventVar::Label::POKEID) != pokeID)
        return;

    if (!Common::GetWorkValue(args, WorkValue::PERSIST))
        return;

    int32_t turnCounter = Common::GetWorkValue(args, WorkValue::A) + 1;
    Common::SetWorkValue(args, WorkValue::A, turnCounter);

    if (turnCounter < 3 && !Common::GetPokeParam(args, pokeID)->CheckNemuri(BTL_POKEPARAM::NemuriCheckMode::NEMURI_CHECK_ONLY_SICK))
        return;

    Common::SetWorkValue(args, WorkValue::A, 0);
    Common::SetWorkValue(args, WorkValue::B, 0);
    Handler::Waza::common_Korogaru_Unlock(args, pokeID);
}

EventFactor::EventHandlerTable::Array* ADD_Bide() {
    EventFactor::EventHandlerTable::Array* table = getExtraMoveHandlers()->HandlerTable_Bide;
    if (table == nullptr) {
        table = CreateEventHandlerTable(7);
        table->m_Items[0] = CreateMoveEventHandler(EventID::CHECK_DELAY_WAZA, (Il2CppMethodPointer)&HandlerBideCheckDelayWaza);
        table->m_Items[1] = CreateMoveEventHandler(EventID::WAZA_EXECUTE_CHECK_3RD, (Il2CppMethodPointer)&HandlerBideWazaExecuteCheck3rd);
        table->m_Items[2] = CreateMoveEventHandler(EventID::WAZA_CALL_DECIDE, (Il2CppMethodPointer)&HandlerBideWazaCallDecide);
        table->m_Items[3] = CreateMoveEventHandler(EventID::DECIDE_TARGET, (Il2CppMethodPointer)&HandlerBideDecideTarget);
        table->m_Items[4] = CreateMoveEventHandler(EventID::WAZA_DMG_PROC1, (Il2CppMethodPointer)&HandlerBideWazaDmgProc1);
        table->m_Items[5] = CreateMoveEventHandler(EventID::WAZA_DMG_REACTION, (Il2CppMethodPointer)&HandlerBideWazaDmgReaction);
        table->m_Items[6] = CreateMoveEventHandler(EventID::TURNCHECK_END, (Il2CppMethodPointer)&HandlerBideTurncheckEnd);
        getExtraMoveHandlers()->HandlerTable_Bide = table;
    }
    return table;
}

void Handlers_Bide(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Bide"), (Il2CppMethodPointer)&ADD_Bide);
    getExtraMoveHandlers()->currentIndex++;
}