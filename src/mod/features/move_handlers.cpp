#include "features/move_handlers/move_handlers.h"

#include "data/moves.h"
#include "data/utils.h"
#include "logger/logger.h"

static ExtraMoveHandlers extraMoveHandlers {};
static bool ACTIVATED_MOVE_HANDLERS[MOVE_COUNT];

void SetActivatedMoveHandlers(int32_t move)
{
    ACTIVATED_MOVE_HANDLERS[move] = true;
    getExtraMoveHandlers()->count++;
}

ExtraMoveHandlers* getExtraMoveHandlers() {
    return &extraMoveHandlers;
}

using namespace Dpr::Battle::Logic;
EventFactor::EventHandlerTable::Array* CreateMoveEventHandlerTable(long size) {
    return EventFactor::EventHandlerTable::newArrayMAlloc(size);
}

EventFactor::EventHandlerTable::Object* CreateMoveEventHandler(EventID eventID, Il2CppMethodPointer methodPointer) {
    MethodInfo* method = (*Handler::Waza::PTR_Method$$handler_Karagenki_WazaPow)->copyWith(methodPointer);
    auto evtHandler = EventFactor::EventHandler::newInstanceMAlloc(0, method);
    evtHandler->fields.delegates = nullptr;
    return EventFactor::EventHandlerTable::newInstanceMAlloc(eventID, evtHandler);
}

EventFactor::EventHandlerTable::Object* CreateMoveEventHandler(EventID eventID, MethodInfo* method) {
    auto evtHandler = EventFactor::EventHandler::newInstanceMAlloc(0, method);
    evtHandler->fields.delegates = nullptr;
    return EventFactor::EventHandlerTable::newInstanceMAlloc(eventID, evtHandler);
}

void SetMoveFunctionTable(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable, uint64_t index, int32_t wazaNo, Il2CppMethodPointer methodPointer) {
    MethodInfo* method = (*Handler::Waza::PTR_Method$$ADD_Karagenki)->copyWith(methodPointer);
    Handler::Waza::GET_FUNC_TABLE_ELEM::Object* elem = &getFuncTable->m_Items[index];
    auto func = Handler::Waza::HandlerGetFunc::newInstance(0, method);
    elem->ctor(wazaNo, func);
}

void SetMoveFunctionTable(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable, uint64_t index, int32_t wazaNo, MethodInfo* method) {
    Handler::Waza::GET_FUNC_TABLE_ELEM::Object* elem = &getFuncTable->m_Items[index];
    auto func = Handler::Waza::HandlerGetFunc::newInstance(0, method);
    elem->ctor(wazaNo, func);
}

HOOK_DEFINE_INLINE(Handler_Waza_newGetFunc) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto typeInfo = (Il2CppClass*)ctx->X[0];
        uint64_t size = ctx->X[1];

        auto array = (Handler::Waza::GET_FUNC_TABLE_ELEM::Array*)system_array_new(typeInfo, size + getExtraMoveHandlers()->count);
        getExtraMoveHandlers()->currentIndex = size;

        // DO NOT REMOVE ANY OF THESE! Disable the moves in exl_move_handlers_main() below instead!
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Jump Kick")]) Handlers_Move_JumpKick(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Sonic Boom")]) Handlers_Move_SonicBoom(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Dragon Rage")]) Handlers_Move_DragonRage(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Rage")]) Handlers_Move_Rage(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Bide")]) Handlers_Move_Bide(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Mirror Move")]) Handlers_Move_MirrorMove(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Psywave")]) Handlers_Move_Psywave(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Spider Web")]) Handlers_Move_SpiderWeb(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Nightmare")]) Handlers_Move_Nightmare(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Return")]) Handlers_Move_Return(array);

        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Frustration")]) Handlers_Move_Frustration(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Magnitude")]) Handlers_Move_Magnitude(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Pursuit")]) Handlers_Move_Pursuit(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Smelling Salts")]) Handlers_Move_SmellingSalts(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Assist")]) Handlers_Move_Assist(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Refresh")]) Handlers_Move_Refresh(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Snatch")]) Handlers_Move_Snatch(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Secret Power")]) Handlers_Move_SecretPower(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Camouflage")]) Handlers_Move_Camouflage(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Mud Sport")]) Handlers_Move_MudSport(array);

        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Ice Ball")]) Handlers_Move_IceBall(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Silver Wind")]) Handlers_Move_SilverWind(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Sky Uppercut")]) Handlers_Move_SkyUppercut(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Water Sport")]) Handlers_Move_WaterSport(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Wake-Up Slap")]) Handlers_Move_WakeUpSlap(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Natural Gift")]) Handlers_Move_NaturalGift(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Trump Card")]) Handlers_Move_TrumpCard(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Wring Out")]) Handlers_Move_WringOut(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Lucky Chant")]) Handlers_Move_LuckyChant(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Me First")]) Handlers_Move_MeFirst(array);

        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Punishment")]) Handlers_Move_Punishment(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Captivate")]) Handlers_Move_Captivate(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Ominous Wind")]) Handlers_Move_OminousWind(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Flame Burst")]) Handlers_Move_FlameBurst(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Synchronoise")]) Handlers_Move_Synchronoise(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Chip Away")]) Handlers_Move_ChipAway(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Sky Drop")]) Handlers_Move_SkyDrop(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Bestow")]) Handlers_Move_Bestow(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Rototiller")]) Handlers_Move_Rototiller(array);
        if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Ion Deluge")]) Handlers_Move_IonDeluge(array);

        ctx->X[0] = (uint64_t)array;
    }
};

void InstallActivatedMoveHooks() {
    // DO NOT REMOVE ANY OF THESE! Disable the moves in exl_move_handlers_main() below instead!
    if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Thief")]) InstallHooks_Move_Thief();
    if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Pursuit")]) InstallHooks_Move_Pursuit();
    if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Sky Drop")]) InstallHooks_Move_SkyDrop();
    if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Relic Song")]) InstallHooks_Move_RelicSong();
    if (ACTIVATED_MOVE_HANDLERS[array_index(MOVES, "Hyperspace Fury")]) InstallHooks_Move_HyperspaceFury();
}

void exl_move_handlers_main() {
    Handler_Waza_newGetFunc::InstallAtOffset(0x018298a8);

    SetActivatedMoveHandlers(array_index(MOVES, "Jump Kick"));
    SetActivatedMoveHandlers(array_index(MOVES, "Sonic Boom"));
    SetActivatedMoveHandlers(array_index(MOVES, "Dragon Rage"));
    SetActivatedMoveHandlers(array_index(MOVES, "Rage"));
    //SetActivatedMoveHandlers(array_index(MOVES, "Bide"));
    SetActivatedMoveHandlers(array_index(MOVES, "Mirror Move"));
    SetActivatedMoveHandlers(array_index(MOVES, "Psywave"));
    SetActivatedMoveHandlers(array_index(MOVES, "Thief"));
    SetActivatedMoveHandlers(array_index(MOVES, "Spider Web"));
    SetActivatedMoveHandlers(array_index(MOVES, "Nightmare"));

    SetActivatedMoveHandlers(array_index(MOVES, "Return"));
    SetActivatedMoveHandlers(array_index(MOVES, "Frustration"));
    SetActivatedMoveHandlers(array_index(MOVES, "Magnitude"));
    SetActivatedMoveHandlers(array_index(MOVES, "Pursuit"));
    SetActivatedMoveHandlers(array_index(MOVES, "Smelling Salts"));
    SetActivatedMoveHandlers(array_index(MOVES, "Assist"));
    SetActivatedMoveHandlers(array_index(MOVES, "Refresh"));
    SetActivatedMoveHandlers(array_index(MOVES, "Snatch"));
    SetActivatedMoveHandlers(array_index(MOVES, "Secret Power"));
    SetActivatedMoveHandlers(array_index(MOVES, "Camouflage"));

    SetActivatedMoveHandlers(array_index(MOVES, "Mud Sport"));
    SetActivatedMoveHandlers(array_index(MOVES, "Ice Ball"));
    SetActivatedMoveHandlers(array_index(MOVES, "Silver Wind"));
    SetActivatedMoveHandlers(array_index(MOVES, "Sky Uppercut"));
    SetActivatedMoveHandlers(array_index(MOVES, "Water Sport"));
    SetActivatedMoveHandlers(array_index(MOVES, "Wake-Up Slap"));
    SetActivatedMoveHandlers(array_index(MOVES, "Natural Gift"));
    SetActivatedMoveHandlers(array_index(MOVES, "Trump Card"));
    SetActivatedMoveHandlers(array_index(MOVES, "Wring Out"));
    SetActivatedMoveHandlers(array_index(MOVES, "Lucky Chant"));

    SetActivatedMoveHandlers(array_index(MOVES, "Me First"));
    SetActivatedMoveHandlers(array_index(MOVES, "Punishment"));
    SetActivatedMoveHandlers(array_index(MOVES, "Captivate"));
    SetActivatedMoveHandlers(array_index(MOVES, "Ominous Wind"));
    SetActivatedMoveHandlers(array_index(MOVES, "Flame Burst"));
    SetActivatedMoveHandlers(array_index(MOVES, "Synchronoise"));
    SetActivatedMoveHandlers(array_index(MOVES, "Chip Away"));
    SetActivatedMoveHandlers(array_index(MOVES, "Sky Drop"));
    SetActivatedMoveHandlers(array_index(MOVES, "Bestow"));
    SetActivatedMoveHandlers(array_index(MOVES, "Relic Song"));

    SetActivatedMoveHandlers(array_index(MOVES, "Rototiller"));
    SetActivatedMoveHandlers(array_index(MOVES, "Ion Deluge"));
    SetActivatedMoveHandlers(array_index(MOVES, "Hyperspace Fury"));
    SetActivatedMoveHandlers(array_index(MOVES, "Spotlight"));
    SetActivatedMoveHandlers(array_index(MOVES, "Guardian of Alola"));
    SetActivatedMoveHandlers(array_index(MOVES, "Genesis Supernova"));
    SetActivatedMoveHandlers(array_index(MOVES, "Light That Burns the Sky"));
    SetActivatedMoveHandlers(array_index(MOVES, "Searing Sunraze Smash"));
    SetActivatedMoveHandlers(array_index(MOVES, "Menacing Moonraze Maelstrom"));
    SetActivatedMoveHandlers(array_index(MOVES, "Splintered Stormshards"));

    SetActivatedMoveHandlers(array_index(MOVES, "Pika Papow"));
    SetActivatedMoveHandlers(array_index(MOVES, "Glitzy Glow"));
    SetActivatedMoveHandlers(array_index(MOVES, "Baddy Bad"));
    SetActivatedMoveHandlers(array_index(MOVES, "Freezy Frost"));
    SetActivatedMoveHandlers(array_index(MOVES, "Sparkly Swirl"));
    SetActivatedMoveHandlers(array_index(MOVES, "Veevee Volley"));

    InstallActivatedMoveHooks();
}