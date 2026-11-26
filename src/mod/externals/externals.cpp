#include "exlaunch.hpp"
#include "externals/System/_Object.h"

void system_load_typeinfo(game::TypeInfo metadata) {
    const auto load = reinterpret_cast<void (*)(long)>(exl::util::modules::GetTargetOffset(game::Address::Il2Cpp_Metadata_Initialize));
    load(static_cast<uint32_t>(metadata));
}
void* (*system_array_new_raw)(Il2CppClass* type, long length) = reinterpret_cast<typeof(system_array_new_raw)>(exl::util::modules::GetTargetOffset(game::Address::Il2Cpp_Array_New));
void* (*il2cpp_object_new_raw)(Il2CppClass* klass) = reinterpret_cast<typeof(il2cpp_object_new_raw)>(exl::util::modules::GetTargetOffset(game::Address::Il2Cpp_Object_New));
void (*il2cpp_runtime_class_init_raw)(Il2CppClass* klass) = reinterpret_cast<typeof(il2cpp_runtime_class_init_raw)>(exl::util::modules::GetTargetOffset(game::Address::Il2Cpp_Class_Initialize));
void (*system_array_init)(void* ptr, void* obj) = reinterpret_cast<typeof(system_array_init)>(exl::util::modules::GetTargetOffset(game::Address::Il2Cpp_Array_Initialize));
void* (*resolve_unity_method)(const char* signature) = reinterpret_cast<typeof(resolve_unity_method)>(exl::util::modules::GetTargetOffset(game::Address::Il2Cpp_ResolveUnityMethod));
