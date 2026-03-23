#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Pml/PokePara/PokemonParam.h"

struct TradeSelectPokeModel : ILClass<TradeSelectPokeModel> {
    enum class TradeSelectState : int32_t {
        NONE = 0,
        WAIT = 1,
        COMPLETE = 2,
        CANCAL_MINE = 3,
        CANCAL_OTHER = 4,
        ERROR_MINE = 5,
        ERROR_OTHER = 6,
    };

    struct Fields {
        void* _SendPokeParam;               // 0x10 Action<PokemonParam>
        void* _SetTargetSelectPokeData;     // 0x18 Action<PokemonParam>
        void* _SetBoxData;                  // 0x20 Action<BoxPokeData>
        void* _Cancel;                      // 0x28 Action
        void* _Error;                       // 0x30 Action
        void* _OpenErrorDialog;             // 0x38 Action
        void* _LeaveUnion;                  // 0x40 Action
        int32_t tradeTargetIndex;           // 0x48
        void* boxWindow;                    // 0x50 BoxWindow*
        void* selectPokemon;               // 0x58 BoxWindow.SelectedPokemon*
        int32_t nextTradePhase;            // 0x60 BoxWindow.NetTradePhase
        Pml::PokePara::PokemonParam::Object* targetPokemonParam;     // 0x68
        Pml::PokePara::PokemonParam::Object* targetDemoPokemonParam; // 0x70
        bool isRecivePokeParam;            // 0x78
        bool isSendPokeParam;             // 0x79
        bool isTrading;                    // 0x7A
        bool isWaitingOK;                 // 0x7B
        bool isWaitingSelect;             // 0x7C
        int32_t myTradeState;             // 0x80 TradeStateModel.TradeState
        int32_t targetTradeState;         // 0x84 TradeStateModel.TradeState
        TradeSelectState currentState;     // 0x88
        void* targetTranerParam;          // 0x90 UnionTradeManager.TargetTranerParam*
    };
};
