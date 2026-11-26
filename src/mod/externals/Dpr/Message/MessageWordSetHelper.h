#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Pml/PokePara/CoreParam.h"
#include "externals/System/String.h"
#include "externals/Dpr/Message/MessageMsgFile.h"
#include "externals/Dpr/Message/MessageManager.h"

namespace Dpr::Message {
    struct MessageWordSetHelper : ILClass<MessageWordSetHelper, game::Address::Dpr_Message_MessageWordSetHelper_TypeInfo> {
        struct Fields {
        };

        struct StaticFields {
            Dpr::Message::MessageManager::Object* manager;
        };

        static inline void SetPokemonNickNameWord(int32_t tagIndex, Pml::PokePara::CoreParam::Object* pp, bool isShowNickName) {
            external<void>(game::Address::Dpr_Message_MessageWordSetHelper_SetPokemonNickNameWord, tagIndex, pp, isShowNickName);
        }

        static inline void SetDigitWord(int32_t tagIndex,int32_t number) {
            external<void>(game::Address::Dpr_Message_MessageWordSetHelper_SetDigitWord, tagIndex, number);
        }

        static inline void SetStringWord(int32_t tagIndex,System::String::Object* str) {
            external<void>(game::Address::Dpr_Message_MessageWordSetHelper_SetStringWord, tagIndex, str);
        }

        static inline void SetMonsNameWord(int32_t tagIndex, int32_t monsNo) {
            external<void>(game::Address::Dpr_Message_MessageWordSetHelper_SetMonsNameWord_Int32_Int32_Wrapper, tagIndex, monsNo);
        }

        static inline void SetGlossaryWord(int32_t index, System::String::Object* msbtName, System::String::Object* labelName) {
            external<void>(game::Address::Dpr_Message_MessageWordSetHelper_SetGlossaryWord_Int32_String_String, index, msbtName, labelName);
        }

        static inline void SetGlossaryWord(int32_t index, System::String::Object* msbtName, int32_t labelIndex) {
            external<void>(game::Address::Dpr_Message_MessageWordSetHelper_SetGlossaryWord_Int32_String_Int32, index, msbtName, labelIndex);
        }

        static inline void SetStringWordFromMsgFile(int32_t index, Dpr::Message::MessageMsgFile::Object* msgFile,
                                                    int32_t labelIndex) {
            external<void>(game::Address::Dpr_Message_MessageWordSetHelper_SetStringWordFromMsgFile, index, msgFile, labelIndex);
        }

        static inline void SetTrainerNameWord(int32_t tagIndex, System::String::Object* labelName) {
            external<void>(game::Address::Dpr_Message_MessageWordSetHelper_SetTrainerNameWord, tagIndex, labelName);
        }

        static inline void SetWazaNameWord(int32_t tagIndex, int32_t wazaNo) {
            external<void>(game::Address::Dpr_Message_MessageWordSetHelper_SetWazaNameWord, tagIndex, wazaNo);
        }
    };
}