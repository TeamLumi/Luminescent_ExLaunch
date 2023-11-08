#pragma once

#include <cstdint>
#include <cstddef>
#include "imgui.h"
#include "exlaunch.hpp"
#include "il2cpp.h"

extern void (*system_load_typeinfo)(long obj);
extern void* (*system_array_new_raw)(Il2CppClass* type, long length);
extern void* (*il2cpp_object_new_raw)(Il2CppClass* klass);
extern void (*il2cpp_runtime_class_init_raw)(Il2CppClass* klass);
extern void (*system_array_init)(void* ptr, void* obj);

template <typename T>
inline T::Array* system_array_new(typename T::Class* type, long length) { return reinterpret_cast<T::Array*(*)(Il2CppClass*, long)>(system_array_new_raw)(type, length); }
inline void* system_array_new(Il2CppClass* type, long length) { return reinterpret_cast<void*(*)(Il2CppClass*, long)>(system_array_new_raw)(type, length); }

template <typename T>
inline T::Object* il2cpp_object_new(typename T::Class* klass) { return reinterpret_cast<T::Object*(*)(Il2CppClass*)>(il2cpp_object_new_raw)(klass); }
inline void* il2cpp_object_new(Il2CppClass* klass) { return reinterpret_cast<void*(*)(Il2CppClass*)>(il2cpp_object_new_raw)(klass); }

template <typename T>
inline void il2cpp_runtime_class_init(typename T::Class* klass) { reinterpret_cast<void(*)(Il2CppClass*)>(il2cpp_runtime_class_init_raw)(klass); }
inline void il2cpp_runtime_class_init(Il2CppClass* klass) { reinterpret_cast<void(*)(Il2CppClass*)>(il2cpp_runtime_class_init_raw)(klass); }


struct _ILExternal {
    char _zeroSizedFilled[0];

    template <typename Ret, typename... Args>
    inline static Ret external(long addr, Args... args) {
        return reinterpret_cast<Ret(*)(Args...)>(exl::util::modules::GetTargetOffset(addr))(args...);
    }
};

template <typename T>
struct ILStruct : _ILExternal  {
public:
    struct Fields { };

    struct Object : T {
        T::Fields fields;
    };

    struct Array {
        Il2CppObject obj;
        Il2CppArrayBounds* bounds;
        uint64_t max_length;
        T::Object m_Items[1];

        inline void copyInto(T::Object* dst) {
            for (uint64_t i = 0; i < max_length; i++) {
                dst[i] = m_Items[i];
            }
        }

        inline void fillWith(T::Object value) {
            for (uint64_t i = 0; i < max_length; i++) {
                m_Items[i] = value;
            }
        }

        struct iterator {
            using iterator_category = std::forward_iterator_tag;
            using difference_type = ptrdiff_t;
            using value_type = T::Object;
            using pointer = value_type*;
            using reference = value_type&;

            explicit iterator(T::Object* ptr) : m_ptr(ptr) {}

            reference operator*() const { return *m_ptr; }
            pointer operator->() { return m_ptr; }
            iterator& operator++() { m_ptr++; return *this; }
            iterator operator++(int) { iterator tmp = *this; m_ptr++; return tmp; }

            friend bool operator==(const iterator& a, const iterator& b) {
                return a.m_ptr == b.m_ptr;
            }
            friend bool operator!=(const iterator& a, const iterator& b) {
                return a.m_ptr != b.m_ptr;
            }

        private:
            T::Object* m_ptr;
        };

        iterator begin() {
            return iterator(&this->m_Items[0]);
        }

        iterator end() {
            return iterator(&this->m_Items[this->max_length]);
        }
    };
};

template <typename T, long TypeInfo = 0, long ArrayTypeInfo = 0>
struct ILClass : _ILExternal {
protected:
    ILClass() = default;

public:
    struct StaticFields { };
    struct RGCTXs { };
    struct VTable { };
    struct Fields { };

    struct Class {
        Il2CppClass_1 _1;
        T::StaticFields* static_fields;
        T::RGCTXs* rgctx_data;
        Il2CppClass_2 _2;
        T::VTable* vtable;

        inline void initIfNeeded() {
            if ((_2.bitflags2 >> 1 & 1) && (_2.cctor_finished == 0)) {
                il2cpp_runtime_class_init((Il2CppClass*)this);
            }
        }

        template <typename... Args>
        T::Object* newInstance(Args... args) {
            auto obj = reinterpret_cast<T::Object*>(il2cpp_object_new((Il2CppClass*)this));
            obj->ctor(args...);
            return obj;
        }

        T::Array* newInstance(long length) {
            return reinterpret_cast<T::Array*>(system_array_new((Il2CppClass*)this, length));
        }
    };

    struct Object : T {
        T::Class* klass;
        void* monitor;
        T::Fields fields;
    };

    struct Array {
        Il2CppObject obj;
        Il2CppArrayBounds* bounds;
        uint64_t max_length;
        T::Object* m_Items[1];

        inline void copyInto(T::Object** dst) {
            for (uint64_t i = 0; i < max_length; i++) {
                dst[i] = m_Items[i];
            }
        }

        inline void fillWith(T::Object value) {
            for (uint64_t i = 0; i < max_length; i++) {
                *m_Items[i] = value;
            }
        }

        struct iterator {
            using iterator_category = std::forward_iterator_tag;
            using difference_type = ptrdiff_t;
            using value_type = T::Object;
            using pointer = value_type*;
            using reference = value_type&;

            explicit iterator(T::Object** ptr) : m_ptr(ptr) {}

            reference operator*() const { return **m_ptr; }
            pointer operator->() { return *m_ptr; }
            iterator& operator++() { m_ptr++; return *this; }
            iterator operator++(int) { iterator tmp = *this; m_ptr++; return tmp; }

            friend bool operator==(const iterator& a, const iterator& b) {
                return a.m_ptr == b.m_ptr;
            }
            friend bool operator!=(const iterator& a, const iterator& b) {
                return a.m_ptr != b.m_ptr;
            }

        private:
            T::Object** m_ptr;
        };

        iterator begin() {
            return iterator(&this->m_Items[0]);
        }

        iterator end() {
            return iterator(&this->m_Items[this->max_length]);
        }
    };

    template <typename Target>
    Target::Object* cast() {
        static_assert(std::is_base_of<typename Target::Fields, typename T::Fields>::value, "T cannot be cast to Target");
        return reinterpret_cast<Target::Object*>(this);
    }

    Object* instance() {
        return reinterpret_cast<Object*>(this);
    }

    template <typename... Args>
    static Object* newInstance(Args... args) {
        auto klass = getClass();
        klass->initIfNeeded();
        return klass->newInstance(args...);
    }

    static Array* newArray(long length) {
        auto klass = getArrayClass();
        klass->initIfNeeded();
        return klass->newInstance(length);
    }

    static Class* getClass() {
        static_assert(TypeInfo != 0, "TypeInfo address not set");
        return *reinterpret_cast<T::Class**>(exl::util::modules::GetTargetOffset(TypeInfo));
    }

    static Class* getClass(long ti) {
        return *reinterpret_cast<T::Class**>(exl::util::modules::GetTargetOffset(ti));
    }

    static Class* getArrayClass() {
        static_assert(ArrayTypeInfo != 0, "ArrayTypeInfo address not set");
        return *reinterpret_cast<T::Class**>(exl::util::modules::GetTargetOffset(ArrayTypeInfo));
    }
};

#define PRIMITIVE_ARRAY(name)                               \
struct name##_array {                                       \
    Il2CppObject obj;                                       \
    Il2CppArrayBounds* bounds;                              \
    uint64_t max_length;                                    \
    name m_Items[65535];                                    \
    inline void copyInto(name* dst) {                       \
        for (uint64_t i = 0; i < max_length; i++) {         \
            dst[i] = m_Items[i];                            \
        }                                                   \
    }                                                       \
    inline void fillWith(name value) {                      \
        for (uint64_t i = 0; i < max_length; i++) {         \
            m_Items[i] = value;                             \
        }                                                   \
    }                                                       \
};

template <typename... Args>
struct ILMethod {
    explicit ILMethod(MethodInfo* method) : method(method) { }

    virtual MethodInfo* operator*() {
        return method;
    }

    MethodInfo* copyWith(Il2CppMethodPointer function) {
        return method->copyWith(function);
    }

private:
    MethodInfo* method;
};

template <long MI, typename... Args>
struct StaticILMethod : ILMethod<Args...> {
    StaticILMethod() : ILMethod<Args...>(nullptr) { }

    MethodInfo* operator*() override {
        auto ptr = (MethodInfo**)exl::util::modules::GetTargetOffset(MI);
        return *ptr;
    }
};
