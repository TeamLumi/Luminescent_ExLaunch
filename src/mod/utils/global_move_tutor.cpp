#include "externals/il2cpp.h"

#include "utils/utils.h"

#include "logger/logger.h"

static nn::vector<int32_t> gTutorTables = nn::vector<int32_t>();

nn::vector<int32_t>* GetGTutorTables() {
    return &gTutorTables;
}