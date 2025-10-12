#include "exlaunch.hpp"
#include "externals/il2cpp-api.h"

#include "externals/Dpr/Contest/ContestPlayerEntity.h"
#include "externals/Dpr/UI/UIManager.h"
#include "externals/Dpr/UI/PoketchAppEggChecker.h"
#include "externals/Dpr/UI/ZukanCompareWeightController.h"
#include "externals/PlayerWork.h"
#include "externals/XLSXContent/UIDatabase.h"

#include "logger/logger.h"

System::String::Object* GetWeightSpriteName() {
    if (PlayerWork::get_playerSex()) // Lucas
        return System::String::Create("dex_ico_compare_player_01_000_00");
    else // Dawn
        return System::String::Create("dex_ico_compare_player_01_100_00");
}

System::String::Object* GetTownMapSpriteName() {
    if (PlayerWork::get_playerSex()) // Lucas
        return System::String::Create("map_ico_player_01_000_00");
    else // Dawn
        return System::String::Create("map_ico_player_01_100_00");
}

System::String::Object* GetEncSeqPath() {
    if (PlayerWork::get_playerSex()) // Lucas
        return System::String::Create("encseq/tex/p0000");
    else // Dawn
        return System::String::Create("encseq/tex/p0100");
}

XLSXContent::UIDatabase::SheetCharacterBag::Object* FindBagDataById(XLSXContent::UIDatabase::SheetCharacterBag::Array* source, int id)
{
    for (uint64_t i=0; i<source->max_length; i++)
    {
        XLSXContent::UIDatabase::SheetCharacterBag::Object* item = source->m_Items[i];
        if (item->fields.Index == id) return item;
    }

    Logger::log("Could not find bag data for id: %d\n", id);
    return nullptr;
}

HOOK_DEFINE_REPLACE(Dpr_UI_UIManager_GetCharacterBagData) {
    static XLSXContent::UIDatabase::SheetCharacterBag::Object* Callback(Dpr::UI::UIManager::Object* __this, int32_t characterDressId) {
        system_load_typeinfo(0x9bd6);

        XLSXContent::UIDatabase::SheetCharacterBag::Array* characterBags = __this->fields._mdUis->fields.CharacterBag;

        if (PlayerWork::get_playerSex()) // Lucas
        {
            XLSXContent::UIDatabase::SheetCharacterBag::Object* item = FindBagDataById(characterBags, 0);
            if (item != nullptr) return item;
            else return characterBags->m_Items[0]; // Shouldn't happen
        }
        else // Dawn
        {
            XLSXContent::UIDatabase::SheetCharacterBag::Object* item = FindBagDataById(characterBags, 100);
            if (item != nullptr) return item;
            else return characterBags->m_Items[0]; // Shouldn't happen
        }
    }
};

HOOK_DEFINE_REPLACE(Dpr_UI_UIManager_LoadSpritePokemonDot) {
    static void Callback(Dpr::UI::UIManager::Object* __this, int32_t monsNo, uint16_t formNo, Pml::Sex sex, Pml::PokePara::RareType rareType, bool isEgg, UnityEngine::Events::UnityAction::Object* onComplete) {
        __this->LoadSpritePokemon(monsNo, formNo, sex, rareType, isEgg, onComplete);
    }
};

HOOK_DEFINE_REPLACE(Dpr_UI_UIManager_LoadSpritePokemonLarge) {
    static void Callback(Dpr::UI::UIManager::Object* __this, int32_t monsNo, uint16_t formNo, Pml::Sex sex, Pml::PokePara::RareType rareType, bool isEgg, UnityEngine::Events::UnityAction::Object* onComplete) {
        __this->LoadSpritePokemon(monsNo, formNo, sex, rareType, isEgg, onComplete);
    }
};

HOOK_DEFINE_REPLACE(Dpr_UI_UIManager_LoadSpritePokemonDotParam) {
    static void Callback(Dpr::UI::UIManager::Object* __this, Pml::PokePara::PokemonParam::Object* pokemonParam, UnityEngine::Events::UnityAction::Object* onComplete) {
        __this->LoadSpritePokemon(pokemonParam, onComplete);
    }
};

HOOK_DEFINE_REPLACE(Dpr_UI_UIManager_LoadSpritePokemonLargeParam) {
    static void Callback(Dpr::UI::UIManager::Object* __this, Pml::PokePara::PokemonParam::Object* pokemonParam, UnityEngine::Events::UnityAction::Object* onComplete) {
        __this->LoadSpritePokemon(pokemonParam, onComplete);
    }
};

HOOK_DEFINE_REPLACE(ContestPlayerEntity$$AppendLoadPokemonIcon) {
    static void Callback(Dpr::Contest::ContestPlayerEntity::Object* __this) {
        system_load_typeinfo(0x2fc3);
        Dpr::UI::UIManager::getClass()->initIfNeeded();

        auto uiManager = Dpr::UI::UIManager::get_Instance();
        auto param = __this->fields.playerData->fields.pokemonInfo->fields.pokeParam;

        auto callback = UnityEngine::Events::UnityAction::getClass(UnityEngine::Events::UnityAction::Sprite_TypeInfo)
                ->newInstance(__this, *Dpr::Contest::ContestPlayerEntity::Method$$AppendLoadPokemonIcon_b_99);

        uiManager->LoadSpritePokemon(param, callback);
    }
};

HOOK_DEFINE_INLINE(ZukanCompareWeightController$$RequestLoadModel) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto uiManager = (Dpr::UI::UIManager::Object*)ctx->X[0];
        auto param = ((Dpr::UI::ZukanCompareWeightController::Object*)ctx->X[19])->fields.currentZukanInfo->GetCurrentPokemonParam();
        auto callback = (UnityEngine::Events::UnityAction::Object*)ctx->X[6];

        uiManager->LoadSpritePokemon(param, callback);
    }
};

HOOK_DEFINE_TRAMPOLINE(PoketchAppEggChecker$$OnInitialize) {
    static void Callback(Dpr::UI::PoketchAppEggChecker::Object* __this) {
        system_load_typeinfo(0x6d81);
        Dpr::UI::UIManager::getClass()->initIfNeeded();

        auto sprite = Dpr::UI::UIManager::get_Instance()->GetAtlasSprite(SpriteAtlasID::TEXTUREMASS, System::String::Create("pm0000_00_20"));
        __this->fields._eggImage->set_sprite(sprite);

        Orig(__this);
    }
};

void exl_uniform_ui_main() {
    // Outfit-neutral UI
    Dpr_UI_UIManager_GetCharacterBagData::InstallAtOffset(0x017c5710);

    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p(0);

    p.Seek(0x01bb17b8);
    p.BranchLinkInst((void*)&GetWeightSpriteName);

    p.Seek(0x0184fc08);
    p.BranchLinkInst((void*)&GetTownMapSpriteName);

    p.Seek(0x01f07380);
    p.BranchLinkInst((void*)&GetEncSeqPath);
    p.Seek(0x01f077e0);
    p.BranchLinkInst((void*)&GetEncSeqPath);

    // Repoint Large and DP/Pixel sprites to base one
    Dpr_UI_UIManager_LoadSpritePokemonDot::InstallAtOffset(0x017c46a0);
    Dpr_UI_UIManager_LoadSpritePokemonLarge::InstallAtOffset(0x017c4140);
    Dpr_UI_UIManager_LoadSpritePokemonDotParam::InstallAtOffset(0x017c4600);
    Dpr_UI_UIManager_LoadSpritePokemonLargeParam::InstallAtOffset(0x017c4090);

    // Repoint all LoadSpritePokemon calls to the PokemonParam overload
    ContestPlayerEntity$$AppendLoadPokemonIcon::InstallAtOffset(0x01c77ae0);
    ZukanCompareWeightController$$RequestLoadModel::InstallAtOffset(0x01bb1a0c);

    p.Seek(0x01d8e12c); // PokemonIcon$$Load
    p.WriteInst(MovRegister(X1, X20));
    p.WriteInst(MovRegister(X2, X21));
    p.BranchLinkInst(0x017c3e40); // Address of LoadPokemonIcon with the PokemonParam overload

    p.Seek(0x01d8e4cc); // PokemonIcon$$Load
    p.WriteInst(MovRegister(X1, X20));
    p.WriteInst(MovRegister(X2, X22));
    p.BranchLinkInst(0x017c3e40); // Address of LoadPokemonIcon with the PokemonParam overload

    // Dynamically grab the egg sprite for the Daycare Pokétch app
    PoketchAppEggChecker$$OnInitialize::InstallAtOffset(0x01a06710);
};