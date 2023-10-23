#include "features/move_handlers/move_handlers.h"

#include "data/moves.h"
#include "data/utils.h"
#include "logger/logger.h"

static ExtraMoveHandlers extraMoveHandlers {};
static bool ACTIVATED_MOVE_HANDLERS[MOVE_COUNT];

void SetActivatedMoveHandler(int32_t move)
{
    ACTIVATED_MOVE_HANDLERS[move] = true;
    getExtraMoveHandlers()->count++;
}

ExtraMoveHandlers* getExtraMoveHandlers() {
    return &extraMoveHandlers;
}

using namespace Dpr::Battle::Logic;
EventFactor::EventHandlerTable::Array* CreateEventHandlerTable(long size) {
    return (EventFactor::EventHandlerTable::Array*)system_array_new(EventFactor::EventHandlerTable_array_TypeInfo(), size);
}

EventFactor::EventHandlerTable::Object* CreateMoveEventHandler(EventID eventID, Il2CppMethodPointer methodPointer) {
    MethodInfo* method = (*Handler::Waza::PTR_Method$$handler_Karagenki_WazaPow)->copyWith(methodPointer);
    auto evtHandler = EventFactor::EventHandler::newInstance(0, method);
    evtHandler->fields.delegates = nullptr;
    return EventFactor::EventHandlerTable::newInstance(eventID, evtHandler);
}

EventFactor::EventHandlerTable::Object* CreateMoveEventHandler(EventID eventID, MethodInfo* method) {
    auto evtHandler = EventFactor::EventHandler::newInstance(0, method);
    evtHandler->fields.delegates = nullptr;
    return EventFactor::EventHandlerTable::newInstance(eventID, evtHandler);
}

void SetMoveFunctionTable(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable, uint64_t index, int32_t wazaNo, Il2CppMethodPointer methodPointer) {
    MethodInfo* method = (*Handler::Waza::PTR_Method$$ADD_Karagenki)->copyWith(methodPointer);
    Handler::Waza::GET_FUNC_TABLE_ELEM::Object* elem = &getFuncTable->m_Items[index];
    auto func = Handler::Waza::HandlerGetFunc::newInstance((intptr_t)methodPointer, method);
    elem->fields.waza = wazaNo;
    elem->fields.func = func;
}

HOOK_DEFINE_INLINE(Handler_Waza_newGetFunc) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto typeInfo = (Il2CppClass*)ctx->X[0];
        uint64_t size = ctx->X[1];

        auto array = (Handler::Waza::GET_FUNC_TABLE_ELEM::Array*)system_array_new(typeInfo, size + getExtraMoveHandlers()->count);
        getExtraMoveHandlers()->currentIndex = size;

        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Jump Kick")]) Handlers_JumpKick(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Sonic Boom")]) Handlers_SonicBoom(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Dragon Rage")]) Handlers_DragonRage(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Rage")]) Handlers_Rage(array);

        ctx->X[0] = (uint64_t)array;
    }
};

void exl_move_handlers_main() {
    Handler_Waza_newGetFunc::InstallAtOffset(0x018298a8);

    SetActivatedMoveHandler(array_index(MOVES, "Jump Kick"));
    SetActivatedMoveHandler(array_index(MOVES, "Sonic Boom"));
    SetActivatedMoveHandler(array_index(MOVES, "Dragon Rage"));
    SetActivatedMoveHandler(array_index(MOVES, "Rage"));
};