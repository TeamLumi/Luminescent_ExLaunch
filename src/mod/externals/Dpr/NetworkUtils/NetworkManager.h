#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/NetworkUtils/RequestValidateResult.h"
#include "externals/Dpr/NetworkUtils/ValidateCheckPluralResult.h"
#include "externals/Dpr/NetworkUtils/ValidateCheckResult.h"
#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"
#include "externals/System/String.h"

namespace Dpr::NetworkUtils {
    struct NetworkManager : ILClass<NetworkManager, 0x04c59da0> {
        struct Fields : SmartPoint::AssetAssistant::SingletonMonoBehaviour::Fields {
            System::Collections::Generic::Dictionary$$int32_t$$String::Object* sessionErrorLogTable;
            void* networkErrorDialogTable; // System_Collections_Generic_Dictionary_int__NetworkData_SheetErrorDialogInfo__o*
            bool processingInternetGo;
            void* sessionConnector; // Dpr_NetworkUtils_SessionConnector_o*
            void* networkParam; // Dpr_NetworkUtils_NetworkParam_o*
            void* packetReader; // INL1_PacketReader_o*
            void* packetWriter; // INL1_PacketWriter_o*
            void* packetWriterRe; // INL1_PacketWriterRe_o*
            void* checkRequest; // INL1_IlcaNetServerValidate_CheckRequest_o*
            void* checkResponse; // INL1_IlcaNetServerValidate_CheckResponse_o*
            RequestValidateResult::Object validateResult;
            ValidateCheckResult::Object singleValidateCheckResult;
            ValidateCheckPluralResult::Object pluralValidateCheckResult;
            void* gmsTradeResult; // Dpr_NetworkUtils_GMSTradeResult_o*
            void* appletResult; // Dpr_NetworkUtils_ErrorAppletResult_o*
            void* showMsgWindow; // Dpr_SubContents_ShowMessageWindow_o*
            int32_t systemErrorDialogOpenCount;
            uint32_t systemErrorDialogCode;
        };

        static_assert(sizeof(Fields) == 0x98);

        static inline bool IsShowApplicationErrorDialog() {
            return external<bool>(0x01deb4d0);
        }
    };
}
