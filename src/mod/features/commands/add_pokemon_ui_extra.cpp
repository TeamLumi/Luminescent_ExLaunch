#include "data/abilities.h"
#include "data/utils.h"

#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Dpr/UI/UIManager.h"
#include "externals/Dpr/UI/UIZukanRegister.h"
#include "externals/FlagWork.h"
#include "externals/FlagWork_Enums.h"
#include "externals/PlayerWork.h"
#include "externals/poketool/poke_memo/poketool_poke_memo.h"
#include "externals/RandomGroupWork.h"
#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"

#include "features/commands/utils/cmd_utils.h"

#include "logger/logger.h"

void EvCmdAddPokemonUIExtra(Dpr::EvScript::EvDataManager::Object* manager, int32_t addMemberResult) {
    EvData::Aregment::Array* args = manager->fields._evArg;
    if (args->max_length >= 7) {
        manager->fields._azukariyaSequence = -1;
        return;
    }
}

bool AddPokemonUIExtra(Dpr::EvScript::EvDataManager::Object* manager)
{
    //Logger::log("_ADD_POKEMON_UI_EXTRA\n");
    system_load_typeinfo(0x43bd);

    EvData::Aregment::Array* args = manager->fields._evArg;
    auto azukariyaSeq = manager->fields._azukariyaSequence;

    if (args->max_length >= 4) {
        if (azukariyaSeq == 0) {
            SmartPoint::AssetAssistant::SingletonMonoBehaviour::getClass()->initIfNeeded();
            auto uiManager = Dpr::UI::UIManager::get_Instance();
            auto uiZukanReg = uiManager->CreateUIWindow<Dpr::UI::UIZukanRegister>(UIWindowID::ZUKAN_REGISTER);

            MethodInfo* mi = Dpr::EvScript::EvDataManager::getMethod$$EvCmdAddPokemonUIExtra((Il2CppMethodPointer) &EvCmdAddPokemonUIExtra);
            auto onComplete = System::Action::getClass(
                    System::Action::UIZukanRegister_AddMemberResult_TypeInfo)->newInstance(manager, mi);

            uiZukanReg->add_OnComplete(onComplete);

            auto monsNo = GetWorkOrIntValue(args->m_Items[1]);
            auto formNo = GetWorkOrIntValue(args->m_Items[2]);
            auto level = GetWorkOrIntValue(args->m_Items[3]);
            auto item = -1; // Defaults to none in case arg not given
            auto maxIVs = -1; // Defaults to none in case arg not given
            auto ball = -1; // Defaults to regular poke ball in case arg not given
            auto shiny = -1; // Defaults to regular shiny calculation in case arg not given
            auto gender = -1; // Defaults to regular gender calculation in case arg not given
            auto formArg = -1; // Defaults to no variant in case arg not given
            auto nature = -1; // Defaults to regular nature calculation in case arg not given
            auto ability = -1; // Defaults to regular ability calculation in case arg not given

            if (args->max_length >= 5) {
                item = GetWorkOrIntValue(args->m_Items[4]);
                if (item == -1) {
                    item = 0;
                }
            }
            if (args->max_length >= 6) {
                maxIVs = GetWorkOrIntValue(args->m_Items[5]);
            }
            if (args->max_length >= 7) {
                ball = GetWorkOrIntValue(args->m_Items[6]);
            }
            if (args->max_length >= 8) {
                shiny = GetWorkOrIntValue(args->m_Items[7]);
            }
            if (args->max_length >= 9) {
                gender = GetWorkOrIntValue(args->m_Items[8]);
            }
            if (args->max_length >= 10) {
                formArg = GetWorkOrIntValue(args->m_Items[9]);
            }
            if (args->max_length >= 11) {
                nature = GetWorkOrIntValue(args->m_Items[10]);
            }
            if (args->max_length >= 12) {
                ability = GetWorkOrIntValue(args->m_Items[11]);
            }

            Pml::PokePara::InitialSpec::Object* initialSpec = Pml::PokePara::InitialSpec::newInstance();
            initialSpec->fields.monsno = monsNo;
            initialSpec->fields.formno = formNo;
            initialSpec->fields.level = level;
            if (maxIVs >= 0) initialSpec->fields.talentVNum = maxIVs;
            if (gender >= 0) initialSpec->fields.sex = gender;
            if (nature >= 0) initialSpec->fields.seikaku = nature;
            if (ability >= 0) initialSpec->fields.tokuseiIndex = ability;


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
            if (item != 0) coreParam->SetItem(item);

            if (ball == -1) {
                ball = 4;
            }
            coreParam->SetGetBall(ball);

            if (shiny == 0) coreParam->SetRareType(Pml::PokePara::RareType::NOT_RARE); // Never shiny
            if (shiny == 1) coreParam->SetRareType(Pml::PokePara::RareType::CAPTURED); // Shiny
            if (shiny == 2) coreParam->SetRareType(Pml::PokePara::RareType::DISTRIBUTED); // Square shiny
            if (formArg >= 0) coreParam->SetMultiPurposeWork(formArg);

            PlayerWork::getClass()->initIfNeeded();
            auto pMyStatus = PlayerWork::get_playerStatus();
            auto placeNo = PlayerWork::get_zoneID();
            poketool::poke_memo::poketool_poke_memo::SetFromCapture(coreParam, pMyStatus, placeNo);

            if (FlagWork::GetWork(FlagWork_Work::WK_SCENE_KASEKI_MONSNO) == monsNo) {
                auto playReport = PlayerWork::get_playReportDataRef();
                playReport->fields.fossil_restore += 1;
            }

            uiZukanReg->Open(reinterpret_cast<Pml::PokePara::PokemonParam::Object*>(coreParam), false, -1);
            azukariyaSeq = 0;
            manager->fields._azukariyaSequence = 1;
        }
    }
    return azukariyaSeq < 0;
}