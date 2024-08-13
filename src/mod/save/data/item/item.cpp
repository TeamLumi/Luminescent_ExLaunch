#include "helpers/fsHelper.h"
#include "save/save.h"

static Dpr::Item::SaveItem::Array* cache_items;

static Dpr::Item::SaveItem::Array* tmp_items;

void loadItems(bool isBackup)
{
    if (!isBackup && FsHelper::isFileExist(getCustomSaveData()->items.fileName))
    {
        long actualSize = FsHelper::getFileSize(getCustomSaveData()->items.fileName);
        long expectedSize = getCustomSaveData()->items.GetByteCount();
        long size = std::max(actualSize, expectedSize);
        FsHelper::LoadData data {
            .path = getCustomSaveData()->items.fileName,
            .alignment = 0x1000,
            .bufSize = size,
        };
        FsHelper::loadFileFromPath(data);
        getCustomSaveData()->items.FromBytes((char*)data.buffer, actualSize, 0);
        Logger::log("Loaded Lumi_Items!\n");
    }
    else if (FsHelper::isFileExist(getCustomSaveData()->items.backupFileName))
    {
        long actualSize = FsHelper::getFileSize(getCustomSaveData()->items.backupFileName);
        long expectedSize = getCustomSaveData()->items.GetByteCount();
        long size = std::max(actualSize, expectedSize);
        FsHelper::LoadData data {
            .path = getCustomSaveData()->items.backupFileName,
            .alignment = 0x1000,
            .bufSize = size,
        };
        FsHelper::loadFileFromPath(data);
        getCustomSaveData()->items.FromBytes((char*)data.buffer, actualSize, 0);
        Logger::log("Loaded Lumi_Items_BK!\n");
    }
}

void linkItems(PlayerWork::Object* playerWork)
{
    // Create new array
    auto newItems = Dpr::Item::SaveItem::newArray(SaveItemCount);

    // Fill the new array with the custom save data
    memcpy(newItems->m_Items, getCustomSaveData()->items.items, sizeof(getCustomSaveData()->items.items));

    // Cache the data that is in the vanilla save
    auto& savedata = playerWork->fields._saveData.fields;
    cache_items = savedata.saveItem;

    // Set the data in PlayerWork to our custom save data
    savedata.saveItem = newItems;
}

void unlinkItems(PlayerWork::Object* playerWork)
{
    auto& savedata = playerWork->fields._saveData.fields;

    // Copy PlayerWork data to our Custom save data
    memcpy(getCustomSaveData()->items.items, savedata.saveItem->m_Items, sizeof(getCustomSaveData()->items.items));

    // Create a temp copy of the PlayerWork data
    tmp_items = savedata.saveItem;

    // Set PlayerWork to our cached data
    savedata.saveItem = cache_items;
}

void saveItems(bool isMain, bool isBackup)
{
    char buffer[getCustomSaveData()->items.GetByteCount()];
    getCustomSaveData()->items.ToBytes((char*)buffer, 0);

    if (isMain)
        FsHelper::writeFileToPath(buffer, sizeof(buffer), getCustomSaveData()->items.fileName);
    if (isBackup)
        FsHelper::writeFileToPath(buffer, sizeof(buffer), getCustomSaveData()->items.backupFileName);
}

void relinkItems(PlayerWork::Object* playerWork)
{
    auto& savedata = playerWork->fields._saveData.fields;

    // Restore PlayerWork to our temp copy
    savedata.saveItem = tmp_items;
}