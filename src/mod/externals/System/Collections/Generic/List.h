#pragma once

#include "externals/il2cpp-api.h"
#include "externals/System/Primitives.h"

namespace System::Collections::Generic {
    template <typename T, typename E>
    struct List : ILClass<T> {
        struct Fields {
            E::Array* _items;
            int32_t _size;
            int32_t _version;
            Il2CppObject* _syncRoot;
        };

        void swap(int32_t i, int32_t j) {
            auto arr = this->instance()->fields._items->m_Items;
            auto tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
        }

        inline void Add(E* item) {
            ILClass<T>::template external<void>(0x02a3c680, this, item, *T::Method$$Add);
        }

        inline void Clear() {
            ILClass<T>::template external<void>(0x02a3ca20, this, *T::Method$$Clear);
        }

        inline int32_t IndexOf(E* item) {
            return ILClass<T>::template external<int32_t>(0x02a3d630, this, item, *T::Method$$IndexOf);
        }
    };

    struct List$$int32_t : ILClass<List$$int32_t> {
        struct Fields {
            System::Int32_array* _items;
            int32_t _size;
            int32_t _version;
            Il2CppObject* _syncRoot;
        };

        static inline StaticILMethod<0x04c8a3a8> Method$$Add {};
        static inline StaticILMethod<0x04c8a3b8> Method$$Clear {};

        inline void Add(int32_t item) {
            external<void>(0x02a70aa0, this, item, Method$$Add);
        }

        inline void Clear() {
            external<void>(0x02a70e40, this, Method$$Clear);
        }
    };
}
