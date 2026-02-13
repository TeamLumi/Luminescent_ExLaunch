#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/Collections/Generic/IEnumerable.h"
#include "externals/System/Primitives.h"

namespace System::Collections::Generic {
    template <typename T, typename E, long TypeInfo = 0>
    struct List : ILClass<T, TypeInfo> {
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
            external<void>(0x02a70aa0, this, item, *Method$$Add);
        }

        inline void Clear() {
            external<void>(0x02a70e40, this, *Method$$Clear);
        }
    };

    struct List$$WazaNo : ILClass<List$$WazaNo, 0x04c63248> {
        struct Fields {
            System::Int32_array* _items;
            int32_t _size;
            int32_t _version;
            Il2CppObject* _syncRoot;
        };

        static inline StaticILMethod<0x04c68988> Method$$ToArray {};
        static inline StaticILMethod<0x04c8a0a0> Method$$ctor {};
        static inline StaticILMethod<0x04c8a0a8> Method$$Add {};
        static inline StaticILMethod<0x04c8a0b0> Method$$AddRange {};
        static inline StaticILMethod<0x04c8a0b8> Method$$Clear {};

        inline void ctor() {
            external<void>(0x02a72c00, this, *Method$$ctor);
        }

        inline void Add(int32_t item) {
            external<void>(0x02a70aa0, this, item, *Method$$Add);
        }

        // Argument is IEnumerable<T>*
        inline void AddRange(void* item) {
            external<void>(0x02a73b10, this, item, *Method$$AddRange);
        }

        inline void Clear() {
            external<void>(0x02a70e40, this, *Method$$Clear);
        }

        inline System::Int32_array* ToArray() {
            return external<System::Int32_array*>(0x02a75910, this, *Method$$ToArray);
        }
    };
}
