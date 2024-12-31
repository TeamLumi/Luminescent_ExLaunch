#pragma once
#include "externals/il2cpp-api.h"
#include "externals/Pml/PokeParty.h"
#include "externals/System/Collections/Generic/List.h"

struct FrontierPtr {

    // PokeParty Pointers
    Pml::PokeParty::Object* currentTeam;
    Pml::PokeParty::Object* enemyTeam;
    Pml::PokeParty::Object* rentalTeam;

    System::Collections::Generic::List$$int32_t::Object* joinIndexList;

    void Clear();
    static void Free();

};

FrontierPtr* GetFrontierPtr();