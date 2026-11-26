#include "exlaunch.hpp"
#include "externals/Dpr/UI/UIBag.h"
#include "externals/Pml/PokePara/CoreParam.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/Pml/Personal/PersonalSystem.h"

// Checks if Ability Patch can be used.
HOOK_DEFINE_REPLACE(AbilityPatchUsability) {
static bool Callback(Dpr::UI::UIBag::__c__DisplayClass135_0::Object *bagObject, Pml::PokePara::PokemonParam::Object *pp)
    {
        system_load_typeinfo(game::TypeInfo::Dpr_UI_UIBag_c_DisplayClass135_0_UseApplicationItem_b_1);

        auto coreParam = reinterpret_cast<Pml::PokePara::CoreParam::Object *>(pp);

        int32_t monsno = coreParam->GetMonsNo();
        int32_t formno = coreParam->GetFormNo();

        Pml::Personal::PersonalSystem::LoadPersonalData(monsno, formno);
        int32_t pattern = Pml::Personal::PersonalSystem::GetTokuseiPattern();

        switch (pattern) {
            case 0: // A1 == A2 == HA
                return false;
            case 1: // A1 == A2 != HA
            case 2: // A1 != A2 != HA

                uint8_t abilitySlot = coreParam->GetTokuseiIndex();

                if (abilitySlot != 2) // Change to Hidden Ability
                {
                    coreParam->SetTokuseiIndex(2);
                } else // Change to first Ability
                {
                    coreParam->SetTokuseiIndex(0);
                }
                return true;
        }

        return false;
    }
};

void exl_items_ability_patch_main(){
    AbilityPatchUsability::InstallAtOffset(game::Address::Dpr_UI_UIBag_c_DisplayClass135_0_UseApplicationItem_b_1);
};