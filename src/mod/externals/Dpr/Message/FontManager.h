#pragma once

#include "externals/il2cpp-api.h"

#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"
#include "externals/TMPro/TextMeshProUGUI.h"

namespace Dpr::Message {
    struct FontManager : ILClass<FontManager, 0x04c59b38> {
        struct Fields : SmartPoint::AssetAssistant::SingletonMonoBehaviour::Fields{
            void* textFontData;
            void* fontMaterialData;
            void* spriteFontAsset;
            void* fontInfoTable;
            void* materialPropertyTable;
            void* materialTable;
            void* fontAssetLoader;
            int32_t useEFIGSCount;
        };

        static inline StaticILMethod<0x04c676d8, FontManager> Method$$FontManager$$get_Instance {};

        static inline FontManager::Object* get_Instance() {
            return SmartPoint::AssetAssistant::SingletonMonoBehaviour::get_Instance(Method$$FontManager$$get_Instance);
        }

        inline void ChangeFontMaterial(TMPro::TextMeshProUGUI::Object* text, int32_t materialIndex) {
            external<void>(0x01cab970, this, text, materialIndex);
        }
    };
}

