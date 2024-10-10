#include "helpers/fsHelper.h"
#include "save/save.h"

void linkBoxes(PlayerWork::Object* playerWork) {
    auto& saveData = playerWork->fields._saveData.fields;

    // Create new array
    auto newBoxNames = Dpr::Box::SaveBoxData::_STR17::newArray(BoxCount);
    auto wallPapers = System::Byte_array::newArray(BoxCount);
    auto newBoxData = Dpr::Box::SaveBoxTrayData::newArray(BoxCount);

    // Fill the new array with the custom save data
    for (uint64_t i=0; i<BoxCount; i++) {
        memcpy(&newBoxNames->m_Items[i], &getCustomSaveData()->boxes.boxNames[i], sizeof(Dpr::Box::SaveBoxData::_STR17::Object));
        memcpy(&wallPapers->m_Items[i], &getCustomSaveData()->boxes.wallpapers[i], sizeof(System::Byte));
        memcpy(&newBoxData->m_Items[i], &getCustomSaveData()->boxes.pokemonParams[i], sizeof(Dpr::Box::SaveBoxTrayData::Object));
    }

    // Set the data in PlayerWork to our custom save data
    saveData.boxData.fields.trayName = newBoxNames;
    saveData.boxData.fields.wallPaper = wallPapers;
    saveData.boxTray = newBoxData;
}