#include "exlaunch.hpp"

#include "data/species.h"
#include "data/utils.h"

#include "externals/Dpr/UI/UIManager.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/UnityEngine/Events/UnityAction.h"
#include "logger/logger.h"

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
            case array_index(SPECIES, "Alcremie"):
            {
                // TODO: Change this to be a proper two digit number please :(
                uint32_t formArg = coreParam->GetMultiPurposeWork();
                assetName = System::String::Concat(data->fields.AssetName, System::String::Create("_0" + nn::to_string(formArg)));
            }
            break;

            default:
            {
                assetName = data->fields.AssetName;
            }
            break;
        }

        Logger::log("Looking for icon %s\n", assetName->asCString().c_str());
        auto sprite = __this->GetAtlasSprite(SpriteAtlasID::TEXTUREMASS, assetName);
        onComplete->Invoke((Il2CppObject*)sprite);
    }
};

void exl_form_arg_icons_main() {
    UIManager$$LoadSpritePokemon_PokemonParam::InstallAtOffset(0x017c3e40);
}