#pragma once
#include "externals/il2cpp-api.h"
#include "externals/System/String.h"

namespace SmartPoint::AssetAssistant {

   struct Serializer : ILClass<Serializer> {

       static inline System::String::Object* ToBase64String(Il2CppObject* content) {
           return external<System::String::Object*>(0x02bc5e20, content);
       }

       static inline Il2CppObject* FromBase64String(System::String::Object* base64string) {
           return external<Il2CppObject*>(0x02bc6060, base64string);
       }
   };

}