#pragma once

#include "externals/il2cpp-api.h"

#include "externals/FieldCamera.h"
#include "externals/GameSettings.h"
#include "externals/PeriodOfDay.h"
#include "externals/ScriptableObjectFormat/PokemonData.h"
#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"
#include "externals/System/DateTime.h"
#include "externals/System/String.h"
#include "externals/TMPro/TextMeshProUGUI.h"
#include "externals/UnityEngine/Coroutine.h"
#include "externals/UnityEngine/GameObject.h"
#include "externals/UnityEngine/Transform.h"
#include "externals/UnityEngine/Vector2Int.h"
#include "externals/XLSXContent/FieldEncountTable.h"
#include "externals/XLSXContent/MapAttributeTable.h"
#include "externals/XLSXContent/MapInfo.h"

struct GameManager : ILClass<GameManager, game::Address::GameManager_TypeInfo> {
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

    struct StaticFields {
        void* connector;
        bool _isReady_k__BackingField;
        bool _afterStarting_k__BackingField;
        bool _playEnding_k__BackingField;
        System::DateTime::Object _nowTime;
        int64_t _tickOffset_k__BackingField;
        int64_t _timeScale_k__BackingField;
        bool _pause_k__BackingField;
        FieldCamera::Object* fieldCamera;
        bool isControllable;
        bool useSubAttribute;
    };

    static_assert(offsetof(StaticFields, useSubAttribute) == 0x39);

    static inline XLSXContent::FieldEncountTable::Sheettable::Object* GetFieldEncountData(int32_t zoneID) {
        return external<XLSXContent::FieldEncountTable::Sheettable::Object*>(game::Address::GameManager_GetFieldEncountData, zoneID);
    }

    static inline XLSXContent::MapInfo::Object* get_mapInfo() {
        return external<XLSXContent::MapInfo::Object*>(game::Address::GameManager_get_mapInfo);
    }

    static inline PeriodOfDay get_currentPeriodOfDay() {
        return external<PeriodOfDay>(game::Address::GameManager_get_currentPeriodOfDay);
    }

    static inline int32_t GetUrayamaMonsNo(int32_t index) {
        return external<int32_t>(game::Address::GameManager_GetUrayamaMonsNo, index);
    }

    static inline int32_t GetSafariMonsNo(int32_t index) {
        return external<int32_t>(game::Address::GameManager_GetSafariMonsNo, index);
    }

    static inline void GetAttribute(UnityEngine::Vector2Int::Object grid, int32_t *code, int32_t *stop, bool debugdraw) {
        external<void>(game::Address::GameManager_GetAttribute, grid, code, stop, debugdraw);
    }

    static inline XLSXContent::MapAttributeTable::SheetData::Object* GetAttributeTable(int32_t code) {
        return external<XLSXContent::MapAttributeTable::SheetData::Object*>(game::Address::GameManager_GetAttributeTable, code);
    }

    static inline XLSXContent::MapAttributeExTable::SheetData::Object* GetAttributeEx(UnityEngine::Vector2Int::Object grid, float height, bool debugdraw) {
        return external<XLSXContent::MapAttributeExTable::SheetData::Object*>(game::Address::GameManager_GetAttributeEx, grid, height, debugdraw);
    }
};
