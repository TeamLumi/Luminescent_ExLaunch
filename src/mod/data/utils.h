#pragma once

#include <cstdint>
#include <array>
#include <string_view>
#include "common.hpp"

template <int N>
consteval bool array_contains(const char* const (&arr)[N], const char* element) {
    for (int i = 0; i < N; i++) {
        if (std::string_view(element) == arr[i]) {
            return true;
        }
    }
    return false;
}

template <int N>
consteval int array_index(const char* const (&arr)[N], const char* element) {
    for (int i = 0; i < N; ++i) {
        if (std::string_view(element) == arr[i]) {
            return i;
        }
    }
}
