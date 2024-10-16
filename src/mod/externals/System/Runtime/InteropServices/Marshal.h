#pragma once
#include "externals/il2cpp-api.h"
#include "externals/System/Primitives.h"
namespace System::Runtime::InteropServices {
    struct Marshal : ILClass<Marshal> {

        static intptr_t AllocHGlobal(int32_t cb) {
            return external<intptr_t>(0x022c67a0, cb);
        }

        static void Copy(intptr_t source, System::Byte_array* destination, int32_t startIndex, int32_t length) {
            external<void>(0x022c6970, source, destination, startIndex, length);
        }

        static void FreeHGlobal(intptr_t hglobal) {
            external<void>(0x022c6ab0, hglobal);
        }

        template <typename T>
        static int32_t SizeOf(T::Object* structure) {
            return external<int32_t>(0x01ef6280, reinterpret_cast<Il2CppObject*>(structure));
        }

        template <typename T>
        static void StructureToPtr(T::Object* structure, intptr_t ptr, bool fDeleteOld) {
            external<void>(0x01cef680, reinterpret_cast<Il2CppObject*>(structure), ptr, fDeleteOld);
        }
    };
}