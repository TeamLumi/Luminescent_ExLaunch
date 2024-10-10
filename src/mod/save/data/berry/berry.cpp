#include "helpers/fsHelper.h"
#include "save/save.h"

void linkBerries(PlayerWork::Object* playerWork) {
    auto& kinomigrow = playerWork->fields._saveData.fields.kinomiGrowSaveData.fields;
    // Create new array
    auto newBerries = DPData::KinomiGrow::newArray(BerryCount);

    // Fill the new array with the custom save data
    memcpy(newBerries->m_Items, getCustomSaveData()->berries.items, sizeof(getCustomSaveData()->berries.items));

    // Set the data in PlayerWork to our custom save data
    kinomigrow.kinomiGrows = newBerries;
}