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

void AddSituationDetailSideEffectLabels(System::Collections::Generic::Dictionary$$BtlSideEffect$$String::Object* sideIDs) {
    // No extra ones currently
}

HOOK_DEFINE_INLINE(Side_system_array_new) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        uint64_t size = ctx->X[1];

        auto array = Handler::Side::GET_FUNC_TABLE_ELEM::newArray(size + getExtraSideEffectHandlers()->count);
        getExtraSideEffectHandlers()->currentIndex = size;

        // DO NOT REMOVE ANY OF THESE! Disable the side effects in exl_side_handlers_main() below instead!

        ctx->X[0] = (uint64_t)array;
    }
};

void InstallActivatedSideEffectHooks() {
    // DO NOT REMOVE ANY OF THESE! Disable the side effects in exl_side_handlers_main() below instead!
    if (ACTIVATED_SIDE_HANDLERS[array_index(SIDE_EFFECTS, "Lucky Chant")]) InstallHooks_SideEffect_LuckyChant();
}

void exl_side_handlers_main() {
    Side_system_array_new::InstallAtOffset(game::Address::Dpr_Battle_Logic_Handler_Side_cctor + 0x54);

    SetActivatedSideHandlers(array_index(SIDE_EFFECTS, "Lucky Chant"));

    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p;
    auto inst = nn::vector<exl::patch::Instruction> {
        { game::Address::Dpr_Battle_Logic_SideEffectManager_ctor + 0x40, Movz(W1, SIDE_EFFECT_COUNT) },
        { game::Address::Dpr_Battle_Logic_SideEffectManager_CreateEffectStatusArray + 0x120, CmpImmediate(X27, SIDE_EFFECT_COUNT) },
        { game::Address::Dpr_Battle_Logic_SideEffectManager_CreateEffectStatusArray + 0x1E0, CmpImmediate(X28, (SIDE_EFFECT_COUNT + 4) * 8) },
        { game::Address::Dpr_Battle_Logic_SideEffectManager_DeleteEffectStatusArray + 0x50, CmpImmediate(X20, SIDE_EFFECT_COUNT) },
        { game::Address::Dpr_Battle_Logic_SideEffectManager_DeleteEffectStatusArray + 0x98, CmpImmediate(X21, (SIDE_EFFECT_COUNT + 4) * 8) },
        { game::Address::Dpr_Battle_Logic_SideEffectManager_CopyFrom + 0x60, CmpImmediate(X8, SIDE_EFFECT_COUNT + 4) },
        { game::Address::Dpr_Battle_Logic_SideEffectManager_CopyFrom + 0xE8, CmpImmediate(X8, SIDE_EFFECT_COUNT + 4) },
        { game::Address::Dpr_Battle_Logic_SideEffectManager_RemoveAllSideEffect + 0x1C, CmpImmediate(W20, SIDE_EFFECT_COUNT) },
        { game::Address::Dpr_Battle_Logic_SideEffectManager_RemoveAllSideEffect + 0x70, CmpImmediate(W20, SIDE_EFFECT_COUNT) },
        { game::Address::Dpr_Battle_Logic_SideEffectManager_GetSideEffectStatusConst + 0x74, CmpImmediate(W19, SIDE_EFFECT_COUNT - 1) },
    };
    p.WriteInst(inst);

    InstallActivatedSideEffectHooks();
}