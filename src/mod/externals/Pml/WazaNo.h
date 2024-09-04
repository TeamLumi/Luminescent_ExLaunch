#pragma once

#include "externals/il2cpp.h"
#include "externals/il2cpp-api.h"
#include "externals/System/Collections/Generic/HashSet.h"

namespace Pml {
    typedef int32_t WazaNo;
    PRIMITIVE_ARRAY(WazaNo, 0x04c5c7e8)
}

namespace System::Collections::Generic {
    struct HashSet$$WazaNo : HashSet<HashSet$$WazaNo, Pml::WazaNo> {
        static inline StaticILMethod<0x04c6f310, Pml::WazaNo> Method$$Add {};
    };
}