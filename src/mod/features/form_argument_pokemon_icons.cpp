#include "exlaunch.hpp"

#include "data/species.h"
#include "data/utils.h"

#include "externals/Dpr/UI/UIManager.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/UnityEngine/Events/UnityAction.h"
#include "logger/logger.h"

System::String::Object* GetEggSpriteName(Pml::PokePara::CoreParam::Object* coreParam, System::String::Object* baseName)
{
    uint8_t type = coreParam->GetType1();
    return System::String::Concat(baseName, System::String::Create("_" + nn::to_string(type, "%02d")));
}

System::String::Object* GetVariantSpriteName(Pml::PokePara::CoreParam::Object* coreParam, System::String::Object* baseName)
{
    uint32_t formArg = coreParam->GetMultiPurposeWork();
    return System::String::Concat(baseName, System::String::Create("_" + nn::to_string(formArg, "%02d")));
}

System::String::Object* GetSpriteNameForMonWithVariant(Pml::PokePara::CoreParam::Object* coreParam, System::String::Object* baseName)
{
    if (coreParam->IsEgg(Pml::PokePara::EggCheckType::BOTH_EGG))
        return GetEggSpriteName(coreParam, baseName);
    else
        return GetVariantSpriteName(coreParam, baseName);
}

System::String::Object* GetSpriteNameForMonWithoutVariant(Pml::PokePara::CoreParam::Object* coreParam, System::String::Object* baseName)
{
    if (coreParam->IsEgg(Pml::PokePara::EggCheckType::BOTH_EGG))
        return GetEggSpriteName(coreParam, baseName);
    else
        return baseName;
}

HOOK_DEFINE_REPLACE(UIManager$$LoadSpritePokemon_PokemonParam) {
    static void Callback(Dpr::UI::UIManager::Object* __this, Pml::PokePara::PokemonParam::Object* pokemonParam, UnityEngine::Events::UnityAction::Object* onComplete) {
        system_load_typeinfo(0x9c0b);

        auto coreParam = (Pml::PokePara::CoreParam::Object*)pokemonParam;
        int32_t monsno = coreParam->GetMonsNo();
        uint16_t formno = coreParam->GetFormNo();
        Pml::Sex sex = coreParam->GetSex();
        Pml::PokePara::RareType rareType = coreParam->GetRareType();
        bool isEgg = coreParam->IsEgg(Pml::PokePara::EggCheckType::BOTH_EGG);

        auto data = __this->GetPokemonIconData(monsno, formno, sex, rareType, isEgg);
        System::String::Object* assetName = nullptr;

        switch (monsno)
        {
            case array_index(SPECIES, "Arbok"):
            case array_index(SPECIES, "Magikarp"):
            //case array_index(SPECIES, "Gyarados"): TODO: Eventually add Gyarados icons to this
            case array_index(SPECIES, "Alcremie"):
            {
                assetName = GetSpriteNameForMonWithVariant(coreParam, data->fields.AssetName);
            }
            break;

            case array_index(SPECIES, "Pikachu"):
            {
                if (coreParam->GetFormNo() == 7)
                    assetName = GetSpriteNameForMonWithVariant(coreParam, data->fields.AssetName);
                else
                    assetName = GetSpriteNameForMonWithoutVariant(coreParam, data->fields.AssetName);
            }
            break;

            case array_index(SPECIES, "Manaphy"):
            {
                // Don't do the special egg type logic
                assetName = data->fields.AssetName;
            }
            break;

            default:
            {
                assetName = GetSpriteNameForMonWithoutVariant(coreParam, data->fields.AssetName);
            }
            break;
        }

        auto sprite = __this->GetAtlasSprite(SpriteAtlasID::TEXTUREMASS, assetName);
        onComplete->Invoke((Il2CppObject*)sprite);
    }
};

void exl_form_arg_icons_main() {
    UIManager$$LoadSpritePokemon_PokemonParam::InstallAtOffset(0x017c3e40);
}