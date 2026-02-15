#include "externals/DPData/PoffinData.h"
#include "externals/DPData/PoffinSaveData.h"
#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/PlayerWork.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

bool GivePoffin(Dpr::EvScript::EvDataManager::Object* manager) {
    Logger::log("_GIVE_POFFIN\n");

    system_load_typeinfo(0x4521);

    EvData::Aregment::Array* args = manager->fields._evArg;

    auto nameID = GetWorkOrIntValue(args->m_Items[1]);
    auto level = GetWorkOrIntValue(args->m_Items[2]);
    auto spicy = GetWorkOrIntValue(args->m_Items[3]);
    auto dry = GetWorkOrIntValue(args->m_Items[4]);
    auto sweet = GetWorkOrIntValue(args->m_Items[5]);
    auto bitter = GetWorkOrIntValue(args->m_Items[6]);
    auto sour = GetWorkOrIntValue(args->m_Items[7]);
    auto smoothness = GetWorkOrIntValue(args->m_Items[8]);

    DPData::PoffinSaveData::Object poffinSaveData = PlayerWork::get_poffinSaveData();
    DPData::PoffinData::Object newPoffin = {};

    auto flavors = System::Byte_array::newArray(5);
    flavors->m_Items[0] = (uint8_t)spicy;
    flavors->m_Items[1] = (uint8_t)dry;
    flavors->m_Items[2] = (uint8_t)sweet;
    flavors->m_Items[3] = (uint8_t)bitter;
    flavors->m_Items[4] = (uint8_t)sour;
    newPoffin.ctor(nameID, level, (uint8_t)smoothness, flavors);
    (&poffinSaveData)->AddPoffin(newPoffin);

    return true;
}