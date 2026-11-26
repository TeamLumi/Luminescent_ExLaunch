#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/POKECON.h"
#include "externals/Dpr/Message/MessageMsgFile.h"

namespace Dpr::Battle::Logic {
    struct MainModule;

    struct BattleStr : ILClass<BattleStr, game::Address::Dpr_Battle_Logic_BattleStr_TypeInfo> {
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

        inline System::String::Object* GetFormatUITrainerName(System::String::Object* label, uint8_t clientID, uint8_t bufID) {
            return external<System::String::Object*>(game::Address::Dpr_Battle_Logic_BattleStr_GetFormatUITrainerName, this, label, clientID, bufID);
        }

        inline System::String::Object* GetFormatUIText(System::String::Object* label, Dpr::Message::MessageMsgFile::Object* msgFile) {
            return external<System::String::Object*>(game::Address::Dpr_Battle_Logic_BattleStr_GetFormatUIText, this, label, msgFile);
        }

        inline System::String::Object* GetFormatUIPokeName(System::String::Object* label, uint8_t pokeID, bool isTruth, uint8_t bufID) {
            return external<System::String::Object*>(game::Address::Dpr_Battle_Logic_BattleStr_GetFormatUIPokeName, this, label, pokeID, isTruth, bufID);
        }
    };
}
