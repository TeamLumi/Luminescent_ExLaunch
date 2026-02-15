#pragma once

#include <string>
#include "nn_allocator.h"

namespace nn {
    using string = std::basic_string<char, std::char_traits<char>, nn_allocator<char>>;

    string to_string(int32_t value);
    string to_string(int32_t value, const char* format);
    string to_string(uint32_t value, const char* format);
}