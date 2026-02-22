#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/System/String.h"
#include "externals/UnityEngine/Transform.h"
#include "externals/XLSXContent/PokemonInfo.h"

namespace Dpr::SubContents {
    struct BindModel : ILClass<BindModel> {
        struct Fields {
            bool StartActive;
            UnityEngine::Transform::Object* parent;
            System::String::Object* assetBundlePath;
            int32_t monsNo;
            bool isEgg;
            bool isBattleScale;
            UnityEngine::_Object::Object* LoadedAsset;
            System::String::Object* Rename;
            XLSXContent::PokemonInfo::SheetCatalog::Object* catalog;
            bool isVariants;
            float scale;
            Pml::PokePara::PokemonParam::Object* pokeParam;
        };

        static_assert(offsetof(Fields, pokeParam) == 0x40);
        static_assert(sizeof(Fields) == 0x48);
    };
}
