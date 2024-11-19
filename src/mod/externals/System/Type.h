#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/Reflection/MemberInfo.h"
#include "externals/System/RuntimeTypeHandle.h"

namespace System {
    struct Type : ILClass<Type, 0x04c552a0> {
        struct Fields : System::Reflection::MemberInfo::Fields {
            System::RuntimeTypeHandle::Object _impl;
        };

        struct VTable {
            VirtualInvokeData _0_Equals;
            VirtualInvokeData _1_Finalize;
            VirtualInvokeData _2_GetHashCode;
            VirtualInvokeData _3_ToString;
            VirtualInvokeData _4_unknown;
            VirtualInvokeData _5_unknown;
            VirtualInvokeData _6_get_MemberType;
            VirtualInvokeData _7_get_Name;
            VirtualInvokeData _8_get_DeclaringType;
            VirtualInvokeData _9_get_ReflectedType;
            VirtualInvokeData _10_unknown;
            VirtualInvokeData _11_GetCustomAttributes;
            VirtualInvokeData _12_IsDefined;
            VirtualInvokeData _13_GetCustomAttributesData;
            VirtualInvokeData _14_get_MetadataToken;
            VirtualInvokeData _15_unknown;
            VirtualInvokeData _16_get_DeclaringMethod;
            VirtualInvokeData _17_MakePointerType;
            VirtualInvokeData _18_MakeByRefType;
            VirtualInvokeData _19_MakeArrayType;
            VirtualInvokeData _20_MakeArrayType;
            VirtualInvokeData _21_GetTypeCodeImpl;
            VirtualInvokeData _22_unknown;
            VirtualInvokeData _23_unknown;
            VirtualInvokeData _24_get_TypeHandle;
            VirtualInvokeData _25_unknown;
            VirtualInvokeData _26_unknown;
            VirtualInvokeData _27_unknown;
            VirtualInvokeData _28_GetArrayRank;
            VirtualInvokeData _29_unknown;
            VirtualInvokeData _30_GetConstructor;
            VirtualInvokeData _31_GetConstructor;
            VirtualInvokeData _32_GetConstructor;
            VirtualInvokeData _33_unknown;
            VirtualInvokeData _34_unknown;
            VirtualInvokeData _35_GetMethod;
            VirtualInvokeData _36_GetMethod;
            VirtualInvokeData _37_GetMethod;
            VirtualInvokeData _38_GetMethod;
            VirtualInvokeData _39_unknown;
            VirtualInvokeData _40_GetMethods;
            VirtualInvokeData _41_unknown;
            VirtualInvokeData _42_unknown;
            VirtualInvokeData _43_unknown;
            VirtualInvokeData _44_unknown;
            VirtualInvokeData _45_unknown;
            VirtualInvokeData _46_GetProperty;
            VirtualInvokeData _47_GetProperty;
            VirtualInvokeData _48_unknown;
            VirtualInvokeData _49_unknown;
            VirtualInvokeData _50_GetMember;
            VirtualInvokeData _51_GetMember;
            VirtualInvokeData _52_GetMember;
            VirtualInvokeData _53_get_Attributes;
            VirtualInvokeData _54_get_GenericParameterAttributes;
            VirtualInvokeData _55_get_IsNotPublic;
            VirtualInvokeData _56_get_IsPublic;
            VirtualInvokeData _57_get_IsNestedPublic;
            VirtualInvokeData _58_get_IsNestedAssembly;
            VirtualInvokeData _59_get_IsExplicitLayout;
            VirtualInvokeData _60_get_IsClass;
            VirtualInvokeData _61_get_IsInterface;
            VirtualInvokeData _62_get_IsValueType;
            VirtualInvokeData _63_get_IsAbstract;
            VirtualInvokeData _64_get_IsSealed;
            VirtualInvokeData _65_get_IsEnum;
            VirtualInvokeData _66_get_IsSerializable;
            VirtualInvokeData _67_get_IsArray;
            VirtualInvokeData _68_get_IsSzArray;
            VirtualInvokeData _69_get_IsGenericType;
            VirtualInvokeData _70_get_IsGenericTypeDefinition;
            VirtualInvokeData _71_get_IsGenericParameter;
            VirtualInvokeData _72_get_GenericParameterPosition;
            VirtualInvokeData _73_get_ContainsGenericParameters;
            VirtualInvokeData _74_GetGenericParameterConstraints;
            VirtualInvokeData _75_get_IsByRef;
            VirtualInvokeData _76_get_IsPointer;
            VirtualInvokeData _77_get_IsPrimitive;
            VirtualInvokeData _78_get_IsCOMObject;
            VirtualInvokeData _79_get_HasElementType;
            VirtualInvokeData _80_get_IsContextful;
            VirtualInvokeData _81_get_IsMarshalByRef;
            VirtualInvokeData _82_IsValueTypeImpl;
            VirtualInvokeData _83_unknown;
            VirtualInvokeData _84_unknown;
            VirtualInvokeData _85_unknown;
            VirtualInvokeData _86_unknown;
            VirtualInvokeData _87_unknown;
            VirtualInvokeData _88_unknown;
            VirtualInvokeData _89_MakeGenericType;
            VirtualInvokeData _90_IsContextfulImpl;
            VirtualInvokeData _91_IsMarshalByRefImpl;
            VirtualInvokeData _92_unknown;
            VirtualInvokeData _93_GetGenericArguments;
            VirtualInvokeData _94_GetGenericTypeDefinition;
            VirtualInvokeData _95_unknown;
            VirtualInvokeData _96_GetEnumNames;
            VirtualInvokeData _97_GetEnumValues;
            VirtualInvokeData _98_GetEnumUnderlyingType;
            VirtualInvokeData _99_IsEnumDefined;
            VirtualInvokeData _100_GetEnumName;
            VirtualInvokeData _101_unknown;
            VirtualInvokeData _102_IsSubclassOf;
            VirtualInvokeData _103_IsInstanceOfType;
            VirtualInvokeData _104_IsAssignableFrom;
            VirtualInvokeData _105_IsEquivalentTo;
            VirtualInvokeData _106_FormatTypeName;
            VirtualInvokeData _107_Equals;
            VirtualInvokeData _108_GetType;
        };

        inline static System::Type::Object* GetTypeFromHandle(System::RuntimeTypeHandle::Object handle) {
            return external<System::Type::Object*>(0x02b194d0, handle);
        }
    };
}
