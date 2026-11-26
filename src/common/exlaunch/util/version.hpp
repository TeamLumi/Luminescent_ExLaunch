#pragma once
#include <program/version.hpp>

namespace exl::util {
    namespace impl {
        extern GameVersion s_GameVersion;
        void InitVersion();
    }
    inline GameVersion GetGameVersion() { return impl::s_GameVersion; }
}
