#pragma once

#include "externals/il2cpp-api.h"
#include "externals/System/Primitives.h"

namespace System::Collections::Generic {
    template <typename T, typename E, long typeInfo = 0>
    struct List : ILClass<T, typeInfo> {
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

        template <typename O>
        inline void Add(E* item, ILMethod<O>& method) {
            ILClass<T>::template external<void>(0x02a3c680, this, item, *method);
        }

        template <typename O>
        inline void Clear(ILMethod<O>& method) {
            ILClass<T>::template external<void>(0x02a3ca20, this, *method);
        }

        inline void Clear() {
            ILClass<T>::template external<void>(0x02a3ca20, this, *T::Method$$Clear);
        }

        inline int32_t IndexOf(E* item) {
            return ILClass<T>::template external<int32_t>(0x02a3d630, this, item, *T::Method$$IndexOf);
        }

        inline void RemoveRange(int32_t index, int32_t count) {
            ILClass<T>::template external<void>(0x02a3e310, this, index, count, *T::Method$$RemoveRange);
        }

        inline void ctor() {
            ILClass<T>::template external<void>(0x02a3ba70, this, *T::Method$$ctor);
        }
    };

    struct List$$int32_t : ILClass<List$$int32_t, 0x04c56ea> {
        struct Fields {
            System::Int32_array* _items;
            int32_t _size;
            int32_t _version;
            Il2CppObject* _syncRoot;
        };

        static inline StaticILMethod<0x04c8a3a8> Method$$Add {};
        static inline StaticILMethod<0x04c6fa08, bool> Method$$Remove {};
        static inline StaticILMethod<0x04c8a3c8> Method$$RemoveAt {};
        static inline StaticILMethod<0x04c8a3b8> Method$$Clear {};
        static inline StaticILMethod<0x04c8a390> Method$$ctor {};

        inline void Add(int32_t item) {
            external<void>(0x02a70aa0, this, item, *Method$$Add);
        }

        inline bool Remove(int32_t item) {
            return external<bool>(0x02a722c0, this, item, *Method$$Remove);
        }

        inline void RemoveAt(int32_t index) {
            external<void>(0x02a72620, this, index, *Method$$RemoveAt);
        }

        inline void Clear() {
            external<void>(0x02a70e40, this, *Method$$Clear);
        }

        inline void ctor() {
            external<void>(0x02a6fde0, this, *Method$$ctor);
        }
    };
}
