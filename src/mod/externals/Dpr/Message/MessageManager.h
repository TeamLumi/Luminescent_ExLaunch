#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Message/MessageGlossaryParseDataModel.h"
#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"
#include "externals/Dpr/Message/MessageMsgFile.h"

namespace Dpr::Message {
    struct MessageManager : ILClass<MessageManager, 0x04c5fe18> {
        struct Fields : SmartPoint::AssetAssistant::SingletonMonoBehaviour::Fields{
            void* msbtLoader; //Dpr_Message_MsgDataFileLoader_o*
            void* dataModel; //Dpr_Message_MessageDataModel_o*
            void* tagWordBuffer; //Dpr_Message_TagWordBuffer_o*
            void* wordDataPool; //Dpr_Message_MessageWordDataPool_o*
        };

        static inline Dpr::Message::MessageManager::Object* get_Instance() {
            return SmartPoint::AssetAssistant::SingletonMonoBehaviour::get_Instance(SmartPoint::AssetAssistant::SingletonMonoBehaviour::Method$$MessageManager$$get_Instance);
        }

        inline System::String::Object* GetSimpleMessage(System::String::Object* fileName, System::String::Object* label) {
            return external<System::String::Object *>(0x0210d000, this, fileName, label);
        }

        inline Dpr::Message::MessageGlossaryParseDataModel::Object* GetNameMessageDataModel(System::String::Object* fileName, int32_t labelIndex) {
            return external<Dpr::Message::MessageGlossaryParseDataModel::Object*>(0x02109230, this, fileName, labelIndex);
        }

        inline int32_t get_UserLanguageID() {
            return external<int32_t>(0x0210a0a0, this);
        }

        inline System::String::Object* GetNameMessage(System::String::Object* fileName, System::String::Object* label) {
            return external<System::String::Object*>(0x0210ce50, this, fileName, label);
        }

        inline Dpr::Message::MessageMsgFile::Object* GetMsgFile(System::String::Object* fileName) {
            return external<Dpr::Message::MessageMsgFile::Object*>(0x0210c050, this, fileName);
        }
    };
}

