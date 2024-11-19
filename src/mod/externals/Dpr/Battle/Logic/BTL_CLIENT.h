#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "externals/Dpr/Battle/Logic/ServerSendData.h"
#include "externals/Dpr/Battle/Logic/MainModule.h"
#include "externals/Dpr/Battle/Logic/BattleEnv.h"
#include "externals/Dpr/Battle/Logic/BTL_ACTION.h"

namespace Dpr::Battle::Logic {
    struct BTL_CLIENT : ILClass<BTL_CLIENT> {
        struct Fields {
            Dpr::Battle::Logic::MainModule::Object* m_mainModule;
            Dpr::Battle::Logic::BattleEnv::Object* m_pBattleEnv;
            void* m_procPoke; // BTL_POKEPARAM_o*
            uint8_t m_actCountSum;
            int32_t m_procActionIndex;
            Dpr::Battle::Logic::BTL_ACTION::PARAM_OBJ::Object* m_procActionUIRet;
            int32_t m_currentActionIndex;
            void* m_recData; // rec_Data_o*
            void* m_btlRecReader; // rec_Reader_o*
            void* m_recPlayer; // RECPLAYER_CONTROL_o*
            void* m_mainProc; // ClientMainProc_o*
            void* m_fldSim; // FieldStatus_o*
            uint64_t m_randContext;
            void* m_adapter; // Adapter_o*
            void* m_sendDataContainer; // SendDataContainer_o*
            void* m_receiveDataContainer; // SendDataContainer_o*
            void* m_random; // Random_o*
            void* m_AIRand; // Random_o*
            void* m_trainerMessageManager; // TrainerMessageManager_o*
            void* m_seq_TrainerMessage; // ClientSeq_TrainerMessage_o*
            void* m_seq_WinWild; // ClientSeq_WinWild_o*
            void* m_seq_Capture; // ClientSeq_Capture_o*
            void* m_battleSimulator; // BattleSimulator_o*
            void* m_battleDriver; // BattleDriver_o*
            void* m_serverCmdQueue; // ServerCommandQueue_o*
            void* m_gameTimer; // GameTimer_o*
            Dpr::Battle::Logic::ServerSendData::CLIENT_LIMIT_TIME::Object m_syncClientTime; // ServerSendData_CLIENT_LIMIT_TIME_o
            void* m_raidBossCaptureResult; // ServerSendData_RAIDBOSS_CAPTURE_RESULT_o
            void* CUSTOM_PADDING1;
            void* CUSTOM_PADDING2;
            void* CUSTOM_PADDING3;
            void* CUSTOM_PADDING4;
            uint32_t m_turnCount;
            uint16_t m_EnemyPokeHPBase;
            void* m_selItemWork; // BTL_CLIENT_SEL_ITEM_WORK_o*
            uint8_t m_myID;
            uint8_t m_myType;
            uint8_t m_myState;
            bool m_commWaitInfoOn;
            uint8_t m_bagMode;
            uint8_t* m_change_escape_code;
            bool m_fForceQuitSelAct;
            uint8_t m_cmdCheckTimingCode;
            uint8_t* m_actionCountWork; // uint8_t[8]
            uint8_t m_wazaInfoPokeIdx;
            uint8_t m_wazaInfoWazaIdx;
            bool m_fAITrainerBGMChanged;
            bool m_fCommError;
            bool m_fSelActForceFinish;
            bool m_fCmdCheckEnable;
            bool m_fRecPlayEndTimeOver;
            bool m_fRecPlayEndBufOver;
            bool m_bRecPlayFadeStarted;
            bool m_isWaitingAdapterCommand;
            bool m_isGSelectedThisTurn;
            bool m_isFirstActionSelectDone;
            uint8_t m_myChangePokeCnt;
            uint8_t m_myPuttablePokeCnt;
            void* m_myChangePokePos; // BtlPokePos_o*
            ushort m_returnDataSerialNumber;
            void* m_returnDataServerSeq; // ServerSequence_o*
            void* m_returnDataServerRequest; // ServerRequest_o*
            void* m_returnDataPtr;
            uint m_returnDataSize;
            uint* m_dummyReturnData;
            void* m_returnData_ActionSelect; // ClientSendData_ACTION_SELECT_o*
            void* m_returnData_ClientLimitTime; // ClientSendData_CLIENT_LIMIT_TIME_o*
            void* m_returnData_RaidBallSelect; // ClientSendData_RAID_BALL_SELECT_o*
            ushort m_cmdLimitTime;
            ushort m_gameLimitTime;
            ushort m_clientLimitTime;
            void* m_myParty; // BTL_PARTY_o*
            uint8_t m_procPokeIdx;
            uint8_t m_procPokeActIdx;
            int8_t m_prevPokeIdx;
            uint8_t m_firstPokeIdx;
            bool m_fStdMsgChanged;
            bool m_b1stReadyMsgDisped;
            void* m_cmdCheckServer; // BTL_SERVER_o*
            void* _m_viewCore; // BattleViewBase_o*
            void* m_viewExpGetDesc; // BattleViewBase_ExpGetDesc_o*
            void* m_viewExpGetResult; // BattleViewBase_ExpGetResult_o*
            void* m_actionParam; // BTL_ACTION_PARAM_o*
            System::Int32_array* m_cmdArgs;
            void* m_stdVariableArgs; // BTL_CLIENT_VariableArgs_o*
            void* m_tmpVariableArgs; // BTL_CLIENT_VariableArgs_o*
            void* m_ai; // BattleAi_o*
            void* m_AIItem; // ushort*
            void* m_AIChangeIndex; // suint8_t*
            void* m_AITrainerMsgCheckedFlag; // bool*
            void* m_subProc; // BTL_CLIENT_ClientSubProc_o*
            int32_t m_subSeq;
            void* m_selActProc; // BTL_CLIENT_ClientSubProc_o*
            int32_t m_selActSeq;
            void* m_strParam; // BTLV_STRPARAM_o*
            void* m_strParamSub; // BTLV_STRPARAM_o*
            void* m_pokeSelParam; // PokeSelParam_o*
            void* m_pokeSelResult; // PokeSelResult_o*
            void* m_scProc; // BTL_CLIENT_ServerCmdProc_o*
            void* m_serverCmd; // ServerCommand_o*
            int32_t m_scSeq;
            void* m_deadPokePos; // BtlPokePos_o*
            bool m_isLiveRecSeedSetup;
            uint32_t m_liveRecWaitCameraSeq;
            uint32_t m_liveRecSizeSave;
            void* m_wazaEffDmgSyncWork; // BTL_CLIENT_WAZAEFF_SYNCDAMAGE_CMD_WORK_o*
            bool m_bWazaEffectDone;
            bool m_bSyncEffectDone;
            void* m_frEffectProc; // BTL_CLIENT_FriendshipEffectProc_o*
            uint32_t m_JK3Joker_PrevTurnAttackToLegends;
            uint32_t m_JK3Legend_PrevTurnUseKyozyuuzan;
            int32_t scProc_MSG_StdSE_subSeq;
            int32_t scProc_MSG_SetSE_subSeq;
            uint32_t scProc_ACT_KinomiPrevWaza_procIdx;
            int32_t scProc_ACT_FriendshipEffectMsg_effSeq;
            int32_t scProc_ACT_FriendshipEffectMsg_msgSeq;
            uint32_t scProc_ACTOP_SwapTokusei_timer;
        };
        
        inline int32_t checkForbidChangeEscapeCommon(BTL_POKEPARAM *procPoke, uint8_t *pokeID, uint16_t *tokuseiID) {
            return external<int32_t>(0x01f574e0, this, procPoke, pokeID, tokuseiID);
        }
    };
}
