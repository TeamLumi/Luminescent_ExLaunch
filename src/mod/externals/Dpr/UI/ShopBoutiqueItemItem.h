#pragma once

#include "externals/il2cpp-api.h"
#include "externals/System/Collections/Generic/List.h"
#include "externals/XLSXContent/ShopTable.h"
#include "externals/XLSXContent/CharacterDressData.h"
#include "ShopItemItem.h"

namespace Dpr::UI {
    struct ShopBoutiqueItemItem : ILClass<ShopBoutiqueItemItem, game::Address::Dpr_UI_ShopBoutiqueItemItem_TypeInfo> {
        struct Param : ILClass<Param, game::Address::Dpr_UI_ShopBoutiqueItemItem_Param_TypeInfo> {
            struct Fields{
                XLSXContent::ShopTable::SheetBoutiqueShop::Object* data;
                XLSXContent::CharacterDressData::SheetData::Object* dressData;
            };

            inline void ctor() {
                external<void>(game::Address::Dpr_UI_ShopBoutiqueItemItem_Param_ctor, this);
            }
        };

        struct Fields : Dpr::UI::ShopItemItem::Fields {
            UnityEngine::UI::Image::Object* _waredIcon;
            void* _purchased; //Dpr_UI_ShopBoutiqueItemItem_Purchased_o*
            void* _colors; //UnityEngine_Color_array*
            Dpr::UI::ShopBoutiqueItemItem::Param::Object* _param;
            XLSXContent::CharacterDressData::SheetData::Object* _characterDressData;
            int32_t _boutiqueType;
        };
    };
}


namespace System::Collections::Generic {
    struct List$$ShopBoutiqueItemItem_Param : List<List$$ShopBoutiqueItemItem_Param, Dpr::UI::ShopBoutiqueItemItem::Param> {
        static inline StaticILMethod<game::Address::Method_System_Collections_Generic_List_ShopBoutiqueItemItem_Param_Add, Dpr::UI::ShopBoutiqueItemItem::Param> Method$$Add {};
    };
}