#include "string.h"
#include "util.h"

namespace nn {
    string to_string(int32_t value) {
        char buffer[100] = {};
        if (nn::util::SNPrintf(buffer, sizeof(buffer), "%d", value) >= 0)
            return string(buffer);
        else
            return string("");
    }

    string to_string(int32_t value, const char* format) {
        char buffer[100] = {};
        if (nn::util::SNPrintf(buffer, sizeof(buffer), format, value) >= 0)
            return string(buffer);
        else
            return string("");
    }
}