#include "features/field_handlers/field_handlers.h"

#include "externals/BTL_STRID_STD.h"
#include "externals/Dpr/Battle/Logic/BtlGround.h"
#include "externals/Dpr/Battle/Logic/FieldStatus.h"
#include "externals/Dpr/Battle/Logic/Section_FieldEffect_End.h"
#include "externals/Dpr/Battle/Logic/ServerCommandPutter.h"

#include "data/field_effects.h"
#include "data/utils.h"
#include "logger/logger.h"

static ExtraFieldEffectHandlers extraFieldEffectHandlers {};
static bool ACTIVATED_FIELD_HANDLERS[FIELD_EFFECT_COUNT];

void SetActivatedFieldHandlers(int32_t fieldEffect)
{
    ACTIVATED_FIELD_HANDLERS[fieldEffect] = true;
    getExtraFieldEffectHandlers()->count++;
}

ExtraFieldEffectHandlers* getExtraFieldEffectHandlers() {
    return &extraFieldEffectHandlers;
}

using namespace Dpr::Battle::Logic;
EventFactor::EventHandlerTable::Array* CreateFieldEffectEventHandlerTable(long size) {
    return EventFactor::EventHandlerTable::newArrayMAlloc(size);
}

EventFactor::EventHandlerTable::Object* CreateFieldEffectEventHandler(EventID eventID, Il2CppMethodPointer methodPointer) {
    MethodInfo* method = (*Handler::Field::PTR_Method$$handler_elec_checkFail)->copyWith(methodPointer);
    auto evtHandler = EventFactor::EventHandler::newInstanceMAlloc(0, method);
    evtHandler->fields.delegates = nullptr;
    return EventFactor::EventHandlerTable::newInstanceMAlloc(eventID, evtHandler);
}

EventFactor::EventHandlerTable::Object* CreateFieldEffectEventHandler(EventID eventID, MethodInfo* method) {
    auto evtHandler = EventFactor::EventHandler::newInstanceMAlloc(0, method);
    evtHandler->fields.delegates = nullptr;
    return EventFactor::EventHandlerTable::newInstanceMAlloc(eventID, evtHandler);
}

void SetFieldEffectFunctionTable(Handler::Field::GET_FUNC_TABLE_ELEM::Array* getFuncTable, uint64_t index, int32_t fieldEffect, Il2CppMethodPointer methodPointer) {
    MethodInfo* method = (*Handler::Field::PTR_Method$$ADD_Fld_Ground)->copyWith(methodPointer);
    Handler::Field::GET_FUNC_TABLE_ELEM::Object* elem = &getFuncTable->m_Items[index];
    auto func = Handler::Field::HandlerGetFunc::newInstance(0, method);
    elem->ctor(fieldEffect, func);
}

void SetFieldEffectFunctionTable(Handler::Field::GET_FUNC_TABLE_ELEM::Array* getFuncTable, uint64_t index, int32_t fieldEffect, MethodInfo* method) {
    Handler::Field::GET_FUNC_TABLE_ELEM::Object* elem = &getFuncTable->m_Items[index];
    auto func = Handler::Field::HandlerGetFunc::newInstance(0, method);
    elem->ctor(fieldEffect, func);
}

HOOK_DEFINE_REPLACE(Dpr_Battle_Logic_FieldStatus_initWork) {
    static void Callback(FieldStatus::Object* __this) {
        for (int32_t i=0; i<10+getExtraFieldEffectHandlers()->count; i++)
            __this->clearFactorWork(i);

        __this->fields.m_data->fields.weather = 0;
        __this->fields.m_data->fields.weatherTurn = 255;
        __this->fields.m_data->fields.weatherTurnCount = 0;
        __this->fields.m_data->fields.weatherCausePokeID = 31;
    }
};

HOOK_DEFINE_REPLACE(Section_FieldEffect_End_Execute) {
    static void Callback(Section_FieldEffect_End::Object* __this, Section_FieldEffect_End::Result::Object* pResult, Section_FieldEffect_End::Description::Object** description) {
        pResult->fields.isRemoved = false;

        int32_t effectID = (*description)->fields.effect;
        BTL_POKEPARAM::Object* causedPoke = (*description)->fields.pDependPoke;

        if (effectID == array_index(FIELD_EFFECTS, "Terrain"))
        {
            BtlGround currentTerrain = __this->fields.m_pBattleEnv->instance()->fields.m_fieldStatus->GetGround();
            BtlGround initialTerrain = __this->fields.m_pMainModule->instance()->GetDefaultGround();
            if (currentTerrain == initialTerrain)
                return;
        }

        ServerCommandPutter::Object* serverCommandPutter = __this->fields.m_pServerCmdPutter->instance();
        if (causedPoke == nullptr && !serverCommandPutter->RemoveFieldEffect(effectID))
            return;

        if (causedPoke != nullptr && !serverCommandPutter->RemoveFieldEffect_DependPoke(causedPoke, effectID))
            return;

        serverCommandPutter->RemoveFieldHandler(effectID);

        switch(effectID)
        {
            case array_index(FIELD_EFFECTS, "Trick Room"):
                serverCommandPutter->Message_Std(BTL_STRID_STD::TrickRoomOff);
                break;

            case array_index(FIELD_EFFECTS, "Gravity"):
                serverCommandPutter->Message_Std(BTL_STRID_STD::JyuryokuOff);
                break;

            case array_index(FIELD_EFFECTS, "Wonder Room"):
                serverCommandPutter->Message_Std(BTL_STRID_STD::WonderRoom_End);
                break;

            case array_index(FIELD_EFFECTS, "Magic Room"):
                serverCommandPutter->Message_Std(BTL_STRID_STD::MagicRoom_End);
                __this->checkItemReaction_All();
                break;

            case array_index(FIELD_EFFECTS, "Terrain"):
                switch (__this->fields.m_pBattleEnv->instance()->fields.m_fieldStatus->GetGround())
                {
                    case BtlGround::BTL_GROUND_GRASS:
                        serverCommandPutter->Message_Std(BTL_STRID_STD::GrassGround_End);
                        break;

                    case BtlGround::BTL_GROUND_MIST:
                        serverCommandPutter->Message_Std(BTL_STRID_STD::MistGround_End);
                        break;

                    case BtlGround::BTL_GROUND_ELEKI:
                        serverCommandPutter->Message_Std(BTL_STRID_STD::ElecField_End);
                        break;

                    case BtlGround::BTL_GROUND_PHYCHO:
                        serverCommandPutter->Message_Std(BTL_STRID_STD::PhychoField_End);
                        break;

                    default:
                        break;
                }
                __this->resetGround();
                break;

            case array_index(FIELD_EFFECTS, "Neutralizing Gas"):
                serverCommandPutter->Message_Std(BTL_STRID_STD::KagakuHenkaGas_End);
                __this->resetKagakuhenkaGas();
                break;

            case array_index(FIELD_EFFECTS, "Mud Sport"):
                if (ACTIVATED_FIELD_HANDLERS[array_index(FIELD_EFFECTS, "Mud Sport")])
                    FieldEffect_End_MudSport(__this, pResult, description);
                break;

            case array_index(FIELD_EFFECTS, "Water Sport"):
                if (ACTIVATED_FIELD_HANDLERS[array_index(FIELD_EFFECTS, "Water Sport")])
                    FieldEffect_End_WaterSport(__this, pResult, description);
                break;

            default:
                break;
        }

        pResult->fields.isRemoved = true;
    }
};

HOOK_DEFINE_INLINE(Field_system_array_new) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto typeInfo = (Il2CppClass*)ctx->X[0];
        uint64_t size = ctx->X[1];

        auto array = (Handler::Field::GET_FUNC_TABLE_ELEM::Array*)system_array_new(typeInfo, size + getExtraFieldEffectHandlers()->count);
        getExtraFieldEffectHandlers()->currentIndex = size;

        // DO NOT REMOVE ANY OF THESE! Disable the field effects in exl_field_handlers_main() below instead!
        if (ACTIVATED_FIELD_HANDLERS[array_index(FIELD_EFFECTS, "Mud Sport")]) Handlers_FieldEffect_MudSport(array);
        if (ACTIVATED_FIELD_HANDLERS[array_index(FIELD_EFFECTS, "Water Sport")]) Handlers_FieldEffect_WaterSport(array);

        ctx->X[0] = (uint64_t)array;
    }
};

void exl_field_handlers_main() {
    Dpr_Battle_Logic_FieldStatus_initWork::InstallAtOffset(0x018fa2b0);
    Section_FieldEffect_End_Execute::InstallAtOffset(0x021b53f0);
    Field_system_array_new::InstallAtOffset(0x01904274);

    SetActivatedFieldHandlers(array_index(FIELD_EFFECTS, "Mud Sport"));
    SetActivatedFieldHandlers(array_index(FIELD_EFFECTS, "Water Sport"));

    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p(0);
    auto inst = nn::vector<exl::patch::Instruction> {
        { 0x018f54b8, CmpImmediate(W1, 10 + getExtraFieldEffectHandlers()->count) },
        { 0x018fa1c4, Movz(W20, 11 + getExtraFieldEffectHandlers()->count) },
        { 0x018fabc8, CmpImmediate(W1, 10 + getExtraFieldEffectHandlers()->count) },
        { 0x018facc8, CmpImmediate(W1, 10 + getExtraFieldEffectHandlers()->count) },
        { 0x018fb028, CmpImmediate(W1, 10 + getExtraFieldEffectHandlers()->count) },
        { 0x018fb200, CmpImmediate(W1, 10 + getExtraFieldEffectHandlers()->count) },
        { 0x018fb2e8, CmpImmediate(W1, 10 + getExtraFieldEffectHandlers()->count) },
        { 0x018fb370, CmpImmediate(W1, 10 + getExtraFieldEffectHandlers()->count) },
        { 0x018fb418, CmpImmediate(W1, 10 + getExtraFieldEffectHandlers()->count) },
        { 0x018fb470, CmpImmediate(W1, 10 + getExtraFieldEffectHandlers()->count) },
        { 0x018fb528, CmpImmediate(W1, 10 + getExtraFieldEffectHandlers()->count) },
        { 0x018fb598, CmpImmediate(W1, 10 + getExtraFieldEffectHandlers()->count) },
        { 0x02170b2c, CmpImmediate(W20, 11 + getExtraFieldEffectHandlers()->count) },
    };
    p.WriteInst(inst);
}