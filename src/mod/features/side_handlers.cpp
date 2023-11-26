#include "features/side_handlers/side_handlers.h"

#include "externals/Dpr/Battle/Logic/BtlGround.h"

#include "data/side_effects.h"
#include "data/utils.h"
#include "logger/logger.h"

static ExtraSideEffectHandlers extraSideEffectHandlers {};
static bool ACTIVATED_SIDE_HANDLERS[SIDE_EFFECT_COUNT];

void SetActivatedSideHandlers(int32_t sideEffect)
{
    ACTIVATED_SIDE_HANDLERS[sideEffect] = true;
    getExtraSideEffectHandlers()->count++;
}

ExtraSideEffectHandlers* getExtraSideEffectHandlers() {
    return &extraSideEffectHandlers;
}

using namespace Dpr::Battle::Logic;
EventFactor::EventHandlerTable::Array* CreateSideEffectEventHandlerTable(long size) {
    return EventFactor::EventHandlerTable::newArrayMAlloc(size);
}

EventFactor::EventHandlerTable::Object* CreateSideEffectEventHandler(EventID eventID, Il2CppMethodPointer methodPointer) {
    MethodInfo* method = (*Handler::Side::PTR_Method$$handler_Rainbow)->copyWith(methodPointer);
    auto evtHandler = EventFactor::EventHandler::newInstanceMAlloc(0, method);
    evtHandler->fields.delegates = nullptr;
    return EventFactor::EventHandlerTable::newInstanceMAlloc(eventID, evtHandler);
}

EventFactor::EventHandlerTable::Object* CreateSideEffectEventHandler(EventID eventID, MethodInfo* method) {
    auto evtHandler = EventFactor::EventHandler::newInstanceMAlloc(0, method);
    evtHandler->fields.delegates = nullptr;
    return EventFactor::EventHandlerTable::newInstanceMAlloc(eventID, evtHandler);
}

void SetSideEffectFunctionTable(Handler::Side::GET_FUNC_TABLE_ELEM::Array* getFuncTable, uint64_t index, int32_t sideEffect, Il2CppMethodPointer methodPointer) {
    MethodInfo* method = (*Handler::Side::PTR_Method$$ADD_SIDE_AuroraVeil)->copyWith(methodPointer);
    Handler::Side::GET_FUNC_TABLE_ELEM::Object* elem = &getFuncTable->m_Items[index];
    auto func = Handler::Side::HandlerGetFunc::newInstance(0, method);
    elem->ctor(sideEffect, func);
}

void SetSideEffectFunctionTable(Handler::Side::GET_FUNC_TABLE_ELEM::Array* getFuncTable, uint64_t index, int32_t sideEffect, MethodInfo* method) {
    Handler::Side::GET_FUNC_TABLE_ELEM::Object* elem = &getFuncTable->m_Items[index];
    auto func = Handler::Side::HandlerGetFunc::newInstance(0, method);
    elem->ctor(sideEffect, func);
}

HOOK_DEFINE_INLINE(Side_system_array_new) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto typeInfo = (Il2CppClass*)ctx->X[0];
        uint64_t size = ctx->X[1];

        auto array = (Handler::Side::GET_FUNC_TABLE_ELEM::Array*)system_array_new(typeInfo, size + getExtraSideEffectHandlers()->count);
        getExtraSideEffectHandlers()->currentIndex = size;

        // DO NOT REMOVE ANY OF THESE! Disable the side effects in exl_side_handlers_main() below instead!
        //if (ACTIVATED_SIDE_HANDLERS[array_index(SIDE_EFFECTS, "Lucky Chant")]) Handlers_SideEffect_MudSport(array);

        ctx->X[0] = (uint64_t)array;
    }
};

void InstallActivatedSideEffectHooks() {
    // Existing Stubbed Vanilla Methods
    InstallHooks_SideEffect_LuckyChant();

    // DO NOT REMOVE ANY OF THESE! Disable the side effects in exl_side_handlers_main() below instead!
    //if (ACTIVATED_SIDE_HANDLERS[array_index(SIDE_EFFECTS, "Lucky Chant")]) InstallHooks_SideEffect_LuckyChant();
}

void exl_side_handlers_main() {
    Side_system_array_new::InstallAtOffset(0x0193cbe4);

    //SetActivatedSideHandlers(array_index(SIDE_EFFECTS, "Lucky Chant"));

    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p(0);
    auto inst = nn::vector<exl::patch::Instruction> {
        { 0x01f70db0, Movz(W1, 22 + getExtraSideEffectHandlers()->count) },
        { 0x01f70f10, CmpImmediate(X27, 22 + getExtraSideEffectHandlers()->count) },
        { 0x01f70fd0, CmpImmediate(X28, (22 + getExtraSideEffectHandlers()->count + 4) * 8) },
        { 0x01f710f0, CmpImmediate(X20, 22 + getExtraSideEffectHandlers()->count) },
        { 0x01f71138, CmpImmediate(X21, (22 + getExtraSideEffectHandlers()->count + 4) * 8) },
        { 0x01f711c0, CmpImmediate(X8, 22 + getExtraSideEffectHandlers()->count + 4) },
        { 0x01f71248, CmpImmediate(X8, 22 + getExtraSideEffectHandlers()->count + 4) },
        { 0x01f712dc, CmpImmediate(W20, 22 + getExtraSideEffectHandlers()->count) },
        { 0x01f71330, CmpImmediate(W20, 22 + getExtraSideEffectHandlers()->count) },
        { 0x01f716a4, CmpImmediate(W19, 22 + getExtraSideEffectHandlers()->count - 1) },
    };
    p.WriteInst(inst);

    InstallActivatedSideEffectHooks();
}