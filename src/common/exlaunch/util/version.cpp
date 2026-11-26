#include "version.hpp"

namespace exl::util::impl {
    constinit UserVersion s_UserVersion{};
    void InitVersion() { s_UserVersion = DetermineUserVersion(); }
}
