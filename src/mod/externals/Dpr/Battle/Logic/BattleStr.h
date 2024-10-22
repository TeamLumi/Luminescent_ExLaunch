#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/POKECON.h"
#include "externals/Dpr/Message/MessageMsgFile.h"

namespace Dpr::Battle::Logic {
    struct MainModule;

    struct BattleStr : ILClass<BattleStr, 0x04c5acf0> {
        struct Fields {
            MainModule* mainModule;
            POKECON::Object* pokeCon;
            Dpr::Message::MessageMsgFile::Array* _msgFiles;
            uint32_t clientID;
            bool fIgnoreFormat;
            bool fForceTrueName;
            void* checkTagSb;
        };

        struct StaticFields {
            System::String::Array* MSBT_MSG_FILES;
            BattleStr::Object* s_Instance;
            bool isInitializedOwner;
            System::UInt16_array* USE_TRMSG_SUB_TABLE;
            void* textSb;
        };

        static_assert(offsetof(Fields, checkTagSb) == 0x20);
        static_assert(offsetof(StaticFields, textSb) == 0x20);

        inline System::String::Object* GetFormatUIText(System::String::Object* label, Dpr::Message::MessageMsgFile::Object* msgFile) {
            return external<System::String::Object*>(0x01886530, this, label, msgFile);
        }
    };
}
