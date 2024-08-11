#include "helpers/fsHelper.h"
#include "save/save.h"


static Dpr::Box::SaveBoxTrayData::Array* cache_boxTray;
static Dpr::Box::SaveBoxData::_STR17::Array* cache_trayNames;
static System::Byte_array* cache_wallPapers;

static Dpr::Box::SaveBoxTrayData::Array* tmp_boxTray;
static Dpr::Box::SaveBoxData::_STR17::Array* tmp_trayNames;
static System::Byte_array* tmp_wallPapers;

void loadBoxes(bool isBackup)
{
    if (!isBackup && FsHelper::isFileExist(getCustomSaveData()->boxes.fileName))
    {
        long actualSize = FsHelper::getFileSize(getCustomSaveData()->boxes.fileName);
        long expectedSize = getCustomSaveData()->boxes.GetByteCount();
        long size = std::max(actualSize, expectedSize);
        FsHelper::LoadData data {
                .path = getCustomSaveData()->boxes.fileName,
                .alignment = 0x1000,
                .bufSize = size,
        };
        FsHelper::loadFileFromPath(data);
        getCustomSaveData()->boxes.FromBytes((char*)data.buffer, actualSize, 0);
        Logger::log("Loaded Lumi_Boxes!\n");
    }
    else if (FsHelper::isFileExist(getCustomSaveData()->boxes.backupFileName))
    {
        long actualSize = FsHelper::getFileSize(getCustomSaveData()->boxes.backupFileName);
        long expectedSize = getCustomSaveData()->boxes.GetByteCount();
        long size = std::max(actualSize, expectedSize);
        FsHelper::LoadData data {
                .path = getCustomSaveData()->boxes.backupFileName,
                .alignment = 0x1000,
                .bufSize = size,
        };
        FsHelper::loadFileFromPath(data);
        getCustomSaveData()->boxes.FromBytes((char*)data.buffer, actualSize, 0);
        Logger::log("Loaded Lumi_Boxes_BK!\n");
    }
}

void linkBoxes(PlayerWork::Object* playerWork)
{
    auto str17Cls = Dpr::Box::SaveBoxData::_STR17_array_TypeInfo();
    auto saveBoxTrayCls = Dpr::Box::SaveBoxTrayData_array_TypeInfo();
    auto byteCls = System::Byte_array_TypeInfo();

    // Create new array
    auto newBoxNames = (Dpr::Box::SaveBoxData::_STR17::Array*) system_array_new(str17Cls, BoxCount);
    auto wallPapers = (System::Byte_array*) system_array_new(byteCls, BoxCount);
    auto newBoxData = (Dpr::Box::SaveBoxTrayData::Array*) system_array_new(saveBoxTrayCls, BoxCount);

    // Fill the new array with the custom save data
    for (uint64_t i=0; i<BoxCount; i++) {
        memcpy(&newBoxNames->m_Items[i], &getCustomSaveData()->boxes.boxNames[i], sizeof(Dpr::Box::SaveBoxData::_STR17::Object));
        memcpy(&wallPapers->m_Items[i], &getCustomSaveData()->boxes.wallpapers[i], sizeof(System::Byte));

        memcpy(&newBoxData->m_Items[i], &getCustomSaveData()->boxes.pokemonParams[i], sizeof(Dpr::Box::SaveBoxTrayData::Object));
    }

    // Cache the data that is in the vanilla save
    auto& savedata = playerWork->fields._saveData.fields;
    cache_trayNames = savedata.boxData.fields.trayName;
    cache_wallPapers = savedata.boxData.fields.wallPaper;
    cache_boxTray = savedata.boxTray;

    // Set the data in PlayerWork to our custom save data
    savedata.boxData.fields.trayName = newBoxNames;
    savedata.boxData.fields.wallPaper = wallPapers;
    savedata.boxTray = newBoxData;

}

void unlinkBoxes(PlayerWork::Object* playerWork)
{
    auto& savedata = playerWork->fields._saveData.fields;

    // Copy PlayerWork data to our Custom save data
    for (uint64_t i=0; i<BoxCount; i++) {
        memcpy(&getCustomSaveData()->boxes.boxNames[i], &savedata.boxData.fields.trayName->m_Items[i], sizeof(Dpr::Box::SaveBoxData::_STR17::Object));
        memcpy(&getCustomSaveData()->boxes.wallpapers[i], &savedata.boxData.fields.wallPaper->m_Items[i], sizeof(System::Byte));

        memcpy(&getCustomSaveData()->boxes.pokemonParams[i], &savedata.boxTray->m_Items[i], sizeof(Dpr::Box::SaveBoxTrayData::Object));
    }

    // Create a temp copy of the PlayerWork data
    tmp_trayNames = savedata.boxData.fields.trayName;
    tmp_wallPapers = savedata.boxData.fields.wallPaper;
    tmp_boxTray = savedata.boxTray;

    // Set PlayerWork to our cached data
    savedata.boxData.fields.trayName = cache_trayNames;
    savedata.boxData.fields.wallPaper = cache_wallPapers;
    savedata.boxTray = cache_boxTray;
}

void saveBoxes(bool isMain, bool isBackup)
{
    char buffer[getCustomSaveData()->boxes.GetByteCount()];
    getCustomSaveData()->boxes.ToBytes((char*)buffer, 0);

    if (isMain)
        FsHelper::writeFileToPath(buffer, sizeof(buffer), getCustomSaveData()->boxes.fileName);
    if (isBackup)
        FsHelper::writeFileToPath(buffer, sizeof(buffer), getCustomSaveData()->boxes.backupFileName);
}

void relinkBoxes(PlayerWork::Object* playerWork)
{
    auto& savedata = playerWork->fields._saveData.fields;
    savedata.boxData.fields.trayName = tmp_trayNames;
    savedata.boxData.fields.wallPaper = tmp_wallPapers;
    savedata.boxTray = tmp_boxTray;
}