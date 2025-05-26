#include "externals/Dpr/Box/BoxPokemonWork.h"
#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Dpr/UI/UIManager.h"
#include "externals/Dpr/UI/UIWazaManage.h"
#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"
#include "romdata/romdata.h"
#include "utils/utils.h"

int32_t tutorSequence = 0;

bool CanAnyMoveBeLearned(Pml::PokePara::PokemonParam::Object* pokemonParam)
{
    auto monsno = pokemonParam->cast<Pml::PokePara::CoreParam>()->GetMonsNo();
    auto formno = pokemonParam->cast<Pml::PokePara::CoreParam>()->GetFormNo();

    for (uint64_t i=0; i<GetGTutorTables()->size(); i++)
    {
        auto moves = GetMoveTutorTable((*GetGTutorTables())[i]);

        for (uint64_t j=0; j<moves.size(); j++)
        {
            if (IsMoveLearnableByTutor(monsno, formno, moves[j]) && !pokemonParam->cast<Pml::PokePara::CoreParam>()->HaveWaza(moves[j]))
                return true;
        }
    }

    return false;
}

void LearnMoveAfterMoveTutor_Callback(Dpr::EvScript::EvDataManager::DisplayClass1544_0::Object* __this, int32_t learnWazaNo, int32_t unlearnWazaNo)
{
    Logger::log("LearnMoveAfterMoveTutor_Callback\n");
    tutorSequence = 2;

    EvData::Aregment::Array* args = __this->fields.__4__this->fields._evArg;

    int32_t index = GetWorkOrIntValue(args->m_Items[2]);
    int32_t trayIndex = GetWorkOrIntValue(args->m_Items[3]);

    // User decided to not learn the move
    if (learnWazaNo == 0 || unlearnWazaNo == 0)
    {
        SetWorkToValue(args->m_Items[1], 0);
        return;
    }

    __this->fields.__4__this->LearnWaza(__this->fields.param, learnWazaNo, unlearnWazaNo);
    SetWorkToValue(args->m_Items[1], learnWazaNo);
    Dpr::Box::BoxPokemonWork::UpdatePokemon(__this->fields.param, trayIndex, index);
}

void MoveTutorUI_Callback(Dpr::EvScript::EvDataManager::DisplayClass1544_0::Object *__this, int32_t learnWazaNo, int32_t unlearnWazaNo)
{
    Logger::log("MoveTutorUI_Callback\n");

    system_load_typeinfo(0x31aa);
    system_load_typeinfo(0x43f0);

    EvData::Aregment::Array* args = __this->fields.__4__this->fields._evArg;

    int32_t index = GetWorkOrIntValue(args->m_Items[2]);
    int32_t trayIndex = GetWorkOrIntValue(args->m_Items[3]);

    // unlearnWazaNo is always zero, because we're just selecting the move to tutor
    // learnWazaNo is zero if we canceled
    if (learnWazaNo == 0)
    {
        tutorSequence = 2;
        SetWorkToValue(args->m_Items[1], 0);
        return;
    }

    // Already learned the move because you had less than 4 moves
    if (__this->fields.param->cast<Pml::PokePara::CoreParam>()->AddWazaIfEmptyExist(learnWazaNo) == 0)
    {
        tutorSequence = 2;
        SetWorkToValue(args->m_Items[1], learnWazaNo);
        Dpr::Box::BoxPokemonWork::UpdatePokemon(__this->fields.param, trayIndex, index);
        return;
    }

    // Prepare another move UI to select one to overwrite
    MethodInfo* mi = Dpr::EvScript::EvDataManager::getMethod$$EvCmdCallWazaOshieUiTutor((Il2CppMethodPointer)&LearnMoveAfterMoveTutor_Callback);
    System::Action::Object* resultCallback = System::Action::getClass(System::Action::WazaNo_WazaNo_TypeInfo)->newInstance(__this, mi);
    auto param = (Dpr::UI::UIWazaManage::Param::Object) {
        .fields = {
            .BootType = 2,
            .IsOpenFromFieldScript = true,
            .PokemonParam = __this->fields.param,
            .LearnWazaNo = learnWazaNo,
            .ResultCallback = resultCallback,
        }
    };

    SmartPoint::AssetAssistant::SingletonMonoBehaviour::getClass()->initIfNeeded();
    auto uiManager = Dpr::UI::UIManager::get_Instance();
    auto uiWazaManage = (Dpr::UI::UIWazaManage::Object *) uiManager->CreateUIWindow(UIWindowID::WAZA_MANAGE, Dpr::UI::UIManager::Method$$CreateUIWindow_UIWazaManage_);
    uiWazaManage->Open(param, nullptr);
}

bool MoveTutorUI(Dpr::EvScript::EvDataManager::Object* manager)
{
    //Logger::log("_MOVE_TUTOR_UI\n");

    system_load_typeinfo(0x31aa);
    system_load_typeinfo(0x43ef);
    system_load_typeinfo(0x43f0);

    EvData::Aregment::Array* args = manager->fields._evArg;

    int32_t index = GetWorkOrIntValue(args->m_Items[2]);
    int32_t trayIndex = GetWorkOrIntValue(args->m_Items[3]);

    switch (tutorSequence)
    {
        case 0:
        {
            auto pokeParam = manager->GetPokemonParam(trayIndex, index);

            // No available move to learn
            if (!CanAnyMoveBeLearned(pokeParam))
            {
                SetWorkToValue(args->m_Items[1], -1);
                return true;
            }

            auto evDataManagerDispClass = Dpr::EvScript::EvDataManager::DisplayClass1544_0::newInstance();
            evDataManagerDispClass->fields.__4__this = manager;
            evDataManagerDispClass->fields.param = pokeParam;
            evDataManagerDispClass->fields.tray = trayIndex;
            evDataManagerDispClass->fields.idx = index;

            MethodInfo* mi = Dpr::EvScript::EvDataManager::getMethod$$EvCmdCallWazaOmoidashiUiTutor((Il2CppMethodPointer)&MoveTutorUI_Callback);
            System::Action::Object* callback = System::Action::getClass(System::Action::WazaNo_WazaNo_TypeInfo)->newInstance(evDataManagerDispClass, mi);
            manager->CallWazaUICommon(3, pokeParam, callback, 0);

            tutorSequence = 1;
            return false;
        }

        case 1:
            return false;

        case 2:
        {
            GetGTutorTables()->clear();
            tutorSequence = 0;
            return true;
        }

        default:
            return true;
    }
}