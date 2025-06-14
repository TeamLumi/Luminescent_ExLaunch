#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/PlayerWork.h"
#include "externals/FlagWork.h"
#include "externals/FlagWork_Enums.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

bool GetCaughtLocation(Dpr::EvScript::EvDataManager::Object* manager) {
    Logger::log("_GET_CAUGHT_LOCATION\n");
    EvData::Aregment::Array* args = manager->fields._evArg;

    if (args->max_length >= 2) {
        int32_t index = GetWorkOrIntValue(args->m_Items[1]);

        auto playerParty = PlayerWork::get_playerParty();
        auto memberPtr = playerParty->GetMemberPointer(index)->cast<Pml::PokePara::CoreParam>();

        int32_t value = (memberPtr != nullptr && !memberPtr->IsNull()) ? memberPtr->fields.m_accessor->GetBirthPlace() : 0;
        FlagWork::SetWork(GetWorkOrIntValue(args->m_Items[2]), value);
    }

    return true;
}