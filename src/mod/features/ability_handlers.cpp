#include "features/ability_handlers/ability_handlers.h"

#include "externals/Dpr/Battle/Logic/BtlGround.h"

#include "data/abilities.h"
#include "data/utils.h"
#include "logger/logger.h"

static ExtraAbilityHandlers extraAbilityHandlers {};
static bool ACTIVATED_ABILITY_HANDLERS[ABILITY_COUNT];

void SetActivatedAbilityHandlers(int32_t ability)
{
    ACTIVATED_ABILITY_HANDLERS[ability] = true;
    getExtraAbilityHandlers()->count++;
}

ExtraAbilityHandlers* getExtraAbilityHandlers() {
    return &extraAbilityHandlers;
}

using namespace Dpr::Battle::Logic;
EventFactor::EventHandlerTable::Array* CreateAbilityEventHandlerTable(long size) {
    return EventFactor::EventHandlerTable::newArrayMAlloc(size);
}

EventFactor::EventHandlerTable::Object* CreateAbilityEventHandler(EventID eventID, Il2CppMethodPointer methodPointer) {
    MethodInfo* method = (*Handler::Tokusei::PTR_Method$$handler_AddSickFailCommon)->copyWith(methodPointer);
    auto evtHandler = EventFactor::EventHandler::newInstanceMAlloc(0, method);
    evtHandler->fields.delegates = nullptr;
    return EventFactor::EventHandlerTable::newInstanceMAlloc(eventID, evtHandler);
}

EventFactor::EventHandlerTable::Object* CreateAbilityEventHandler(EventID eventID, MethodInfo* method) {
    auto evtHandler = EventFactor::EventHandler::newInstanceMAlloc(0, method);
    evtHandler->fields.delegates = nullptr;
    return EventFactor::EventHandlerTable::newInstanceMAlloc(eventID, evtHandler);
}

void SetAbilityFunctionTable(Handler::Tokusei::GET_FUNC_TABLE_ELEM::Array* getFuncTable, uint64_t index, int32_t ability, Il2CppMethodPointer methodPointer) {
    MethodInfo* method = (*Handler::Tokusei::PTR_Method$$ADD_AirLock)->copyWith(methodPointer);
    Handler::Tokusei::GET_FUNC_TABLE_ELEM::Object* elem = &getFuncTable->m_Items[index];
    auto func = Handler::Tokusei::HandlerGetFunc::newInstance(0, method);
    elem->ctor(ability, func);
}

void SetAbilityFunctionTable(Handler::Tokusei::GET_FUNC_TABLE_ELEM::Array* getFuncTable, uint64_t index, int32_t ability, MethodInfo* method) {
    Handler::Tokusei::GET_FUNC_TABLE_ELEM::Object* elem = &getFuncTable->m_Items[index];
    auto func = Handler::Tokusei::HandlerGetFunc::newInstance(0, method);
    elem->ctor(ability, func);
}

HOOK_DEFINE_INLINE(Tokusei_system_array_new) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        uint64_t size = ctx->X[1];

        auto array = Handler::Tokusei::GET_FUNC_TABLE_ELEM::newArray(size + getExtraAbilityHandlers()->count);
        getExtraAbilityHandlers()->currentIndex = size;

        // DO NOT REMOVE ANY OF THESE! Disable the side effects in exl_ability_handlers_main() below instead!
        if (ACTIVATED_ABILITY_HANDLERS[array_index(ABILITIES, "Zen Mode")]) Handlers_Ability_ZenMode(array);
        if (ACTIVATED_ABILITY_HANDLERS[array_index(ABILITIES, "Stance Change")]) Handlers_Ability_StanceChange(array);
        if (ACTIVATED_ABILITY_HANDLERS[array_index(ABILITIES, "Shields Down")]) Handlers_Ability_ShieldsDown(array);
        if (ACTIVATED_ABILITY_HANDLERS[array_index(ABILITIES, "Schooling")]) Handlers_Ability_Schooling(array);
        if (ACTIVATED_ABILITY_HANDLERS[array_index(ABILITIES, "Sharpness")]) Handlers_Ability_Sharpness(array);

        ctx->X[0] = (uint64_t)array;
    }
};

void InstallActivatedAbilityHooks() {
    // DO NOT REMOVE ANY OF THESE! Disable the side effects in exl_ability_handlers_main() below instead!
    if (ACTIVATED_ABILITY_HANDLERS[array_index(ABILITIES, "Run Away")]) InstallHooks_Ability_RunAway();
    if (ACTIVATED_ABILITY_HANDLERS[array_index(ABILITIES, "Iron Fist")]) InstallHooks_Ability_IronFist();
    if (ACTIVATED_ABILITY_HANDLERS[array_index(ABILITIES, "Gale Wings")]) InstallHooks_Ability_GaleWings();
    if (ACTIVATED_ABILITY_HANDLERS[array_index(ABILITIES, "Mega Launcher")]) InstallHooks_Ability_MegaLauncher();
}

void exl_ability_handlers_main() {
    Tokusei_system_array_new::InstallAtOffset(0x01964f24);

    SetActivatedAbilityHandlers(array_index(ABILITIES, "Run Away"));
    SetActivatedAbilityHandlers(array_index(ABILITIES, "Iron Fist"));
    SetActivatedAbilityHandlers(array_index(ABILITIES, "Zen Mode"));
    SetActivatedAbilityHandlers(array_index(ABILITIES, "Stance Change"));
    SetActivatedAbilityHandlers(array_index(ABILITIES, "Gale Wings"));
    SetActivatedAbilityHandlers(array_index(ABILITIES, "Mega Launcher"));
    SetActivatedAbilityHandlers(array_index(ABILITIES, "Shields Down"));
    SetActivatedAbilityHandlers(array_index(ABILITIES, "Schooling"));
    SetActivatedAbilityHandlers(array_index(ABILITIES, "Sharpness"));

    InstallActivatedAbilityHooks();
}