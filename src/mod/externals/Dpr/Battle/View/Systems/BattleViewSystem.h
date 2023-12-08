#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BattleViewBase.h"
#include "externals/Dpr/Battle/View/BtlvBallInfo.h"
#include "externals/Dpr/Battle/View/Systems/BTLV_WAZA_EFF_PARAM.h"
#include "externals/Dpr/Battle/View/Systems/BattleCameraSystem.h"
#include "externals/Dpr/Battle/View/Systems/DeadActParam.h"
#include "externals/Dpr/Battle/View/Systems/MemberChangeActParam.h"
#include "externals/Dpr/Battle/View/Systems/MemberOutActParam.h"
#include "externals/Dpr/Battle/View/Systems/StartGActParam.h"
#include "externals/Dpr/Battle/View/Systems/EndGActParam.h"
#include "externals/Dpr/Battle/View/Systems/BattleCharacterSystem.h"
#include "externals/Dpr/Battle/View/Systems/TrainerTalkParam.h"
#include "externals/Dpr/Battle/View/Systems/BTLV_ATTR_EFF_PARAM.h"
#include "externals/Dpr/Battle/View/Objects/BtlvSound.h"
#include "externals/System/String.h"
#include "externals/System/Nullable.h"

namespace Dpr::Battle::View::Systems {
    struct BattleViewSystem : ILClass<BattleViewSystem> {
        struct Fields : Dpr::Battle::Logic::BattleViewBase::Fields {
            bool m_isFinalize;
            bool m_isFinalizeFadeSkip;
            bool m_isFinishLoadObjects;
            void* m_iPtrSequenceSystem;
            Dpr::Battle::View::Systems::BattleCharacterSystem::Object* m_iPtrCharacterSystem;
            Dpr::Battle::View::Systems::BattleCameraSystem::Object* m_iPtrCameraSystem;
            void* m_iPtrWeatherSystem;
            void* m_iPtrBattleGroundEffectSystem;
            void* m_iPtrStatusEffectObserverSystem;
            void* m_battleDataTable;
            void* m_defaultPlacementData;
            void* m_iPtrScreenObject;
            void* m_skrtModelDict;
            Dpr::Battle::View::Systems::BTLV_WAZA_EFF_PARAM::Object m_wazaParam;
            void* m_comTargetPoke;
            void* m_comTargetDamage;
            void* m_comTargetTrainer;
            int32_t m_comEffectId;
            void* m_effectBallId;
            Dpr::Battle::View::BtlvBallInfo::Array* m_effectBallInfo;
            int32_t m_comIsVisibleTame;
            bool m_comIsMigawari;
            bool m_comMigawariReserve;
            int32_t m_comMigawariReserveTarget;
            int32_t m_comRepeatNum;
            bool m_comIsSuccsess;
            bool m_comIsCritical;
            int32_t m_subSequence;
            int32_t m_sequenceSeq;
            bool m_seqKeepResource;
            System::String::Object* m_playingSeqFileNameHash;
            void* m_pComWaitFunc;
            bool m_isStartMsgDisplay;
            int32_t m_wazaMsgFrame;
            int32_t m_beforePlayEffPos;
            int32_t m_beforePlayEffNo;
            bool m_isFieldIndoor;
            bool m_isFieldStadium;
            bool m_isFieldRoseTower;
            bool m_isFieldStadiumWide;
            bool m_isFieldWaitcamExcept;
            bool m_reqCheckPinch;
            bool m_canChangePinch;
            bool m_isSoundPlayingFinishCheckInvalid;
            int32_t m_soundPlayingFinishWaitCount;
            Dpr::Battle::View::Objects::BtlvSound* m_pokePinchSound;
            bool m_preUpdateAudioListenerByMainCamera;
            Dpr::Battle::View::Systems::DeadActParam::Object m_deadActParam;
            Dpr::Battle::View::Systems::MemberChangeActParam::Object m_memberChangeActParam;
            Dpr::Battle::View::Systems::MemberOutActParam::Object m_memberOutActParam;
            Dpr::Battle::View::Systems::StartGActParam::Object m_startGActParam;
            Dpr::Battle::View::Systems::EndGActParam::Object m_endGActParam;
            Dpr::Battle::View::Systems::TrainerTalkParam::Object m_trainerTalkParam;
            Dpr::Battle::View::Systems::BTLV_ATTR_EFF_PARAM::Object m_attrEffParam;
            System::String::Object* m_attrSoundEvent;
            void* m_iPtrTaskManager; //Dpr_Battle_View_TaskManager_o*
            void* m_iPtrTaskManagerLate; //Dpr_Battle_View_TaskManager_o*
            void* m_uPtrSoundPlayingIDHash; //System_Collections_Generic_Stack_Tuple_int__uint__uint___o*
            int32_t m_shadowResolutionBackup;
            int32_t _InitializeSequenceState_k__BackingField;
            int32_t _SetupSequenceState_k__BackingField;
            int32_t _SetupCommSequence_k__BackingField;
            int32_t _SetupRaidSequence_k__BackingField;
            int32_t _WazaSequence_k__BackingField;
            int32_t _EndSequence_k__BackingField;
            bool _IsStencilEnable_k__BackingField;
            float _blurry_k__BackingField;
            bool _IsApplicationPause_k__BackingField;
        };

        static_assert(offsetof(Fields, m_effectBallInfo) == 0xE0);

        struct VirtualInvokeData_GetBattlePokeParam_forUI {
            typedef bool(*Il2CppMethodPointer)(Dpr::Battle::View::Systems::BattleViewSystem::Object* __this, Dpr::Battle::Logic::BTL_POKEPARAM::Object* pDest, int32_t vpos, const MethodInfo*);
            Il2CppMethodPointer methodPtr;
            const MethodInfo* method;
        };

        struct VirtualInvokeData_GetBattleParty {
            typedef Dpr::Battle::Logic::BTL_PARTY::Object*(*Il2CppMethodPointer)(Dpr::Battle::View::Systems::BattleViewSystem::Object* __this, uint8_t clientID, const MethodInfo*);
            Il2CppMethodPointer methodPtr;
            const MethodInfo* method;
        };

        struct VirtualInvokeData_IsClientTrainerExist {
            typedef bool(*Il2CppMethodPointer)(Dpr::Battle::View::Systems::BattleViewSystem::Object* __this, uint8_t clientID, const MethodInfo*);
            Il2CppMethodPointer methodPtr;
            const MethodInfo* method;
        };

        struct VirtualInvokeData_GetBtlvPosToClientId {
            typedef uint8_t(*Il2CppMethodPointer)(Dpr::Battle::View::Systems::BattleViewSystem::Object* __this, int32_t vpos, const MethodInfo*);
            Il2CppMethodPointer methodPtr;
            const MethodInfo* method;
        };

        struct VTable {
            VirtualInvokeData _0_Equals;
            VirtualInvokeData _1_Finalize;
            VirtualInvokeData _2_GetHashCode;
            VirtualInvokeData _3_ToString;
            VirtualInvokeData _4_GetMainModule;
            VirtualInvokeData _5_GetBattleEnv;
            VirtualInvokeData _6_GetBattleContainer;
            VirtualInvokeData _7_GetBattlePokeParam;
            VirtualInvokeData_GetBattlePokeParam_forUI _8_GetBattlePokeParam_forUI;
            VirtualInvokeData _9_GetViewSrcData;
            VirtualInvokeData _10_GetBattleParty_Self;
            VirtualInvokeData _11_GetBattleParty_Friend;
            VirtualInvokeData _12_GetBattleParty_Enemy;
            VirtualInvokeData_GetBattleParty _13_GetBattleParty;
            VirtualInvokeData _14_GetClient;
            VirtualInvokeData _15_GetFieldStatus;
            VirtualInvokeData _16_GetSideEffectStatus;
            VirtualInvokeData _17_GetPosEffectStatus;
            VirtualInvokeData _18_GetBattleCompetitor;
            VirtualInvokeData _19_GetBattleRule;
            VirtualInvokeData _20_GetCommMode;
            VirtualInvokeData _21_IsCommMode;
            VirtualInvokeData _22_IsCommChild;
            VirtualInvokeData _23_IsMultiMode;
            VirtualInvokeData _24_IsPokeExist;
            VirtualInvokeData _25_IsFriendExist;
            VirtualInvokeData_IsClientTrainerExist _26_IsClientTrainerExist;
            VirtualInvokeData _27_IsWatchMode;
            VirtualInvokeData _28_GetClientID;
            VirtualInvokeData _29_GetFriendCleintID;
            VirtualInvokeData _30_GetEnemyClientID;
            VirtualInvokeData _31_GetEnemyClientNum;
            VirtualInvokeData _32_IsClientNpc;
            VirtualInvokeData _33_GetClientIdToBtlvPos;
            VirtualInvokeData_GetBtlvPosToClientId _34_GetBtlvPosToClientId;
            VirtualInvokeData _35_GetBtlvPosToPosIdx;
            VirtualInvokeData _36_GetBtlvPosToTrainerIdx;
            VirtualInvokeData _37_GetTrainerID;
            VirtualInvokeData _38_GetTrainerType;
            VirtualInvokeData _39_GetTrainerModelID;
            VirtualInvokeData _40_GetTrainerColorID;
            VirtualInvokeData _41_GetTrainerWinEffect;
            VirtualInvokeData _42_GetTrainerName;
            VirtualInvokeData _43_GetTrainerNameLabel;
            VirtualInvokeData _44_GetTrainerSex;
            VirtualInvokeData _45_IsClientRatingDisplay;
            VirtualInvokeData _46_GetClientRating;
            VirtualInvokeData _47_GetMessageSpeed;
            VirtualInvokeData _48_IsClientCheerMode;
            VirtualInvokeData _49_GetFrontPosCount_Self;
            VirtualInvokeData _50_GetFrontPosCount_Friend;
            VirtualInvokeData _51_GetBenchPosIndex_Self;
            VirtualInvokeData _52_GetBenchPosIndex_Friend;
            VirtualInvokeData _53_IsSkyBattle;
            VirtualInvokeData _54_IsZukanRegistered;
            VirtualInvokeData _55_IsEnableWazaEffect;
            VirtualInvokeData _56_IsItemEnable;
            VirtualInvokeData _57_GetSetupStatusFlag;
            VirtualInvokeData _58_GetEnableTimeStop;
            VirtualInvokeData _59_GetEnableVoiceChat;
            VirtualInvokeData _60_IsRecordPlayMode;
            VirtualInvokeData _61_GetEscapeMode;
            VirtualInvokeData _62_CanUseEscapeItem;
            VirtualInvokeData _63_GetMiseaiData;
            VirtualInvokeData _64_GetTurnCount;
            VirtualInvokeData _65_IsWazaEffectEnable;
            VirtualInvokeData _66_IsPlayerInLeftSide;
            VirtualInvokeData _67_BtlPosToViewPos;
            VirtualInvokeData _68_ViewPosToBtlPos;
            VirtualInvokeData _69_ViewPosToBtlSide;
            VirtualInvokeData _70_PokeIDToViewPos;
            VirtualInvokeData _71_IsPlayerSide;
            VirtualInvokeData _72_GetGameTime;
            VirtualInvokeData _73_GetCommandTime;
            VirtualInvokeData _74_GetClientTime;
            VirtualInvokeData _75_GetUiDisplay_Turn_Weather;
            VirtualInvokeData _76_GetUiDisplay_Turn_Ground;
            VirtualInvokeData _77_GetUiDisplay_Turn_Hikarinokabe;
            VirtualInvokeData _78_GetUiDisplay_Turn_Reflector;
            VirtualInvokeData _79_GetUiDisplay_Turn_AuroraVeil;
            VirtualInvokeData _80_GetUiDisplay_Turn_SideEffect;
            VirtualInvokeData _81_GetUiDisplay_Turn;
            VirtualInvokeData _82_GetUiDisplay_PokeType;
            VirtualInvokeData _83_GetUiDisplay_WazaType;
            VirtualInvokeData _84_GetUiDisplay_WazaPower;
            VirtualInvokeData _85_GetUiDisplay_WazaHitPer;
            VirtualInvokeData _86_GetUiDisplay_WazaDamageType;
            VirtualInvokeData _87_GetUiDisplay_IsPokemonAppearedOnBattleField;
            VirtualInvokeData _88_GetUiDisplay_GetClientPublicInfomation;
            VirtualInvokeData _89_GetUiDisplay_IsCheerMode;
            VirtualInvokeData _90_GetUIDisplay_GetMultiMode;
            VirtualInvokeData _91_GetUiDisplay_GetPlayerName;
            VirtualInvokeData _92_IsWazaAffinityDisplayEnable;
            VirtualInvokeData _93_CalcWazaAffinityAbout;
            VirtualInvokeData _94_GetRaidActionIconID;
            VirtualInvokeData _95_IsGWallGaugeDisplay;
            VirtualInvokeData _96_GetGWallGaugeMax;
            VirtualInvokeData _97_GetGWallGaugeNow;
            VirtualInvokeData _98_CheckTrainerActionRequest;
            VirtualInvokeData _99_SetTrainerActionRequest;
            VirtualInvokeData _100_ClearTrainerActionRequest;
            VirtualInvokeData _101_IsTutorial;
            VirtualInvokeData _102_GetSafariBallNum;
            VirtualInvokeData _103_Initialize;
            VirtualInvokeData _104_FinalizeApp;
            VirtualInvokeData _105_FinalizeAppForce;
            VirtualInvokeData _106_CMD_StartSetup;
            VirtualInvokeData _107_CMD_WaitSetup;
            VirtualInvokeData _108_CMD_StartCleanUp;
            VirtualInvokeData _109_CMD_WaitCleanUp;
            VirtualInvokeData _110_CMD_InitStartWaitCameraEffect;
            VirtualInvokeData _111_CMD_WaitStartWaitCameraEffect;
            VirtualInvokeData _112_CMD_InitFinishWaitCameraEffect;
            VirtualInvokeData _113_CMD_WaitFinishWaitCameraEffect;
            VirtualInvokeData _114_CMD_UI_OnFirstSelectActionStart;
            VirtualInvokeData _115_CMD_UI_SelectAction_Start;
            VirtualInvokeData _116_CMD_UI_SelectAction_Wait;
            VirtualInvokeData _117_CMD_UI_SelectAction_ForceQuit;
            VirtualInvokeData _118_CMD_UI_SelectAction_AllFinished;
            VirtualInvokeData _119_CMD_UI_SelectAction_AllFinished_Wait;
            VirtualInvokeData _120_CMD_UI_SelectWaza_Start;
            VirtualInvokeData _121_CMD_UI_SelectWaza_Restart;
            VirtualInvokeData _122_CMD_UI_SelectWaza_Wait;
            VirtualInvokeData _123_CMD_UI_SelectWaza_End;
            VirtualInvokeData _124_CMD_UI_SelectWaza_ForceQuit;
            VirtualInvokeData _125_CMD_UI_SelectTarget_Start;
            VirtualInvokeData _126_CMD_UI_SelectTarget_Wait;
            VirtualInvokeData _127_CMD_UI_SelectTarget_ForceQuit;
            VirtualInvokeData _128_CMD_UI_RestartIfNotStandBy;
            VirtualInvokeData _129_CMD_UI_Restart;
            VirtualInvokeData _130_CMD_UI_WaitRestart;
            VirtualInvokeData _131_CMD_StartMemberChangeAct;
            VirtualInvokeData _132_CMD_WaitMemberChangeAct;
            VirtualInvokeData _133_CMD_StartMsgInBuffer;
            VirtualInvokeData _134_CMD_StartPokeList;
            VirtualInvokeData _135_CMD_WaitPokeList;
            VirtualInvokeData _136_CMD_ForceQuitPokeList;
            VirtualInvokeData _137_CMD_WaitForceQuitPokeList;
            VirtualInvokeData _138_CMD_StartPokeSelect;
            VirtualInvokeData _139_CMD_WaitPokeSelect;
            VirtualInvokeData _140_CMD_ForceQuitPokeSelect;
            VirtualInvokeData _141_CMD_WaitForceQuitPokeSelect;
            VirtualInvokeData _142_CMD_StartMsg;
            VirtualInvokeData _143_CMD_StartMsgWaza;
            VirtualInvokeData _144_CMD_StartMsgStd;
            VirtualInvokeData _145_CMD_StartMsgSet;
            VirtualInvokeData _146_CMD_StartMsgTrainer;
            VirtualInvokeData _147_CMD_WaitMsg;
            VirtualInvokeData _148_CMD_WaitMsg_WithoutHide;
            VirtualInvokeData _149_CMD_HideMsg;
            VirtualInvokeData _150_CMD_ACT_WazaEffect_Start;
            VirtualInvokeData _151_CMD_ACT_WazaEffect_Wait;
            VirtualInvokeData _152_CMD_ACT_WazaEffect_NotView;
            VirtualInvokeData _153_CMD_ACT_DamageEffectSingle_Start;
            VirtualInvokeData _154_CMD_ACT_DamageEffectSingle_Wait;
            VirtualInvokeData _155_CMD_ACT_DamageEffectPlural_Start;
            VirtualInvokeData _156_CMD_ACT_DamageEffectPlural_Wait;
            VirtualInvokeData _157_CMD_ACT_MigawariDamageEffect_Start;
            VirtualInvokeData _158_CMD_ACT_MigawariDamageEffect_Wait;
            VirtualInvokeData _159_CMD_StartDeadAct;
            VirtualInvokeData _160_CMD_WaitDeadAct;
            VirtualInvokeData _161_CMD_StartReliveAct;
            VirtualInvokeData _162_CMD_WaitReliveAct;
            VirtualInvokeData _163_CMD_ACT_MemberOut_Start;
            VirtualInvokeData _164_CMD_ACT_MemberOut_Wait;
            VirtualInvokeData _165_CMD_ACT_TameWazaHide;
            VirtualInvokeData _166_CMD_ACT_SimpleHPEffect_Start;
            VirtualInvokeData _167_CMD_ACT_SimpleHPEffect_Wait;
            VirtualInvokeData _168_CMD_AddEffect;
            VirtualInvokeData _169_CMD_WaitEffect;
            VirtualInvokeData _170_CMD_AddEffectByPos;
            VirtualInvokeData _171_CMD_WaitEffectByPos;
            VirtualInvokeData _172_CMD_AddEffectByDir;
            VirtualInvokeData _173_CMD_WaitEffectByDir;
            VirtualInvokeData _174_CMD_AddEffectBySide;
            VirtualInvokeData _175_CMD_WaitEffectBySide;
            VirtualInvokeData _176_CMD_TokWin_DispStart;
            VirtualInvokeData _177_CMD_TokWin_DispWait;
            VirtualInvokeData _178_CMD_QuitTokWin;
            VirtualInvokeData _179_CMD_QuitTokWinWait;
            VirtualInvokeData _180_CMD_TokWin_Renew_Start;
            VirtualInvokeData _181_CMD_TokWin_Renew_Wait;
            VirtualInvokeData _182_CMD_StartRankDownEffect;
            VirtualInvokeData _183_CMD_StartRankUpEffect;
            VirtualInvokeData _184_CMD_WaitRankEffect;
            VirtualInvokeData _185_CMD_StartCommWait;
            VirtualInvokeData _186_CMD_WaitCommWait;
            VirtualInvokeData _187_CMD_ResetCommWaitInfo;
            VirtualInvokeData _188_CMD_ItemAct_Start;
            VirtualInvokeData _189_CMD_ItemAct_Wait;
            VirtualInvokeData _190_CMD_KinomiAct_Start;
            VirtualInvokeData _191_CMD_KinomiAct_Wait;
            VirtualInvokeData _192_CMD_FakeDisable_Start;
            VirtualInvokeData _193_CMD_FakeDisable_Wait;
            VirtualInvokeData _194_CMD_ChangeForm_Start;
            VirtualInvokeData _195_CMD_ChangeForm_Wait;
            VirtualInvokeData _196_CMD_Hensin_Start;
            VirtualInvokeData _197_CMD_Hensin_Wait;
            VirtualInvokeData _198_CMD_ACT_MoveMember_Start;
            VirtualInvokeData _199_CMD_ACT_MoveMember_Wait;
            VirtualInvokeData _200_CMD_ITEMSELECT_Start;
            VirtualInvokeData _201_CMD_ITEMSELECT_Wait;
            VirtualInvokeData _202_CMD_ITEMSELECT_ForceQuit;
            VirtualInvokeData _203_CMD_ITEMSELECT_GetItemID;
            VirtualInvokeData _204_CMD_ITEMSELECT_GetTargetIdx;
            VirtualInvokeData _205_CMD_ITEMSELECT_GetWazaIdx;
            VirtualInvokeData _206_CMD_ITEMSELECT_ReflectUsedItem;
            VirtualInvokeData _207_CMD_YESNO_Start;
            VirtualInvokeData _208_CMD_YESNO_Wait;
            VirtualInvokeData _209_CMD_YESNO_ForceQuit;
            VirtualInvokeData _210_CMD_YESNO_Delete;
            VirtualInvokeData _211_CMD_ExpGet_Start;
            VirtualInvokeData _212_CMD_ExpGet_Wait;
            VirtualInvokeData _213_CMD_RecPlayFadeOut_Start;
            VirtualInvokeData _214_CMD_RecPlayFadeOut_Wait;
            VirtualInvokeData _215_CMD_RecPlayFadeIn_Start;
            VirtualInvokeData _216_CMD_RecPlayFadeIn_Wait;
            VirtualInvokeData _217_CMD_RecDispTurnGauge_Start;
            VirtualInvokeData _218_CMD_RecDispTurnGauge_Wait;
            VirtualInvokeData _219_CMD_Naderu_Start;
            VirtualInvokeData _220_CMD_Naderu_Wait;
            VirtualInvokeData _221_CMD_VsNusiWinEffect_Start;
            VirtualInvokeData _222_CMD_VsNusiWinEffect_Wait;
            VirtualInvokeData _223_CMD_GRightsGet_Start;
            VirtualInvokeData _224_CMD_GRightsGet_Wait;
            VirtualInvokeData _225_CMD_MsgWinHide_Start;
            VirtualInvokeData _226_CMD_MsgWinHide_Wait;
            VirtualInvokeData _227_CMD_ForceQuitInput_Notify;
            VirtualInvokeData _228_CMD_ForceQuitInput_Wait;
            VirtualInvokeData _229_CMD_RecPlayer_Init;
            VirtualInvokeData _230_CMD_CheckRecPlayerInput;
            VirtualInvokeData _231_CMD_UpdateRecPlayerInput;
            VirtualInvokeData _232_CMD_RecPlayer_StartSkip;
            VirtualInvokeData _233_CMD_RecPlayer_StartSkipDisplay;
            VirtualInvokeData _234_CMD_RecPlayer_StartQuit;
            VirtualInvokeData _235_CMD_TrainerIn_Win;
            VirtualInvokeData _236_CMD_TrainerIn_Lose;
            VirtualInvokeData _237_CMD_TrainerIn_Event;
            VirtualInvokeData _238_CMD_WaitTrainerIn;
            VirtualInvokeData _239_CMD_EFFECT_SetGaugeStatus;
            VirtualInvokeData _240_CMD_EFFECT_BallThrow;
            VirtualInvokeData _241_CMD_EFFECT_BallThrowTrainer;
            VirtualInvokeData _242_CMD_EFFECT_WaitBallThrow;
            VirtualInvokeData _243_CMD_EFFECT_WaitBallThrowTrainer;
            VirtualInvokeData _244_CMD_EFFECT_DrawEnableTimer;
            VirtualInvokeData _245_CMD_ChangeWheather;
            VirtualInvokeData _246_CMD_ChangeWheather_Wait;
            VirtualInvokeData _247_CMD_ChangeGround;
            VirtualInvokeData _248_CMD_ChangeGround_Wait;
            VirtualInvokeData _249_CMD_FadeOutBGM;
            VirtualInvokeData _250_CMD_FadeInBGM;
            VirtualInvokeData _251_CMD_CheckFadeOnBGM;
            VirtualInvokeData _252_CMD_StopBGM;
            VirtualInvokeData _253_CMD_PlayWinBGM;
            VirtualInvokeData _254_CMD_PlaySE;
            VirtualInvokeData _255_CMD_IsSEFinished;
            VirtualInvokeData _256_CMD_StopAllSE;
            VirtualInvokeData _257_CMD_StartGMode;
            VirtualInvokeData _258_CMD_StartGMode_Wait;
            VirtualInvokeData _259_CMD_EndGMode;
            VirtualInvokeData _260_CMD_EndGMode_Wait;
            VirtualInvokeData _261_CMD_SummarizedGShockEffect;
            VirtualInvokeData _262_CMD_SummarizedGShockEffect_Wait;
            VirtualInvokeData _263_CMD_Raid_StartCoopCapture;
            VirtualInvokeData _264_CMD_Raid_StartCoopCapture_Wait;
            VirtualInvokeData _265_CMD_Raid_SelectBall;
            VirtualInvokeData _266_CMD_Raid_SelectBall_Wait;
            VirtualInvokeData _267_CMD_Raid_SelectBall_ForceQuit;
            VirtualInvokeData _268_CMD_Raid_Capture;
            VirtualInvokeData _269_CMD_Raid_Capture_Wait;
            VirtualInvokeData _270_CMD_Raid_Capture_vsJoker;
            VirtualInvokeData _271_CMD_Raid_Capture_vsJoker_Wait;
            VirtualInvokeData _272_CMD_Raid_Win;
            VirtualInvokeData _273_CMD_Raid_Win_Wait;
            VirtualInvokeData _274_CMD_Raid_Result;
            VirtualInvokeData _275_CMD_Raid_Result_Wait;
            VirtualInvokeData _276_CMD_Raid_Lose;
            VirtualInvokeData _277_CMD_Raid_Lose_Wait;
            VirtualInvokeData _278_CMD_FinalizeFadeSkip;
            VirtualInvokeData _279_CMD_Tips_G;
            VirtualInvokeData _280_CMD_Tips_G_Wait;
            VirtualInvokeData _281_CMD_DemoCapture_Start;
            VirtualInvokeData _282_CMD_DemoCapture_Wait;
            VirtualInvokeData _283_GetBagMode;
            VirtualInvokeData _284_RaidReward_Start;
            VirtualInvokeData _285_RaidReward_Wait;
            VirtualInvokeData _286_SetUIControlEnableForLiveCup;
            VirtualInvokeData _287_CMD_ACT_Safari_Start;
            VirtualInvokeData _288_CMD_ACT_Safari_Wait;
            VirtualInvokeData _289_OnUpdate;
            VirtualInvokeData _290_OnLateUpdate;
            VirtualInvokeData _291_ResetPokemon;
            VirtualInvokeData _292_GetDefaultPokePos;
            VirtualInvokeData _293_GetPokeModel;
            VirtualInvokeData _294_GetPokeSize;
            VirtualInvokeData _295_ResetTrainer;
            VirtualInvokeData _296_GetTrainerModel;
            VirtualInvokeData _297_GetBallModelPath;
            VirtualInvokeData _298_ResetAll;
            VirtualInvokeData _299_SEQ_CMD_ResetDefaultCamera;
            VirtualInvokeData _300_SeqComFunc_IsFlipEffect;
            VirtualInvokeData _301_SeqComFunc_GetTargetChara;
            VirtualInvokeData _302_SeqComFunc_GetTargetChara;
            VirtualInvokeData _303_SeqComFunc_GetTargetPoke;
            VirtualInvokeData _304_SeqComFunc_GetTargetPokeSub;
            VirtualInvokeData _305_SeqComFunc_GetTargetPoke_Org;
            VirtualInvokeData _306_SeqComFunc_GetPokeRelativePos;
            VirtualInvokeData _307_SeqComFunc_GetSpecialPos;
            VirtualInvokeData _308_SeqComFunc_GetPokeFiledPos;
            VirtualInvokeData _309_SeqComFunc_GetTargetPokeNum;
            VirtualInvokeData _310_SeqComFunc_GetTargetCharaVPos;
            VirtualInvokeData _311_SeqComFunc_CalcPokeDir;
            VirtualInvokeData _312_SeqComFunc_CalcPosDir;
            VirtualInvokeData _313_SeqComFunc_MoveRelativePoke;
            VirtualInvokeData _314_SeqComFunc_MoveSpecialPos;
            VirtualInvokeData _315_SeqComFunc_GetEffectBallInfo;
            VirtualInvokeData _316_SetTerrainChipVisibility;
            VirtualInvokeData _317_StartWeather;
            VirtualInvokeData _318_SetSuspendSequenceFunc;
            VirtualInvokeData _319_GetTaskManager;
            VirtualInvokeData _320_GetTaskManagerLate;
            VirtualInvokeData _321_unknown;
            VirtualInvokeData _322_unknown;
            VirtualInvokeData _323_GetWazaParam;
            VirtualInvokeData _324_SetWazaParam;
            VirtualInvokeData _325_GetBttleWazaModelPath;
            VirtualInvokeData _326_CheckWazaDataPath_Particle;
            VirtualInvokeData _327_GetSequenceSystem;
            VirtualInvokeData _328_GetCharacterSystem;
            VirtualInvokeData _329_GetCameraSystem;
            VirtualInvokeData _330_GetBattleWeatherSystem;
            VirtualInvokeData _331_GetBattleGroundEffectSystem;
            VirtualInvokeData _332_GetSoundPlayingIDHashTable;
            VirtualInvokeData _333_get_IsStencilEnable;
            VirtualInvokeData _334_set_IsStencilEnable;
            VirtualInvokeData _335_get_blurry;
            VirtualInvokeData _336_set_blurry;
            VirtualInvokeData _337_SetIsSoundPlayingFinishCheckInvalid;
        };

        inline bool Virtual_GetBattlePokeParam_forUI(Dpr::Battle::Logic::BTL_POKEPARAM::Object* pDest, int32_t vpos) {
            return (*(this->instance()->klass->vtable)._8_GetBattlePokeParam_forUI.methodPtr)
                (this->instance(), pDest, vpos, this->instance()->klass->vtable._8_GetBattlePokeParam_forUI.method);
        }

        inline Dpr::Battle::Logic::BTL_PARTY::Object* Virtual_GetBattleParty(uint8_t clientID) {
            return (*(this->instance()->klass->vtable)._13_GetBattleParty.methodPtr)
                    (this->instance(), clientID, this->instance()->klass->vtable._13_GetBattleParty.method);
        }

        inline bool Virtual_IsClientTrainerExist(uint8_t clientID) {
            return (*(this->instance()->klass->vtable)._26_IsClientTrainerExist.methodPtr)
                    (this->instance(), clientID, this->instance()->klass->vtable._26_IsClientTrainerExist.method);
        }

        inline uint8_t Virtual_GetBtlvPosToClientId(int32_t vpos) {
            return (*(this->instance()->klass->vtable)._34_GetBtlvPosToClientId.methodPtr)
                (this->instance(), vpos, this->instance()->klass->vtable._34_GetBtlvPosToClientId.method);
        }
    };
}

