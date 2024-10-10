#include "helpers/fsHelper.h"
#include "save/save.h"


void linkTrainers(PlayerWork::Object* playerWork) {
    auto& saveData = playerWork->fields._saveData.fields;

    // Create new array
    auto newTrainers = DPData::TR_BATTLE_DATA::newArray(TrainerCount);

    // Fill the new array with the custom save data
    memcpy(newTrainers->m_Items, getCustomSaveData()->trainers.items, sizeof(getCustomSaveData()->trainers.items));

    // Set the data in PlayerWork to our custom save data
    saveData.tr_battleData = newTrainers;
}