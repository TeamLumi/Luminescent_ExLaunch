#pragma once

#include "externals/il2cpp-api.h"
#include "externals/System/String.h"
#include "externals/UnityEngine/ScriptableObject.h"

namespace XLSXContent {
    struct ContestMasterDatas : ILClass<ContestMasterDatas> {
        struct SheetNPCData : ILClass<SheetNPCData> {
            struct Fields {
                bool valid_flag;
                uint16_t id;
                uint16_t monsNo;
                System::String::Object* monsLabelName;
                System::String::Object* trainerLabelName;
                int32_t trainerType;
                int32_t npcLevel;
                bool entryStyleContest;
                bool entryBeautifulContest;
                bool entryCuteContest;
                bool entryCleverContest;
                bool entryStrongContest;
                bool entryTutorial;
                bool entryDPContest;
                int32_t entryRank;
                uint16_t waza1;
                uint16_t waza2;
                uint16_t waza3;
                uint16_t waza4;
                System::Int32_array* probabilityArray;
                int32_t styleValue;
                int32_t beautifulValue;
                int32_t cuteValue;
                int32_t cleverValue;
                int32_t strongValue;
                int32_t furValue;
                int32_t useCapsuleNo;
                int32_t styleVisualScore;
                int32_t beautifulVisualScore;
                int32_t cuteVisualScore;
                int32_t cleverVisualScore;
                int32_t strongVisualScore;
                int32_t tutorialVisualScore;
                int32_t furVisualScore;
            };
        };

        struct SheetNPCLevelData : ILClass<SheetNPCLevelData> {
            struct Fields {
                bool valid_flag;
                uint16_t id;
                int32_t npcLevel;
                float visualScoreFactor;
                float tapTimingRandMin;
                float tapTimingRandMax;
                int32_t densityOrder;
                int32_t UseComboPersent;
            };
        };

        struct Fields : UnityEngine::ScriptableObject::Fields {
            void* ContestData; // XLSXContent_ContestMasterDatas_SheetContestData_array*
            void* BgmData; // XLSXContent_ContestMasterDatas_SheetBgmData_array*
            void* RewardData; // XLSXContent_ContestMasterDatas_SheetRewardData_array*
            void* RewardItemData; // XLSXContent_ContestMasterDatas_SheetRewardItemData_array*
            void* RankData; // XLSXContent_ContestMasterDatas_SheetRankData_array*
            void* ConditionScoreData; // XLSXContent_ContestMasterDatas_SheetConditionScoreData_array*
            void* FurScoreData; // XLSXContent_ContestMasterDatas_SheetFurScoreData_array*
            void* SealData; // XLSXContent_ContestMasterDatas_SheetSealData_array*
            void* ItemData; // XLSXContent_ContestMasterDatas_SheetItemData_array*
            SheetNPCData::Array* NPCData;
            SheetNPCLevelData::Array* NPCLevelData;
            void* NPCCapsuleData; // XLSXContent_ContestMasterDatas_SheetNPCCapsuleData_array*
        };
    };
}