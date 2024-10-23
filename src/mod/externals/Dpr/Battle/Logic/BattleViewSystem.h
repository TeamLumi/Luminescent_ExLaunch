#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BTL_CLIENT.h"
#include "externals/Dpr/Battle/Logic/MainModule.h"
#include "externals/Dpr/Battle/Logic/BattleViewBase.h"
#include "externals/Dpr/Battle/View/Systems/BattleCharacterSystem.h"
#include "externals/Dpr/Battle/View/Systems/BattleCameraSystem.h"
#include "externals/Dpr/Battle/View/Systems/BTLV_WAZA_EFF_PARAM.h"
#include "externals/System/String.h"
#include "externals/Dpr/Battle/View/Systems/TrainerTalkParam.h"

namespace Dpr::Battle::Logic {
    struct BattleViewSystem : ILClass<BattleViewSystem> {
        struct Fields : Dpr::Battle::Logic::BattleViewBase::Fields {
            bool m_isFinalize;
            bool m_isFinalizeFadeSkip;
            bool m_isFinishLoadObjects;
            void* m_iPtrSequenceSystem; // Dpr_Battle_View_Systems_BattleSequenceSystem_o*
            Dpr::Battle::View::Systems::BattleCharacterSystem::Object* m_iPtrCharacterSystem;
            Dpr::Battle::View::Systems::BattleCameraSystem::Object* m_iPtrCameraSystem;
            void* m_iPtrWeatherSystem; // Dpr_Battle_View_Systems_BattleWeatherSystem_o*
            void* m_iPtrBattleGroundEffectSystem; // Dpr_Battle_View_Systems_BattleGroundEffectSystem_o*
            void* m_iPtrStatusEffectObserverSystem; // Dpr_Battle_View_Systems_BattleStatusEffectObserverSystem_o*
            void* m_battleDataTable; // Dpr_Battle_Logic_BattleDataTable_o*
            void* m_defaultPlacementData; // Dpr_Battle_Logic_BattleDefaultPlacementData_o*
            void* m_iPtrScreenObject; // Dpr_Battle_View_Systems_BattleScreenObject_o*
            void* m_skrtModelDict; // System_Collections_Generic_Dictionary_BtlvPos_string_o*
            Dpr::Battle::View::Systems::BTLV_WAZA_EFF_PARAM::Object* m_wazaParam;
            void* m_comTargetPoke; // BtlvPos[]
            void* m_comTargetDamage; // BtlvPos[]
            void* m_comTargetTrainer; // BtlvPos[]
            void* m_comEffectId; // BtlEff_o*
            void* m_effectBallId; // BallId[]
            void* m_effectBallInfo; // BtlvBallInfo[]
            int32_t m_comIsVisibleTame;
            bool m_comIsMigawari;
            bool m_comMigawariReserve;
            void* m_comMigawariReserveTarget; // BtlvPos_o*
            int32_t m_comRepeatNum;
            bool m_comIsSuccsess;
            bool m_comIsCritical;
            int32_t m_subSequence;
            void* m_sequenceSeq; // Dpr_Battle_Logic_BattleViewSystem_SequenceSeq_o*
            bool m_seqKeepResource;
            void* m_playingSeqFileNameHash; // System_String_o*
            void* m_pComWaitFunc; // System_Func_bool_o*
            bool m_isStartMsgDisplay;
            int32_t m_wazaMsgFrame;
            void* m_beforePlayEffPos; // BtlvPos_o*
            void* m_beforePlayEffNo; // BtlEff_o*
            bool m_isFieldIndoor;
            bool m_isFieldStadium;
            bool m_isFieldRoseTower;
            bool m_isFieldStadiumWide;
            bool m_isFieldWaitcamExcept;
            bool m_reqCheckPinch;
            bool m_canChangePinch;
            bool m_isSoundPlayingFinishCheckInvalid;
            int32_t m_soundPlayingFinishWaitCount;
            void* m_pokePinchSound; // BtlvSound_o*
            bool m_preUpdateAudioListenerByMainCamera;
            void* m_deadActParam; // DeadActParam_o
            void* m_memberChangeActParam; // MemberChangeActParam_o
            void* m_memberOutActParam; // MemberOutActParam_o
            void* m_startGActParam; // StartGActParam_o
            void* m_endGActParam; // EndGActParam_o
            Dpr::Battle::View::Systems::TrainerTalkParam::Object m_trainerTalkParam;
            void* m_attrEffParam; // BTLV_ATTR_EFF_PARAM_o*
            System::String::Object* m_attrSoundEvent;
            void* m_iPtrTaskManager; // TaskManager_o*
            void* m_iPtrTaskManagerLate; // TaskManager_o*
            void* m_uPtrSoundPlayingIDHash; // System_Collections_Generic_Stack_Tuple_int_uint_uint_o*
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

        static_assert(offsetof(Fields, _IsApplicationPause_k__BackingField) == 0x1b4);
    };
}

