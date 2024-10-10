#include "helpers/fsHelper.h"
#include "save/save.h"

void linkWorks(PlayerWork::Object* playerWork) {
    auto& saveData = playerWork->fields._saveData.fields;

    // Create new array
    auto newWorks = System::Int32_array::newArray(WorkCount);

    // Fill the new array with the custom save data
    memcpy(newWorks->m_Items, getCustomSaveData()->works.items, sizeof(getCustomSaveData()->works.items));

    // Set the data in PlayerWork to our custom save data
    saveData.intValues = newWorks;
}