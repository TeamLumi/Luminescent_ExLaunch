#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/ZukanWork.h"

#include "features/commands/utils/utils.h"
#include "logger/logger.h"
#include "romdata/romdata.h"

bool CheckTutorMove(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_CHECK_TUTOR_MOVE\n");

    system_load_typeinfo(0x4495);
    EvData::Aregment::Array* args = manager->fields._evArg;

    int32_t monsno = GetWorkOrIntValue(args->m_Items[1]);
    int32_t formno = GetWorkOrIntValue(args->m_Items[2]);
    int32_t moveid = GetWorkOrIntValue(args->m_Items[3]);

    bool result = IsMoveLearnableByTutor(monsno, formno, moveid);
    SetWorkToValue(args->m_Items[4], (int32_t)result);

    return true;
}