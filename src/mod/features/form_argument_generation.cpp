#include "exlaunch.hpp"

#include "data/species.h"
#include "data/utils.h"

#include "externals/DPData/Form_Enums.h"
#include "externals/Dpr/UI/ZukanInfo.h"
#include "externals/Dpr/UnderGround/UgPokeLottery.h"
#include "externals/PlayerWork.h"
#include "externals/Pml/Personal/GrowTableExtensions.h"
#include "externals/Pml/Personal/PersonalSystem.h"
#include "externals/Pml/PmlUse.h"
#include "externals/Pml/PokePara/Accessor.h"
#include "externals/Pml/PokePara/CalcTool.h"
#include "externals/Pml/PokePara/EggGenerator.h"
#include "externals/Pml/PokePara/EggParam.h"
#include "externals/Pml/PokePara/InitialSpec.h"
#include "externals/Pml/PokePara/Parents.h"
#include "externals/UnityEngine/Random.h"

#include "logger/logger.h"
#include "romdata/romdata.h"
#include "save/save.h"

void FormArgumentGeneration(Pml::PokePara::Accessor::Object* accessor, int32_t monsno, int32_t zoneID) {
    switch (monsno)
    {
        case array_index(SPECIES, "Smeargle"):
        {
            accessor->SetMultiPurposeWork(RollForSmeargleColor(zoneID));
        }
        break;

        case array_index(SPECIES, "Vivillon"):
        {
            accessor->SetMultiPurposeWork(accessor->GetFormNo());
        }
        break;

        default:
        {
            accessor->SetMultiPurposeWork(RollForVariant(monsno, accessor->GetFormNo(), zoneID));
        }
        break;
    }
}

HOOK_DEFINE_REPLACE(Factory$$InitCoreData) {
    static void Callback(System::Byte_array* coreData, Pml::PokePara::InitialSpec::Object* spec) {
        system_load_typeinfo(0x47e3);
        Pml::PmlUse::getClass()->initIfNeeded();
        Pml::PokePara::CalcTool::getClass()->initIfNeeded();
        Pml::Personal::PersonalSystem::getClass()->initIfNeeded();
        PlayerWork::getClass()->initIfNeeded();

        auto accessor = Pml::PokePara::Accessor::newInstance();
        accessor->AttachDecodedData(coreData, nullptr);
        if (spec->fields.monsno == array_index(SPECIES, "-SPECIES ZERO-"))
            return;

        accessor->SetPersonalRnd(spec->fields.personalRnd);
        accessor->SetID(spec->fields.id);
        accessor->SetColorRnd(spec->fields.rareRnd);
        accessor->SetMonsNo(spec->fields.monsno);

        // Form No
        switch (spec->fields.monsno)
        {
            case array_index(SPECIES, "Toxtricity"):
            {
                if (spec->fields.formno == 255)
                {
                    if (Pml::PokePara::CalcTool::IsSeikakuHigh(spec->fields.seikaku))
                        accessor->SetFormNo((uint16_t)ToxtricityForm::AMPED);
                    else if (Pml::PokePara::CalcTool::IsSeikakuLow(spec->fields.seikaku))
                        accessor->SetFormNo((uint16_t)ToxtricityForm::LOW_KEY);
                    else
                        accessor->SetFormNo(spec->fields.formno);
                }
                else
                {
                    accessor->SetFormNo(spec->fields.formno);
                }
            }
            break;

            default:
            {
                if (spec->fields.formno == 255)
                    accessor->SetFormNo(RollForForm(spec->fields.monsno, PlayerWork::get_zoneID()));
                else
                    accessor->SetFormNo(spec->fields.formno);
            }
            break;
        }

        accessor->SetLevel(spec->fields.level);
        accessor->SetSeikaku(spec->fields.seikaku);
        accessor->SetSeikakuHosei(spec->fields.seikaku);
        accessor->SetSex((Pml::Sex)spec->fields.sex);
        accessor->SetGetLevel(spec->fields.level);
        accessor->SetCassetteVersion(0);
        accessor->SetFriendship(spec->fields.friendship);
        accessor->SetLangID((uint8_t)Pml::PmlUse::get_Instance()->get_LangId());
        accessor->SetEquipRibbonNo(0xFF);
        accessor->SetTalentHp(spec->fields.talentPower->m_Items[0]);
        accessor->SetTalentAtk(spec->fields.talentPower->m_Items[1]);
        accessor->SetTalentDef(spec->fields.talentPower->m_Items[2]);
        accessor->SetTalentSpAtk(spec->fields.talentPower->m_Items[3]);
        accessor->SetTalentSpDef(spec->fields.talentPower->m_Items[4]);
        accessor->SetTalentAgi(spec->fields.talentPower->m_Items[5]);
        accessor->SetTokuseiNo(Pml::PokePara::CalcTool::GetTokuseiNo(spec->fields.monsno, accessor->GetFormNo(), spec->fields.tokuseiIndex));
        accessor->SetTokusei1Flag(spec->fields.tokuseiIndex == 0);
        accessor->SetTokusei2Flag(spec->fields.tokuseiIndex == 1);
        accessor->SetTokusei3Flag(spec->fields.tokuseiIndex == 2);
        accessor->SetFavoriteFlag(false);
        accessor->SetExp(Pml::Personal::GrowTableExtensions::GetMinExp(Pml::Personal::PersonalSystem::GetGrowTable(spec->fields.monsno, spec->fields.formno), spec->fields.level));
        accessor->SetTalentHeight(spec->fields.height);
        accessor->SetTalentWeight(spec->fields.weight);

        FormArgumentGeneration(accessor, spec->fields.monsno, PlayerWork::get_zoneID());
    }
};

HOOK_DEFINE_INLINE(EggGenerator$$CreateEgg_CoreParam_Variants) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto egg = (Pml::PokePara::CoreParam::Object*)ctx->X[0];
        auto egg_param = (Pml::PokePara::EggParam::Object*)ctx->X[1];
        auto parents = (Pml::PokePara::Parents::Object*)ctx->X[23];
        auto setting = (Pml::PokePara::EggGenerator::Setting::Object*)ctx->X[3];

        Pml::PokePara::EggGenerator::SetupEggParam(egg, egg_param, parents, setting);

        bool fastMode = egg->fields.m_accessor->IsFastMode();
        if (!fastMode)
            egg->fields.m_accessor->StartFastMode();

        // Inherit variant
        auto targetParent = parents->fields.mother->GetMonsNo() == array_index(SPECIES, "Ditto") ? parents->fields.father : parents->fields.mother;
        egg->fields.m_accessor->SetMultiPurposeWork(targetParent->fields.m_accessor->GetMultiPurposeWork());

        if (!fastMode && egg->fields.m_accessor->IsFastMode())
            egg->fields.m_accessor->EndFastMode();
    }
};

HOOK_DEFINE_TRAMPOLINE(ZukanInfo$$GetCurrentPokemonParam) {
    static Pml::PokePara::PokemonParam::Object* Callback(Dpr::UI::ZukanInfo::Object* __this) {
        auto param = Orig(__this);
        param->fields.m_accessor->SetMultiPurposeWork(0); // Hardcode the variant to 0 in the dex
        return param;
    }
};

HOOK_DEFINE_TRAMPOLINE(Dpr_UnderGround_UgPokeLottery$$CreatePokemonParam_by_Tokusei) {
    static Pml::PokePara::PokemonParam::Object* Callback(Dpr::UnderGround::UgPokeLottery::Object*__this, int32_t monsNo, uint8_t rareTryCount) {
        auto param = Orig(__this, monsNo, rareTryCount);

        // Regenerate the form argument again for the room we're transitioning to in the Underground
        FormArgumentGeneration(param->fields.m_accessor, param->cast<Pml::PokePara::CoreParam>()->GetMonsNo(), PlayerWork::get_transitionZoneID());

        return param;
    }
};


void exl_form_arg_generation_main() {
    Factory$$InitCoreData::InstallAtOffset(0x02054140);
    EggGenerator$$CreateEgg_CoreParam_Variants::InstallAtOffset(0x0204e28c);
    ZukanInfo$$GetCurrentPokemonParam::InstallAtOffset(0x01bb04e0);

    Dpr_UnderGround_UgPokeLottery$$CreatePokemonParam_by_Tokusei::InstallAtOffset(0x018bfea0);
}