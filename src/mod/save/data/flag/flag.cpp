#include "helpers/fsHelper.h"
#include "save/save.h"

void linkFlags(PlayerWork::Object* playerWork) {
    auto& saveData = playerWork->fields._saveData.fields;

    // Create new array
    auto newFlags = System::Boolean_array::newArray(FlagCount);

    // Fill the new array with the custom save data
    memcpy(newFlags->m_Items, getCustomSaveData()->flags.items, sizeof(getCustomSaveData()->flags.items));

    // Set the data in PlayerWork to our custom save data
    saveData.boolValues = newFlags;
}