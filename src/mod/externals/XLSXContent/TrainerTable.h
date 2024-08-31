#pragma once
#include "externals/il2cpp-api.h"
#include "externals/UnityEngine/ScriptableObject.h"

struct TrainerTable : ILClass<TrainerTable> {
    struct SheetTrainerData : ILClass<SheetTrainerData> {
        struct Fields {
            int32_t TypeID;
            uint8_t ColorID;
            uint8_t FightType;
            int32_t ArenaID;
            int32_t EffectID;
            uint8_t Gold;
            uint16_t UseItem1;
            uint16_t UseItem2;
            uint16_t UseItem3;
            uint16_t UseItem4;
            bool HpRecoverFlag;
            uint16_t GiftItem;
            System::String::Object* NameLabel;
            System::String::Object* MsgFieldPokeOne;
            System::String::Object* MsgFieldBefore;
            System::String::Object* MsgFieldRevenge;
            System::String::Object* MsgFieldAfter;
            System::String::Array* MsgBattle;
            System::String::Array* SeqBattle;
            uint32_t AIBit;
        };
    };

    struct Fields : UnityEngine::ScriptableObject::Fields {
        void* TrainerType;
        SheetTrainerData::Array* TrainerData;
        void* TrainerPoke;
        void* TrainerRematch;
        void* SealTemplate;
        void* SkirtGraphicsChara;
    };

};
