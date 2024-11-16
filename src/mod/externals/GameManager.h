#pragma once

#include "externals/il2cpp-api.h"

#include "externals/GameSettings.h"
#include "externals/PeriodOfDay.h"
#include "externals/ScriptableObjectFormat/PokemonData.h"
#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"
#include "externals/System/String.h"
#include "externals/TMPro/TextMeshProUGUI.h"
#include "externals/UnityEngine/Coroutine.h"
#include "externals/UnityEngine/GameObject.h"
#include "externals/UnityEngine/Transform.h"
#include "externals/UnityEngine/Vector2Int.h"
#include "externals/XLSXContent/FieldEncountTable.h"
#include "externals/XLSXContent/MapAttributeTable.h"
#include "externals/XLSXContent/MapInfo.h"

struct GameManager : ILClass<GameManager, 0x04c59b50> {
    struct Fields : SmartPoint::AssetAssistant::SingletonMonoBehaviour::Fields {
        UnityEngine::Transform::Object* _fieldObjectHolder;
        UnityEngine::Transform::Object* _battleObjectHolder;
        UnityEngine::GameObject::Object* _licenseTextObject;
        UnityEngine::GameObject::Object* _errorTextObject;
        TMPro::TextMeshProUGUI::Object* _officeTextUGUI;
        TMPro::TextMeshProUGUI::Object* _userTextUGUI;
        System::String::Object* _securityFileName;
        System::String::Object* _primarySecurityKey;
        System::String::Object* _secondarySecurityKey;
        ScriptableObjectFormat::PokemonData::Object* _debugPokemonData;
        GameSettings::Object* gameSettings;
        int32_t _cacheAreaID;
        int32_t _targetSceneID;
        UnityEngine::Coroutine::Object* _loadingOperation;
        bool _switchingNow;
    };

    static inline XLSXContent::FieldEncountTable::Sheettable::Object* GetFieldEncountData(int32_t zoneID) {
        return external<XLSXContent::FieldEncountTable::Sheettable::Object*>(0x02cd1150, zoneID);
    }

    static inline XLSXContent::MapInfo::Object* get_mapInfo() {
        return external<XLSXContent::MapInfo::Object*>(0x02cca5f0);
    }

    static inline PeriodOfDay get_currentPeriodOfDay() {
        return external<PeriodOfDay>(0x02cd0c60);
    }

    static inline int32_t GetUrayamaMonsNo(int32_t index) {
        return external<int32_t>(0x02cd1590, index);
    }

    static inline int32_t GetSafariMonsNo(int32_t index) {
        return external<int32_t>(0x02cd1480, index);
    }

    static inline void GetAttribute(UnityEngine::Vector2Int::Object grid, int32_t *code, int32_t *stop, bool debugdraw) {
        external<void>(0x02cd3b60, grid, code, stop, debugdraw);
    }

    static inline XLSXContent::MapAttributeTable::SheetData::Object* GetAttributeTable(int32_t code) {
        return external<XLSXContent::MapAttributeTable::SheetData::Object*>(0x02cd3ed0, code);
    }

    static inline XLSXContent::FieldEncountTable::Object* get_fieldEncountTable() {
        return external<XLSXContent::FieldEncountTable::Object*>(0x02cd03b0);
    }
};
