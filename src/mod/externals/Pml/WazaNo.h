#pragma once

#include "externals/il2cpp.h"
#include "externals/il2cpp-api.h"
#include "externals/System/Collections/Generic/HashSet.h"

typedef int32_t Pml_WazaNo;

PRIMITIVE_ARRAY(Pml_WazaNo, 0x04c5c7e8)

namespace System::Collections::Generic {
    struct HashSet$$Pml_WazaNo : HashSet<HashSet$$Pml_WazaNo, Pml_WazaNo> {
        static inline StaticILMethod<0x04c6f310, Pml_WazaNo> Method$$Add {};
    };
}