#include "exlaunch.hpp"

#include "externals/Dpr/UI/LevelUpPokemonPanel.h"
#include "externals/Dpr/UI/UIManager.h"
#include "externals/UnityEngine/UI/Image.h"

UnityEngine::Transform::Object* NavigateToLevelUpImage(UnityEngine::Transform::Object* root) {
    return root->Find(System::String::Create("Panel"))
        ->Find(System::String::Create("Base"))
        ->Find(System::String::Create("LevelUp"))
        ->Find(System::String::Create("TextImage"));
}

HOOK_DEFINE_TRAMPOLINE(LevelUpPokemonPanel$$Set) {
    static void Callback(Dpr::UI::LevelUpPokemonPanel::Object* __this, Pml::PokePara::PokemonParam::Object* param, int32_t index) {
        system_load_typeinfo(0xab8f);

        auto tf = NavigateToLevelUpImage(__this->cast<UnityEngine::Component>()->get_transform()->instance());
        auto image = tf->cast<UnityEngine::Component>()->GetComponent(UnityEngine::Component::Method$$Image$$GetComponent);

        Dpr::UI::UIManager::getClass()->initIfNeeded();
        auto sprite = Dpr::UI::UIManager::get_Instance()->GetAtlasSprite(SpriteAtlasID::COMMON_LANG, System::String::Create("cmn_txt_LvUp_01_01"));
        image->set_sprite(sprite);

        Orig(__this, param, index);
    }
};

void exl_language_ui_fixes_main() {
    LevelUpPokemonPanel$$Set::InstallAtOffset(0x01c5a4f0);
}
