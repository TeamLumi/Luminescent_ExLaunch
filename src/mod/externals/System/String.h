#pragma once

#include "externals/il2cpp-api.h"

#include "memory/string.h"
#include "externals/System/_Object.h"
#include "externals/System/Collections/Generic/Dictionary.h"
#include "externals/System/Collections/Generic/List.h"
#include "externals/System/Primitives.h"
#include "externals/System/Text/Encoding.h"
#include "externals/System/ValueTuple.h"

namespace System {
    struct String : ILClass<String> {
        struct Fields {
            int32_t m_stringLength;
            int16_t m_firstChar;
        };

        static String::Object* Create(const char* value) {
            return external<String::Object*>(game::Address::System_String_CreateString, nullptr, (int8_t*)value);
        }

        static String::Object* Create(const nn::string& value) {
            return Create(value.c_str());
        }

        static inline bool op_Equality(String::Object* a, String::Object* b) {
            return external<bool>(game::Address::System_String_op_Equality, a, b);
        }

        static inline bool IsNullOrEmpty(String::Object* value) {
            return external<bool>(game::Address::System_String_IsNullOrEmpty, value);
        }

        static inline String::Object* Concat(String::Object* str0, String::Object* str1) {
            return external<String::Object*>(game::Address::System_String_Concat, str0, str1);
        }

        static inline String::Object* Concat(String::Object* str0, String::Object* str1, String::Object* str2) {
            return external<String::Object*>(game::Address::System_String_Concat_String_String_String, str0, str1, str2);
        }

        static inline String::Object* Concat(String::Object* str0, String::Object* str1, String::Object* str2, String::Object* str3) {
            return external<String::Object*>(game::Address::System_String_Concat_String_String_String_String, str0, str1, str2, str3);
        }

        static inline String::Object* Format(String::Object* format, Il2CppObject* arg0) {
            return external<String::Object*>(game::Address::System_String_Format_String_Object, format, arg0);
        }

        static inline String::Object* Format(String::Object* format, Il2CppObject* arg0, Il2CppObject* arg1) {
            return external<String::Object*>(game::Address::System_String_Format_String_Object_Object, format, arg0, arg1);
        }

        static inline String::Object* Format(String::Object* format, Il2CppObject* arg0, Il2CppObject* arg1, Il2CppObject* arg2) {
            return external<String::Object*>(game::Address::System_String_Format_String_Object_Object_Object, format, arg0, arg1, arg2);
        }

        static inline String::Object* Format(String::Object* format, System::_Object::Array* args) {
            return external<String::Object*>(game::Address::System_String_Format_String_ObjectArray, format, args);
        }

        inline String::Object* Substring(int32_t startIndex, int32_t length) {
            return external<String::Object*>(game::Address::System_String_Substring_Int32_Int32, this, startIndex, length);
        }

        inline String::Object* Substring(int32_t startIndex) {
            return external<String::Object*>(game::Address::System_String_Substring_Int32, this, startIndex);
        }

        inline int32_t LastIndexOf(uint16_t value) {
            return external<int32_t>(game::Address::System_String_LastIndexOf, this, value);
        }

        inline bool StartsWith(String::Object* value) {
            return external<bool>(game::Address::System_String_StartsWith, this, value);
        }

        inline String::Array* Split(System::Char_array* separator) {
            return external<String::Array*>(game::Address::System_String_Split, this, separator);
        }

        // Only tested to work on int
        static Il2CppObject* ConvertToStringObject(Il2CppClass* klass, void* ptr) {
            return external<Il2CppObject*>(game::Address::System_String_ConvertToStringObject_Native, klass, ptr);
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

namespace System {
    struct ValueTuple2$$Bool$$String : ValueTuple2<ValueTuple2$$Bool$$String, bool, System::String::Object*> {};
    struct ValueTuple2$$String$$int32 : ValueTuple2<ValueTuple2$$String$$int32, System::String::Object*, int32_t> {};
    struct ValueTuple3$$String$$String$$String : ValueTuple3<ValueTuple3$$String$$String$$String, System::String::Object*, System::String::Object*, System::String::Object*> {};
}

namespace System::Collections::Generic {
    struct List$$String : List<List$$String, System::String> {
        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_List_string_Add> Method$$Add {};
        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_List_string_Clear> Method$$Clear {};
    };

    struct List$$ValueTuple3$$String$$String$$String : ILClass<List$$ValueTuple3$$String$$String$$String> {
        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_List_ValueTuple_string_string_string_Add> Method$$Add{};
        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_List_ValueTuple_string_string_string_Clear> Method$$Clear{};

        struct Fields {
            ValueTuple3$$String$$String$$String::Array* _items;
            int32_t _size;
            int32_t _version;
            Il2CppObject* _syncRoot;
        };

        inline void Add(ValueTuple3$$String$$String$$String::Object* item) {
            external<void>(game::Address::System_Collections_Generic_List_ValueTuple_object_object_object_Add, this, item, *Method$$Add);
        }

        inline void Clear() {
            external<void>(game::Address::System_Collections_Generic_List_ValueTuple_object_object_object_Clear, this, *Method$$Clear);
        }
    };

    struct Dictionary$$int32_t$$String : Dictionary$$int32_t<Dictionary$$int32_t$$String, System::String> {
        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_Dictionary_int_string_ctor> Method$$ctor {};
        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_Dictionary_int_string_Add> Method$$Add {};
        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_Dictionary_int_string_set_Item> Method$$set_Item {};
        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_Dictionary_int_string_get_Values> Method$$get_Values {}; // NOTE: not for that specific dictionary but it's probably fine
    };

    struct Dictionary$$EffectType$$String : Dictionary$$int32_t<Dictionary$$EffectType$$String, System::String> {
        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_Dictionary_EffectType_string_ctor> Method$$ctor {};
        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_Dictionary_EffectType_string_Add> Method$$Add {};
        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_Dictionary_EffectType_string_ContainsKey> Method$$ContainsKey {};
        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_Dictionary_EffectType_string_get_Item> Method$$get_Item {};
    };

    struct Dictionary$$BtlSideEffect$$String : Dictionary$$int32_t<Dictionary$$BtlSideEffect$$String, System::String> {
        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_Dictionary_BtlSideEffect_string_ctor> Method$$ctor {};
        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_Dictionary_BtlSideEffect_string_Add> Method$$Add {};
        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_Dictionary_EffectType_string_ContainsKey> Method$$ContainsKey {}; // NOTE: not for that specific dictionary and it'S NOT FINE
        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_Dictionary_EffectType_string_get_Item> Method$$get_Item {}; // NOTE: not for that specific dictionary but it's probably fine
    };

    struct Dictionary$$BtlWeather$$String : Dictionary$$uint8_t<Dictionary$$BtlWeather$$String, System::String> {
        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_Dictionary_BtlWeather_string_ctor> Method$$ctor {};
        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_Dictionary_BtlWeather_string_Add> Method$$Add {};
        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_Dictionary_BtlWeather_string_ContainsKey> Method$$ContainsKey {};
        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_Dictionary_BtlWeather_string_get_Item> Method$$get_Item {};
    };

    struct Dictionary$$BtlGround$$String : Dictionary$$uint8_t<Dictionary$$BtlGround$$String, System::String> {
        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_Dictionary_BtlGround_string_ctor> Method$$ctor {};
        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_Dictionary_BtlGround_string_Add> Method$$Add {};
        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_Dictionary_BtlGround_string_ContainsKey> Method$$ContainsKey {};
        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_Dictionary_BtlGround_string_get_Item> Method$$get_Item {};
    };

    struct IEnumerable$$String : IEnumerable<IEnumerable$$String, System::String> {
        static inline StaticILMethod<game::Address::Method_System_Linq_Enumerable_ToList_string> Method$$ToList {};
    };
}