#include "exlaunch.hpp"
#include "externals/Dpr/Battle/Logic/Section_ProcessActionCore.h"
#include "externals/Dpr/Battle/Logic/PokeAction.h"
#include "externals/Dpr/Battle/Logic/PokeActionContainer.h"
#include "externals/Dpr/Battle/Logic/BTL_ACTION.h"
#include "externals/Dpr/Battle/Logic/BTL_PARTY.h"
#include "externals/Dpr/Battle/Logic/Section_CalcActionPriority.h"
#include "externals/Dpr/Battle/Logic/Section_FromEvent_FormChange.h"
#include "externals/Dpr/Battle/Logic/Section_StoreActions.h"
#include "externals/Dpr/Battle/Logic/SVCL_ACTION.h"
#include "externals/Dpr/Battle/Logic/ActPri.h"
#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/View/UI/BUIWazaList.h"
#include "externals/Dpr/Battle/View/UI/BUIWazaButton.h"
#include "data/utils.h"
#include "data/species.h"
#include "logger/logger.h"

const int32_t mega_flag_sz = 1;
const int32_t mega_flag_loc = 34; // Assuming the first 34 bits are used.
const long mega_flag_mask = 1L << mega_flag_loc;


uint8_t get_mega_flag(Dpr::Battle::Logic::BTL_ACTION::PARAM::Object __this) {
    return (*(uint64_t*)&__this.fields.raw >> 34) & 1;
}

void MegaEvolutionFormHandler(Dpr::Battle::Logic::SectionContainer::Object* __this, Dpr::Battle::Logic::PokeAction::Object* pokeAction) {
    system_load_typeinfo(0x8910);
    Dpr::Battle::Logic::Common::getClass()->initIfNeeded();
    Dpr::Battle::Logic::BTL_POKEPARAM::Object* param = pokeAction->fields.bpp;

    Dpr::Battle::Logic::Section_FromEvent_FormChange::Description::Object* description =
            Dpr::Battle::Logic::Section_FromEvent_FormChange::Description::newInstance();

    switch (param->GetMonsNo()) {
        case array_index(SPECIES, "Lucario"): {
            description->fields.formNo = 1;
            break;
        }

        default: {
            break;
        }

    }

    description->fields.pokeID = param->GetID();

    description->fields.successMessage->Setup(2, 0x130); // Placeholder need to change
    description->fields.successMessage->AddArg(param->GetID());

    system_load_typeinfo(0x2c56);
    Dpr::Battle::Logic::Section_FromEvent_FormChange::Result::Object* result =
            Dpr::Battle::Logic::Section_FromEvent_FormChange::Result::newInstance();

    auto formChange = __this->fields.m_section_FromEvent_FormChange;

    formChange->Execute(result, &description);

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
                MegaEvolutionFormHandler(__this->fields.m_pSectionContainer, pokeAction);
                Logger::log("[ProcessActionCore$$action] Mega Evolved.\n");
                break;
            }

            default: {
                Orig(__this, pokeAction);
                break;
            }
        }
    }
};

//HOOK_DEFINE_INLINE(createPokeAction_FromClientInstruction) {
//    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
//        auto instructions = reinterpret_cast<Dpr::Battle::Logic::SVCL_ACTION::Object*>(ctx->X[0]);
//        auto clientID = static_cast<uint8_t>(ctx->X[1]);
//        auto posIdx = static_cast<uint8_t>(ctx->X[2]);
//
//        Dpr::Battle::Logic::BTL_ACTION::PARAM::Object* action = instructions->Get(clientID, posIdx);
//        action->fields.raw
//
//        ctx->X[0] = reinterpret_cast<u64>(instructions->Get(clientID, posIdx));
//
//
//    }
//};

HOOK_DEFINE_TRAMPOLINE(createPokeAction_FromClientInstruction) {
    static void Callback(Dpr::Battle::Logic::Section_StoreActions::Object* __this,
                         Dpr::Battle::Logic::PokeActionContainer::Object* actionContainer,
                         Dpr::Battle::Logic::SVCL_ACTION::Object* instructions, uint8_t clientID) {
        system_load_typeinfo(0x78dd);
        system_load_typeinfo(0x78c7);
        auto party = reinterpret_cast<Dpr::Battle::Logic::Section::Object*>(__this)->GetPokeParty(clientID);
        Dpr::Battle::Logic::BTL_ACTION::PARAM::Object firstInstruction{};
        firstInstruction.fields.raw = Dpr::Battle::Logic::SVCL_ACTION::Get(instructions, clientID, 0)->fields.raw;

        if (!get_mega_flag(firstInstruction)) {
            Orig(__this, actionContainer, instructions, clientID);
        }

        else {
            auto numAction = Dpr::Battle::Logic::SVCL_ACTION::GetNumAction(instructions, clientID);
            if (party != nullptr && numAction != 0) {
                for (uint64_t i = 0; i < numAction + 1; i++) {
                    Dpr::Battle::Logic::PokeAction::Object* newAction = Dpr::Battle::Logic::PokeAction::newInstance();

                    if (i == numAction) {
                        firstInstruction.fields.raw &= ~mega_flag_mask;
                    }

                    bool setupPokeAction = __this->setupPokeAction_FromClientInstruction(newAction, &firstInstruction, clientID);

                    if (setupPokeAction) {
                        actionContainer->Add(&newAction);
                    }

                }
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
    createPokeAction_FromClientInstruction::InstallAtOffset(0x021cdad0);
}