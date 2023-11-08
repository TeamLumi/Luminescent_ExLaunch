#include "externals/Dpr/Demo/Demo_Gosanke.h"
#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/FieldCanvas.h"
#include "externals/PlayerWork.h"
#include "externals/Pml/PokePara/InitialSpec.h"
#include "externals/System/Action.h"
#include "externals/System/String.h"

#include "features/commands/utils/utils.h"
#include "logger/logger.h"
#include "romdata/romdata.h"

bool FirstPokeSelectProc(Dpr::EvScript::EvDataManager::Object* manager)
{
    //Logger::log("_FIRST_POKE_SELECT_PROC\n");
    system_load_typeinfo(0x438a);

    EvData::Aregment::Array* args = manager->fields._evArg;

    switch (manager->fields._talkStart)
    {
        case Dpr::EvScript::EvDataManager::TalkState::Init: {
            manager->fields._talkStart = Dpr::EvScript::EvDataManager::TalkState::EndWait;

            auto demo = Dpr::Demo::Demo_Gosanke::newInstance();

            MethodInfo* mi = *Dpr::EvScript::EvDataManager::Method$$EvDataManager_CmdFirstPokeSelectProc;
            auto onEndDemo = System::Action::getClass(System::Action::void_TypeInfo)->newInstance(manager, mi);

            demo->fields.OnEndDemo = onEndDemo;

            FieldCanvas::getClass()->initIfNeeded();
            FieldCanvas::PlayDemoOrStock((Dpr::Demo::DemoBase::Object*)demo);
            return false;
        }

        case Dpr::EvScript::EvDataManager::TalkState::EndWait: {
            return false;
        }

        case Dpr::EvScript::EvDataManager::TalkState::CloseWait: {
        default:
            manager->fields._custumWindow_msbt = System::String::Create("");

            manager->fields._custumWindow_Labels->Clear();
            manager->fields._custumWindow_RetIndex->Clear();

            int32_t starterId = PlayerWork::get_defaultPokeType();
            RomData::Starter starter = GetStarter(starterId);

            bool addSuccess = AddPokemonToParty(starter.monsNo, starter.formNo, starter.level, 0, starter.itemNo);

            if (args->max_length >= 2)
            {
                SetWorkToValue(args->m_Items[1], (int32_t)addSuccess);
            }

            manager->fields._talkStart = Dpr::EvScript::EvDataManager::TalkState::Init;
            return true;
        }
    }
}