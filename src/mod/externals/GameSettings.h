#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/ScriptableObject.h"
#include "externals/XLSXContent/ArenaInfo.h"
#include "externals/XLSXContent/CalenderEncTable.h"
#include "externals/XLSXContent/FieldEncountTable.h"
#include "externals/XLSXContent/MapAttributeExTable.h"
#include "externals/XLSXContent/MapAttributeTable.h"
#include "externals/XLSXContent/MapInfo.h"
#include "externals/XLSXContent/WaterSettings.h"

struct GameSettings : ILClass<GameSettings, game::Address::GameSettings_TypeInfo> {
    struct Fields : UnityEngine::ScriptableObject::Fields {
        XLSXContent::MapInfo::Object* mapInfo;
        XLSXContent::ArenaInfo::Object* arenaInfo;
        XLSXContent::MapAttributeTable::Object* mapAttributeTable;
        XLSXContent::MapAttributeExTable::Object* mapAttributeExTable;
        XLSXContent::CalenderEncTable::Object* calenderEncTable;
        XLSXContent::FieldEncountTable::Object* fieldEncountTable_d;
        XLSXContent::FieldEncountTable::Object* fieldEncountTable_p;
        XLSXContent::WaterSettings::Object* waterSettings;
    };
};
