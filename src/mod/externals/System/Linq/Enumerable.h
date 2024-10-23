#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/Collections/Generic/IEnumerable.h"
#include "externals/System/Collections/Generic/List.h"

namespace System::Linq {
    struct Enumerable : ILClass<Enumerable> {
        struct Fields {}; // Empty for some reason?

        template <typename E, typename L, typename T>
        static inline System::Collections::Generic::List<L, T>* ToList(System::Collections::Generic::IEnumerable<E, T>::Object* source) {
            return external<System::Collections::Generic::List<L, T>*>(0x01a6c1b0, source, *E::Method$$ToList);
        }
    };
}
