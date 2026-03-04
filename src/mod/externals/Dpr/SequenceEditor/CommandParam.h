#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/SequenceEditor/Macro.h"

namespace Dpr::SequenceEditor {
    struct CommandParam : ILClass<CommandParam, 0x04c61318> {
        struct Fields {
            void* Group; // Dpr_SequenceEditor_Group_o*
            bool IsAlreadyCalled;
            int32_t StartFrame;
            int32_t EndFrame;
            int32_t GroupNo;
            int32_t CommandNo;
            Dpr::SequenceEditor::Macro::Object* Macro;
            void* GroupOption; // System_Collections_Generic_List_GroupOption__o*
        };

        inline void ctor() {
            external<void>(0x01e416e0, this);
        }
    };
}
