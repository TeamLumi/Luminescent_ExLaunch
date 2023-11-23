#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/BallDeco/CapsuleData.h"
#include "externals/Dpr/Battle/Logic/BATTLE_SETUP_PARAM.h"
#include "externals/Dpr/Battle/Logic/BattleEnv.h"
#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "externals/Dpr/Battle/Logic/BtlEscapeMode.h"
#include "externals/Dpr/Battle/Logic/BtlRule.h"
#include "externals/Dpr/Battle/Logic/MyStatus.h"
#include "externals/Pml/PokeParty.h"
#include "externals/System/Primitives.h"

namespace Dpr::Battle::Logic {
    struct MainModule : ILClass<MainModule> {
        struct Fields {
            Dpr::Battle::Logic::BATTLE_SETUP_PARAM::Object* m_setupParam;
            void* m_viewCore;
            void* m_randSys;
            uint64_t m_randomSeed;
            void* m_subProc;
            int32_t m_subSeq;
            int32_t m_setupStep;
            void* m_mainLoop;
            Pml::PokeParty::Array* m_srcParty;
            Pml::PokeParty::Array* m_srcPartyForServer;
            Pml::PokeParty::Object* m_tmpParty;
            Dpr::Battle::Logic::MyStatus::Object* m_playerStatus;
            System::Boolean_array* m_fClientQuit;
            BtlRule m_rule;
            uint32_t m_regularMoney;
            uint32_t m_bonusMoney;
            uint32_t m_loseMoney;
            int32_t m_msgSpeed;
            uint16_t m_LimitTimeGame;
            uint16_t m_LimitTimeClient;
            uint16_t m_LimitTimeCommand;
            int32_t m_serverResult;
            int32_t m_serverResultCause;
            uint8_t m_myClientID;
            uint8_t m_myOrgPos;
            int32_t m_changeMode;
            uint8_t m_MultiAIDataSeq;
            uint8_t m_MultiAIClientNum;
            System::Byte_array* m_MultiAIClientID;
            bool m_fCommError;
            bool m_fCommErrorMainQuit;
            bool m_fWazaEffectEnable;
            bool m_fGetMoneyFixed;
            bool m_fLoseMoneyFixed;
            bool m_padding;
            bool m_isFinalizeStarted;
            void* m_server;
            void* m_cmdCheckServer;
            void* m_client;
            void* m_dummyClient;
            void* m_trainerParam;
            void* m_clientPublicInformation;
            void* m_adapterFactory;
            void* m_recReader;
            System::Boolean_array* m_moneyDblUpCause;
            void* m_zukanDataOnBattleStart;
            void* m_deadPokeIDRec;
            Dpr::Battle::Logic::BattleEnv::Object* m_battleEnvForClient;
            Dpr::Battle::Logic::BattleEnv::Object* m_battleEnvForServer;
            void* m_iPtrNetClient;
            bool isInitialized;
            bool isFinalized;
            Dpr::BallDeco::CapsuleData::Object DummyCapsuleData;
        };

        inline int32_t GetCompetitor(bool isDemoCaptureConvert) {
            return external<int32_t>(0x0202de60, this, isDemoCaptureConvert);
        }

        inline uint8_t GetExpLevelCap() {
            return external<uint8_t>(0x02034a10, this);
        }

        inline uint8_t GetPokeFriendship(Dpr::Battle::Logic::BTL_POKEPARAM* bpp) {
            return external<uint8_t>(0x0202c3d0, this, bpp);
        }

        inline Pml::PokeParty::Object* GetSrcParty(uint8_t clientID, bool fForServer) {
            return external<Pml::PokeParty::Object*>(0x020325a0, this, clientID, fForServer);
        }

        inline bool IsFriendPokeID(uint8_t pokeID1, uint8_t pokeID2) {
            return external<bool>(0x02037470, this, pokeID1, pokeID2);
        }

        inline BtlEscapeMode GetEscapeMode() {
            return external<BtlEscapeMode>(0x02034bb0, this);
        }

        inline BtlGround GetDefaultGround() {
            return external<BtlGround>(0x02035090, this);
        }
    };
}
