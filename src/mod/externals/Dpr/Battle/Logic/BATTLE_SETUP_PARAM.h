#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/BallDeco/CapsuleData.h"
#include "externals/Dpr/Battle/Logic/BtlCompetitor.h"
#include "externals/Dpr/Battle/Logic/BtlRule.h"
#include "externals/Dpr/Battle/Logic/MyStatus.h"
#include "externals/Dpr/Battle/Logic/PokeResult.h"
#include "externals/Pml/PokePara/EvolveSituation.h"
#include "externals/Pml/PokeParty.h"
#include "externals/System/Primitives.h"
#include "externals/Dpr/Battle/Logic/BattleEffectComponentData.h"

namespace Dpr::Battle::Logic {
    struct BATTLE_SETUP_PARAM : ILClass<BATTLE_SETUP_PARAM> {
        struct Fields {
            BtlCompetitor competitor;
            BtlRule rule;
            void* raidBattleParam; // Dpr_Battle_Logic_RaidBattleParam_o*
            void* fieldSituation; // Dpr_Battle_Logic_BTL_FIELD_SITUATION_o*
            BattleEffectComponentData::Object* btlEffComponent; // Dpr_Battle_Logic_BattleEffectComponentData_o*
            Pml::PokePara::EvolveSituation::Object* evolveSituation;
            uint8_t commMode;
            uint8_t multiMode;
            uint8_t commPos;
            uint8_t recordDataMode;
            void* miseaiData; // Dpr_Battle_Logic_MiseaiData_array*
            System::Int32_array* stations;
            bool isPlayerRatingDisplay;
            bool isLiveRecSendEnable;
            Pml::PokeParty::Array* party;
            void* partyDesc; // Dpr_Battle_Logic_PartyDesc_array*
            Dpr::Battle::Logic::MyStatus::Array* playerStatus;
            System::UInt16_array* playerRating;
            void* ballDecoDesc; // Dpr_BallDeco_CapsuleData_array_array*
            void* tr_data; // Dpr_Battle_Logic_BSP_TRAINER_DATA_array*
            bool bGakusyuSouti;
            uint16_t LimitTimeGame;
            uint16_t LimitTimeClient;
            uint16_t LimitTimeCommand;
            bool bEnableTimeStop;
            void* shooterBitWork; // Dpr_Battle_Logic_SHOOTER_ITEM_BIT_WORK_o*
            bool reduceHighLevelCaptureRate;
            uint8_t maxFollowPokeLevel;
            uint8_t captureLevelCap;
            uint8_t expLevelCap;
            uint16_t commNetIDBit;
            uint32_t btl_status_flag;
            uint32_t wildPokeAiBitFlag;
            float moneyRate;
            uint8_t forceQuitTurnCount;
            bool bSkyBattle;
            bool bSakasaBattle;
            bool bMustCapture;
            bool bNoGainBattle;
            int32_t safariBallNum;
            bool bVoiceChat;
            void* conventionInfo; // Dpr_Battle_Logic_BATTLE_CONVENTION_INFO_array*
            int32_t getMoney;
            int32_t result;
            uint8_t fairyGymResult;
            bool honooGymResult_wonByPlayer;
            uint8_t capturedPokeIdx;
            uint8_t capturedClientID;
            uint8_t commServerVer;
            int32_t commError;
            bool isDisplayedCommError;
            uint8_t* recBuffer;
            uint32_t recDataSize;
            uint64_t recRandSeed;
            void* recHeader; // Dpr_Battle_Logic_BattleRecordHeader_o*
            void* kenteiResult; // Dpr_Battle_Logic_BATTLE_KENTEI_RESULT_o*
            bool cmdIllegalFlag;
            bool recPlayCompleteFlag;
            bool WifiBadNameFlag;
            bool isDisconnectOccur;
            bool isWatchMember;
            System::Boolean_array* fightPokeIndex;
            System::Boolean_array* turnedLvUpPokeIndex;
            Dpr::Battle::Logic::PokeResult::Array* pokeResult;
            void* party_state; // System_Byte_array_array*
            System::UInt32_array* restHPRatio;
            void* PGL_Record; // Dpr_Battle_Logic_BATTLE_PGL_RECORD_SET_o*
            void* tvNaviData; // Dpr_Battle_Logic_BATTLE_TVNAVI_DATA_o*
            void* pokeMemoryResult; // Dpr_Battle_Logic_PokeMemoryResult_o*
            int32_t _throwBallNum_k__BackingField;
        };

        inline void Clear() {
            external<void>(0x01ac3ac0, this);
        }
    };
}
