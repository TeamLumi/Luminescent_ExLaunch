#pragma once

#include "externals/il2cpp-api.h"
#include "externals/UnityEngine/ScriptableObject.h"
#include "externals/UnityEngine/Vector2.h"

namespace XLSXContent {
    struct TownMapTable : ILClass<TownMapTable> {
        struct Sheetdata : ILClass<Sheetdata> {
            struct Fields {
                int32_t zoneID;
                int32_t Width;
                int32_t Index1;
                int32_t Index2;
                int32_t Index3;
                UnityEngine::Vector2::Object NowPosXZ;
                UnityEngine::Vector2::Object TownPosXZ;
                UnityEngine::Vector2::Object MarkingMapPosXZ;
                System::String::Object* GuideText1;
                System::String::Object* GuideText2;
                System::String::Object* TownMapName;
                int32_t ViewFlag;
                int32_t ColorOnFlag;
                System::String::Object* SymbolName;
                int32_t AddGuideFlag;
                int32_t ChangFacilityFlag;
                int32_t ChangFacilityNo;
                System::String::Object* Facility00;
                System::String::Object* Facility01;
                System::String::Object* Facility02;
                System::String::Object* Facility03;
                System::String::Object* Facility04;
                System::String::Object* Facility05;
                System::String::Object* Facility06;
                System::String::Object* FacilitySP;
                bool FlyingAvailablePlace;
                int32_t MapInfoLocatorIndex;
                bool AreaHideFlag;
            };
        };

        struct Fields : UnityEngine::ScriptableObject::Fields {
            XLSXContent::TownMapTable::Sheetdata::Array* data;
        };
    };
}
