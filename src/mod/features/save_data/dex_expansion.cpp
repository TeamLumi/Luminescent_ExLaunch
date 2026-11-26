#include "exlaunch.hpp"

#include "externals/Dpr/UI/UIText.h"
#include "externals/Dpr/UI/ZukanInfo.h"
#include "save/save.h"

#include "logger/logger.h"

void ZukanInfo_SetupUITexts_SetupMessage(Dpr::UI::UIText::Object* __this, System::String::Object* messageFile, int32_t monsno, Dpr::UI::ZukanInfo::Object* zukanInfo)
{
    int32_t currentIndex = zukanInfo->fields.modelIndexSelector->fields._CurrentIndex_k__BackingField;
    Dpr::UI::ZukanInfo::ModelParam::Object * modelParam = zukanInfo->fields.modelParams->m_Items[currentIndex];

    int32_t messageIndex;
    if (modelParam->fields.FormNo == 0 || zukanInfo->fields.formIndex == 0)
    {
        messageIndex = monsno;
    }
    else
    {
        messageIndex = zukanInfo->fields.formIndex + modelParam->fields.FormNo - 1;
    }

    __this->SetupMessage(messageFile, messageIndex);
}

void exl_save_dex_expansion_main() {
    // Patch dex limit checks
    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p;
    auto inst = nn::vector<exl::patch::Instruction> {
        { game::Address::ZukanWork_Reset + 0x98, Movz(W19, DexSize) },
        { game::Address::ZukanWork_SetPoke_UInt32_Int32_UInt8_Int32_Bool + 0x5C, CmpImmediate(W19, DexSize - 1) },
        { game::Address::ZukanWork_AddLangFlag + 0x44, CmpImmediate(W20, DexSize - 1) },
        { game::Address::ZukanWork_IsGet_UInt32 + 0x40, CmpImmediate(W19, DexSize - 1) },
        { game::Address::ZukanWork_IsSee + 0x40, CmpImmediate(W19, DexSize - 1) },
        { game::Address::ZukanWork_IsUwasa + 0x40, CmpImmediate(W19, DexSize - 1) },
        { game::Address::ZukanWork_GetStatus + 0x40, CmpImmediate(W19, DexSize - 1) },
        { game::Address::ZukanWork_IsGet_UInt32_UInt8_Int32_Bool + 0xA4, CmpImmediate(W20, DexSize - 1) },
        { game::Address::ZukanWork_GetViewModelUniqueID + 0x38, CmpImmediate(W19, DexSize - 1) },
        { game::Address::ZukanWork_SetViewModelUniqueID + 0x40, CmpImmediate(W20, DexSize - 1) },
        { game::Address::ZukanWork_IsLangFlag + 0x44, CmpImmediate(W20, DexSize - 1) },
        { game::Address::ZukanWork_cctor + 0x120, Movz(W1, DexSize) },
        { game::Address::UgMainProc_CreatePoke + 0x9C, CmpImmediate(W0, 0) },
        { game::Address::UgMainProc_LoadSymbols + 0x1C0, CmpImmediate(W8, DexSize - 1) },
        { game::Address::PokemonSelector_Update + 0x28, CmpImmediate(W9, DexSize + 1) },
        { game::Address::PokemonSelector_Update + 0x34, SubImmediate(W8, W8, DexSize - 1) },
        { game::Address::PokemonSelector_Update + 0x68, AddImmediate(W8, W8, DexSize - 1) },
        { game::Address::Dpr_UI_UIZukan_SetupZukan + 0x110, CmpImmediate(W22, DexSize + 1) },
        { game::Address::Dpr_Field_Walking_FieldWalkingManager_UpdatePartnerPokeIndex + 0x6C, CmpImmediate(W8, DexSize - 1) },
        { game::Address::Dpr_Field_Walking_FieldWalkingManager_IsPokeParaOK + 0x44, CmpImmediate(W8, DexSize) },
        { game::Address::Dpr_Field_Walking_FieldWalkingManager_LoadPartnerPoke + 0xF8, CmpImmediate(W8, DexSize - 1) },
        { game::Address::Dpr_Field_Walking_FieldWalkingManager_CheckPartnerPokeChange + 0x10C, CmpImmediate(W8, DexSize - 1) },
        { game::Address::Dpr_Field_Walking_FieldWalkingManager_IsCanTurearuki + 0x4C, CmpImmediate(W8, DexSize - 1) },
        { game::Address::Dpr_Field_Walking_FieldWalkingManager_CreatePartner_d_43_MoveNext + 0x18C, CmpImmediate(W8, DexSize - 1) },
        { game::Address::Dpr_Field_Walking_FieldWalkingManager_CreatePartner_d_43_MoveNext + 0x348, CmpImmediate(W8, DexSize - 1) },
        { game::Address::Dpr_Field_Walking_FieldWalkingManager_CreatePartner_d_43_MoveNext + 0x5D8, CmpImmediate(W8, DexSize - 1) },
        { game::Address::Dpr_Battle_Logic_tables_cctor + 0x850, Movz(W8, DexSize) },
        { game::Address::Dpr_Battle_Logic_MainModule_setupCommon_ClientPublicInformation + 0x1B0, Movz(W26, DexSize + 1) },
        { game::Address::Pml_PokePara_CoreParam_ChangeEgg + 0x9C, Movz(W0, DexSize + 1) },
        { game::Address::Pml_PokePara_EggGenerator_SetupEggParam + 0xD0, Movz(W0, DexSize + 1) },
        { game::Address::Pml_PokePara_EggGenerator_IsWazaMachineAbleToUse + 0x40, CmpImmediate(W21, DexSize + 1) },
        { game::Address::Pml_PokePara_InitialSpec_ctor + 0x50, Movz(W8, DexSize + 1) },
        { game::Address::Pml_Personal_PersonalSystem_GetMonsLabel + 0x4, CmpImmediate(W8, DexSize + 1) },
        { game::Address::Pml_Personal_PersonalSystem_GetMonsLabel + 0x14, CmpImmediate(W0, DexSize) },
        { game::Address::Pml_Personal_PersonalSystem_GetMonsName_Int32 + 0x8C, CmpImmediate(W8, DexSize + 1) },
        { game::Address::Pml_Personal_PersonalSystem_GetMonsName_Int32 + 0x90, Movz(W8, DexSize) },
        { game::Address::Pml_Personal_PersonalSystem_GetMonsName_Int32_Int32 + 0xD4, CmpImmediate(W8, DexSize + 1) },
        { game::Address::Pml_Personal_PersonalSystem_GetMonsName_Int32_Int32 + 0xD8, Movz(W8, DexSize) },
        { game::Address::Pml_Personal_PersonalSystem_GetDataID + 0x5C, CmpImmediate(W19, DexSize) },
        { game::Address::Pml_Personal_PersonalSystem_GetDataID + 0x104, CmpImmediate(W19, DexSize) },
        { game::Address::Pml_Personal_PersonalSystem_GetDataID_AddPersonal + 0x5C, CmpImmediate(W19, DexSize) },
        { game::Address::Pml_Personal_PersonalSystem_GetDataID_AddPersonal + 0x158, CmpImmediate(W19, DexSize) },
        { game::Address::Pml_PokePara_Accessor_Initialize + 0x178, Movz(W9, DexSize + 1) },
        { game::Address::Pml_PokePara_Accessor_GetNickName + 0x188, Movz(W0, DexSize + 1) },
        { game::Address::ZukanWork_Reset + 0x98, Movz(W19, DexSize)}, // Dex Array Expansion
    };
    p.WriteInst(inst);

    // Patch dex description for new forms (?)
    p.Seek(game::Address::Dpr_UI_ZukanInfo_SetupUITexts + 0x244);
    p.WriteInst(MovRegister(X3, X21));
    p.BranchLinkInst((void*)&ZukanInfo_SetupUITexts_SetupMessage);
}
