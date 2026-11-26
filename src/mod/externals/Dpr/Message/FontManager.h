#pragma once

#include "externals/il2cpp-api.h"

#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"
#include "externals/TMPro/TextMeshProUGUI.h"

namespace Dpr::Message {
    struct FontManager : ILClass<FontManager, game::Address::SmartPoint_AssetAssistant_SingletonMonoBehaviour_FontManager_TypeInfo> {
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

        static inline StaticILMethod<game::Address::Method_SmartPoint_AssetAssistant_SingletonMonoBehaviour_FontManager_get_Instance, FontManager> Method$$FontManager$$get_Instance {};

        static inline FontManager::Object* get_Instance() {
            return SmartPoint::AssetAssistant::SingletonMonoBehaviour::get_Instance(Method$$FontManager$$get_Instance);
        }

        inline void ChangeFontMaterial(TMPro::TextMeshProUGUI::Object* text, int32_t materialIndex) {
            external<void>(game::Address::Dpr_Message_FontManager_ChangeFontMaterial, this, text, materialIndex);
        }
    };
}

