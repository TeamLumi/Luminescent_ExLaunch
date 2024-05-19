#pragma once

#include "externals/il2cpp-api.h"

struct MessageEnumData : ILClass<MessageEnumData> {
    enum class MsgLangId : int32_t {
        JPN = 1,
        USA = 2,
        FRA = 3,
        ITA = 4,
        DEU = 5,
        ESP = 7,
        KOR = 8,
        SCH = 9,
        TCH = 10,
        Num = 11,
    };
};