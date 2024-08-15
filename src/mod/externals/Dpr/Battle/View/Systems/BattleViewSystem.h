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

        inline void PlaySequenceEffect(int32_t no, bool isKeepResource) {
            external<void>(0x01c7ea40, this, no, isKeepResource);
        }

        inline void PlaySequenceCore(System::String::Object* path, bool keepResource) {
            external<void>(0x01c85970, this, path, keepResource);
        }
    };
}

