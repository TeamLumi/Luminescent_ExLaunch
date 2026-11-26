#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Pml/Sex.h"
#include "externals/XLSXContent/CharacterDressData.h"
#include "externals/XLSXContent/LocalKoukanData.h"
#include "externals/XLSXContent/MonohiroiTable.h"
#include "externals/XLSXContent/PokemonInfo.h"
#include "externals/XLSXContent/ShopTable.h"

namespace GameData {
    struct DataManager : ILClass<DataManager, game::Address::GameData_DataManager_TypeInfo> {
        struct StaticFields {
            void* CharacterGraphics;
            void* TrainerTable;
            void* PlaceData;
            void* MapWarpData;
            void* StopData;
            void* GimmickGraphics;
            void* PokemonInfo;
            void* ContestWazaInfo;
            void* ShopTable;
            void* CharacterDressData;
            void* KinomiData;
            void* KinomiPlaceData;
            void* HoneyTree;
            XLSXContent::MonohiroiTable::Object* MonohiroiTable;
            void* UgJumpPos;
            void* TowerTrainerTable;
            void* TowerMatchingTable;
            void* TowerSingleStockTable;
            void* TowerDoubleStockTable;
            void* AdventureNoteData;
            void* AdventureNoteDataDict; //System::Collections::Generic::Dictionary$$int$$List_AdventureNoteData_SheetData::Object*
            void* TowerBattlePoint;
            void* TagPlaceData;
            XLSXContent::LocalKoukanData::Object* LocalKoukanData;
            void* ContestCommonData;
            void* TvDataTable;
            void* TvSchedule;
            void* onPostLoadData;
            void* FieldCommonParam;
            void* FieldWazaCutInParam;
            void* ZenmetuZone;
            void* MoveAfterSaveGrid;
            void* _comparerCatalog;
            void* _comparerKinomi;
            void* statueEffectRawData;
        };

        static inline XLSXContent::ShopTable::SheetBoutiqueShop::Object * GetBoutiqueShopData(int32_t dressId) {
            return external<XLSXContent::ShopTable::SheetBoutiqueShop::Object *>(game::Address::GameData_DataManager_GetBoutiqueShopData, dressId);
        }

        static inline XLSXContent::CharacterDressData::SheetData::Object * GetCharacterDressData(int32_t dressId) {
            return external<XLSXContent::CharacterDressData::SheetData::Object *>(game::Address::GameData_DataManager_GetCharacterDressData, dressId);
        }

        static inline XLSXContent::PokemonInfo::SheetCatalog::Object* GetPokemonCatalog(int32_t monsNo, int32_t formNo, Pml::Sex sex, bool isRare, bool isEgg) {
            return external<XLSXContent::PokemonInfo::SheetCatalog::Object*>(game::Address::GameData_DataManager_GetPokemonCatalog_Int32_Int32_UInt8_Bool_Bool, monsNo, formNo, sex, isRare, isEgg);
        }

        static inline float GetFieldCommonParam(int32_t paramIndex) {
            return external<float>(game::Address::GameData_DataManager_GetFieldCommonParam, paramIndex);
        }
    };
}
