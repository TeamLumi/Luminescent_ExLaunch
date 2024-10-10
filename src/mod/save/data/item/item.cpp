#include "helpers/fsHelper.h"
#include "save/save.h"

void linkItems(PlayerWork::Object* playerWork) {
    auto& saveData = playerWork->fields._saveData.fields;

    // Create new array
    auto newItems = Dpr::Item::SaveItem::newArray(SaveItemCount);

    // Fill the new array with the custom save data
    memcpy(newItems->m_Items, getCustomSaveData()->items.items, sizeof(getCustomSaveData()->items.items));

    // Set the data in PlayerWork to our custom save data
    saveData.saveItem = newItems;
}