#include "exlaunch.hpp"
#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "externals/Pml/PokePara/CoreParam.h"
#include "externals/Pml/WazaData/WazaDataSystem.h"
#include "data/moves.h"
#include "data/utils.h"

static Pml::PokePara::CoreParam::Object * gPokemonParam;

uint8_t WazaDataSystem_GetType(int32_t id, Pml::PokePara::CoreParam::Object * pokemonParam)
{
    if (pokemonParam == nullptr || id != array_index(MOVES, "Hidden Power"))
    {
        return Pml::WazaData::WazaDataSystem::GetType(id);
    }

    return pokemonParam->GetMezapaType();
}

uint8_t WAZADATA_GetType(int32_t id, Dpr::Battle::Logic::BTL_POKEPARAM::Object * bpp)
{
    Pml::PokePara::CoreParam::Object * pokemonParam = (Pml::PokePara::CoreParam::Object *)bpp->GetSrcDataConst();

    return WazaDataSystem_GetType(id, pokemonParam);
}

uint8_t WazaManageSubWazaPanel_GetType(int32_t id)
{
    uint8_t type = WazaDataSystem_GetType(id, gPokemonParam);
    return type;
}

int32_t WazaManagePokemonStausPanel_GetWazaNo(Pml::PokePara::CoreParam::Object * coreParam, uint8_t index)
{
    gPokemonParam = coreParam;
    return coreParam->GetWazaNo(index);
}

void exl_hidden_power_ui_main(){
    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;

    exl::patch::CodePatcher p(game::Address::Dpr_UI_PokemonStatusWazaItem_Setup + 0xCC);
    p.WriteInst(MovRegister(X1, X21));

    p.Seek(game::Address::Dpr_UI_PokemonStatusWazaItem_Setup + 0xD4);
    p.BranchLinkInst((void*)&WazaDataSystem_GetType);

    p.Seek(game::Address::Dpr_Battle_View_UI_BUIWazaList_Initialize + 0x2DC);
    p.WriteInst(MovRegister(X1, X19));

    p.Seek(game::Address::Dpr_Battle_View_UI_BUIWazaList_Initialize + 0x2E4);
    p.BranchLinkInst((void*)&WAZADATA_GetType);

    p.Seek(game::Address::Dpr_Battle_Logic_BattleViewBase_GetUiDisplay_WazaType + 0x8);
    p.WriteInst(MovRegister(X1, X2));
    p.BranchInst((void*)&WazaDataSystem_GetType);

    p.Seek(game::Address::Dpr_UI_WazaManagePokemonStausPanel_Setup + 0x6B8);
    p.BranchLinkInst((void*)&WazaManagePokemonStausPanel_GetWazaNo);

    p.Seek(game::Address::Dpr_UI_WazaManageSubWazaPanel_Setup + 0xB4);
    p.BranchLinkInst((void*)&WazaManageSubWazaPanel_GetType);
};