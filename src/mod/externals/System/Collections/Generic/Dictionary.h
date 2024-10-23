#pragma once

#include "externals/il2cpp-api.h"
#include "externals/System/Collections/Generic/IEnumerable.h"
#include "externals/System/Primitives.h"

namespace System::Collections::Generic {
    template <typename T, typename K, typename V>
    struct Dictionary$$Object : ILClass<T> {
        struct Entry : ILStruct<Entry> {
            struct Fields {
                int32_t hashCode;
                int32_t next;
                K::Object* key;
                V::Object* value;
            };
        };

        struct Fields {
            System::Int32_array* buckets;
            Entry::Array* entries;
            int32_t count;
            int32_t version;
            int32_t freeList;
            int32_t freeCount;
            void* comparer;
            void* keys;
            void* values;
            Il2CppObject* _syncRoot;
        };
    };

    template <typename T, typename V>
    struct Dictionary$$int32_t : ILClass<T> {
        struct Entry : ILStruct<Entry> {
            struct Fields {
                int32_t hashCode;
                int32_t next;
                int32_t key;
                V::Object* value;
            };
        };

        struct Fields {
            System::Int32_array* buckets;
            Entry::Array* entries;
            int32_t count;
            int32_t version;
            int32_t freeList;
            int32_t freeCount;
            void* comparer;
            void* keys;
            void* values;
            Il2CppObject* _syncRoot;
        };

        inline void ctor() {
            ILClass<T>::template external<void>(0x028e55b0, this, *T::Method$$ctor);
        }

        inline void set_Item(int32_t key, V::Object* value) {
            ILClass<T>::template external<void>(0x028da060, this, key, value, *T::Method$$set_Item);
        }

        inline V* get_Item(int32_t key) {
            return ILClass<T>::template external<V*>(0x028d9f90, this, key, *T::Method$$get_Item);
        }

        template <typename E>
        inline IEnumerable<E, T>* get_Values() {
            return ILClass<T>::template external<IEnumerable<E, T>*>(0x028d9ef0, this, *T::Method$$get_Values);
        }

        inline void Add(int32_t key, V::Object* value) {
            ILClass<T>::template external<void>(0x028e5a70, this, key, value, *T::Method$$Add);
        }
    };

    template <typename T, typename V>
    struct Dictionary$$uint8_t : ILClass<T> {
        struct Entry : ILStruct<Entry> {
            struct Fields {
                int32_t hashCode;
                int32_t next;
                uint8_t key;
                V::Object* value;
            };
        };

        struct Fields {
            System::Int32_array* buckets;
            Entry::Array* entries;
            int32_t count;
            int32_t version;
            int32_t freeList;
            int32_t freeCount;
            void* comparer;
            void* keys;
            void* values;
            Il2CppObject* _syncRoot;
        };

        inline void ctor() {
            ILClass<T>::template external<void>(0x027c0280, this, *T::Method$$ctor);
        }

        inline void Add(uint8_t key, V::Object* value) {
            ILClass<T>::template external<void>(0x027c0740, this, key, value, *T::Method$$Add);
        }
    };
}
