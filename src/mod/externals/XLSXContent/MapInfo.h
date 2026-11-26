#pragma once

#include "externals/il2cpp-api.h"

#include "externals/EnvironmentSettings.h"
#include "externals/System/Primitives.h"
#include "externals/System/String.h"
#include "externals/UnityEngine/ScriptableObject.h"
#include "externals/UnityEngine/Vector2.h"
#include "externals/UnityEngine/Vector4.h"
#include "externals/XLSXContent/AttributeMatrix.h"
#include "externals/XLSXContent/ZoneTable.h"

namespace XLSXContent {
    struct MapInfo : ILClass<MapInfo> {
        struct SheetZoneData : ILClass<SheetZoneData> {
            struct Fields {
                System::String::Object* Caption;
                System::String::Object* MSLabel;
                System::String::Object* PokePlaceName;
                System::String::Object* FlyingPlaceName;
                int32_t MapType;
                bool IsField;
                int32_t LandmarkType;
                UnityEngine::Vector2::Object MiniMapOffset;
                bool Bicycle;
                bool Escape;
                bool Fly;
                bool BattleSearcher;
                bool TureAruki;
                bool KuruKuru;
                bool Fall;
                System::Int32_array* BattleBg;
                int32_t ZoneID;
                int32_t AreaID;
                XLSXContent::ZoneTable::Object* ZoneGrid;
                XLSXContent::AttributeMatrix::Object* Attribute;
                XLSXContent::AttributeMatrix::Object* AttributeEx;
                XLSXContent::AttributeMatrix::Object* SubAttribute;
                XLSXContent::AttributeMatrix::Object* SubAttributeEx;
                System::String::Array* BGM;
                System::String::Object* EnvironmentalSound;
                int32_t Weather;
                EnvironmentSettings::Object* RenderSettings;
                bool ReflectionCamera;
                bool FixedTime;
                System::String::Object* AssetBundleName;
                bool RoomPanCamera;
                UnityEngine::Vector4::Array* Locators;
                bool Reload;
            };
        };

        struct SheetCamera : ILClass<SheetCamera> {
            struct Fields {
                int32_t ariaID;
                float pitch;
                float fov;
                float targetRange;
                float panDistance;
                float panPitch;
                float panFov;
                bool panpos_useflag;
                float panMinposY;
                float panMaxposY;
                float panMinposZ;
                float panMaxposZ;
                float defocusStart;
                float defocusEnd;
                float distance;
            };
        };

        struct Fields : UnityEngine::ScriptableObject::Fields {
            XLSXContent::MapInfo::SheetZoneData::Array* ZoneData;
	        XLSXContent::MapInfo::SheetCamera::Array* Camera;
        };

        inline XLSXContent::MapInfo::SheetZoneData::Object* get_Item(int32_t index) {
            return external<XLSXContent::MapInfo::SheetZoneData::Object*>(game::Address::XLSXContent_MapInfo_get_Item, this, index);
        }
    };
}
