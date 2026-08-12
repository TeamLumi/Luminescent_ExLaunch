#include "data/balls.h"
#include "data/utils.h"

#include "externals/Dpr/Box/BoxPokemonWork.h"
#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Dpr/Demo/Demo_MysteryGift.h"
#include "externals/FieldCanvas.h"
#include "externals/FieldPoketch.h"
#include "externals/Dpr/Message/MessageManager.h"
#include "externals/Dpr/Message/MessageWordSetHelper.h"
#include "externals/Dpr/MsgWindow/MsgWindowManager.h"
#include "externals/Dpr/MsgWindow/MsgWindowParam.h"
#include "externals/PlayerWork.h"
#include "externals/poketool/poke_memo/poketool_poke_memo.h"
#include "externals/ZukanWork.h"

#include "logger/logger.h"

#include "romdata/romdata.h"

#include "utils/cmd_utils.h"

static float giftDeltaTime = 0.0f;
static Dpr::Demo::Demo_MysteryGift::Object* giftDemo = nullptr;
static Dpr::MsgWindow::MsgWindowParam::Object* giftMsgParam = nullptr;

bool AddMysteryGiftPokemon(Dpr::EvScript::EvDataManager::Object* manager) {

    system_load_typeinfo(0x9358);

    EvData::Aregment::Array* args = manager->fields._evArg;

    if (manager->fields._azukariyaSequence == 1) { // Checks if enough time has elapsed to display the received message
        giftDeltaTime += manager->fields._deltatime;
        if (giftDeltaTime < 13.5f) {
            return false;
        }

        Logger::log("_ADD_MYSTERY_GIFT_POKEMON Opening Gift Message at %f\n", giftDeltaTime);
        Dpr::MsgWindow::MsgWindowManager::OpenMsg(giftMsgParam);

        manager->fields._azukariyaSequence = 2;
        return false;
    }

    if (manager->fields._azukariyaSequence == 2) { // Checks if the Mystery Gift Demo is still active before releasing the command
        if (giftDemo->fields.manager != nullptr) {
            return false;
        }

        Logger::log("_ADD_MYSTERY_GIFT_POKEMON Demo Ended\n");
        giftDeltaTime = 0.0f;
        giftDemo = nullptr;
        giftMsgParam = nullptr;
        manager->fields._azukariyaSequence = -1;
        return true;
    }

    if (args->max_length >= 2) {
        if (manager->fields._azukariyaSequence == 0) {

            auto index = GetWorkOrIntValue(args->m_Items[1]);
            Logger::log("_ADD_MYSTERY_GIFT_POKEMON Index %d\n", index);

            auto extraData = GetMysteryGiftPokemon(index);

            Dpr::Message::MessageManager::Object* messageManager = Dpr::Message::MessageManager::get_Instance();
            PlayerWork::getClass()->initIfNeeded();
            auto pMyStatus = PlayerWork::get_playerStatus();
            auto playerID = pMyStatus->fields.id;
            auto playerName = pMyStatus->fields.name;

            Pml::PokePara::InitialSpec::Object* initialSpec = Pml::PokePara::InitialSpec::newInstance();
            initialSpec->fields.monsno = extraData.monsNo;
            initialSpec->fields.formno = extraData.formNo;
            if (!extraData.isEgg) initialSpec->fields.level = extraData.level;

            if (extraData.gender != 255) initialSpec->fields.sex = extraData.gender;
            if (extraData.nature != 65535) initialSpec->fields.seikaku = extraData.nature;
            if (extraData.ability >= 0) initialSpec->fields.tokuseiIndex = extraData.ability;

            if (extraData.rand != 0) initialSpec->fields.personalRnd = extraData.rand;
            if (extraData.rand != 0) initialSpec->fields.randomSeed = extraData.rand;
            if (extraData.rand != 0) initialSpec->fields.isRandomSeedEnable = true;

            if (extraData.maxIvs > 0) initialSpec->fields.talentVNum = extraData.maxIvs;
            if (!extraData.ivs.empty()) {
                for (uint64_t i=0; i<initialSpec->fields.talentPower->max_length; i++)
                    initialSpec->fields.talentPower->m_Items[i] = extraData.ivs[i];
            }

            if (extraData.trainerId > 0) {
                initialSpec->fields.id = extraData.trainerId;
            }
            else {
                initialSpec->fields.id = playerID;
            }

            auto coreParam = Pml::PokePara::PokemonParam::newInstance(initialSpec)->cast<Pml::PokePara::CoreParam>();

            if (extraData.isEgg) coreParam->ChangeEgg();

            System::String::Object* msgFile = System::String::Create("dp_scenario3");

            auto trainerLabel = System::String::Create(extraData.trainerLabel);
            if (!System::String::IsNullOrEmpty(trainerLabel)) {
                coreParam->SetParentName(messageManager->GetNameMessage(msgFile, trainerLabel));
            }
            else {
                coreParam->SetParentName(playerName);
            }

            auto nicknameLabel = System::String::Create(extraData.nicknameLabel);
            if (!System::String::IsNullOrEmpty(nicknameLabel)) {
                coreParam->SetNickName(messageManager->GetNameMessage(msgFile, nicknameLabel));
            }

            if (extraData.formArg >= 0) coreParam->SetMultiPurposeWork(extraData.formArg);
            if (extraData.item != 0) coreParam->SetItem(extraData.item);
            coreParam->SetLangId(extraData.langId);
            coreParam->SetGetBall(extraData.ballId == array_index(BALLS, "--BALL ZERO--") ? array_index(BALLS, "Poké Ball") : extraData.ballId);

            if (!extraData.evs.empty()) {
                for (int32_t i=0; i<6; i++)
                    coreParam->ChangeEffortPower((Pml::PokePara::PowerID)i, extraData.evs[i]);
            }

            if (!extraData.contestStats.empty()) {
                for (int32_t i=0; i<5; i++)
                    coreParam->SetCondition((Pml::PokePara::Condition)i, extraData.contestStats[i]);

                if (extraData.contestStats.size() >= 6)
                    coreParam->SetCondition(Pml::PokePara::Condition::FUR, extraData.contestStats[5]);
            }

            for (uint64_t i=0; i<4; i++)
                coreParam->SetWaza(i, extraData.moves[i]);

            for (uint32_t i : extraData.ribbons) {
                if (i < 128)
                    coreParam->SetRibbon(i);
            }

            if (extraData.shiny == 0) coreParam->SetRareType(Pml::PokePara::RareType::NOT_RARE); // Never shiny
            if (extraData.shiny == 1) coreParam->SetRareType(Pml::PokePara::RareType::CAPTURED); // Shiny
            if (extraData.shiny == 2) coreParam->SetRareType(Pml::PokePara::RareType::DISTRIBUTED); // Square shiny

            if (extraData.friendship >= 0) coreParam->SetFriendship(extraData.friendship);
            if (extraData.pokerus >= 0) coreParam->SetPokerus(extraData.pokerus);
            if (extraData.fateful) coreParam->SetEventPokeFlag(extraData.fateful);

            PlayerWork::getClass()->initIfNeeded();
            auto placeNo = PlayerWork::get_zoneID();
            if (extraData.placeNo >= 0) {
                placeNo = extraData.placeNo;
            }

            if (extraData.trainerId == 0) {
                if (extraData.isEgg) {
                    poketool::poke_memo::poketool_poke_memo::SetFromEggTaken(coreParam, pMyStatus, placeNo);
                }
                else {
                    poketool::poke_memo::poketool_poke_memo::SetFromCapture(coreParam, pMyStatus, placeNo);
                }
            }
            else {
                if (extraData.isEgg) {
                    coreParam->SetMemories(0, extraData.eggYear);
                    coreParam->SetMemories(1, extraData.eggMonth);
                    coreParam->SetMemories(2, extraData.eggDay);
                    coreParam->SetMemories(3, extraData.eggPlaceNo);
                    coreParam->SetMemories(7, 65535);
                }
                if (extraData.wasEgg) {
                    coreParam->SetMemories(0, extraData.eggYear);
                    coreParam->SetMemories(1, extraData.eggMonth);
                    coreParam->SetMemories(2, extraData.eggDay);
                    coreParam->SetMemories(3, extraData.eggPlaceNo);
                    coreParam->SetMemories(4, extraData.metYear);
                    coreParam->SetMemories(5, extraData.metMonth);
                    coreParam->SetMemories(6, extraData.metDay);
                    coreParam->SetMemories(7, placeNo);
                }
                if (!extraData.wasEgg) {
                    coreParam->SetMemories(3, 65535);
                    coreParam->SetMemories(4, extraData.metYear);
                    coreParam->SetMemories(5, extraData.metMonth);
                    coreParam->SetMemories(6, extraData.metDay);
                    coreParam->SetMemories(7, placeNo);
                }
            }

            coreParam->SetMemories(8, extraData.ballId);
            coreParam->SetMemories(9, extraData.originalLevel);

            poketool::poke_memo::poketool_poke_memo::SetVersion(coreParam);

            bool fastMode = coreParam->StartFastMode();
            coreParam->EndFastMode(fastMode);

            auto pokemonParam = reinterpret_cast<Pml::PokePara::PokemonParam::Object*>(coreParam);

            ZukanWork::getClass()->initIfNeeded();
            bool isGetMons = ZukanWork::IsGet(coreParam->GetMonsNo());

            if (!extraData.isEgg) {
                ZukanWork::SetPoke(pokemonParam, 3);
                FieldPoketch::getClass()->initIfNeeded();
                FieldPoketch::AddPokemonHistory(pokemonParam);
            }

            auto party = PlayerWork::get_playerParty();
            bool partyFull = party->IsFull();

            giftMsgParam = Dpr::MsgWindow::MsgWindowParam::newInstance();
            giftMsgParam->fields.useMsgFile = messageManager->GetMsgFile(msgFile);
            giftMsgParam->fields.inputCloseEnabled = true;
            Dpr::Message::MessageWordSetHelper::SetPokemonNickNameWord(0, coreParam, true);

            if (partyFull) {
                int32_t tray = 0;
                int32_t slot = 0;
                Dpr::Box::BoxPokemonWork::PutPokemonEmptyTrayAll(pokemonParam, &tray, &slot);
                giftMsgParam->fields.labelName = System::String::Create("MYSTERY_GIFT_RECEIVED_BOXES");
            }
            else {
                party->AddMember(pokemonParam);
                giftMsgParam->fields.labelName = System::String::Create("MYSTERY_GIFT_RECEIVED_PARTY");
            }

            giftDemo = Dpr::Demo::Demo_MysteryGift::newInstance();
            giftDemo->fields.gift_Pokemon = pokemonParam;
            giftDemo->fields.isGetMons = isGetMons;

            FieldCanvas::getClass()->initIfNeeded();
            FieldCanvas::PlayDemoOrStock(giftDemo->cast<Dpr::Demo::DemoBase>());

            manager->fields._azukariyaSequence = 1;
            return false;
        }
        return true;
    }
    return true;
}