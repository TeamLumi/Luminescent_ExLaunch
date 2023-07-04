#pragma once

#include "externals/il2cpp-api.h"

#include "externals/SmartPoint/Components/DebugMenu.h"
#include "externals/System/Collections/IEnumerator.h"
#include "externals/System/String.h"
#include "externals/UnityEngine/Coroutine.h"
#include "externals/UnityEngine/GameObject.h"
#include "externals/UnityEngine/MonoBehaviour.h"

struct Viewer : ILClass<Viewer> {
    struct Fields : UnityEngine::MonoBehaviour::Fields {
        void* _pokemonData;
        void* _characterData;
        void* _effectData;
        void* _audioData;
        void* _selectedObjectLabel;
        void* _renderSettings;
        void* _skyColorSettings;
        void* _screenEffectCamera;
        void* petrifyData;
        void* petrifyShader;
        void* petrifyFireShader;
        void* petrifySmokeShader;
        void* _fieldCloudTex;
        void* _battleCloudTex;
        void* _mapInfo;
        void* _arenaInfo;
        void* _statueData;
        SmartPoint::Components::DebugMenu::MenuInstance::Object* _rootMenu;
        SmartPoint::Components::DebugMenu::MenuInstance::Object* _fieldPokemonMenu;
        SmartPoint::Components::DebugMenu::MenuInstance::Object* _battlePokemonMenu;
        SmartPoint::Components::DebugMenu::MenuInstance::Object* _instanceMenu;
        SmartPoint::Components::DebugMenu::MenuInstance::Object* _animationMenu;
        SmartPoint::Components::DebugMenu::MenuInstance::Object* _settingMenu;
        void* _pokemonInfos;
        void* _instancePlayableLookup;
        void* _instanceSettingLookup;
        void* _effectMenuAdd;
        void* _effectMenuBoot;
        System::String::Object* _savedInputAxisH;
        System::String::Object* _savedInputAxisV;
        int32_t _currentPokemonIndex;
        int32_t _currentStatueIndex;
        int32_t _theaterTrackIndex;
        UnityEngine::GameObject::Object* _bgPrefab;
        UnityEngine::Coroutine::Object* _loadingCoroutine;
        void* _simpleCamera;
        void* _camera;
        uint8_t _weather;
        void* _effecctInstances;
        void* _effectStopMenu;
        void* _seInstances;
        void* _seStopMenu;
        void* _audioGroupDict;
        void* _boxMesh;
        void* _boxMat;
    };

    inline System::Collections::IEnumerator::Object* Start() {
        return external<System::Collections::IEnumerator::Object*>(0x017ec490, this);
    }
};
