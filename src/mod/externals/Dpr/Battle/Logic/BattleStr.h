#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Dpr/Message/MessageMsgFile.h"
#include "externals/System/Primitives.h"
#include "externals/System/String.h"

namespace Dpr::Battle::Logic {
    struct MainModule;
    struct POKECON;

    struct BattleStr : ILClass<BattleStr, 0x04c5acf0> {
        struct Fields {
            MainModule* mainModule;
            POKECON* pokeCon;
            void* _msgFiles; // Dpr_Message_MessageMsgFile_array*
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
            void* textSb; // System_Text_StringBuilder_o*
        };

        inline System::String::Object* GetFormatUITrainerName(System::String::Object* label, uint8_t clientID, uint8_t bufID) {
            return external<System::String::Object*>(0x01886720, this, label, clientID, bufID);
        }

        inline System::String::Object* GetFormatUIText(System::String::Object* label, Dpr::Message::MessageMsgFile::Object* msgFile) {
            return external<System::String::Object*>(0x01886530, this, label, msgFile);
        }

        inline System::String::Object* GetFormatUIPokeName(System::String::Object* label, uint8_t pokeID, bool isTruth, uint8_t bufID) {
            return external<System::String::Object*>(0x01886620, this, label, pokeID, isTruth, bufID);
        }
    };
}
