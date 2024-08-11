#include "exlaunch.hpp"
#include "externals/Dpr/Battle/Logic/Section_ProcessActionCore.h"
#include "externals/Dpr/Battle/Logic/PokeAction.h"
#include "externals/Dpr/Battle/Logic/BTL_ACTION.h"
#include "externals/Dpr/Battle/Logic/Section_CalcActionPriority.h"
#include "externals/Dpr/Battle/Logic/Section_FromEvent_FormChange.h"
#include "externals/Dpr/Battle/Logic/Section_StoreActions.h"
#include "externals/Dpr/Battle/Logic/ActPri.h"
#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventFactor.h"
#include "externals/Dpr/Battle/View/UI/BUIWazaList.h"
#include "externals/Dpr/Battle/View/UI/BUIWazaButton.h"
#include "externals/Dpr/Battle/Logic/ActionSerialNoManager.h"
#include "data/utils.h"
#include "data/species.h"
#include "logger/logger.h"

const int32_t mega_flag_sz = 1;
const int32_t mega_flag_loc = 34; // Assuming the first 34 bits are used.
const long mega_flag_mask = 1L << mega_flag_loc;

//void set_mega_cmd(Dpr::Battle::Logic::BTL_ACTION::PARAM::Object __this) {
//    *(ulong *)__this.fields.raw =
//            (*(ulong *)__this.fields.raw & ~0xF) | (0x0B & 0xF);
//}

uint8_t get_mega_flag(Dpr::Battle::Logic::BTL_ACTION::PARAM::Object __this) {
    return (*(uint64_t*)&__this.fields.raw >> 34) & 1;
}

void MegaEvolutionFormHandler(Dpr::Battle::Logic::EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    system_load_typeinfo(0x8910);
    Dpr::Battle::Logic::Common::getClass()->initIfNeeded();
    Dpr::Battle::Logic::BTL_POKEPARAM::Object* __this = Dpr::Battle::Logic::Common::GetPokeParam(args, pokeID);

    Dpr::Battle::Logic::Section_FromEvent_FormChange::Description::Object* description =
            Dpr::Battle::Logic::Section_FromEvent_FormChange::Description::newInstance();

    switch (__this->GetMonsNo()) {
        case array_index(SPECIES, "Lucario"): {
            description->fields.formNo = 1;
            break;
        }

        default: {
            break;
        }

    }

    description->fields.pokeID = pokeID;

    description->fields.successMessage->Setup(2, 0x130); // Placeholder need to change
    description->fields.successMessage->AddArg(pokeID);

    Dpr::Battle::Logic::Common::FormChange(args, &description);

}

HOOK_DEFINE_INLINE(OnSubmitWazaButton) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto __this = reinterpret_cast<Dpr::Battle::View::UI::BUIWazaList::Object*>(ctx->X[19]);
        auto destActionParam = __this->fields._destActionParam;
        ctx->X[8] = reinterpret_cast<u64>(destActionParam);
        destActionParam->fields.value.fields.raw |= mega_flag_mask;
        Logger::log("[OnSubmitWazaButton] Set Mega Flag\n");
    }
};

HOOK_DEFINE_TRAMPOLINE(ProcessActionCore$$action) {
    static void Callback(Dpr::Battle::Logic::Section_ProcessActionCore::Object* __this,
                         Dpr::Battle::Logic::PokeAction::Object* pokeAction) {

        switch(pokeAction->fields.actionCategory) {
            case Dpr::Battle::Logic::PokeActionCategory::Mega_Evolution: {
                //ToDo
                Logger::log("[ProcessActionCore$$action] Mega_Evolution\n");
                break;
            }

            default: {
                Orig(__this, pokeAction);
                break;
            }
        }
    }
};

HOOK_DEFINE_TRAMPOLINE(setupPokeAction_FromClientInstruction) {
    static bool Callback(Dpr::Battle::Logic::Section_StoreActions::Object* __this, Dpr::Battle::Logic::PokeAction::Object* pokeAction,
                         Dpr::Battle::Logic::BTL_ACTION::PARAM::Object* clientInstruction, uint8_t clientID) {

        Dpr::Battle::Logic::BTL_ACTION::PARAM::Object instruction{};
        instruction.fields.raw = clientInstruction->fields.raw;

        if (get_mega_flag(instruction)) {
            Logger::log("[setupPokeAction_FromClientInstruction] Mega_Evolution\n");
            (pokeAction->fields).actionCategory = Dpr::Battle::Logic::PokeActionCategory::Mega_Evolution;

            pokeAction->fields.bpp = __this->getActionPoke(clientInstruction, clientID);
            pokeAction->fields.priority = 2;
            pokeAction->fields.clientID = clientID;
            pokeAction->fields.fDone = false;
            pokeAction->fields.fIntrCheck = false;
            pokeAction->fields.fRecalcPriority = false;
            __this->fields.m_pBattleEnv->fields.m_actionSerialNoManager->AssignSerialNo(pokeAction->fields.actionDesc);

            return true;
        }

        else {
            return Orig(__this, pokeAction, clientInstruction, clientID);
        }
    }
};

HOOK_DEFINE_TRAMPOLINE(CalcActionPriority$$Execute) {
    static void Callback(Dpr::Battle::Logic::Section_CalcActionPriority::Object* __this,
                         Dpr::Battle::Logic::Section_CalcActionPriority::Result::Object* pResult,
                         Dpr::Battle::Logic::Section_CalcActionPriority::Description::Object** description) {
        auto pokeAction = (*description)->fields.pokeAction;

        switch (pokeAction->fields.actionCategory) {
            case Dpr::Battle::Logic::PokeActionCategory::Mega_Evolution: {
                uint8_t operationPri = 2;
                uint8_t dominantPri = 2;
                uint8_t wazaPri = 0;
                uint8_t agility = 0;
                auto desc = *(description);
                auto spPri = static_cast<uint8_t>(desc->fields.specialPriority);
                pResult->fields.priority = Dpr::Battle::Logic::ActPri::Make(dominantPri, operationPri, wazaPri, spPri, agility);
                break;
            }

            default: {
                Orig(__this, pResult, description);
                break;
            }
        }
    }
};

void exl_mega_evolution_main() {
    OnSubmitWazaButton::InstallAtOffset(0x01d2cee8);
    CalcActionPriority$$Execute::InstallAtOffset(0x021ad570);
    ProcessActionCore$$action::InstallAtOffset(0x021c0de0);
    setupPokeAction_FromClientInstruction::InstallAtOffset(0x021cdbf0);
}