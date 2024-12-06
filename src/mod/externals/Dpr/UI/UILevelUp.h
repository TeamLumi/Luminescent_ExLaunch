#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/UI/UIMsgWindowController.h"
#include "externals/Dpr/UI/UIWindow.h"
#include "externals/Pml/PokeParty.h"
#include "externals/System/Primitives.h"

namespace Dpr::UI {
    struct LevelUpPokemonPanel;

    struct UILevelUp : ILClass<UILevelUp> {
        struct Param : ILStruct<Param> {
            struct Fields {
                System::UInt32_array* AddExpValues;
                int32_t TargetIndex;
                int32_t LevelUpCount;
                Pml::PokeParty::Object* PokeParty;
                void* BattleExpGetResult;
            };
        };

        struct PokemonStatus : ILStruct<PokemonStatus> {
            struct Fields {
                uint32_t Level;
                uint32_t Exp;
                uint32_t Hp;
                uint32_t Attack;
                uint32_t Deffence;
                uint32_t SpecialAttack;
                uint32_t SpecialDeffence;
                uint32_t Agility;
            };
        };

        struct Fields : Dpr::UI::UIWindow::Fields {
            int32_t _animStateIn;
            int32_t _animStateOut;
            void* levelUpPokemonPanels; // This causes a circular definition, since it has to be an array
            void* statusPanel;
            Param::Object param;
            UIMsgWindowController::Object* msgWindowController;
            bool isAnimateGauge;
            bool isPlayGaugeUpSe;
            bool isPlayLevelUpSe;
            bool isWaitExit;
        };

        inline void Set(Pml::PokePara::PokemonParam::Object* param, int32_t index) {
            external<void>(0x01c5a4f0, this, param, index);
        }
    };
}