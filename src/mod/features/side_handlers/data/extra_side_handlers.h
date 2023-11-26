#pragma once

#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/EventFactor.h"

struct ExtraSideEffectHandlers {
    int32_t count = 0;
    int32_t currentIndex = 0;
    Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_GMaxCannonade = nullptr;
    Dpr::Battle::Logic::EventFactor::EventHandlerTable::Array* HandlerTable_GMaxVineLash = nullptr;
    // ...
};