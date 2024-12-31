#pragma once
#include "../externals/il2cpp-api.h"

enum class Facility : int32_t {
    BATTLE_TOWER,
    BATTLE_FACTORY,
    BATTLE_ARCADE,
    BATTLE_CASTLE,
    BATTLE_HALL,
    FACILITY_COUNT
};

enum Rank : int32_t {
    RANK_1 = 0,
    RANK_2 = 1,
    RANK_3 = 2,
    RANK_4 = 3,
    RANK_5 = 4,
    RANK_6 = 5,
    RANK_7 = 6,
    RANK_8 = 7,
    RANK_9 = 8,
    RANK_10 = 9,
    RANK_COUNT = 10
};

enum Group : int32_t {
    GROUP_1,
    GROUP_2,
    GROUP_3,
    GROUP_4,
    GROUP_COUNT
};

enum HallRound : int32_t {
    HALL_ROUND_1,
    HALL_ROUND_2,
    HALL_ROUND_3,
    HALL_ROUND_4,
    HALL_ROUND_5,
    HALL_ROUND_6,
    HALL_ROUND_7,
    HALL_ROUND_8,
    HALL_ROUND_9,
    HALL_ROUND_10,
    HALL_ROUND_COUNT
};

enum FrontierRound : int32_t {
    ROUND_1,
    ROUND_2,
    ROUND_3,
    ROUND_4,
    ROUND_5,
    ROUND_6,
    ROUND_7,
    ROUND_COUNT
};

enum FrontierSet : int32_t {
    SET_1,
    SET_2,
    SET_3,
    SET_4,
    SET_5,
    SET_6,
    SET_7,
    SET_8,
    SET_COUNT
};

enum TypeSelectorIndex : int32_t {
    NORMAL,
    FIRE,
    WATER,
    ELECTRIC,
    GRASS,
    ICE,
    FIGHTING,
    POISON,
    GROUND,
    FLYING,
    PSYCHIC,
    BUG,
    ROCK,
    GHOST,
    DRAGON,
    DARK,
    SUMMARY,
    STEEL,
    FAIRY,
    MATRON,
    INDEX_COUNT
};

enum FactoryLevel : int32_t {
    NOT_SET,
    LEVEL_50,
    OPEN_LEVEL,
    MODE_COUNT
};

enum FactoryIndex : int32_t {
    RENTAL_1,
    RENTAL_2,
    RENTAL_3,
    RENTAL_4,
    RENTAL_5,
    RENTAL_6,
    RENTAL_COUNT
};

enum class FactoryDisplayState : int32_t {
    NO_MON_SELECTED,
    FIRST_MON_SELECTED,
    SECOND_MON_SELECTED,
    THIRD_MON_SELECTED,
    DISPLAY_STATE_COUNT
};



extern const char* frontierFolderPath;

