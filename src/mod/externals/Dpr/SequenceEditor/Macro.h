#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Sequence/SequenceSystem.h"
#include "externals/System/Collections/Generic/List.h"
#include "externals/System/String.h"

namespace System::Collections::Generic {
    struct List$$Macro_Value;
}

namespace Dpr::SequenceEditor {
    struct Macro : ILClass<Macro, 0x04c60a20> {
        struct Value : ILStruct<Value, 0x04c65df0> {
            struct Fields {
                System::String::Object* Name;
                System::Collections::Generic::List$$String::Object* Values;
            };
        };

        struct Fields {
            int32_t CommandNo;
            System::String::Object* Name;
            System::Collections::Generic::List$$Macro_Value* Values;
            void* CamFile; // Dpr_Battle_View_Playables_CameraFilePlayable_o*
        };

        static_assert(sizeof(Fields) == 0x20);

        inline void ctor() {
            external<void>(0x02359d30, this);
        }
    };
}

namespace System::Collections::Generic {
    struct List$$Macro_Value : List<List$$Macro_Value, Dpr::SequenceEditor::Macro::Value> {};
}
