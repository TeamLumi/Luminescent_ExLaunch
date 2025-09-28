#pragma once

#include "externals/il2cpp-api.h"

namespace FieldDoor {
    enum class ExitLabel : int32_t {
        dummy = 0,
        door_exit_simple = 1,
        door_exit_pokecen = 2,
        door_exit_walk_u = 3,
        door_exit_walk_d = 4,
        door_exit_walk_l = 5,
        door_exit_walk_r = 6,
        door_exit_doukutu_u = 7,
        door_exit_doukutu_d = 8,
        door_exit_doukutu_l = 9,
        door_exit_doukutu_r = 10,
        door_escalator1_2 = 11,
        door_escalator2_2 = 12,
        door_warp_panel_end = 13,
        door_escalator3_2 = 14,
        door_escalator4_2 = 15,
        door_exit_elevator = 16,
        door_exit_tobari = 17,
        door_exit_d31 = 18,
        door_doukutu_out = 19,
        door_sekisyo_exit_walk_u = 20,
        door_sekisyo_exit_walk_d = 21,
        door_sekisyo_exit_walk_l = 22,
        door_sekisyo_exit_walk_r = 23,
        door_exit_zui = 24,
        none = 999,
    };
}