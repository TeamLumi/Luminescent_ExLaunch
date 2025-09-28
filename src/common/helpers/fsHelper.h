#pragma once

#include "nn/fs.h"
#include "nn/result.h"
#include "memory/json.h"

namespace FsHelper {

    struct LoadData {
        const char *path;
        int alignment = 8;
        void *buffer;
        long bufSize;

        ~LoadData();
    };

    nn::Result writeFileToPath(void *buf, size_t size, const char *path);

    nn::Result Commit(const char *path);

    void loadFileFromPath(LoadData &loadData);

    long getFileSize(const char *path);

    bool isFileExist(const char *path);

    nn::json loadJsonFileFromPath(const char *path);

    nn::string loadJsonSaveFromPath(const char *path);
}
