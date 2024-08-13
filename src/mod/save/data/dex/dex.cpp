#include "helpers/fsHelper.h"
#include "save/save.h"

static DPData::GET_STATUS_array* cache_get_status;
static System::Boolean_array* cache_male_color_flag;
static System::Boolean_array* cache_female_color_flag;
static System::Boolean_array* cache_male_flag;
static System::Boolean_array* cache_female_flag;
static System::Int32_array* cache_language_flags;

static DPData::GET_STATUS_array* tmp_get_status;
static System::Boolean_array* tmp_male_color_flag;
static System::Boolean_array* tmp_female_color_flag;
static System::Boolean_array* tmp_male_flag;
static System::Boolean_array* tmp_female_flag;
static System::Int32_array* tmp_language_flags;

void loadZukan(bool isBackup)
{
    if (!isBackup && FsHelper::isFileExist(getCustomSaveData()->dex.fileName))
    {
        long actualSize = FsHelper::getFileSize(getCustomSaveData()->dex.fileName);
        long expectedSize = getCustomSaveData()->dex.GetByteCount();
        long size = std::max(actualSize, expectedSize);
        FsHelper::LoadData data {
            .path = getCustomSaveData()->dex.fileName,
            .alignment = 0x1000,
            .bufSize = size,
        };
        FsHelper::loadFileFromPath(data);
        getCustomSaveData()->dex.FromBytes((char*)data.buffer, actualSize, 0);
        Logger::log("Loaded Lumi_Dex!\n");
    }
    else if (FsHelper::isFileExist(getCustomSaveData()->dex.backupFileName))
    {
        long actualSize = FsHelper::getFileSize(getCustomSaveData()->dex.backupFileName);
        long expectedSize = getCustomSaveData()->dex.GetByteCount();
        long size = std::max(actualSize, expectedSize);
        FsHelper::LoadData data {
            .path = getCustomSaveData()->dex.backupFileName,
            .alignment = 0x1000,
            .bufSize = size,
        };
        FsHelper::loadFileFromPath(data);
        getCustomSaveData()->dex.FromBytes((char*)data.buffer, actualSize, 0);
        Logger::log("Loaded Lumi_Dex_BK!\n");
    }
}

void linkZukan(PlayerWork::Object* playerWork)
{
    // Create new arrays
    auto newStatus = DPData::GET_STATUS_array::newArray(DexSize);
    auto newMaleColorFlag = System::Boolean_array::newArray(DexSize);
    auto newFemaleColorFlag = System::Boolean_array::newArray(DexSize);
    auto newMaleFlag = System::Boolean_array::newArray(DexSize);
    auto newFemaleFlag = System::Boolean_array::newArray(DexSize);
    auto newLanguageFlags = System::Int32_array::newArray(DexSize);

    // Fill the new arrays with the custom save data
    for (uint64_t i=0; i<DexSize; i++)
    {
        memcpy(&newStatus->m_Items[i], &getCustomSaveData()->dex.elements[i].get_status, sizeof(DPData::GET_STATUS));
        memcpy(&newMaleColorFlag->m_Items[i], &getCustomSaveData()->dex.elements[i].male_color_flag, sizeof(System::Boolean));
        memcpy(&newFemaleColorFlag->m_Items[i], &getCustomSaveData()->dex.elements[i].female_color_flag, sizeof(System::Boolean));
        memcpy(&newMaleFlag->m_Items[i], &getCustomSaveData()->dex.elements[i].male_flag, sizeof(System::Boolean));
        memcpy(&newFemaleFlag->m_Items[i], &getCustomSaveData()->dex.elements[i].female_flag, sizeof(System::Boolean));
        memcpy(&newLanguageFlags->m_Items[i], &getCustomSaveData()->dex.elements[i].language_flags, sizeof(System::Int32));
    }

    // Cache the data that is in the vanilla save
    auto& zukan = playerWork->fields._saveData.fields.zukanData.fields;
    cache_get_status = zukan.get_status;
    cache_male_color_flag = zukan.male_color_flag;
    cache_female_color_flag = zukan.female_color_flag;
    cache_male_flag = zukan.male_flag;
    cache_female_flag = zukan.female_flag;
    cache_language_flags = zukan.TextVersionUp;

    // Set the data in PlayerWork to our custom save data
    zukan.get_status = newStatus;
    zukan.male_color_flag = newMaleColorFlag;
    zukan.female_color_flag = newFemaleColorFlag;
    zukan.male_flag = newMaleFlag;
    zukan.female_flag = newFemaleFlag;
    zukan.TextVersionUp = newLanguageFlags;
}

void unlinkZukan(PlayerWork::Object* playerWork)
{
    auto& zukan = playerWork->fields._saveData.fields.zukanData.fields;

    // Copy PlayerWork data to our Custom save data
    for (uint64_t i=0; i<DexSize; i++)
    {
        memcpy(&getCustomSaveData()->dex.elements[i].get_status, &zukan.get_status->m_Items[i], sizeof(DPData::GET_STATUS));
        memcpy(&getCustomSaveData()->dex.elements[i].male_color_flag, zukan.male_color_flag->m_Items, sizeof(System::Boolean));
        memcpy(&getCustomSaveData()->dex.elements[i].female_color_flag, zukan.female_color_flag->m_Items, sizeof(System::Boolean));
        memcpy(&getCustomSaveData()->dex.elements[i].male_flag, zukan.male_flag->m_Items, sizeof(System::Boolean));
        memcpy(&getCustomSaveData()->dex.elements[i].female_flag, zukan.female_flag->m_Items, sizeof(System::Boolean));
        memcpy(&getCustomSaveData()->dex.elements[i].language_flags, zukan.TextVersionUp->m_Items, sizeof(System::Int32));
    }

    // Create a temp copy of the PlayerWork data
    tmp_get_status = zukan.get_status;
    tmp_male_color_flag = zukan.male_color_flag;
    tmp_female_color_flag = zukan.female_color_flag;
    tmp_male_flag = zukan.male_flag;
    tmp_female_flag = zukan.female_flag;
    tmp_language_flags = zukan.TextVersionUp;

    // Set PlayerWork to our cached data
    zukan.get_status = cache_get_status;
    zukan.male_color_flag = cache_male_color_flag;
    zukan.female_color_flag = cache_female_color_flag;
    zukan.male_flag = cache_male_flag;
    zukan.female_flag = cache_female_flag;
    zukan.TextVersionUp = cache_language_flags;
}

void saveZukan(bool isMain, bool isBackup)
{
    char buffer[getCustomSaveData()->dex.GetByteCount()];
    getCustomSaveData()->dex.ToBytes((char*)buffer, 0);

    if (isMain)
        FsHelper::writeFileToPath(buffer, sizeof(buffer), getCustomSaveData()->dex.fileName);
    if (isBackup)
        FsHelper::writeFileToPath(buffer, sizeof(buffer), getCustomSaveData()->dex.backupFileName);
}

void relinkZukan(PlayerWork::Object* playerWork)
{
    auto& zukan = playerWork->fields._saveData.fields.zukanData.fields;

    // Restore PlayerWork to our temp copy
    zukan.get_status = tmp_get_status;
    zukan.male_color_flag = tmp_male_color_flag;
    zukan.female_color_flag = tmp_female_color_flag;
    zukan.male_flag = tmp_male_flag;
    zukan.female_flag = tmp_female_flag;
    zukan.TextVersionUp = tmp_language_flags;
}