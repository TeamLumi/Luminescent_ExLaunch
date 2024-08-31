#pragma once

#include "externals/Dpr/Box/SaveBoxData.h"
#include "externals/Dpr/Box/SaveBoxTrayData.h"
#include "externals/PlayerWork.h"
#include "externals/System/Primitives.h"
#include "externals/System/String.h"
#include "externals/Pml/PokePara/SerializedPokemonFull.h"

static const int64_t VANILLA_BOXSIZE = 40;
static const uint64_t INIT_WALLPAPER_OFFSET = 8;

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
            auto newBoxNames = Dpr::Box::SaveBoxData::_STR17::newArray(size);

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

            auto newBoxData = Dpr::Box::SaveBoxTrayData::newArray(size);
            for (uint64_t i=0; i<newBoxData->max_length; i++)
            {
                auto newPokeParams = Pml::PokePara::SerializedPokemonFull::newArray(30);
                newBoxData->m_Items[i].fields.pokemonParam = newPokeParams;
                for (uint64_t j=0; j<newPokeParams->max_length; j++)
                {
                    void* pokeData = (void*)(buffer+index);
                    auto pokeByteArray = System::Byte_array::newArray(Pml::PokePara::SerializedPokemonFull::GetByteCount());
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