#include "exlaunch.hpp"
#include "externals/il2cpp-api.h"

#include "memory/vector.h"
#include "data/outfits.h"
#include "data/utils.h"
#include "externals/Dpr/UI/ShopBoutiqueItemItem.h"
#include "externals/GameData/DataManager.h"

void AddOutfit(System::Collections::Generic::List$$ShopBoutiqueItemItem_Param::Object* itemParams, int32_t dressId) {
    Dpr::UI::ShopBoutiqueItemItem::Param::Object* item = Dpr::UI::ShopBoutiqueItemItem::Param::newInstance();

    item->fields.data = GameData::DataManager::GetBoutiqueShopData(dressId);
    item->fields.dressData = GameData::DataManager::GetCharacterDressData(dressId);
    itemParams->Add(item);
}

HOOK_DEFINE_REPLACE(Dpr_UI_ShopBoutiqueChange_SetupBoutiqueItemParams) {
    static int32_t Callback(System::Collections::Generic::List$$ShopBoutiqueItemItem_Param::Object *itemParams) {
        system_load_typeinfo(0x7cbb);

        for (int32_t dressId = 0; dressId < OUTFIT_COUNT; dressId++)
        {
            if (dressId == array_index(OUTFITS, "Bicycle Style Masculine") ||
                dressId == array_index(OUTFITS, "Bicycle Style Feminine") ||
                dressId == array_index(OUTFITS, "Cyber Style 2.0 Masculine")||
                dressId == array_index(OUTFITS, "Renegade Style Feminine"))
            {
                // Don't add these outfits
                continue;
            }
            else
            {
                AddOutfit(itemParams, dressId);
            }
        }

        return 0;
    }
};

void exl_gender_neutral_boutique_main() {
    Dpr_UI_ShopBoutiqueChange_SetupBoutiqueItemParams::InstallAtOffset(0x01d46b00);

    // Remove bike outfit override for battles on cycling road
    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p(0);
    auto inst = nn::vector<exl::patch::Instruction> {
        { 0x0202f120, Movz(X0, 0) }, // Remove bike outfit override for battles on cycling road
    };
    p.WriteInst(inst);
};