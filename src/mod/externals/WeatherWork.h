#pragma once

#include "externals/il2cpp-api.h"

struct WeatherWork : ILClass<WeatherWork> {
    struct Fields {

    };

    static inline void set_WeatherID(int32_t value) {
        external<void>(game::Address::WeatherWork_set_WeatherID, value);
    }
};
