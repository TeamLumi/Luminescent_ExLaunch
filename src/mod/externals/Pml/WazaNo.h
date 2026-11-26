#pragma once

#include "externals/il2cpp.h"
#include "externals/il2cpp-api.h"
#include "externals/System/Collections/Generic/HashSet.h"

namespace Pml {
    typedef int32_t WazaNo;
    PRIMITIVE_ARRAY(WazaNo, game::Address::Pml_WazaNo_Array_TypeInfo)
}

namespace System::Collections::Generic {
    struct HashSet$$WazaNo : HashSet<HashSet$$WazaNo, Pml::WazaNo> {
        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_HashSet_WazaNo_Add, Pml::WazaNo> Method$$Add {};
    };
}