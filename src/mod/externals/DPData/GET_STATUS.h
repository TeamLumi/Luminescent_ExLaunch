#pragma once

namespace DPData {
    enum class GET_STATUS : int32_t
    {
        NONE = 0,
        UWASA = 1,
        SEE = 2,
        GET = 3,
    };

    PRIMITIVE_ARRAY(GET_STATUS, game::Address::DPData_GET_STATUS_Array_TypeInfo)
}
