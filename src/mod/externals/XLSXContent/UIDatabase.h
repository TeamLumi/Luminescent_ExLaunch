#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/String.h"
#include "externals/UnityEngine/ScriptableObject.h"
#include "externals/UnityEngine/Vector2.h"

namespace XLSXContent {
    struct UIDatabase : ILClass<UIDatabase> {
        struct SheetPokemonIcon : ILClass<SheetPokemonIcon> {
            struct Fields {
                int32_t UniqueID;
                System::String::Object* AssetBundleName;
                System::String::Object* AssetName;
                System::String::Object* AssetBundleNameLarge;
                System::String::Object* AssetNameLarge;
                System::String::Object* AssetBundleNameDP;
                System::String::Object* AssetNameDP;
                UnityEngine::Vector2::Object HallofFameOffset;
            };
        };

        struct SheetCharacterBag : ILClass<SheetCharacterBag> {
            struct Fields {
                int32_t Index;
                System::String::Object* XMenuDefault;
                System::String::Object* XMenuSelect;
                System::String::Object* XMenuShadow;
                System::String::Object* BagHeader;
                System::String::Object* BagBase;
                System::String::Array* BagPockets;
            };
        };

        struct SheetUIWindow : ILClass<SheetUIWindow> {
            struct Fields {
                int32_t WindowAnimId;
                System::String::Object* AssetBundleName;
                System::String::Object* AssetName;
                bool Resident;
                int32_t UiCameraMode;
                bool Firstload;
            };
        };

        struct Fields : UnityEngine::ScriptableObject::Fields {
            SheetUIWindow::Array* UIWindow;
            void* Transition; // XLSXContent_UIDatabase_SheetTransition_array*
            void* SpriteAtlas; // XLSXContent_UIDatabase_SheetSpriteAtlas_array*
            void* SharedSprite; // XLSXContent_UIDatabase_SheetSharedSprite_array*
            void* CommonSprite; // XLSXContent_UIDatabase_SheetCommonSprite_array*
            SheetPokemonIcon::Array* PokemonIcon;
            void* AshiatoIcon; // XLSXContent_UIDatabase_SheetAshiatoIcon_array*
            void* PokemonVoice; // XLSXContent_UIDatabase_SheetPokemonVoice_array*
            void* MonsterBall; // XLSXContent_UIDatabase_SheetMonsterBall_array*
            void* ContextMenu; // XLSXContent_UIDatabase_SheetContextMenu_array*
            void* Keyguide; // XLSXContent_UIDatabase_SheetKeyguide_array*
            SheetCharacterBag::Array* CharacterBag;
            void* ZukanDisplay; // XLSXContent_UIDatabase_SheetZukanDisplay_array*
            void* ZukanComparePlayer; // XLSXContent_UIDatabase_SheetZukanComparePlayer_array*
            void* ZukanCompareHeight; // XLSXContent_UIDatabase_SheetZukanCompareHeight_array*
            void* ZukanCompareWeight; // XLSXContent_UIDatabase_SheetZukanCompareWeight_array*
            void* FloorDisplay; // XLSXContent_UIDatabase_SheetFloorDisplay_array*
            void* ShopMessage; // XLSXContent_UIDatabase_SheetShopMessage_array*
            void* PlaceMark; // XLSXContent_UIDatabase_SheetPlaceMark_array*
            void* MarkColor; // XLSXContent_UIDatabase_SheetMarkColor_array*
            void* Wallpaper; // XLSXContent_UIDatabase_SheetWallpaper_array*
            void* Box; // XLSXContent_UIDatabase_SheetBox_array*
            void* PokeColor; // XLSXContent_UIDatabase_SheetPokeColor_array*
            void* DistributionMapchip; // XLSXContent_UIDatabase_SheetDistributionMapchip_array*
            void* PolishedBadge; // XLSXContent_UIDatabase_SheetPolishedBadge_array*
            void* SearchPokeIconSex; // XLSXContent_UIDatabase_SheetSearchPokeIconSex_array*
            void* HideWazaName; // XLSXContent_UIDatabase_SheetHideWazaName_array*
            void* HideTokusei; // XLSXContent_UIDatabase_SheetHideTokusei_array*
            void* ZukanRating; // XLSXContent_UIDatabase_SheetZukanRating_array*
            void* BoxOpenParam; // XLSXContent_UIDatabase_SheetBoxOpenParam_array*
            void* SealTemplate; // XLSXContent_UIDatabase_SheetSealTemplate_array*
            void* RankingDisplay; // XLSXContent_UIDatabase_SheetRankingDisplay_array*
        };
    };
}

static_assert(offsetof(XLSXContent::UIDatabase::Fields, CharacterBag) == 0x60);