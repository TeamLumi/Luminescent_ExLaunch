#pragma once

#include <cstdint>
#include <cstddef>
#include "memory/nn_allocator.h"
#include "exlaunch.hpp"

typedef void(*Il2CppMethodPointer)();

struct Il2CppClass;

struct Il2CppType {
    void* data;
    unsigned int bits;
};

struct MethodInfo {
    Il2CppMethodPointer methodPointer;
    void* invoker_method;
    const char* name;
    Il2CppClass* klass;
    const Il2CppType* return_type;
    const void* parameters;
    union {
        const void* rgctx_data;
        const void* methodDefinition;
    };
    union {
        const void* method;
        const void* genericMethod;
    };
    uint32_t token;
    uint16_t flags;
    uint16_t iflags;
    uint16_t slot;
    uint8_t parameters_count;
    uint8_t bitflags;

    // Make sure to free once you're 100% sure it won't be used anymore.
    // Alternatively, keep a reference to re-use.
    MethodInfo* copyWith(Il2CppMethodPointer method) {
        auto m = (MethodInfo*)nn_malloc(sizeof(MethodInfo));
        memcpy(m, this, sizeof(MethodInfo));
        m->methodPointer = method;
        return m;
    }
};


struct VirtualInvokeData {
    Il2CppMethodPointer methodPtr;
    const MethodInfo* method;
};

struct Il2CppClass_1 {
    void* image;
    void* gc_desc;
    const char* name;
    const char* namespaze;
    Il2CppType byval_arg;
    Il2CppType this_arg;
    Il2CppClass* element_class;
    Il2CppClass* castClass;
    Il2CppClass* declaringType;
    Il2CppClass* parent;
    void *generic_class;
    void* typeDefinition;
    void* interopData;
    Il2CppClass* klass;
    void* fields;
    void* events;
    void* properties;
    void* methods;
    Il2CppClass** nestedTypes;
    Il2CppClass** implementedInterfaces;
    void* interfaceOffsets;
};

struct Il2CppClass_2 {
    Il2CppClass** typeHierarchy;
    void *unity_user_data;
    uint32_t initializationExceptionGCHandle;
    uint32_t cctor_started;
    uint32_t cctor_finished;
    size_t cctor_thread;
    int32_t genericContainerIndex;
    uint32_t instance_size;
    uint32_t actualSize;
    uint32_t element_size;
    int32_t native_size;
    uint32_t static_fields_size;
    uint32_t thread_static_fields_size;
    int32_t thread_static_fields_offset;
    uint32_t flags;
    uint32_t token;
    uint16_t method_count;
    uint16_t property_count;
    uint16_t field_count;
    uint16_t event_count;
    uint16_t nested_type_count;
    uint16_t vtable_count;
    uint16_t interfaces_count;
    uint16_t interface_offsets_count;
    uint8_t typeHierarchyDepth;
    uint8_t genericRecursionDepth;
    uint8_t rank;
    uint8_t minimumAlignment;
    uint8_t naturalAligment;
    uint8_t packingSize;
    uint8_t bitflags1;
    uint8_t bitflags2;
};

struct Il2CppObject {
    Il2CppClass* klass;
    void* monitor;
};

struct Il2CppClass {
    Il2CppClass_1 _1;
    void* static_fields;
    void* rgctx_data;
    Il2CppClass_2 _2;
    void* vtable;
};

struct Il2CppArrayBounds {
    uint64_t length;
    uint32_t lower_bound;
};
