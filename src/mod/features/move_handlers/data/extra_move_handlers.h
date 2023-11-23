#pragma once

#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/EventFactor.h"

struct ExtraMoveHandlers {
    int32_t count = 0;
    int32_t currentIndex = 0;
    Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_JumpKick = nullptr;
    Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_SonicBoom = nullptr;
    Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_DragonRage = nullptr;
    Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Rage = nullptr;
    Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Bide = nullptr;
    Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_MirrorMove = nullptr;
    Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Psywave = nullptr;
    Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Nightmare = nullptr;
    Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Return = nullptr;
    Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Frustration = nullptr;
    Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Magnitude = nullptr;
    Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Pursuit = nullptr;
    Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_SmellingSalts = nullptr;
    Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Assist = nullptr;
    Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Refresh = nullptr;
    Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Snatch = nullptr;
    Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_SecretPower = nullptr;
    Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Camouflage = nullptr;
    Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_MudSport = nullptr;
    Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_SkyUppercut = nullptr;
    // ...
};