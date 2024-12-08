#pragma once

#include "types.h"

namespace nn {

struct Result
{
    Result(u32 value = 0) : value(value) { }

    inline bool isSuccess()
    {
        return value == 0;
    }

    inline bool isFailure()
    {
        return !isSuccess();
    }

    inline int32_t GetModule() const {
        return static_cast<int32_t>(value & 0x1ff);
    }

    inline int32_t GetDescription() const {
        return static_cast<int32_t>(value >> 9 & 0x1fff);
    }

    u32 value;

    bool operator==(int other) const {
        return (int) value == other;
    }
    bool operator!=(int other) const {
        return (int) value != other;
    }
};

}
