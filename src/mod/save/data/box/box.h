#pragma once

#include "externals/Dpr/Box/SaveBoxData.h"
#include "externals/Dpr/Box/SaveBoxTrayData.h"
#include "externals/PlayerWork.h"
#include "externals/System/Primitives.h"
#include "externals/System/String.h"
#include "externals/Pml/PokePara/SerializedPokemonFull.h"

static const int64_t VANILLA_BOXSIZE = 40;
const uint64_t INIT_WALLPAPER_OFFSET = 8;
static const nn::vector<const char*> boxDefaultStrings {
        "Box 1", "Box 2", "Box 3", "Box 4", "Box 5", "Box 6", "Box 7", "Box 8", "Box 9", "Box 10",
        "Box 11", "Box 12", "Box 13", "Box 14", "Box 15", "Box 16", "Box 17", "Box 18", "Box 19", "Box 20",
        "Box 21", "Box 22", "Box 23", "Box 24", "Box 25", "Box 26", "Box 27", "Box 28", "Box 29", "Box 30",
        "Box 31", "Box 32", "Box 33", "Box 34", "Box 35", "Box 36", "Box 37", "Box 38", "Box 39", "Box 40",
        "Box 41", "Box 42", "Box 43", "Box 44", "Box 45", "Box 46", "Box 47", "Box 48", "Box 49", "Box 50",
        "Box 51", "Box 52", "Box 53", "Box 54", "Box 55", "Box 56", "Box 57", "Box 58", "Box 59", "Box 60",
        "Box 61", "Box 62", "Box 63", "Box 64", "Box 65", "Box 66", "Box 67", "Box 68", "Box 69", "Box 70",
        "Box 71", "Box 72", "Box 73", "Box 74", "Box 75", "Box 76", "Box 77", "Box 78", "Box 79", "Box 80"
};

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

    long ToBytes(char* buffer, long index) {
        for (uint64_t i=0; i<size; i++)
        {
            void* strData = (void*)(buffer+index);
            System::String::Object* str = boxNames[i].fields.str->Truncate(16);
            auto bytes = str->asUnicodeBytes();
            memcpy(strData, (void*)bytes->m_Items, Dpr::Box::SaveBoxData::_STR17::GetByteCount());

            index += Dpr::Box::SaveBoxData::_STR17::GetByteCount();
        }

        memcpy((void*)(buffer+index), &wallpapers, sizeof(System::Byte)*size);
        index += sizeof(System::Byte)*size;

        for (uint64_t i=0; i<size; i++)
        {
            auto serializedPoke = (Pml::PokePara::SerializedPokemonFull::Array*) pokemonParams[i].fields.pokemonParam;
            for (uint64_t j=0; j<serializedPoke->max_length; j++)
            {
                auto pokeByteArray = serializedPoke->m_Items[j].fields.buffer;
                void* pokeData = (void*)(buffer+index);
                memcpy(pokeData, (void*)pokeByteArray->m_Items, Pml::PokePara::SerializedPokemonFull::GetByteCount());
                index += Pml::PokePara::SerializedPokemonFull::GetByteCount();

            }
        }

        return index;
    }

    long FromBytes(char* buffer, long buffer_size, long index) {
        if (buffer_size >= GetByteCount() + index)
        {
            auto newBoxNames = (Dpr::Box::SaveBoxData::_STR17::Array*)system_array_new(
                    Dpr::Box::SaveBoxData::_STR17_array_TypeInfo(), size);

            for (uint64_t i=0; i<size; i++)
            {
                void* strData = (void*)(buffer+index);
                newBoxNames->m_Items[i].fields.str = System::String::fromUnicodeBytes(
                        strData, Dpr::Box::SaveBoxData::_STR17::GetByteCount());
                index += Dpr::Box::SaveBoxData::_STR17::GetByteCount();
            }
            memcpy(&boxNames, (void*)newBoxNames->m_Items, sizeof(Dpr::Box::SaveBoxData::_STR17::Object)*size);

            memcpy(&wallpapers, (void*)(buffer+index), sizeof(System::Byte)*size);
            index += sizeof(System::Byte)*size;

            auto newBoxData = (Dpr::Box::SaveBoxTrayData::Array*)system_array_new(
                    Dpr::Box::SaveBoxTrayData_array_TypeInfo(), size);
            for (uint64_t i=0; i<newBoxData->max_length; i++)
            {
                auto newPokeParams = (Pml::PokePara::SerializedPokemonFull::Array*)system_array_new(
                        Pml::PokePara::SerializedPokemonFull_array_TypeInfo(), 30);
                newBoxData->m_Items[i].fields.pokemonParam = newPokeParams;
                for (uint64_t j=0; j<newPokeParams->max_length; j++)
                {
                    void* pokeData = (void*)(buffer+index);
                    auto pokeByteArray = (System::Byte_array*)system_array_new(
                            System::Byte_array_TypeInfo(),
                            Pml::PokePara::SerializedPokemonFull::GetByteCount());
                    memcpy(pokeByteArray->m_Items, pokeData, Pml::PokePara::SerializedPokemonFull::GetByteCount());
                    newPokeParams->m_Items[j].fields.buffer = pokeByteArray;
                    newPokeParams->m_Items[j].CreateWorkIfNeed();
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

