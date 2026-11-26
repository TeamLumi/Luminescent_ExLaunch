#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/ActionSharedData.h"
#include "externals/Dpr/Battle/Logic/BtlCompetitor.h"
#include "externals/Dpr/Battle/Logic/BtlRule.h"
#include "externals/Dpr/Battle/Logic/PokeAction.h"
#include "externals/Dpr/Battle/Logic/BTL_PARTY.h"

namespace Dpr::Battle::Logic {
    // To avoid cyclical definitions. Make sure to use ->instance() on these fields.
    struct BattleEnv;
    struct EventLauncher;
    struct MainModule;
    struct PokeActionContainer;
    struct SectionContainer;
    struct SectionSharedData;
    struct ServerCommandPutter;

    struct Section : ILClass<Section> {
        struct Fields {
            MainModule* m_pMainModule;
            BattleEnv* m_pBattleEnv;
            void* m_pServerCmdQueue; // Dpr::Battle::Logic::ServerCommandQueue::Object*
            ServerCommandPutter* m_pServerCmdPutter;
            void* m_pWazaCmdPutter; // Dpr::Battle::Logic::WazaCommandPutter::Object*
            void* m_pEventSystem; // Dpr::Battle::Logic::EventSystem::Object*
            EventLauncher* m_pEventLauncher;
            SectionSharedData* m_pSharedData;
            PokeActionContainer* m_pPokemonActionContainer;
            void* m_pPokeChangeRequest; // Dpr::Battle::Logic::PokeChangeRequest::Object*
            void* m_pCaptureInfo; // Dpr::Battle::Logic::CaptureInfo::Object*
            SectionContainer* m_pSectionContainer;
        };

        inline BtlRule GetRule() {
            return external<BtlRule>(game::Address::Dpr_Battle_Logic_Section_GetRule, this);
        }

        inline BtlCompetitor GetCompetitor() {
            return external<BtlCompetitor>(game::Address::Dpr_Battle_Logic_Section_GetCompetitor, this);
        }

        inline int32_t CheckEncoreWazaChange(PokeAction::Object* action) {
            return external<int32_t>(game::Address::Dpr_Battle_Logic_Section_CheckEncoreWazaChange, this, action);
        }

        inline uint8_t GetPokePos(uint8_t pokeID) {
            return external<uint8_t>(game::Address::Dpr_Battle_Logic_Section_GetPokePos, this, pokeID);
        }

        inline ActionSharedData::Object* GetActionSharedData() {
            return external<ActionSharedData::Object*>(game::Address::Dpr_Battle_Logic_Section_GetActionSharedData, this);
        }

        inline BTL_POKEPARAM::Object* GetPokeParam(uint8_t pokeID) {
            return external<BTL_POKEPARAM::Object*>(game::Address::Dpr_Battle_Logic_Section_GetPokeParam, this, pokeID);
        }

        inline BTL_PARTY::Object* GetPokeParty(uint8_t clientID) {
            return external<BTL_PARTY::Object*>(game::Address::Dpr_Battle_Logic_Section_GetPokeParty, this, clientID);
        }
    };
}
