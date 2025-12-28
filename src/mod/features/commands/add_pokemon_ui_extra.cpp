#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/PlayerWork.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"
#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"
#include "externals/Dpr/UI/UIManager.h"
#include "externals/Dpr/UI/UIZukanRegister.h"
#include "externals/poketool/poke_memo/poketool_poke_memo.h"
#include "externals/FlagWork_Enums.h"
#include "externals/FlagWork.h"

void EvCmdAddPokemonUIExtra(Dpr::EvScript::EvDataManager::Object* manager, int32_t addMemberResult) {
    EvData::Aregment::Array* args = manager->fields._evArg;
    if (args->max_length >= 7) {
        manager->fields._azukariyaSequence = -1;
        return;
    }
}

bool AddPokemonUIExtra(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_ADD_POKEMON_UI_EXTRA\n");
    system_load_typeinfo(0x43bd);

    EvData::Aregment::Array* args = manager->fields._evArg;
    auto azukariyaSeq = manager->fields._azukariyaSequence;

    if (args->max_length >= 7) {
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
            auto item = GetWorkOrIntValue(args->m_Items[4]);
            auto maxIVs = GetWorkOrIntValue(args->m_Items[5]);
            auto ball = GetWorkOrIntValue(args->m_Items[6]);
            auto shiny = -1; // Defaults to regular shiny calculation in case arg not given

            if (args->max_length >= 8) {
                shiny = GetWorkOrIntValue(args->m_Items[7]);
            }

            Pml::PokePara::InitialSpec::Object* initialSpec = Pml::PokePara::InitialSpec::newInstance();
            initialSpec->fields.monsno = monsNo;
            initialSpec->fields.formno = formNo;
            initialSpec->fields.level = level;
            initialSpec->fields.talentVNum = maxIVs;
            auto coreParam = Pml::PokePara::PokemonParam::newInstance(initialSpec)->cast<Pml::PokePara::CoreParam>();
            if (item != 0) coreParam->SetItem(item);
            coreParam->SetGetBall(ball);
            if (shiny == 0) coreParam->SetRareType(Pml::PokePara::RareType::NOT_RARE); // Never shiny
            if (shiny == 1) coreParam->SetRareType(Pml::PokePara::RareType::CAPTURED); // Shiny
            if (shiny == 2) coreParam->SetRareType(Pml::PokePara::RareType::DISTRIBUTED); // Square shiny

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