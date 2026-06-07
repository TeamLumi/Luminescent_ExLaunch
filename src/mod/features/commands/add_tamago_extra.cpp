#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/PlayerWork.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"
#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"
#include "externals/Dpr/UI/UIManager.h"
#include "externals/Dpr/UI/UIZukanRegister.h"
#include "externals/poketool/poke_memo/poketool_poke_memo.h"

void EvCmdAddTamagoExtra(Dpr::EvScript::EvDataManager::Object* manager, int32_t addMemberResult) {
    EvData::Aregment::Array* args = manager->fields._evArg;
    if (args->max_length >= 3) {
        manager->fields._azukariyaSequence = -1;
        return;
    }
}

bool AddTamagoExtra(Dpr::EvScript::EvDataManager::Object* manager)
{
    //Logger::log("_ADD_TAMAGO_EXTRA\n");
    system_load_typeinfo(0x43bf);

    EvData::Aregment::Array* args = manager->fields._evArg;
    auto azukariyaSeq = manager->fields._azukariyaSequence;

    if (args->max_length >= 3) {
        if (azukariyaSeq == 0) {
            SmartPoint::AssetAssistant::SingletonMonoBehaviour::getClass()->initIfNeeded();
            auto uiManager = Dpr::UI::UIManager::get_Instance();
            auto uiZukanReg = uiManager->CreateUIWindow<Dpr::UI::UIZukanRegister>(UIWindowID::ZUKAN_REGISTER);

            MethodInfo* mi = Dpr::EvScript::EvDataManager::getMethod$$EvCmdAddTamagoExtra((Il2CppMethodPointer) &EvCmdAddTamagoExtra);
            auto onComplete = System::Action::getClass(
                    System::Action::UIZukanRegister_AddMemberResult_TypeInfo)->newInstance(manager, mi);

            uiZukanReg->add_OnComplete(onComplete);

            auto monsNo = GetWorkOrIntValue(args->m_Items[1]);
            auto formNo = GetWorkOrIntValue(args->m_Items[2]);
            auto receivedFrom = -1; // Defaults to ZoneID in case arg not given
            auto maxIVs = -1; // Defaults to regular IVs calculation in case arg not given
            auto ball = 4; // Defaults to regular Poke Ball in case arg not given
            auto shiny = -1; // Defaults to regular shiny calculation in case arg not given
            auto gender = -1; // Defaults to regular gender calculation in case arg not given
            auto formArg = -1; // Defaults to no variant in case arg not given
            auto nature = -1; // Defaults to regular nature calculation in case arg not given
            auto ability = -1; // Defaults to regular ability calculation in case arg not given

            if (args->max_length >= 4) {
                receivedFrom = GetWorkOrIntValue(args->m_Items[3]);
            }
            if (args->max_length >= 5) {
                maxIVs = GetWorkOrIntValue(args->m_Items[4]);
            }
            if (args->max_length >= 6) {
                ball = GetWorkOrIntValue(args->m_Items[5]);
            }
            if (args->max_length >= 7) {
                shiny = GetWorkOrIntValue(args->m_Items[6]);
            }
            if (args->max_length >= 8) {
                gender = GetWorkOrIntValue(args->m_Items[7]);
            }
            if (args->max_length >= 9) {
                formArg = GetWorkOrIntValue(args->m_Items[8]);
            }
            if (args->max_length >= 10) {
                nature = GetWorkOrIntValue(args->m_Items[9]);
            }
            if (args->max_length >= 11) {
                ability = GetWorkOrIntValue(args->m_Items[10]);
            }

            Pml::PokePara::InitialSpec::Object* initialSpec = Pml::PokePara::InitialSpec::newInstance();
            initialSpec->fields.monsno = monsNo;
            initialSpec->fields.formno = formNo;
            if (maxIVs >= 0) initialSpec->fields.talentVNum = maxIVs;
            if (gender >= 0) initialSpec->fields.sex = gender;
            if (nature >= 0) initialSpec->fields.seikaku = nature;
            if (ability >= 0) initialSpec->fields.tokuseiIndex = ability;
            auto coreParam = Pml::PokePara::PokemonParam::newInstance(initialSpec)->cast<Pml::PokePara::CoreParam>();
            coreParam->SetGetBall(ball);
            if (shiny == 0) coreParam->SetRareType(Pml::PokePara::RareType::NOT_RARE); // Never shiny
            if (shiny == 1) coreParam->SetRareType(Pml::PokePara::RareType::CAPTURED); // Shiny
            if (shiny == 2) coreParam->SetRareType(Pml::PokePara::RareType::DISTRIBUTED); // Square shiny
            if (formArg >= 0) coreParam->SetMultiPurposeWork(formArg);
            coreParam->ChangeEgg();

            PlayerWork::getClass()->initIfNeeded();
            auto pMyStatus = PlayerWork::get_playerStatus();
            auto placeNo = PlayerWork::get_zoneID();
            if (receivedFrom >= 0) {
                placeNo = receivedFrom;
            }
            poketool::poke_memo::poketool_poke_memo::SetFromEggTaken(coreParam, pMyStatus, placeNo);

            uiZukanReg->OpenAddMemberOnly(reinterpret_cast<Pml::PokePara::PokemonParam::Object*>(coreParam), false, -1);
            azukariyaSeq = 0;
            manager->fields._azukariyaSequence = 1;
        }
    }
    return azukariyaSeq < 0;
}