#include "data/abilities.h"
#include "data/balls.h"
#include "data/utils.h"

#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Dpr/UI/UIManager.h"
#include "externals/Dpr/UI/UIZukanRegister.h"
#include "externals/FlagWork_Enums.h"
#include "externals/FlagWork.h"
#include "externals/PlayerWork.h"
#include "externals/poketool/poke_memo/poketool_poke_memo.h"
#include "externals/RandomGroupWork.h"
#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"

#include "features/commands/utils/cmd_utils.h"

#include "logger/logger.h"

#include "romdata/romdata.h"

void EvCmdAddUniquePokemonUI(Dpr::EvScript::EvDataManager::Object* manager, int32_t addMemberResult) {
    EvData::Aregment::Array* args = manager->fields._evArg;
    if (args->max_length >= 2) {
        manager->fields._azukariyaSequence = -1;
    }
}

bool AddUniquePokemonUI(Dpr::EvScript::EvDataManager::Object* manager)
{
    //Logger::log("_ADD_UNIQUE_POKEMON_UI\n");
    system_load_typeinfo(0x43bd);

    EvData::Aregment::Array* args = manager->fields._evArg;
    auto azukariyaSeq = manager->fields._azukariyaSequence;

    if (args->max_length >= 2) {
        if (azukariyaSeq == 0) {
            SmartPoint::AssetAssistant::SingletonMonoBehaviour::getClass()->initIfNeeded();
            auto uiManager = Dpr::UI::UIManager::get_Instance();
            auto uiZukanReg = uiManager->CreateUIWindow<Dpr::UI::UIZukanRegister>(UIWindowID::ZUKAN_REGISTER);

            MethodInfo* mi = Dpr::EvScript::EvDataManager::getMethod$$EvCmdAddUniquePokemonUI((Il2CppMethodPointer) &EvCmdAddUniquePokemonUI);
            auto onComplete = System::Action::getClass(System::Action::UIZukanRegister_AddMemberResult_TypeInfo)->newInstance(manager, mi);

            uiZukanReg->add_OnComplete(onComplete);

            auto index = GetWorkOrIntValue(args->m_Items[1]);

            auto extraData = GetUniquePokemon(index);

            Pml::PokePara::InitialSpec::Object* initialSpec = Pml::PokePara::InitialSpec::newInstance();
            initialSpec->fields.monsno = extraData.monsNo;
            initialSpec->fields.formno = extraData.formNo;
            initialSpec->fields.level = extraData.level;

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

            // Cute Charm and Synchronize
            PlayerWork::getClass()->initIfNeeded();
            auto playerparty = PlayerWork::get_playerParty();
            auto lead = playerparty->GetMemberPointer(0);
            auto leadCore = lead->cast<Pml::PokePara::CoreParam>();

            if (!leadCore->IsEgg(Pml::PokePara::EggCheckType::BOTH_EGG)) {
                switch (leadCore->GetTokuseiNo()) {
                case array_index(ABILITIES, "Synchronize"): {
                        initialSpec->fields.seikaku = leadCore->GetSeikaku();
                        break;
                }
                case array_index(ABILITIES, "Cute Charm"): {
                        auto rndSex = RandomGroupWork::RandomValue(3);
                        if (rndSex < 1)
                            break;
                        auto paramSex = leadCore->GetSex();
                        if (paramSex == Pml::Sex::MALE)
                            initialSpec->fields.sex = 1;
                        else if (paramSex == Pml::Sex::FEMALE)
                            initialSpec->fields.sex = 0;
                        break;
                    }
                }
            }

            auto coreParam = Pml::PokePara::PokemonParam::newInstance(initialSpec)->cast<Pml::PokePara::CoreParam>();

            if (extraData.formArg >= 0) coreParam->SetMultiPurposeWork(extraData.formArg);
            if (extraData.item != 0) coreParam->SetItem(extraData.item);
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
            auto pMyStatus = PlayerWork::get_playerStatus();
            auto placeNo = PlayerWork::get_zoneID();
            if (extraData.placeNo >= 0) {
                placeNo = extraData.placeNo;
            }
            poketool::poke_memo::poketool_poke_memo::SetFromCapture(coreParam, pMyStatus, placeNo);

            if (FlagWork::GetWork(FlagWork_Work::WK_SCENE_KASEKI_MONSNO) == extraData.monsNo) {
                auto playReport = PlayerWork::get_playReportDataRef();
                playReport->fields.fossil_restore += 1;
            }

            bool fastMode = coreParam->StartFastMode();
            coreParam->EndFastMode(fastMode);

            uiZukanReg->Open(reinterpret_cast<Pml::PokePara::PokemonParam::Object*>(coreParam), false, -1);
            azukariyaSeq = 0;
            manager->fields._azukariyaSequence = 1;
        }
    }
    return azukariyaSeq < 0;
}