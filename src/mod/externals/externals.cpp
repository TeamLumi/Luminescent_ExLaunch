#include "exlaunch.hpp"
#include "externals/System/_Object.h"

void (*system_load_typeinfo)(long param) = reinterpret_cast<typeof(system_load_typeinfo)>(exl::util::modules::GetTargetOffset(0x00264fe0));
void* (*system_array_new_raw)(Il2CppClass* type, long length) = reinterpret_cast<typeof(system_array_new_raw)>(exl::util::modules::GetTargetOffset(0x002afb00));
void* (*il2cpp_object_new_raw)(Il2CppClass* klass) = reinterpret_cast<typeof(il2cpp_object_new_raw)>(exl::util::modules::GetTargetOffset(0x00266700));
void (*il2cpp_runtime_class_init_raw)(Il2CppClass* klass) = reinterpret_cast<typeof(il2cpp_runtime_class_init_raw)>(exl::util::modules::GetTargetOffset(0x002afbe0));
void (*system_array_init)(void* ptr, void* obj) = reinterpret_cast<typeof(system_array_init)>(exl::util::modules::GetTargetOffset(0x002afa20));
