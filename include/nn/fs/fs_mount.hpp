#pragma once

#include "fs_types.hpp"

namespace nn::fs {

    /*
        Mount SD card. Must have explicit permission.
        mount: drive to mount to.
    */
    Result MountSdCardForDebug(char const* mount);

    Result QueryMountRomCacheSize(size_t* out);

    Result MountRom(char const* mount, void* cache, size_t cache_size);

    void Unmount(char const* mount);
};
