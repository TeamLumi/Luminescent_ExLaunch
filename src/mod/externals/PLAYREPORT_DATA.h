#pragma once

#include <cstdint>
#include "externals/il2cpp-api.h"

struct PLAYREPORT_DATA : ILStruct<PLAYREPORT_DATA> {
    struct Fields {
        int64_t capture_symbol;
        int64_t capture_wild;
        int64_t capture_fishing;
        int64_t dress_up;
        int64_t training;
        int64_t personal_change;
        int64_t npc_trade;
        int64_t chain_encount;
        int64_t fossil_restore;
        int64_t wild_pokemon_encount;
        int64_t trade;
        int64_t magical_trade;
        int64_t total_walk;
        int64_t total_all_battle;
        int64_t battle_point;
        int64_t use_skill_record;
        int64_t evolution;
        int64_t net_battle;
        int64_t cooking;
        int64_t get_kinomi;
        int64_t btl_lose_cnt;
        int64_t total_money;
        int64_t btower_win_single;
        int64_t btower_win_double;
        int64_t btower_win_single_master;
        int64_t btower_win_double_master;
        int64_t go_to_underground;
        int64_t fossildigging;
        int64_t balldeko_open;
        int64_t statue_set;
        int64_t poketch_open;
    };
};
