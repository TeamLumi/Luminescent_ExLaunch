#pragma once

#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/EventFactor.h"

struct ExtraFieldEffectWeatherHandlers {
    int32_t count = 0;
    int32_t currentIndex = 0;
    Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_HarshSunlight = nullptr;
    Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Rain = nullptr;
    Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Hail = nullptr;
    Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Sandstorm = nullptr;
    Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_HeavyRain = nullptr;
    Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_ExtremelyHarshSunlight = nullptr;
    Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_Snow = nullptr;
    // ...
};

struct ExtraFieldEffectHandlers {
    int32_t count = 0;
    int32_t currentIndex = 0;
    Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_MudSport = nullptr;
    Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_WaterSport = nullptr;
    // ...
};