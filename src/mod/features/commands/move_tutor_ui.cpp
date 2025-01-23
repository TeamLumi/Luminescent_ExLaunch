#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/ZukanWork.h"

#include "features/commands/utils/utils.h"
#include "logger/logger.h"
#include "romdata/romdata.h"

bool MoveTutorUI(Dpr::EvScript::EvDataManager::Object* manager)
{
    //Logger::log("_MOVE_TUTOR_UI\n");

    system_load_typeinfo(0x43ef);
    EvData::Aregment::Array* args = manager->fields._evArg;

    int32_t index = GetWorkOrIntValue(args->m_Items[2]);
    int32_t trayIndex = GetWorkOrIntValue(args->m_Items[3]);
    int32_t table = GetWorkOrIntValue(args->m_Items[4]);

    auto pokeParam = manager->GetPokemonParam(trayIndex, index);

    // Callback sets arg1 to the learned move
    auto callback = System::Action::getClass(System::Action::WazaNo_WazaNo_TypeInfo)->newInstance(manager, *Dpr::EvScript::EvDataManager::Method$$EvDataManager_EvCmdCallWazaOmoidashiUi);
    return manager->CallWazaUICommon(3, pokeParam, callback, table);
}