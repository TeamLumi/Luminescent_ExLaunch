#include "exlaunch.hpp"
#include "externals/Dpr/Battle/Logic/Section_ProcessActionCore.h"
#include "externals/Dpr/Battle/Logic/PokeAction.h"
#include "externals/Dpr/Battle/Logic/PokeActionContainer.h"
#include "externals/Dpr/Battle/Logic/BTL_ACTION.h"
#include "externals/Dpr/Battle/Logic/Section_CalcActionPriority.h"
#include "externals/Dpr/Battle/Logic/Section_FromEvent_FormChange.h"
#include "externals/Dpr/Battle/Logic/Section_StoreActions.h"
#include "externals/Dpr/Battle/Logic/SVCL_ACTION.h"
#include "externals/Dpr/Battle/Logic/ActPri.h"
#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/View/UI/BUIWazaList.h"
#include "externals/Dpr/Battle/View/UI/BUIWazaButton.h"
#include "externals/FlagWork_Enums.h"
#include "externals/FlagWork.h"
#include "externals/PlayerWork.h"
#include "data/utils.h"
#include "data/species.h"
#include "data/items.h"
#include "data/moves.h"
#include "logger/logger.h"

const int32_t mega_flag_loc = 34; // Assuming the first 34 bits are used.
const long mega_flag_mask = 1L << mega_flag_loc;

uint8_t get_mega_flag(Dpr::Battle::Logic::BTL_ACTION::PARAM::Object __this) {
    return (*(uint64_t*)&__this.fields.raw >> 34) & 1;
}

bool CanShowMegaUI(Dpr::Battle::View::UI::BUIWazaList::Object* __this) {

    auto pokeParam = __this->fields._btlPokeParam;
    PlayerWork::getClass()->initIfNeeded();

    if (PlayerWork::GetItem(array_index(ITEMS, "Key Stone")).fields.Count == 0) {
        return false;
    }

    //ToDo Consider other side trainer functionality
    if (FlagWork::GetFlag(FlagWork_Flag::FLAG_MEGA_EVOLUTION_UNAVAILABLE)) {
        return false;
    }

    switch(pokeParam->GetMonsNo()) {

        case array_index(SPECIES, "Venusaur"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Venusaurite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Charizard"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Charizardite X") ||
                pokeParam->GetItem() == array_index(ITEMS, "Charizardite Y")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Blastoise"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Blastoisite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Beedrill"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Beedrillite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Pidgeot"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Pidgeotite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Alakazam"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Alakazite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Slowbro"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Slowbroite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Gengar"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Gengarite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Kangaskhan"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Kangaskhanite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Pinsir"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Pinsirite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Gyarados"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Gyaradosite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Aerodactyl"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Aerodactylite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Mewtwo"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Mewtwonite X") ||
                pokeParam->GetItem() == array_index(ITEMS, "Mewtwonite Y")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Ampharos"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Ampharosite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Steelix"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Steelixite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Scizor"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Scizorite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Heracross"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Heracrossite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Houndoom"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Houndoomite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Tyranitar"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Tyranitarite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Sceptile"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Sceptilite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Blaziken"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Blazikenite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Swampert"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Swampertite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Gardevoir"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Gardevoirite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Sableye"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Sablenite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Mawile"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Mawilite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Aggron"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Aggronite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Medicham"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Medichamite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Sharpedo"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Sharpedonite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Camerupt"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Cameruptite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Altaria"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Altarianite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Banette"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Banettite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Absol"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Absolite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Glalie"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Glalitite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Salamence"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Salamencite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Metagross"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Metagrossite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Latias"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Latiasite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Latios"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Latiosite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Rayquaza"): {
            for (uint8_t i = 0; i < pokeParam->WAZA_GetCount(); i++) {
                if (pokeParam->WAZA_GetID(i) == array_index(MOVES, "Dragon Ascent")) {
                    return true;
                }
            }
            break;
        }

        case array_index(SPECIES, "Lopunny"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Loppunite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Garchomp"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Garchompite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Lucario"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Lucarionite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Abomasnow"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Abomasite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Gallade"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Galladite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Audino"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Audinite")) {
                return true;
            }
            break;
        }

        case array_index(SPECIES, "Diancie"): {
            if (pokeParam->GetItem() == array_index(ITEMS, "Diancite")) {
                return true;
            }
            break;
        }


        default: {
            return false;
        }

    }

    return false;
}

void MegaEvolutionFormHandler(Dpr::Battle::Logic::SectionContainer::Object* __this, Dpr::Battle::Logic::PokeAction::Object* pokeAction) {
    system_load_typeinfo(0x8910);
    Dpr::Battle::Logic::Common::getClass()->initIfNeeded();
    Dpr::Battle::Logic::BTL_POKEPARAM::Object* param = pokeAction->fields.bpp;

    Dpr::Battle::Logic::Section_FromEvent_FormChange::Description::Object* description =
            Dpr::Battle::Logic::Section_FromEvent_FormChange::Description::newInstance();

    switch (param->GetMonsNo()) {

        case array_index(SPECIES, "Charizard"): {
            description->fields.formNo = param->GetItem() == array_index(ITEMS, "Charizardite X") ? 1 : 2;
            break;
        }

        case array_index(SPECIES, "Mewtwo"): {
            description->fields.formNo = param->GetItem() == array_index(ITEMS, "Mewtwonite X") ? 1 : 2;
            break;
        }

        default: {
            description->fields.formNo = 1;
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