#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/Collections/Generic/IEnumerable.h"
#include "externals/System/Primitives.h"

namespace System::Collections::Generic {
    template <typename T, typename E, game::Address TypeInfo = game::Address::None>
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
            ILClass<T>::template external<void>(game::Address::System_Collections_Generic_List_ABSSequentiable_Add, this, item, *T::Method$$Add);
        }

        template <typename O>
        inline void Add(E* item, ILMethod<O>& method) {
            ILClass<T>::template external<void>(game::Address::System_Collections_Generic_List_ABSSequentiable_Add, this, item, *method);
        }

        template <typename O>
        inline void Clear(ILMethod<O>& method) {
            ILClass<T>::template external<void>(game::Address::System_Collections_Generic_List_ABSSequentiable_Clear, this, *method);
        }

        inline void Clear() {
            ILClass<T>::template external<void>(game::Address::System_Collections_Generic_List_ABSSequentiable_Clear, this, *T::Method$$Clear);
        }

        inline int32_t IndexOf(E* item) {
            return ILClass<T>::template external<int32_t>(game::Address::System_Collections_Generic_List_BTL_POKEPARAM_IndexOf, this, item, *T::Method$$IndexOf);
        }

        inline void RemoveRange(int32_t index, int32_t count) {
            ILClass<T>::template external<void>(game::Address::System_Collections_Generic_List_BUISituationDescriptionButton_RemoveRange, this, index, count, *T::Method$$RemoveRange);
        }

        inline void ctor() {
            ILClass<T>::template external<void>(game::Address::System_Collections_Generic_List_ABSSequentiable_ctor, this, *T::Method$$ctor);
        }
    };

    struct List$$int32_t : ILClass<List$$int32_t> {
        struct Fields {
            System::Int32_array* _items;
            int32_t _size;
            int32_t _version;
            Il2CppObject* _syncRoot;
        };

        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_List_int_Add> Method$$Add {};
        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_List_int_Clear> Method$$Clear {};

        inline void Add(int32_t item) {
            external<void>(game::Address::System_Collections_Generic_List_int_Add, this, item, *Method$$Add);
        }

        inline void Clear() {
            external<void>(game::Address::System_Collections_Generic_List_int_Clear, this, *Method$$Clear);
        }
    };

    struct List$$WazaNo : ILClass<List$$WazaNo, game::Address::System_Collections_Generic_List_WazaNo_TypeInfo> {
        struct Fields {
            System::Int32_array* _items;
            int32_t _size;
            int32_t _version;
            Il2CppObject* _syncRoot;
        };

        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_List_WazaNo_ToArray> Method$$ToArray {};
        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_List_WazaNo_ctor> Method$$ctor {};
        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_List_WazaNo_Add> Method$$Add {};
        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_List_WazaNo_AddRange> Method$$AddRange {};
        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_List_WazaNo_Clear> Method$$Clear {};

        inline void ctor() {
            external<void>(game::Address::System_Collections_Generic_List_BattleModeID_ctor, this, *Method$$ctor);
        }

        inline void Add(int32_t item) {
            external<void>(game::Address::System_Collections_Generic_List_int_Add, this, item, *Method$$Add);
        }

        // Argument is IEnumerable<T>*
        inline void AddRange(void* item) {
            external<void>(game::Address::System_Collections_Generic_List_Int32Enum_AddRange, this, item, *Method$$AddRange);
        }

        inline void Clear() {
            external<void>(game::Address::System_Collections_Generic_List_int_Clear, this, *Method$$Clear);
        }

        inline System::Int32_array* ToArray() {
            return external<System::Int32_array*>(game::Address::System_Collections_Generic_List_ContextMenuID_ToArray, this, *Method$$ToArray);
        }
    };
}
