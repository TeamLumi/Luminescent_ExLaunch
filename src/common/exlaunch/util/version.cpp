#include "version.hpp"

namespace exl::util::impl {
    constinit GameVersion s_GameVersion{};
    void InitVersion() { s_GameVersion = DetermineGameVersion(); }
}
