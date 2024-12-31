#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Pml/PokePara/CoreParam.h"
#include "externals/System/String.h"
#include "externals/Dpr/Message/MessageMsgFile.h"
#include "externals/Dpr/Message/MessageManager.h"

namespace Dpr::Message {
    struct MessageWordSetHelper : ILClass<MessageWordSetHelper, 0x04c5ac68> {
        struct Fields {
        };

        struct StaticFields {
            Dpr::Message::MessageManager::Object* manager;
        };

        static inline void SetPokemonNickNameWord(int32_t tagIndex, Pml::PokePara::CoreParam::Object* pp, bool isShowNickName) {
            external<void>(0x01f99b90, tagIndex, pp, isShowNickName);
        }

        static inline void SetDigitWord(int32_t tagIndex,int32_t number) {
            external<void>(0x01f9a580, tagIndex, number);
        }

        static inline void SetStringWord(int32_t tagIndex,System::String::Object* str) {
            external<void>(0x01f9d590, tagIndex, str);
        }

        static inline void SetMonsNameWord(int32_t tagIndex, int32_t monsNo) {
            external<void>(0x01f99fa0, tagIndex, monsNo);
        }

        static inline void SetGlossaryWord(int32_t index, System::String::Object* msbtName, System::String::Object* labelName) {
            external<void>(0x01f9c2c0, index, msbtName, labelName);
        }

        static inline void SetGlossaryWord(int32_t index, System::String::Object* msbtName, int32_t labelIndex) {
            external<void>(0x01f9c410, index, msbtName, labelIndex);
        }

        static inline void SetWazaTypeWord(int32_t tagIndex, uint8_t pokeTypeNo) {
            external<void>(0x01f9a7a0, tagIndex, pokeTypeNo);
        }

        static inline void SetStringWordFromMsgFile(int32_t index, Dpr::Message::MessageMsgFile::Object* msgFile,
                                                    int32_t labelIndex) {
            external<void>(0x01f9f8e0, index, msgFile, labelIndex);
        }

        static inline void SetTrainerNameWord(int32_t tagIndex, System::String::Object* labelName) {
            external<void>(0x01f9bf50, tagIndex, labelName);
        }
    };
}