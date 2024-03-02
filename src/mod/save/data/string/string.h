#pragma once

#include "externals/Dpr/Box/SaveBoxData.h"

template <int32_t size>
struct StringSaveData {
    static constexpr const char* fileName = "SaveData:/Lumi_Strings.bin";
    static constexpr const char* backupFileName = "SaveData:/Lumi_Strings_BK.bin";

    Dpr::Box::SaveBoxData::_STR17::Object items[size];

    long GetByteCount() {
        return Dpr::Box::SaveBoxData::_STR17::GetByteCount()*size;
    }

    long ToBytes(char* buffer, long index) {
        for (uint64_t i=0; i<size; i++)
        {
            void* strData = (void*)(buffer+index);
            System::String::Object* str = items[i].fields.str->Truncate(16);
            auto bytes = str->asUnicodeBytes();
            memcpy(strData, (void*)bytes->m_Items, Dpr::Box::SaveBoxData::_STR17::GetByteCount());

            index += Dpr::Box::SaveBoxData::_STR17::GetByteCount();
        }

        return index;
    }

    long FromBytes(char* buffer, long buffer_size, long index) {
        if (buffer_size >= GetByteCount() + index)
        {
            auto newStrings = (Dpr::Box::SaveBoxData::_STR17::Array*)system_array_new(Dpr::Box::SaveBoxData::_STR17_array_TypeInfo(), size);
            for (uint64_t i=0; i<newStrings->max_length; i++)
            {
                void* strData = (void*)(buffer+index);
                newStrings->m_Items[i].fields.str = System::String::fromUnicodeBytes(strData, Dpr::Box::SaveBoxData::_STR17::GetByteCount());
                index += Dpr::Box::SaveBoxData::_STR17::GetByteCount();
            }
            memcpy(&items, (void*)newStrings->m_Items, sizeof(Dpr::Box::SaveBoxData::_STR17::Object)*size);

            return index;
        }

        return index + GetByteCount();
    }
};

void loadStrings(bool isBackup);
void linkStrings(PlayerWork::Object* playerWork);
void unlinkStrings(PlayerWork::Object* playerWork);
void saveStrings(bool isMain, bool isBackup);
void relinkStrings(PlayerWork::Object* playerWork);