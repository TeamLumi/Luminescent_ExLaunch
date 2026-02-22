#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/FieldManager.h"
#include "externals/PlayerWork.h"
#include "externals/UnityEngine/Random.h"

#include "features/commands/utils/cmd_utils.h"
#include "romdata/romdata.h"
#include "logger/logger.h"

bool HoneyTreeBattleSet(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_HONEY_TREE_BTL_SET\n");
    system_load_typeinfo(0x4452);
    system_load_typeinfo(0x497c);

    int32_t zoneID = PlayerWork::get_zoneID();
    int32_t slot = UnityEngine::Random::Range(0, 10);
    RomData::HoneyTreeSlot slotData = GetHoneyTreeSlot(zoneID, slot);
    int32_t level = UnityEngine::Random::Range(slotData.minlv, slotData.maxlv);

    manager->SetBattleReturn();
    FieldManager::Object* fieldManager = FieldManager::getClass()->static_fields->_Instance_k__BackingField->instance();
    fieldManager->EventWildBattle(slotData.monsNo, level, false, false, true, 0, false, slotData.formNo, false);

    return true;
}