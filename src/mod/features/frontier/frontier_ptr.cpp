#include "frontier_ptr.h"

static FrontierPtr* gFrontierPtr = nullptr;

FrontierPtr* GetFrontierPtr() {
    if (!gFrontierPtr)
        gFrontierPtr = static_cast<FrontierPtr*>(nn_malloc(sizeof(FrontierPtr)));
    return gFrontierPtr;
}

void FrontierPtr::Clear() {
    currentTeam = Pml::PokeParty::newInstance();
    enemyTeam = Pml::PokeParty::newInstance();
    rentalTeam = Pml::PokeParty::newInstance();
}

void FrontierPtr::Free() {
    if (gFrontierPtr) {
        nn_free(gFrontierPtr);
        gFrontierPtr = nullptr;
    }
}