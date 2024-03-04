#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Dpr/Battle/View/BtlvBallInfo.h"
#include "externals/System/String.h"

namespace Dpr::Contest {
    struct EntryPlayerData : ILClass<EntryPlayerData> {
        struct Fields {
            Dpr::Battle::View::BtlvBallInfo::Object ballInfo;
            System::String::Object* playerName;
            System::String::Object* pokemonNickName;
            System::String::Object* trainerModelPath;
            uint16_t fashion;
            int32_t playerType;
            int32_t wazaNo;
            int32_t monsNo;
            uint8_t playerSex;
            uint8_t monsSex;
            uint8_t rareType;
            uint16_t itemNo;
            int32_t userLangID;
            int32_t monsLangID;
            float wazaSeqTime;
            uint32_t formNo;
            int32_t cassetVersion;
            int32_t colorID;
            int32_t npcDataIndex;
            int32_t npcWazaNo;
            uint8_t style;
            uint8_t beautiful;
            uint8_t cute;
            uint8_t clever;
            uint8_t strong;
            uint8_t fur;
            bool isRare;
            bool isDpClear;
        };
    };
}
