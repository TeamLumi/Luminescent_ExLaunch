#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/Collections/Generic/List.h"
#include "externals/System/String.h"
#include "externals/UnityEngine/ScriptableObject.h"

namespace System::Collections::Generic {
    struct List$$Aregment;
    struct List$$Script;
    struct List$$Command;
}

struct EvData : ILClass<EvData, 0x04c5f100> {
    enum class ArgType : int32_t {
        Command = 0,
        Float = 1,
        Work = 2,
        Flag = 3,
        SysFlag = 4,
        String = 5
    };

    struct Aregment : ILStruct<Aregment> {
        struct Fields {
            int32_t argType;
            int32_t data;
        };
    };

    struct Script : ILClass<Script> {
        struct Fields {
            System::String::Object* Label;
            System::Collections::Generic::List$$Command* Commands;
        };
    };

    struct Command : ILClass<Command> {
        struct Fields {
            System::Collections::Generic::List$$Aregment* Arg;
        };
    };

    struct Fields : UnityEngine::ScriptableObject::Fields {
        System::Collections::Generic::List$$Script* Scripts;
        System::Collections::Generic::List$$String::Object* StrList;
    };
    
    inline System::String::Object * GetString(int32_t index) {
        return external<System::String::Object *>(0x01f115b0, this, index);
    }
};

namespace System::Collections::Generic {
    struct List$$Aregment : List<List$$Aregment, EvData::Aregment> {};
    struct List$$Script : List<List$$Script, EvData::Script> {};
    struct List$$Command : List<List$$Command, EvData::Command> {};
}
