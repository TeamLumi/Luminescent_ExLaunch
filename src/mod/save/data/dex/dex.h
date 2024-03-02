#pragma once

#include "externals/DPData/GET_STATUS.h"
#include "externals/PlayerWork.h"
#include "externals/System/Primitives.h"

#include "logger/logger.h"

struct DexSaveDataElement {
    DPData::GET_STATUS get_status;
    System::Boolean male_color_flag;
    System::Boolean female_color_flag;
    System::Boolean male_flag;
    System::Boolean female_flag;
    System::Int32 language_flags;

    void Initialize() {
        get_status = 0;
        male_color_flag = false;
        female_color_flag = false;
        male_flag = false;
        female_flag = false;
        language_flags = 0;
    }

    static long GetByteCount() {
        long count = 0;
        count += sizeof(DPData::GET_STATUS);
        count += sizeof(System::Boolean) * 4;
        count += sizeof(System::Int32);
        return count;
    }

    long ToBytes(char* buffer, long index) {
        memcpy((void*)(buffer+index), &get_status, sizeof(DPData::GET_STATUS));
        index += sizeof(DPData::GET_STATUS);
        memcpy((void*)(buffer+index), &male_color_flag, sizeof(System::Boolean));
        index += sizeof(System::Boolean);
        memcpy((void*)(buffer+index), &female_color_flag, sizeof(System::Boolean));
        index += sizeof(System::Boolean);
        memcpy((void*)(buffer+index), &male_flag, sizeof(System::Boolean));
        index += sizeof(System::Boolean);
        memcpy((void*)(buffer+index), &female_flag, sizeof(System::Boolean));
        index += sizeof(System::Boolean);
        memcpy((void*)(buffer+index), &language_flags, sizeof(System::Int32));
        index += sizeof(System::Int32);

        return index;
    }

    long FromBytes(char* buffer, long buffer_size, long index) {
        if (buffer_size >= GetByteCount() + index)
        {
            get_status = *(DPData::GET_STATUS*)(buffer+index);
            index += sizeof(DPData::GET_STATUS);
            male_color_flag = *(System::Boolean*)(buffer+index);
            index += sizeof(System::Boolean);
            female_color_flag = *(System::Boolean*)(buffer+index);
            index += sizeof(System::Boolean);
            male_flag = *(System::Boolean*)(buffer+index);
            index += sizeof(System::Boolean);
            female_flag = *(System::Boolean*)(buffer+index);
            index += sizeof(System::Boolean);
            language_flags = *(System::Int32*)(buffer+index);
            index += sizeof(System::Int32);
        }

        return index;
    }
};

template <int32_t size>
struct DexSaveData {
    static constexpr const char* fileName = "SaveData:/Lumi_Dex.bin";
    static constexpr const char* backupFileName = "SaveData:/Lumi_Dex_BK.bin";

    DexSaveDataElement elements[size];

    void Initialize() {
        for (int32_t i=0; i<size; i++)
        {
            elements[i].Initialize();
        }
    }

    long GetByteCount() {
        return DexSaveDataElement::GetByteCount()*size;
    }

    long ToBytes(char* buffer, long index) {
        for (int32_t i=0; i<size; i++)
        {
            index = elements[i].ToBytes(buffer, index);
        }

        return index;
    }

    long FromBytes(char* buffer, long buffer_size, long index) {
        Initialize();
        for (int32_t i=0; i<size; i++)
        {
            if (buffer_size < index + DexSaveDataElement::GetByteCount())
                break;

            index = elements[i].FromBytes(buffer, buffer_size, index);
        }

        return index;
    }
};

void loadZukan(bool isBackup);
void linkZukan(PlayerWork::Object* playerWork);
void unlinkZukan(PlayerWork::Object* playerWork);
void saveZukan(bool isMain, bool isBackup);
void relinkZukan(PlayerWork::Object* playerWork);