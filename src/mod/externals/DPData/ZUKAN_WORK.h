#pragma once

#include "CONFIG.h"
#include "MYSTATUS.h"
#include "PLAYTIME.h"
#include "CONTEST_DATA.h"
#include "GET_STATUS.h"
#include "externals/System/Primitives.h"

namespace DPData {
    struct ZUKAN_WORK : ILStruct<ZUKAN_WORK> {
        struct Fields {
            DPData::GET_STATUS_array* get_status;
            System::Boolean_array* male_color_flag;
            System::Boolean_array* female_color_flag;
            System::Boolean_array* male_flag;
            System::Boolean_array* female_flag;
            System::Boolean_array* UnknownTurn;
            System::Boolean_array* UnknownColor;
            System::Boolean_array* PowarunTurn;
            System::Boolean_array* PowarunColor;
            System::Boolean_array* DeokisisuTurn;
            System::Boolean_array* DeokisisuColor;
            System::Boolean_array* MinomuttiTurn;
            System::Boolean_array* MinomuttiColor;
            System::Boolean_array* MinomadamTurn;
            System::Boolean_array* MinomadamColor;
            System::Boolean_array* GaameiruTurn;
            System::Boolean_array* GaameiruColor;
            System::Boolean_array* TherimuTurn;
            System::Boolean_array* TherimuColor;
            System::Boolean_array* KaranakusiTurn;
            System::Boolean_array* KaranakusiColor;
            System::Boolean_array* ToritodonTurn;
            System::Boolean_array* ToritodonColor;
            System::Boolean_array* RotomTurn;
            System::Boolean_array* RotomColor;
            System::Boolean_array* GirathinaTurn;
            System::Boolean_array* GirathinaColor;
            System::Boolean_array* SheimiTurn;
            System::Boolean_array* SheimiColor;
            System::Boolean_array* AruseusuTurn;
            System::Boolean_array* AruseusuColor;
            System::Int32_array* TextVersionUp;
            bool zukan_get;
            bool zenkoku_flag;
        };

        static_assert(offsetof(Fields, zenkoku_flag) == 0x101);
    };
}
