#include "exlaunch.hpp"
#include "externals/Dpr/Battle/Logic/Section_ProcessActionCore.h"
#include "externals/Dpr/Battle/Logic/PokeAction.h"
#include "externals/Dpr/Battle/Logic/PokeActionContainer.h"
#include "externals/Dpr/Battle/Logic/BTL_ACTION.h"
#include "externals/Dpr/Battle/Logic/Section_CalcActionPriority.h"
#include "externals/Dpr/Battle/Logic/Section_FromEvent_FormChange.h"
#include "externals/Dpr/Battle/Logic/Section_FromEvent_Message.h"
#include "externals/Dpr/Battle/Logic/Section_StoreActions.h"
#include "externals/Dpr/Battle/Logic/SectionContainer.h"
#include "externals/Dpr/Battle/Logic/SVCL_ACTION.h"
#include "externals/Dpr/Battle/Logic/ActPri.h"
#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/View/UI/BUIWazaList.h"
#include "externals/Dpr/Battle/View/UI/BUIWazaButton.h"
#include "externals/Dpr/Battle/View/Systems/BattleViewSystem.h"
#include "externals/Dpr/Battle/View/BattleViewCore.h"
#include "externals/BTL_STRID_SET.h"
#include "externals/Dpr/Battle/Logic/BtlStrType.h"
#include "externals/FlagWork_Enums.h"
#include "externals/FlagWork.h"
#include "externals/PlayerWork.h"
#include "externals/Dpr/Battle/Logic/BtlRule.h"
#include "data/utils.h"
#include "data/species.h"
#include "data/items.h"
#include "data/megas.h"
#include "data/moves.h"
#include "logger/logger.h"
#include "helpers/fsHelper.h"
#include "memory/json.h"

const int32_t mega_flag_loc = 34; // Assuming the first 34 bits are used.
const long mega_flag_mask = 1L << mega_flag_loc;
const char* notDoingFilePath = "rom:/Data/ExtraData/NotDoing/RequiredItem.json";

uint8_t get_mega_flag(Dpr::Battle::Logic::BTL_ACTION::PARAM::Object __this) {
    return (*(uint64_t*)&__this.fields.raw >> 34) & 1;
}

bool IsHoldRequiredItem(Dpr::Battle::Logic::BTL_POKEPARAM::Object* pokeParam) {
    nn::string filePath(notDoingFilePath);
    nn::json j = FsHelper::loadJsonFileFromPath(filePath.c_str());
    if (j != nullptr && !j.is_discarded())
    {
        auto lookupMon = SPECIES[pokeParam->GetMonsNo()];
        auto lookupKey = j.at(lookupMon);

        // Check for correct stone
        if (lookupKey.contains("megaFormItem")) {
            Logger::log("[IsHoldRequiredItem] Contains megaFormItem.\n");
            if (lookupKey.at("megaFormItem").is_array()) {
                for (const auto& item : lookupKey.at("megaFormItem").get<nn::vector<int32_t>>()) {
                    if (pokeParam->GetItem() == item) {
                        return true;
                    }
                }
            }
            else {
                Logger::log("[IsHoldRequiredItem] Not Array.\n");
                if (pokeParam->GetItem() == lookupKey.at("megaFormItem").get<int32_t>()) {
                    Logger::log("[IsHoldRequiredItem] %s is holding the required %s.\n",
                                SPECIES[pokeParam->GetMonsNo()], ITEMS[pokeParam->GetItem()]);
                    return true;
                }
            }
        }

            // Otherwise check
        else if (lookupKey.contains("megaFormMove")) {
            for (uint8_t i = 0; i < pokeParam->WAZA_GetCount(); i++) {
                if (pokeParam->WAZA_GetID(i) == lookupKey.at("megaFormMove").get<int32_t>()) {
                    return true;
                }
            }
        }
    }

    else {
        Logger::log("Error when parsing required item data!\n");
    }

    return false;
}

bool CanShowMegaUI(Dpr::Battle::View::UI::BUIWazaList::Object* __this) {

    auto pokeParam = __this->fields._btlPokeParam;

    //ToDo Consider other side trainer functionality
    if (FlagWork::GetFlag(FlagWork_Flag::FLAG_MEGA_EVOLUTION_UNAVAILABLE)) {
        Logger::log("[CanShowMegaUI] Unavailable Flag Active.\n");
        return false;
    }

    for (int32_t i = 0; i < MEGA_COUNT; i++) {
        if (pokeParam->GetMonsNo() == MEGAEVO_MONS[i]) {
            Logger::log("[CanShowMegaUI] %s is an authorised Mega.\n", SPECIES[pokeParam->GetMonsNo()]);
            break; // Exit the loop if a match is found
        }

        // If this is the last iteration and no match was found
        if (i == MEGA_COUNT - 1) {
            Logger::log("[CanShowMegaUI] %s was not found in the mega list.\n", SPECIES[pokeParam->GetMonsNo()]);
            return false;
        }
    }

    PlayerWork::getClass()->initIfNeeded();
    if (PlayerWork::GetItem(array_index(ITEMS, "Key Stone")).fields.Count == 0) {
        Logger::log("[CanShowMegaUI] Player does not have Key Stone.\n");
        return false;
    }

    bool res = IsHoldRequiredItem(pokeParam);
    Logger::log("[CanShowMegaUI] Returning %s.\n", res ? "True" : "False");
    return res;
}

void MegaEvolutionFormHandler(Dpr::Battle::Logic::SectionContainer::Object* __this, Dpr::Battle::Logic::PokeAction::Object* pokeAction) {
    system_load_typeinfo(0x8910);
    system_load_typeinfo(0x2c99);
    system_load_typeinfo(0xa963);
    Dpr::Battle::Logic::Common::getClass()->initIfNeeded();
    Dpr::Battle::Logic::BTL_POKEPARAM::Object* param = pokeAction->fields.bpp;

    auto sectionMessageDesc = Dpr::Battle::Logic::Section_FromEvent_Message::Description::newInstance();

    if (param->GetMonsNo() == array_index(SPECIES, "Rayquaza")) {
        sectionMessageDesc->fields.pokeID = param->GetID();
        sectionMessageDesc->fields.message->Setup(Dpr::Battle::Logic::BtlStrType::BTL_STRTYPE_SET, BTL_STRID_SET::MegaRayquazaEvoStart);
        sectionMessageDesc->fields.message->AddArg(pokeAction->fields.clientID);
        sectionMessageDesc->fields.message->AddArg(param->GetID());
    }

    else {
        sectionMessageDesc->fields.pokeID = param->GetID();
        sectionMessageDesc->fields.message->Setup(Dpr::Battle::Logic::BtlStrType::BTL_STRTYPE_SET, BTL_STRID_SET::MegaEvoStart);
        sectionMessageDesc->fields.message->AddArg(param->GetID());
        sectionMessageDesc->fields.message->AddArg(pokeAction->fields.clientID);
        sectionMessageDesc->fields.message->AddArg(param->GetItem());
        sectionMessageDesc->fields.message->AddArg(array_index(ITEMS, "Key Stone"));
    }

    auto sectionMessageResult = Dpr::Battle::Logic::Section_FromEvent_Message::Result::newInstance();
    __this->fields.m_section_FromEvent_Message->Execute(sectionMessageResult, &sectionMessageDesc);

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
    description->fields.successMessage->Setup(Dpr::Battle::Logic::BtlStrType::BTL_STRTYPE_SET, param->GetMonsNo() ==
            array_index(SPECIES, "Rayquaza") ? BTL_STRID_SET::MegaRayquazaEvo : BTL_STRID_SET::MegaEvo);
    description->fields.successMessage->AddArg(param->GetID());
    description->fields.successMessage->AddArg(param->GetID());

    system_load_typeinfo(0x2c56);
    Dpr::Battle::Logic::Section_FromEvent_FormChange::Result::Object* result =
            Dpr::Battle::Logic::Section_FromEvent_FormChange::Result::newInstance();

    auto formChange = __this->fields.m_section_FromEvent_FormChange;

    formChange->Execute(result, &description);

}

HOOK_DEFINE_INLINE(CMD_ChangeForm_Start) {
    static void Callback(exl::hook::nx64::InlineCtx *ctx) {
//        Dpr::Battle::View::BattleViewCore::instance()->fields._UISystem_k__BackingField->fields.
        reinterpret_cast<Dpr::Battle::View::Systems::BattleViewSystem::Object*>(ctx->X[19])->fields.m_subSequence = 20;
    }
};

HOOK_DEFINE_TRAMPOLINE(CMD_ACT_PokeChangeEffect_WaitCore) {
    static bool Callback(Dpr::Battle::View::Systems::BattleViewSystem::Object* __this, bool isGChange) {
        if (__this->fields.m_subSequence == 20) {
            __this->PlaySequenceEffect(0, false);
            __this->fields.m_subSequence = 7;
            return false;
        }

        else {
            return Orig(__this, isGChange);
        }
    }
};
HOOK_DEFINE_INLINE(OnSubmitWazaButton) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto __this = reinterpret_cast<Dpr::Battle::View::UI::BUIWazaList::Object*>(ctx->X[19]);
        auto destActionParam = __this->fields._destActionParam;
        ctx->X[8] = reinterpret_cast<u64>(destActionParam);
        auto megaButton = reinterpret_cast<UnityEngine::Component::Object*>(__this)->get_transform()->GetChild(3);
        auto megaState = megaButton->GetChild(0)->cast<UnityEngine::Component>()->get_gameObject();
        if (megaState->get_activeSelf()) {
            destActionParam->fields.value.fields.raw |= mega_flag_mask;
        }
    }
};

HOOK_DEFINE_TRAMPOLINE(ProcessActionCore$$action) {
    static void Callback(Dpr::Battle::Logic::Section_ProcessActionCore::Object* __this,
                         Dpr::Battle::Logic::PokeAction::Object* pokeAction) {

        switch(pokeAction->fields.actionCategory) {
            case Dpr::Battle::Logic::PokeActionCategory::Mega_Evolution: {
                MegaEvolutionFormHandler(reinterpret_cast<Dpr::Battle::Logic::SectionContainer::Object*>(
                        __this->fields.m_pSectionContainer), pokeAction);
                FlagWork::SetFlag(FlagWork_Flag::FLAG_MEGA_EVOLUTION_UNAVAILABLE, true);
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
            (pokeAction->fields).actionCategory = Dpr::Battle::Logic::PokeActionCategory::Mega_Evolution;

            pokeAction->fields.bpp = __this->getActionPoke(clientInstruction, clientID);
            pokeAction->fields.priority = 2;
            pokeAction->fields.clientID = clientID;
            pokeAction->fields.fDone = false;
            pokeAction->fields.fIntrCheck = false;
            pokeAction->fields.fRecalcPriority = false;
            reinterpret_cast<Dpr::Battle::Logic::BattleEnv::Object*>(__this->fields.m_pBattleEnv)->
            fields.m_actionSerialNoManager->AssignSerialNo(pokeAction->fields.actionDesc);

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

HOOK_DEFINE_TRAMPOLINE(BUIWazaList$$OnShow) {
    static void Callback(Dpr::Battle::View::UI::BUIWazaList::Object* __this) {
        bool isShowMegaUI = CanShowMegaUI(__this);
        auto megaButton = reinterpret_cast<UnityEngine::Component::Object*>(__this)->get_transform()->GetChild(3);
        auto megaState = megaButton->GetChild(0)->cast<UnityEngine::Component>()->get_gameObject();
        if (megaState->get_activeSelf()) megaState->SetActive(false);
        megaButton->cast<UnityEngine::Component>()->get_gameObject()->SetActive(isShowMegaUI);
        Orig(__this);
    }
};

HOOK_DEFINE_TRAMPOLINE(PlaySequenceCore) {
    static void Callback(Dpr::Battle::View::Systems::BattleViewSystem::Object* __this, System::String::Object* path,
                         bool keepResource) {
        Logger::log("[PlaySequenceCore] Path: %s\n", path->asCString().c_str());
        Orig(__this, path, keepResource);
    }
};

HOOK_DEFINE_TRAMPOLINE(SetupBattleWild) {
    static void Callback(Dpr::Battle::Logic::BATTLE_SETUP_PARAM::Object* battleSetupParam,
                         Pml::PokeParty::Object* iPtrEnemyParty, int32_t arenaID, int32_t mapAttrib, int32_t weatherType,
                         bool isSwim, bool isFishing, int32_t partnerID, bool isCaptureDemo, int32_t safariBallNum,
                         bool isSymbol, bool isMitu, System::String::Object* overlapBgm, int32_t overlapSetupEffectId,
                         bool isCantUseBall) {


        FlagWork::SetFlag(FlagWork_Flag::FLAG_MEGA_EVOLUTION_UNAVAILABLE, (isCaptureDemo ||
        battleSetupParam->fields.rule == Dpr::Battle::Logic::BtlRule::BTL_RULE_SAFARI));

        Orig(battleSetupParam, iPtrEnemyParty, arenaID, mapAttrib, weatherType, isSwim, isFishing, partnerID, isCaptureDemo,
             safariBallNum, isSymbol, isMitu, overlapBgm, overlapSetupEffectId, isCantUseBall);
    }
};

HOOK_DEFINE_TRAMPOLINE(SetupBattleTrainer) {
    static void Callback(Dpr::Battle::Logic::BATTLE_SETUP_PARAM::Object* battleSetupParam, int32_t arenaID,
                         int32_t mapAttrib, int32_t weatherType, int32_t rule, int32_t enemyID0, int32_t enemyID1,
                         int32_t partnerID) {

        FlagWork::SetFlag(FlagWork_Flag::FLAG_MEGA_EVOLUTION_UNAVAILABLE,
                          (rule == static_cast<int32_t>(Dpr::Battle::Logic::BtlRule::BTL_RULE_SAFARI) ||
                                  rule == static_cast<int32_t>(Dpr::Battle::Logic::BtlRule::BTL_RULE_RAID)));

        Orig(battleSetupParam, arenaID, mapAttrib, weatherType, rule, enemyID0, enemyID1, partnerID);
    }
};

void exl_mega_evolution_main() {
    OnSubmitWazaButton::InstallAtOffset(0x01d2cee8);
    CalcActionPriority$$Execute::InstallAtOffset(0x021ad570);
    ProcessActionCore$$action::InstallAtOffset(0x021c0de0);
    setupPokeAction_FromClientInstruction::InstallAtOffset(0x021cdbf0);
    createPokeAction_FromClientInstruction::InstallAtOffset(0x021cdad0);
    BUIWazaList$$OnShow::InstallAtOffset(0x01d2cb70);
    CMD_ACT_PokeChangeEffect_WaitCore::InstallAtOffset(0x01c86aa0);
    CMD_ChangeForm_Start::InstallAtOffset(0x01c86d9c);
    PlaySequenceCore::InstallAtOffset(0x01c85970);
    SetupBattleWild::InstallAtOffset(0x02c3abc0);
    SetupBattleTrainer::InstallAtOffset(0x02c3b800);
}