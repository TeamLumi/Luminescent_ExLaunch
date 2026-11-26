#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/SmartPoint/AssetAssistant/AssetRequestOperation.h"
#include "externals/System/Action.h"
#include "externals/UnityEngine/GameObject.h"

namespace System::Collections::Generic {
    struct List$$UgResManager_AppendData;
}

struct UgResManager : ILClass<UgResManager, game::Address::UgResManager_TypeInfo> {
    struct AppendData : ILClass<AppendData, game::Address::UgResManager_AppendData_TypeInfo> {
        struct Fields {
            SmartPoint::AssetAssistant::AssetRequestOperation::Object* Operation;
            System::Action::Object* callback;
        };

        inline void ctor() {
            external<void>(game::Address::UgResManager_AppendData_ctor, this);
        }
    };

    struct Fields {};

    struct StaticFields {
        void* UgEncounts; // System_Collections_Generic_Dictionary_string__UgEncount__o*
        void* UgPokemonData; // XLSXContent_UgPokemonData_o*
        void* _UgRandMark_k__BackingField; // XLSXContent_UgRandMark_o*
        void* _UgMiniMap_k__BackingField; // XLSXContent_UgMiniMap_o*
        void* _UgIconImages_k__BackingField; // UgIconImages_o*
        UnityEngine::GameObject::Object* _UgCanvasPrefab_k__BackingField;
        UnityEngine::GameObject::Object* _UgFieldManagerPrefab_k__BackingField;
        UnityEngine::GameObject::Object* UgEmoticonSetPrefab;
        void* _UgEmoticonSet_k__BackingField; // Dpr_UnderGround_UgEmoticonSet_o*
        UnityEngine::GameObject::Object* UgInfoSetPrefab;
        void* _UgInfoSet_k__BackingField; // Dpr_UnderGround_UgInfoSet_o*
        void* _UgEncountLevel_k__BackingField; // XLSXContent_UgEncountLevel_o*
        void* _UgSpecialPokemon_k__BackingField; // XLSXContent_UgSpecialPokemon_o*
        void* _UgPokemonPos_k__BackingField; // XLSXContent_UgPokemonPos_o*
        void* _UgNpcPos_k__BackingField; // XLSXContent_UgNpcPos_o*
        void* _UgNpcList_k__BackingField; // XLSXContent_UgNpcList_o*
        void* _UgNpcTalk_k__BackingField; // XLSXContent_UgNpcTalk_o*
        void* _UgDrillUsablePositions_k__BackingField; // UgDrillUsablePositions_o*
        void* _UgDigFossilePosGroups_k__BackingField; // UgDigFossilePosGroups_o*
        void* _UgAllDigFossilePos_k__BackingField; // UgAllDigFossilePos_o*
        void* _UgTamagoWazaIgnoreTable_k__BackingField; // XLSXContent_UgTamagoWazaIgnoreTable_o*
        bool _isDataLoadRequest;
        bool _isDataLoadEnd;
        System::Action::Object* _onPostLoaded;
        UnityEngine::GameObject::Object* UgCanvas;
        UnityEngine::GameObject::Object* _createDigManagerPrefab;
        System::Collections::Generic::List$$UgResManager_AppendData* _assetOpe;
    };
};


namespace System::Collections::Generic {
    struct List$$UgResManager_AppendData : List<List$$UgResManager_AppendData, UgResManager::AppendData, game::Address::System_Collections_Generic_List_UgResManager_AppendData_TypeInfo> {
        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_List_UgResManager_AppendData_ctor> Method$$ctor {};
        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_List_UgResManager_AppendData_Add> Method$$Add {};
    };
}