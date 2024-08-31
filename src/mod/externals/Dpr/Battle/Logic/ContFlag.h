#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Battle::Logic {
    enum class ContFlag : int32_t {
        CONTFLG_ACTION_DONE = 0,
        CONTFLG_CANT_CHANGE = 1,
        CONTFLG_TAME = 2,
        CONTFLG_SORAWOTOBU = 3,
        CONTFLG_DIVING = 4,
        CONTFLG_ANAWOHORU = 5,
        CONTFLG_SHADOWDIVE = 6,
        CONTFLG_MARUKUNARU = 7,
        CONTFLG_TIISAKUNARU = 8,
        CONTFLG_KIAIDAME = 9,
        CONTFLG_POWERTRICK = 10,
        CONTFLG_MIKURUNOMI = 11,
        CONTFLG_CANT_ACTION = 12,
        CONTFLG_MORAIBI = 13,
        CONTFLG_BATONTOUCH = 14,
        CONTFLG_ITEM_LOSE = 15,
        CONTFLG_ELECFIELD_GUARD = 16,
        CONTFLG_MISTFIELD_GUARD = 17,
        CONTFLG_ONNEN = 18,
        CONTFLG_MITIDURE = 19,
        CONTFLG_JUDEN = 20,
        CONTFLG_GAMAN = 21,
        CONTFLG_BODYPURGE = 22,
        CONTFLG_MOETUKIRU = 23,
        CONTFLG_HAISUINOZIN = 24,
        CONTFLG_MAX = 25,
        CONTFLG_NULL = 25,
    };
}