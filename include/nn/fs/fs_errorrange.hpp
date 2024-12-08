#pragma once

#include "fs_types.hpp"
#include "types.h"
#include "result.h"

namespace nn::fs {

    struct ErrorRange {

        template<typename Derived>
        static inline bool Includes(nn::Result result) {
            return result.GetModule() == Derived::Module && Derived::DescriptionBegin <= result.GetDescription() && result.GetDescription() < Derived::DescriptionEnd;
        }
    };

    struct ResultPathNotFound : ErrorRange {
        static constexpr int32_t Module = 2;
        static constexpr int32_t DescriptionBegin = 1;
        static constexpr int32_t DescriptionEnd = 2;
    };

    struct ResultPathAlreadyExists : ErrorRange {
        static constexpr int32_t Module = 2;
        static constexpr int32_t DescriptionBegin = 2;
        static constexpr int32_t DescriptionEnd = 3;
    };

    struct ResultUsableSpaceNotEnough : ErrorRange {
        static constexpr int32_t Module = 2;
        static constexpr int32_t DescriptionBegin = 0x1e;
        static constexpr int32_t DescriptionEnd = 0x2e;
    };

} // namespace nn::fs