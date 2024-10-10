#include "helpers/fsHelper.h"
#include "save/save.h"

void linkZukan(PlayerWork::Object* playerWork) {
    auto& zukan = playerWork->fields._saveData.fields.zukanData.fields;

    // Create new arrays
    auto newStatus = DPData::GET_STATUS_array::newArray(DexSize);
    auto newMaleColorFlag = System::Boolean_array::newArray(DexSize);
    auto newFemaleColorFlag = System::Boolean_array::newArray(DexSize);
    auto newMaleFlag = System::Boolean_array::newArray(DexSize);
    auto newFemaleFlag = System::Boolean_array::newArray(DexSize);
    auto newLanguageFlags = System::Int32_array::newArray(DexSize);

    // Fill the new arrays with the custom save data
    for (uint64_t i=0; i<DexSize; i++) {
        memcpy(&newStatus->m_Items[i], &getCustomSaveData()->dex.elements[i].get_status, sizeof(DPData::GET_STATUS));
        memcpy(&newMaleColorFlag->m_Items[i], &getCustomSaveData()->dex.elements[i].male_color_flag, sizeof(System::Boolean));
        memcpy(&newFemaleColorFlag->m_Items[i], &getCustomSaveData()->dex.elements[i].female_color_flag, sizeof(System::Boolean));
        memcpy(&newMaleFlag->m_Items[i], &getCustomSaveData()->dex.elements[i].male_flag, sizeof(System::Boolean));
        memcpy(&newFemaleFlag->m_Items[i], &getCustomSaveData()->dex.elements[i].female_flag, sizeof(System::Boolean));
        memcpy(&newLanguageFlags->m_Items[i], &getCustomSaveData()->dex.elements[i].language_flags, sizeof(System::Int32));
    }

    // Set the data in PlayerWork to our custom save data
    zukan.get_status = newStatus;
    zukan.male_color_flag = newMaleColorFlag;
    zukan.female_color_flag = newFemaleColorFlag;
    zukan.male_flag = newMaleFlag;
    zukan.female_flag = newFemaleFlag;
    zukan.TextVersionUp = newLanguageFlags;
}