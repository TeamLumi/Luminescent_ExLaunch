#pragma once

#include "fs_types.hpp"

namespace nn::fs {

    /*
        Mount SD card. Must have explicit permission.
        mount: drive to mount to.
    */
    Result MountSdCardForDebug(const char* mount);

    Result QueryMountRomCacheSize(size_t* out);

    Result MountRom(const char* mount, void* cache, size_t cache_size);

    void Unmount(const char* mount);

    Result Commit(const char* name);
};
