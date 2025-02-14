#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Dpr/UI/UIManager.h"
#include "externals/Dpr/UI/UIWazaManage.h"
#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"
#include "externals/ZukanWork.h"

#include "features/commands/utils/utils.h"
#include "logger/logger.h"

int32_t tutorSequence = 0;

void LearnMoveAfterMoveTutor_Callback(Dpr::EvScript::EvDataManager::DisplayClass1544_0::Object* __this, int32_t learnWazaNo, int32_t unlearnWazaNo)
{
    Logger::log("LearnMoveAfterMoveTutor_Callback\n");
    tutorSequence = 2;

    // User decided to not learn the move
    if (learnWazaNo == 0 || unlearnWazaNo == 0)
        return;

    __this->fields.__4__this->LearnWaza(__this->fields.param, learnWazaNo, unlearnWazaNo);
    SetWorkToValue(__this->fields.__4__this->fields._evArg->m_Items[1], learnWazaNo);
}

void MoveTutorUI_Callback(Dpr::EvScript::EvDataManager::Object *__this, int32_t learnWazaNo, int32_t unlearnWazaNo)
{
    Logger::log("MoveTutorUI_Callback\n");

    system_load_typeinfo(0x31aa);

    tutorSequence = 1;

    // unlearnWazaNo is always zero, because we're just selecting the move to tutor
    // learnWazaNo is zero if we canceled
    if (learnWazaNo == 0)
        return;

    Logger::log("[MoveTutorUI_Callback] Let's find the mon\n");

    EvData::Aregment::Array* args = __this->fields._evArg;
    int32_t index = GetWorkOrIntValue(args->m_Items[2]);
    int32_t trayIndex = GetWorkOrIntValue(args->m_Items[3]);
    auto pokeParam = __this->GetPokemonParam(trayIndex, index);

    Logger::log("[MoveTutorUI_Callback] pokeParam = %08X\n", pokeParam);

    // Already learned the move because you had less than 4 moves
    if (pokeParam->cast<Pml::PokePara::CoreParam>()->AddWazaIfEmptyExist(learnWazaNo) == 0)
        return;

    auto evDataManagerDispClass = Dpr::EvScript::EvDataManager::DisplayClass1544_0::newInstance();
    evDataManagerDispClass->fields.__4__this = __this;
    evDataManagerDispClass->fields.param = pokeParam;
    evDataManagerDispClass->fields.tray = 0;
    evDataManagerDispClass->fields.idx = 0;

    Logger::log("[MoveTutorUI_Callback] evDataManagerDispClass = %08X\n", evDataManagerDispClass);

    MethodInfo* mi = Dpr::EvScript::EvDataManager::getMethod$$EvCmdCallWazaOshieUiTutor((Il2CppMethodPointer)&LearnMoveAfterMoveTutor_Callback);
    System::Action::Object* resultCallback = System::Action::getClass(System::Action::WazaNo_WazaNo_TypeInfo)->newInstance(evDataManagerDispClass, mi);
    auto param = (Dpr::UI::UIWazaManage::Param::Object) {
        .fields = {
            .BootType = 2,
            .IsOpenFromFieldScript = true,
            .PokemonParam = pokeParam,
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

    system_load_typeinfo(0x43ef);
    EvData::Aregment::Array* args = manager->fields._evArg;

    int32_t index = GetWorkOrIntValue(args->m_Items[2]);
    int32_t trayIndex = GetWorkOrIntValue(args->m_Items[3]);
    int32_t table = GetWorkOrIntValue(args->m_Items[4]);

    auto pokeParam = manager->GetPokemonParam(trayIndex, index);

    switch (tutorSequence)
    {
        case 0:
        {
            MethodInfo* mi = Dpr::EvScript::EvDataManager::getMethod$$EvCmdCallWazaOmoidashiUiTutor((Il2CppMethodPointer)&MoveTutorUI_Callback);
            System::Action::Object* callback = System::Action::getClass(System::Action::WazaNo_WazaNo_TypeInfo)->newInstance(manager, mi);
            manager->CallWazaUICommon(3, pokeParam, callback, table);
            return false;
        }

        case 1:
            return false;

        case 2:
            tutorSequence = 0;
            return true;

        default:
            return true;
    }
}