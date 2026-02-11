#include "exlaunch.hpp"

#include "data/species.h"
#include "data/utils.h"

#include "externals/DPData/Form_Enums.h"
#include "externals/Dpr/UI/ZukanInfo.h"
#include "externals/Dpr/UnderGround/UgFieldManager.h"
#include "externals/Dpr/UnderGround/UgPokeLottery.h"
#include "externals/PlayerWork.h"
#include "externals/Pml/Local/RandomGenerator.h"
#include "externals/Pml/Personal/GrowTableExtensions.h"
#include "externals/Pml/Personal/PersonalSystem.h"
#include "externals/Pml/Personal/PersonalTableExtensions.h"
#include "externals/Pml/PmlUse.h"
#include "externals/Pml/PokePara/Accessor.h"
#include "externals/Pml/PokePara/CalcTool.h"
#include "externals/Pml/PokePara/EggGenerator.h"
#include "externals/Pml/PokePara/EggParam.h"
#include "externals/Pml/PokePara/Factory.h"
#include "externals/Pml/PokePara/InitialSpec.h"
#include "externals/Pml/PokePara/Parents.h"
#include "externals/UnityEngine/Random.h"

#include "logger/logger.h"
#include "romdata/romdata.h"
#include "save/save.h"

int32_t GetEncounterZoneForPokemonGeneration() {
    Dpr::UnderGround::UgFieldManager::getClass()->initIfNeeded();
    if (Dpr::UnderGround::UgFieldManager::get_isInstantiated() && PlayerWork::get_transitionZoneID() != -1)
        return PlayerWork::get_transitionZoneID();
    else
        return PlayerWork::get_zoneID();
}

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

void FormGeneration(Pml::PokePara::InitialSpec::Object* initSpec, Pml::Local::RandomGenerator::Object* random) {
    auto zone = GetEncounterZoneForPokemonGeneration();
    if (ExistFormRates(initSpec->fields.monsno, zone)) {
        initSpec->fields.formno = RollForForm(initSpec->fields.monsno, zone);
    }
    else {
        auto data = Pml::Personal::PersonalSystem::GetPersonalData(initSpec->fields.monsno, 0);
        auto maxForms = (uint8_t)Pml::Personal::PersonalTableExtensions::GetParam(data, 30);
        initSpec->fields.formno = (uint16_t)Pml::PokePara::Factory::GetRandom(random, maxForms);
    }
}

HOOK_DEFINE_INLINE(FixInitSpec_FormRolling) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        // This is only entered if the formno in the initSpec is 255

        auto initSpec = (Pml::PokePara::InitialSpec::Object*)ctx->X[19];
        auto random = (Pml::Local::RandomGenerator::Object*)ctx->X[20];

        switch (initSpec->fields.monsno)
        {
            case array_index(SPECIES, "Toxtricity"):
            {
                // Roll for a Toxtricity form and then roll for a nature pre-emptively to match the form
                initSpec->fields.formno = (uint16_t)Pml::PokePara::Factory::GetRandom(random, (uint16_t)ToxtricityForm::MAX);
                if (initSpec->fields.formno == (uint16_t)ToxtricityForm::AMPED) {
                    uint8_t num = 0;
                    auto possibleSeikaku = Pml::PokePara::CalcTool::GetSeikakuHigh(&num);
                    initSpec->fields.seikaku = (uint16_t)possibleSeikaku->m_Items[Pml::PokePara::Factory::GetRandom(random, num)];
                }
                else if (initSpec->fields.formno == (uint16_t)ToxtricityForm::LOW_KEY) {
                    uint8_t num = 0;
                    auto possibleSeikaku = Pml::PokePara::CalcTool::GetSeikakuLow(&num);
                    initSpec->fields.seikaku = (uint16_t)possibleSeikaku->m_Items[Pml::PokePara::Factory::GetRandom(random, num)];
                }
            }
            break;

            default:
            {
                FormGeneration(initSpec, random);
            }
            break;
        }
    }
};

HOOK_DEFINE_REPLACE(Factory$$InitCoreData) {
    static void Callback(System::Byte_array* coreData, Pml::PokePara::InitialSpec::Object* spec) {
        // This is POST FixInitSpec

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
        accessor->SetFormNo(spec->fields.formno);
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
        FormArgumentGeneration(param->fields.m_accessor, param->cast<Pml::PokePara::CoreParam>()->GetMonsNo(), GetEncounterZoneForPokemonGeneration());

        return param;
    }
};


void exl_form_arg_generation_main() {
    Factory$$InitCoreData::InstallAtOffset(0x02054140);
    FixInitSpec_FormRolling::InstallAtOffset(0x02053f8c);
    EggGenerator$$CreateEgg_CoreParam_Variants::InstallAtOffset(0x0204e28c);
    ZukanInfo$$GetCurrentPokemonParam::InstallAtOffset(0x01bb04e0);

    Dpr_UnderGround_UgPokeLottery$$CreatePokemonParam_by_Tokusei::InstallAtOffset(0x018bfea0);

    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p(0);
    p.Seek(0x02053f90);
    p.WriteInst(Branch(0xA8)); // Jump to 02054038
}