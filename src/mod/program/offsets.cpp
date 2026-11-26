#include "offsets.hpp"

namespace game {
    constinit const Offset Addresses[] = {
        {0, 0},
#define GAME_ADDRESS(name, bd, sp) {bd, sp},
        GAME_ADDRESS_TABLE(GAME_ADDRESS)
#undef GAME_ADDRESS
    };
}
