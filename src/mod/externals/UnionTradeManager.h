#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/System/Action.h"
#include "externals/System/String.h"
#include "externals/Dpr/Message/MessageEnumData.h"

struct UnionTradeManager : ILClass<UnionTradeManager, 0x04c658e8> {

    enum TradeFlowState : int32_t {
        NONE = 0,
        SELECT_WINDOW = 1,
        SECURIY_TRADE = 2,
        PLAY_DEMO = 3,
        END = 4,
    };

    struct BoxPokeData {
        Pml::PokePara::PokemonParam::Object* pokeParam;  // 0x0
        bool isTeam;                                       // 0x8
        int32_t teamIndex;                                 // 0xC
        int32_t teamPos;                                   // 0x10
    };

    struct TargetTranerParam : ILClass<TargetTranerParam, 0x04c659f0> {
        struct Fields {
            uint32_t tranerId;                                          // 0x10
            int32_t cassetVersion;                                      // 0x14
            Dpr::Message::MessageEnumData::MsgLangId langId;            // 0x18
            System::String::Object* tranerName;                         // 0x20
        };

        inline void ctor() {
            external<void>(0x17E44A0, this);
        }
    };

    struct Fields {
        void* myPlayer;                                    // 0x10  OnlinePlayerCharacter
        void* securityController;                          // 0x18  TradeSecurityController
        int32_t tradeTargetIndex;                          // 0x20
        int32_t _pad_24;                                   // 0x24  padding
        void* tradeSelectModel;                            // 0x28  TradeSelectPokeModel
        BoxPokeData boxMyPokeData;                         // 0x30  (value type, 0x18 bytes)
        Pml::PokePara::PokemonParam::Object* targetPokemonParam; // 0x48
        TargetTranerParam::Object* targetTranerParam;      // 0x50
        Pml::PokePara::PokemonParam::Object* selectMyPokemonParam; // 0x58
        bool isRecruiment;                                 // 0x60
        uint8_t _pad_61[7];                                // 0x61  padding
        void* msgWindow;                                   // 0x68  TradeFlowMsgWindow
        bool isError;                                      // 0x70
        bool isLoadingBox;                                 // 0x71
        uint8_t _pad_72[6];                                // 0x72  padding
        System::Action::Object* _LeaveUnion;               // 0x78
        TradeFlowState currentState;                       // 0x80
    };

    static_assert(offsetof(Fields, currentState) == 0x70);

    // Properties
    inline int32_t get_tradeTargetIndex() {
        return external<int32_t>(0x17E40B0, this);
    }
    inline void set_tradeTargetIndex(int32_t value) {
        external<void>(0x17E40C0, this, value);
    }
    inline bool get_isLoadingBox() {
        return external<bool>(0x17E40D0, this);
    }
    inline void set_isLoadingBox(bool value) {
        external<void>(0x17E40E0, this, value);
    }

    // Methods
    inline void Init() {
        external<void>(0x17E40F0, this);
    }
    inline void SetTargetTranerParam(uint32_t id, System::String::Object* name,
                                      int32_t cassetVersion,
                                      Dpr::Message::MessageEnumData::MsgLangId langId) {
        external<void>(0x17E43D0, this, id, name, cassetVersion, langId);
    }
    inline void SetIsError(bool error) {
        external<void>(0x17E44B0, this, error);
    }
    inline bool GetIsError() {
        return external<bool>(0x17E44C0, this);
    }
    inline void CreateTradeSelectModel(System::Action::Object* reStartFunc,
                                        System::Action::Object* networkErrorFunc,
                                        System::Action::Object* leaveFunc) {
        external<void>(0x17E44E0, this, reStartFunc, networkErrorFunc, leaveFunc);
    }
    inline void SetBoxData(BoxPokeData boxData) {
        external<void>(0x17E46B0, this, boxData);
    }
    inline BoxPokeData GetBoxPokeData() {
        return external<BoxPokeData>(0x17E46D0, this);
    }
    inline void Clear() {
        external<void>(0x17E46F0, this);
    }
    inline void Cancel() {
        external<void>(0x17E4820, this);
    }
    inline void InitPlayerState() {
        external<void>(0x17E49A0, this);
    }
    inline void Error() {
        external<void>(0x17E4C30, this);
    }
    inline void* GetTradeSlectPokeModel() {
        return external<void*>(0x17E4DD0, this);
    }
    inline TradeFlowState GetCurrentState() {
        return external<TradeFlowState>(0x17E4DE0, this);
    }
    inline void NextState() {
        external<void>(0x17E4DF0, this);
    }
    inline void SetCurrentState(TradeFlowState state) {
        external<void>(0x17E4E00, this, state);
    }
    inline void SetTargetIndex(int32_t index) {
        external<void>(0x17E4E10, this, index);
    }
    inline void SendPokeParam(Pml::PokePara::PokemonParam::Object* param) {
        external<void>(0x17E4E60, this, param);
    }
    inline void RecivePokeData(Pml::PokePara::PokemonParam::Object* param) {
        external<void>(0x17E4F20, this, param);
    }
    inline void SetTargetTradePoke(Pml::PokePara::PokemonParam::Object* param) {
        external<void>(0x17E4FD0, this, param);
    }
    inline void StartOpenTradeBoxWindow(int32_t index, bool isFirst) {
        external<void>(0x17E5840, this, index, isFirst);
    }
    inline void OnFinishedTradeInternal() {
        external<void>(0x17E5870, this);
    }
    inline void SecurtyTradeClear() {
        external<void>(0x17E6020, this);
    }
    inline void ctor() {
        external<void>(0x17E6070, this);
    }
};
