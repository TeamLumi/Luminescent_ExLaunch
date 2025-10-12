#pragma once

#include "externals/il2cpp-api.h"
#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/Dpr/UI/PoketchAppBase.h"
#include "externals/XLSXContent/TownMapTable.h"

namespace Dpr::UI {

    struct PoketchAppMapMarking : ILClass<PoketchAppMapMarking> {
        struct Fields : Dpr::UI::PoketchAppBase::Fields {
            UnityEngine::RectTransform::Object* _playerCursorRoot;
            UnityEngine::UI::Image::Object* _playerCursorImage;
            UnityEngine::UI::Image::Array* _wanderingPokemonImages;
            void* _hideMapIDs;
            UnityEngine::UI::Image::Array* _hideMapImages;
            float _blinkSpeed;
            int32_t _gridSize;
            UnityEngine::Vector2::Object _grigBasePosition;
            XLSXContent::TownMapTable::Sheetdata::Array* _mapDatas;
            System::Int32_array* _pokeZoneIDs;
            float _blinkCount;
        };
    };
}