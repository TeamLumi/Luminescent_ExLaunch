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
    auto *array = (EventFactor::EventHandlerTable::Array*)IM_ALLOC(32 + 8 * size);
    array->max_length = size;
    for (int i = 0; i < size; ++i)
        array->m_Items[i] = nullptr;
    return array;
}

EventFactor::EventHandlerTable::Object* CreateMoveEventHandler(EventID eventID, Il2CppMethodPointer methodPointer) {
    MethodInfo* method = (*Handler::Waza::PTR_Method$$handler_Karagenki_WazaPow)->copyWith(methodPointer);
    auto evtHandler = (EventFactor::EventHandler::Object*)IM_ALLOC(sizeof(EventFactor::EventHandler::Object));
    evtHandler->ctor(0, method);
    evtHandler->fields.delegates = nullptr;
    auto table = (EventFactor::EventHandlerTable::Object*)IM_ALLOC(sizeof(EventFactor::EventHandlerTable::Object));
    table->ctor(eventID, evtHandler);
    return table;
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
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Bide")]) Handlers_Bide(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Mirror Move")]) Handlers_MirrorMove(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Psywave")]) Handlers_Psywave(array);

        ctx->X[0] = (uint64_t)array;
    }
};

void exl_move_handlers_main() {
    Handler_Waza_newGetFunc::InstallAtOffset(0x018298a8);

    SetActivatedMoveHandler(array_index(MOVES, "Jump Kick"));
    SetActivatedMoveHandler(array_index(MOVES, "Sonic Boom"));
    SetActivatedMoveHandler(array_index(MOVES, "Dragon Rage"));
    SetActivatedMoveHandler(array_index(MOVES, "Rage"));
    //SetActivatedMoveHandler(array_index(MOVES, "Bide"));
    SetActivatedMoveHandler(array_index(MOVES, "Mirror Move"));
    SetActivatedMoveHandler(array_index(MOVES, "Psywave"));
    SetActivatedMoveHandler(array_index(MOVES, "Spider Web"));
    SetActivatedMoveHandler(array_index(MOVES, "Nightmare"));
    SetActivatedMoveHandler(array_index(MOVES, "Return"));

    SetActivatedMoveHandler(array_index(MOVES, "Frustration"));
    SetActivatedMoveHandler(array_index(MOVES, "Magnitude"));
    SetActivatedMoveHandler(array_index(MOVES, "Pursuit"));
    SetActivatedMoveHandler(array_index(MOVES, "Smelling Salts"));
    SetActivatedMoveHandler(array_index(MOVES, "Assist"));
    SetActivatedMoveHandler(array_index(MOVES, "Refresh"));
    SetActivatedMoveHandler(array_index(MOVES, "Snatch"));
    SetActivatedMoveHandler(array_index(MOVES, "Secret Power"));
    SetActivatedMoveHandler(array_index(MOVES, "Camouflage"));
    SetActivatedMoveHandler(array_index(MOVES, "Mud Sport"));

    SetActivatedMoveHandler(array_index(MOVES, "Ice Ball"));
    SetActivatedMoveHandler(array_index(MOVES, "Silver Wind"));
    SetActivatedMoveHandler(array_index(MOVES, "Sky Uppercut"));
    SetActivatedMoveHandler(array_index(MOVES, "Water Sport"));
    SetActivatedMoveHandler(array_index(MOVES, "Wake-Up Slap"));
    SetActivatedMoveHandler(array_index(MOVES, "Natural Gift"));
    SetActivatedMoveHandler(array_index(MOVES, "Trump Card"));
    SetActivatedMoveHandler(array_index(MOVES, "Wring Out"));
    SetActivatedMoveHandler(array_index(MOVES, "Lucky Chant"));
    SetActivatedMoveHandler(array_index(MOVES, "Me First"));

    SetActivatedMoveHandler(array_index(MOVES, "Punishment"));
    SetActivatedMoveHandler(array_index(MOVES, "Captivate"));
    SetActivatedMoveHandler(array_index(MOVES, "Ominous Wind"));
    SetActivatedMoveHandler(array_index(MOVES, "Flame Burst"));
    SetActivatedMoveHandler(array_index(MOVES, "Synchronoise"));
    SetActivatedMoveHandler(array_index(MOVES, "Chip Away"));
    SetActivatedMoveHandler(array_index(MOVES, "Sky Drop"));
    SetActivatedMoveHandler(array_index(MOVES, "Bestow"));
    SetActivatedMoveHandler(array_index(MOVES, "Rototiller"));
    SetActivatedMoveHandler(array_index(MOVES, "Ion Deluge"));

    SetActivatedMoveHandler(array_index(MOVES, "Spotlight"));
    SetActivatedMoveHandler(array_index(MOVES, "Guardian of Alola"));
    SetActivatedMoveHandler(array_index(MOVES, "Genesis Supernova"));
    SetActivatedMoveHandler(array_index(MOVES, "Light That Burns the Sky"));
    SetActivatedMoveHandler(array_index(MOVES, "Searing Sunraze Smash"));
    SetActivatedMoveHandler(array_index(MOVES, "Menacing Moonraze Maelstrom"));
    SetActivatedMoveHandler(array_index(MOVES, "Splintered Stormshards"));
    SetActivatedMoveHandler(array_index(MOVES, "Pika Papow"));
    SetActivatedMoveHandler(array_index(MOVES, "Glitzy Glow"));
    SetActivatedMoveHandler(array_index(MOVES, "Baddy Bad"));

    SetActivatedMoveHandler(array_index(MOVES, "Freezy Frost"));
    SetActivatedMoveHandler(array_index(MOVES, "Sparkly Swirl"));
    SetActivatedMoveHandler(array_index(MOVES, "Veevee Volley"));
};