#pragma once

#include "externals/il2cpp-api.h"

#include "externals/XLSXContent/ItemTable.h"
#include "externals/XLSXContent/PersonalTable.h"

namespace Pml {
    struct PmlUse : ILClass<PmlUse, 0x04c5b4d0> {
        struct Fields {
            bool isAutoLoad;
            XLSXContent::PersonalTable::Object* personalTotal;
            void* growTableTotal; // XLSXContent_GrowTable_o*
            void* evolveTableTotal; // XLSXContent_EvolveTable_o*
            void* wazaOboeTotal; // XLSXContent_WazaOboeTable_o*
            void* wazaDataTotal; // XLSXContent_WazaTable_o*
            XLSXContent::ItemTable::Object* itemPrmTotal;
            void* tamagoWazaTotal; // XLSXContent_TamagoWazaTable_o*
            void* addPersonalTotal; // XLSXContent_AddPersonalTable_o*
            void* sealTotal; // XLSXContent_SealTable_o*
            void* ugItemPrmTotal; // XLSXContent_UgItemTable_o*
            void* tamaTableTotal; // XLSXContent_TamaTable_o*
            void* pedestalTableTotal; // XLSXContent_PedestalTable_o*
            void* stoneStatuEeffectTotal; // XLSXContent_StoneStatuEeffect_o*
            void* ugfPosTotal; // XLSXContent_UgFatherPos_o*
            void* ugfExpansionTotal; // XLSXContent_UgFatherExpansion_o*
            void* ugfShopTotal; // XLSXContent_UgFatherShopTable_o*
            void* evolveManager; // Pml_PokePara_EvolveManager_o*
            bool isABAppended;
            bool isInitialized;
            bool _IsPersistentTiming_k__BackingField;
        };

        static inline Pml::PmlUse::Object* get_Instance() {
            return external<Pml::PmlUse::Object*>(0x024a0110);
        }

        inline int32_t get_LangId() {
            return external<int32_t>(0x024a3c20, this);
        }

        inline uint8_t get_CassetVersion() {
            return external<uint8_t>(0x024a3cb0, this);
        }
    };
}
