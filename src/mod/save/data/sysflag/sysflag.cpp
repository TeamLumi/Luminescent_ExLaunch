#include "helpers/fsHelper.h"
#include "save/save.h"

void linkSysFlags(PlayerWork::Object* playerWork) {
    auto& savedata = playerWork->fields._saveData.fields;

    // Create new array
    auto newSysFlags = System::Boolean_array::newArray(SysFlagCount);

    // Fill the new array with the custom save data
    memcpy(newSysFlags->m_Items, getCustomSaveData()->sysflags.items, sizeof(getCustomSaveData()->sysflags.items));

    // Set the data in PlayerWork to our custom save data
    savedata.systemFlags = newSysFlags;
}