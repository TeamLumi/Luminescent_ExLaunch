#pragma once

#include "externals/il2cpp-api.h"

#include "memory/string.h"
#include "externals/System/Collections/Generic/List.h"
#include "externals/System/Primitives.h"
#include "externals/System/Text/Encoding.h"

namespace System {
    struct String : ILClass<String> {
        struct Fields {
            int32_t m_stringLength;
            int16_t m_firstChar;
        };

        static String::Object* Create(const char* value) {
            return external<String::Object*>(0x026fb3e0, nullptr, (int8_t*)value);
        }

        static String::Object* Create(const nn::string& value) {
            return Create(value.c_str());
        }

        static inline bool op_Equality(String::Object* a, String::Object* b) {
            return external<bool>(0x026f05c0, a, b);
        }

        static inline bool IsNullOrEmpty(String::Object* value) {
            return external<bool>(0x026f3810, value);
        }

        static inline String::Object* Concat(String::Object* str0, String::Object* str1) {
            return external<String::Object*>(0x026ef430, str0, str1);
        }

        static inline String::Object* Format(String::Object* format, Il2CppObject* arg0) {
            return external<String::Object*>(0x026f8970, format, arg0);
        }

        static inline String::Object* Format(String::Object* format, Il2CppObject* arg0, Il2CppObject* arg1) {
            return external<String::Object*>(0x026f8ab0, format, arg0, arg1);
        }

        inline String::Object* Substring(int32_t startIndex, int32_t length) {
            return external<String::Object*>(0x026f4560, this, startIndex, length);
        }

        inline String::Object* Truncate(int32_t maxLength) {
            String::Object* str = this->instance();
            if (IsNullOrEmpty(str))
                return str;
            else if (str->fields.m_stringLength <= maxLength)
                return str;
            else
                return str->Substring(0, maxLength);
        }

        static inline System::String::Object* fromUnicodeBytes(System::Byte_array* bytes) {
            System::Text::UnicodeEncoding::Object* encoding = System::Text::Encoding::get_Unicode();
            return (System::String::Object *)encoding->GetString(bytes, 0, bytes->max_length);
        }

        static inline System::String::Object* fromUnicodeBytes(void* bytes, long count) {
            auto byteArray = System::Byte_array::newArray(count);
            memcpy((void*)byteArray->m_Items, bytes, count);
            return fromUnicodeBytes(byteArray);
        }

        nn::string asCString() {
            System::String::Object* str = this->instance();
            System::Text::UTF8Encoding::Object* encoding = System::Text::Encoding::get_UTF8();
            uint32_t size = encoding->GetByteCount(str);
            auto arr = System::Byte_array::newArray(size+1);
            encoding->GetBytes(str, 0, str->fields.m_stringLength, arr, 0);
            arr->m_Items[size] = 0;
            auto res = nn::string((char*)&arr->m_Items[0]);
            return res;
        }

        System::Byte_array* asUnicodeBytes() {
            System::String::Object* str = this->instance();
            System::Text::UnicodeEncoding::Object* encoding = System::Text::Encoding::get_Unicode();
            uint32_t size = encoding->GetByteCount(str);
            auto arr = System::Byte_array::newArray(size+2);
            encoding->GetBytes(str, 0, str->fields.m_stringLength, arr, 0);
            arr->m_Items[size] = 0;
            arr->m_Items[size+1] = 0;
            return arr;
        }
    };
}

namespace System::Collections::Generic {
    struct List$$String : List<List$$String, System::String> {
        static inline StaticILMethod<0x04c8a528> Method$$Add {};
        static inline StaticILMethod<0x04c8a538> Method$$Clear {};
    };
}
