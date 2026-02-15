#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Effect/EffectInstance.h"
#include "externals/FieldObjectEntity.h"
#include "externals/Pml/PokeParty.h"
#include "externals/System/Action.h"
#include "externals/UnityEngine/GameObject.h"
#include "externals/UnityEngine/Material.h"
#include "externals/UnityEngine/Transform.h"
#include "externals/UnityEngine/Vector3.h"

struct FieldManager : ILClass<FieldManager, 0x04c5a638> {
    struct Fields {
        System::Action::Object* OnZoneChangeEvent;
        System::Action::Object* OnZoneChangeOnce;
        System::Action::Object* OnSceneInitEvent;
        int32_t _updateType;
        int32_t _encountUpdateType;
        float _encountWait;
        int32_t _encountFadeType;
        void* _attributeEntitySE; // System_Collections_Generic_Queue_FieldManager_AttributeEvent__o*
        void* _attributeEntityEffect; // System_Collections_Generic_Queue_FieldManager_AttributeEvent__o*
        int32_t NowMapType;
        int32_t OldMapType;
        int32_t _now_zoneID;
        int32_t _Before_zoneID_k__BackingField;
        void* _effectDataArray; // FieldManager_LoadEffectData_array*
        Effect::EffectInstance::Object* _weatherEffectInstance;
        int32_t _nowWeather;
        void* _weather; // FieldWeather_o*
        void* MistWork; // FieldMistWork_o*
        void* FlashWork; // FieldFlashWork_o*
        int32_t _btl_trainerID1;
        int32_t _btl_trainerID2;
        UnityEngine::GameObject::Object* _ug_hole;
        bool _is_ugHoleLock;
        UnityEngine::Vector3::Object _ugHolePos;
        void* _ugMainProc; // UgMainProc_o*
        UnityEngine::GameObject::Object* _fldCanvasObject;
        void* _fldCanvasAssetReqOpe; // SmartPoint_AssetAssistant_AssetRequestOperation_o*
        int32_t _oldEncountWalkCount;
        void* _encFadeTextures; // EncountFadeTextures_o*
        UnityEngine::Material::Object* _encFadeMaterial;
        void* _encFadeTexturesReqOpe; // SmartPoint_AssetAssistant_AssetRequestOperation_o*
        void* _encResult; // Dpr_Field_EncountResult_o*
        void* _encEffctController; // EncEffectSequenceController_o*
        int32_t _encEffectCount;
        UnityEngine::GameObject::Array* _encEffectAsset;
        bool _encountAttriLog;
        void* _fishing; // Dpr_Field_FieldFishing_o*
        int32_t _useRod;
        void* _wazaAction; // FieldManager_FieldWazaAction_o*
        bool _IsMenuOpen_k__BackingField;
        bool _isMenuOpenRequest;
        float _shortCutPushTime;
        int32_t _shortCutPushHoldId;
        bool _IsPoketchOpen_k__BackingField;
        bool SND_W_ID_CTRL_BGM_FLAG;
        void* _KinomiResources_k__BackingField; // KinomiResources_o*
        int32_t _demoReturnType;
        bool _demoReturnInput;
        uint32_t _WalkEventName_Attribute;
        bool _initFlag;
        int32_t _autoSaveState;
        UnityEngine::Vector3::Object eventTownMapPos;
        void* unnnoonFormList; // System_Collections_Generic_List_int__o*
        int32_t _shorCutSeq;
        UnityEngine::Transform::Object* _akLisnerTransform;
    };

    struct StaticFields {
        FieldManager::Object* _Instance_k__BackingField;
        void* fwMng; // Dpr_Field_Walking_FieldWalkingManager_o*
        void* abUnloader; // Dpr_SubContents_Utils_AssetUnloader_o*
        bool _IsResume_k__BackingField;
        bool SealPrevFlag;
    };

    static_assert(offsetof(Fields, _akLisnerTransform) == 0x158);

    inline uint16_t GetFormNo(int32_t mons, int32_t karana, int32_t anno) {
        return external<uint16_t>(0x0179f560, this, mons, karana, anno);
    }

    inline void GetLegendPokeEncountInfo(Pml::PokePara::PokemonParam::Object* param, System::String::Object** encSec, int32_t* arenaID, System::String::Object** bgm, int32_t* setupEffect) {
        external<void>(0x0179efe0, this, param, encSec, arenaID, bgm, setupEffect);
    }

    inline void EventWildBattle(int32_t mons, int32_t level, bool isCaptureDemo, bool isSymbol, bool isMitu, uint8_t talentVNum, bool isCantUseBall, int32_t formNo, bool tokusei3rd) {
        external<void>(0x0179f720, this, mons, level, isCaptureDemo, isSymbol, isMitu, talentVNum, isCantUseBall, formNo, tokusei3rd);
    }

    inline void EventWildBattle(Pml::PokeParty::Object* pokeParty, bool isCaptureDemo, bool isSymbol, bool isMitu, bool isCantUseBall) {
        external<void>(0x0179f940, this, pokeParty, isCaptureDemo, isSymbol, isMitu, isCantUseBall);
    }

    inline void EncountStart(int32_t type, int32_t trainerid1, int32_t trainerid2) {
        external<void>(0x0179f330, this, type, trainerid1, trainerid2);
    }

    inline int32_t get_areaID() {
        return external<int32_t>(0x01797f90, this);
    }

    inline void add_OnZoneChangeEvent(System::Action::Object* value) {
        external<void>(0x01797b40, this, value);
    }

    inline void remove_OnZoneChangeEvent(System::Action::Object* value) {
        external<void>(0x01797bf0, this, value);
    }

    inline void add_OnSceneInitEvent(System::Action::Object* value) {
        external<void>(0x01797ca0, this, value);
    }

    inline void remove_OnSceneInitEvent(System::Action::Object* value) {
        external<void>(0x01797d50, this, value);
    }

    inline int32_t GetBatleWeather() {
        return external<int32_t>(0x0179f1a0, this);
    }

    inline void PreLoadEncEffect(System::String::Object* assetname) {
        external<void>(0x0179e6a0, this, assetname);
    }

    inline void RequestAttributeEffect(FieldObjectEntity::Object* entity, int32_t attri) {
        external<void>(0x017a0090, this, entity, attri);
    }

    inline void RequestAttributeSE(FieldObjectEntity::Object* entity, int32_t attri) {
        external<void>(0x017a0130, this, entity, attri);
    }
};
