#pragma once

#include "externals/Dpr/Box/SaveBoxData.h"
#include "externals/Dpr/Box/SaveBoxTrayData.h"
#include "externals/PlayerWork.h"
#include "externals/System/Primitives.h"
#include "externals/System/String.h"

template <int32_t size>
struct BoxSaveData {
    static constexpr const char* fileName = "SaveData:/Lumi_Boxes.bin";
    static constexpr const char* backupFileName = "SaveData:/Lumi_Boxes_BK.bin";

    Dpr::Box::SaveBoxData::_STR17::Object boxNames[size];
    System::Byte wallpapers[size];
    Dpr::Box::SaveBoxTrayData::Object pokemonParams[size];

    long GetByteCount() {
        long count = 0;
        count += Dpr::Box::SaveBoxData::_STR17::GetByteCount()*size;
        count += sizeof(System::Byte)*size;
        count += Dpr::Box::SaveBoxTrayData::GetByteCount()*size;
        return count;
    }

    long FromBytes(char* buffer, long buffer_size, long index) {
        if (buffer_size >= GetByteCount() + index)
        {
            auto newBoxNames = (Dpr::Box::SaveBoxData::_STR17::Array*)system_array_new(Dpr::Box::SaveBoxData::_STR17_array_TypeInfo(), size);
            for (uint64_t i=0; i<newBoxNames->max_length; i++)
            {
                void* strData = (void*)(buffer+index);
                newBoxNames->m_Items[i].fields.str = System::String::fromUnicodeBytes(strData, Dpr::Box::SaveBoxData::_STR17::GetByteCount());
                index += Dpr::Box::SaveBoxData::_STR17::GetByteCount();
            }
            memcpy(&boxNames, (void*)newBoxNames->m_Items, sizeof(Dpr::Box::SaveBoxData::_STR17::Object)*size);

            memcpy(&wallpapers, (void*)(buffer+index), sizeof(System::Byte)*size);
            index += sizeof(System::Byte)*size;

            auto newBoxData = (Dpr::Box::SaveBoxTrayData::Array*)system_array_new(Dpr::Box::SaveBoxTrayData_array_TypeInfo(), size);
            for (uint64_t i=0; i<newBoxData->max_length; i++)
            {
                auto newPokeParams = (Pml::PokePara::SerializedPokemonFull::Array*)system_array_new(Pml::PokePara::SerializedPokemonFull_array_TypeInfo(), 30);
                newBoxData->m_Items[i].fields.pokemonParam = newPokeParams;
                for (uint64_t j=0; j<newBoxData->max_length; j++)
                {
                    void* pokeData = (void*)(buffer+index);
                    auto pokeByteArray = (System::Byte_array*)system_array_new(System::Byte_array_TypeInfo(), Pml::PokePara::SerializedPokemonFull::GetByteCount());
                    memcpy(&pokeByteArray->m_Items, pokeData, Pml::PokePara::SerializedPokemonFull::GetByteCount());
                    newPokeParams->m_Items[j].fields.buffer = pokeByteArray;
                    index += Pml::PokePara::SerializedPokemonFull::GetByteCount();
                }
            }
            memcpy(&pokemonParams, (void*)newBoxData->m_Items, sizeof(Dpr::Box::SaveBoxTrayData::Object)*size);

            return index;
        }

        return index + GetByteCount();
    }
};

void loadBoxes(bool isBackup);
void linkBoxes(PlayerWork::Object* playerWork);
void unlinkBoxes(PlayerWork::Object* playerWork);
void saveBoxes(bool isMain, bool isBackup);
void relinkBoxes(PlayerWork::Object* playerWork);