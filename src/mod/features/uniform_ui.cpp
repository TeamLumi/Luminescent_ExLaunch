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
        system_load_typeinfo(game::TypeInfo::Dpr_UI_UIManager_GetCharacterBagData);

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
        system_load_typeinfo(game::TypeInfo::Dpr_Contest_ContestPlayerEntity_AppendLoadPokemonIcon);
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
        system_load_typeinfo(game::TypeInfo::Dpr_UI_PokemonIcon_Load_Call1);
        Dpr::UI::UIManager::getClass()->initIfNeeded();

        auto sprite = Dpr::UI::UIManager::get_Instance()->GetAtlasSprite(SpriteAtlasID::TEXTUREMASS, System::String::Create("pm0000_00_20"));
        __this->fields._eggImage->set_sprite(sprite);

        Orig(__this);
    }
};

void exl_uniform_ui_main() {
    // Outfit-neutral UI
    Dpr_UI_UIManager_GetCharacterBagData::InstallAtOffset(game::Address::Dpr_UI_UIManager_GetCharacterBagData);

    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p;

    p.Seek(game::Address::Dpr_UI_ZukanCompareWeightController_LoadPlayerIcon + 0x88);
    p.BranchLinkInst((void*)&GetWeightSpriteName);

    p.Seek(game::Address::Dpr_UI_TownmapPlayerIcon_GetSpritePlayerIcon + 0xA8);
    p.BranchLinkInst((void*)&GetTownMapSpriteName);

    p.Seek(game::Address::EncEffectSequenceController_LoadGymEffect_d_13_MoveNext + 0xE0);
    p.BranchLinkInst((void*)&GetEncSeqPath);
    p.Seek(game::Address::EncEffectSequenceController_LoadLeagueEffect_d_14_MoveNext + 0xE0);
    p.BranchLinkInst((void*)&GetEncSeqPath);

    // Repoint Large and DP/Pixel sprites to base one
    Dpr_UI_UIManager_LoadSpritePokemonDot::InstallAtOffset(game::Address::Dpr_UI_UIManager_LoadSpritePokemonDot_Int32_UInt16_UInt8_UInt8_Bool_UnityAction_Sprite);
    Dpr_UI_UIManager_LoadSpritePokemonLarge::InstallAtOffset(game::Address::Dpr_UI_UIManager_LoadSpritePokemonLarge_Int32_UInt16_UInt8_UInt8_Bool_UnityAction_Sprite);
    Dpr_UI_UIManager_LoadSpritePokemonDotParam::InstallAtOffset(game::Address::Dpr_UI_UIManager_LoadSpritePokemonDot_PokemonParam_UnityAction_Sprite);
    Dpr_UI_UIManager_LoadSpritePokemonLargeParam::InstallAtOffset(game::Address::Dpr_UI_UIManager_LoadSpritePokemonLarge_PokemonParam_UnityAction_Sprite);

    // Repoint all LoadSpritePokemon calls to the PokemonParam overload
    ContestPlayerEntity$$AppendLoadPokemonIcon::InstallAtOffset(game::Address::Dpr_Contest_ContestPlayerEntity_AppendLoadPokemonIcon);
    ZukanCompareWeightController$$RequestLoadModel::InstallAtOffset(game::Address::Dpr_UI_ZukanCompareWeightController_RequestLoadModel + 0x12C);

    p.Seek(game::Address::Dpr_UI_PokemonIcon_Load_PokemonParam_Int32_Int32 + 0x16C); // PokemonIcon$$Load
    p.WriteInst(MovRegister(X1, X20));
    p.WriteInst(MovRegister(X2, X21));
    p.BranchLinkInst(game::Address::Dpr_UI_UIManager_LoadSpritePokemon_PokemonParam_UnityAction_Sprite); // Address of LoadPokemonIcon with the PokemonParam overload

    p.Seek(game::Address::Dpr_UI_PokemonIcon_Load_PokemonParam_UInt16_Int32_Int32 + 0x16C); // PokemonIcon$$Load
    p.WriteInst(MovRegister(X1, X20));
    p.WriteInst(MovRegister(X2, X22));
    p.BranchLinkInst(game::Address::Dpr_UI_UIManager_LoadSpritePokemon_PokemonParam_UnityAction_Sprite); // Address of LoadPokemonIcon with the PokemonParam overload

    // Dynamically grab the egg sprite for the Daycare Pokétch app
    PoketchAppEggChecker$$OnInitialize::InstallAtOffset(game::Address::Dpr_UI_PoketchAppEggChecker_OnInitialize);
};