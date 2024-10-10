#pragma once

#include "externals/Dpr/Box/SaveBoxData.h"
#include "externals/Dpr/Box/SaveBoxTrayData.h"
#include "externals/PlayerWork.h"
#include "externals/System/Primitives.h"
#include "externals/System/String.h"
#include "externals/Pml/PokePara/SerializedPokemonFull.h"

static constexpr int64_t VANILLA_BOXSIZE = 40;
static constexpr uint64_t INIT_WALLPAPER_OFFSET = 8;

template <int32_t size>
struct BoxSaveData {

    Dpr::Box::SaveBoxData::_STR17::Object boxNames[size];
    System::Byte wallpapers[size];
    Dpr::Box::SaveBoxTrayData::Object pokemonParams[size];

};

void linkBoxes(PlayerWork::Object* playerWork);