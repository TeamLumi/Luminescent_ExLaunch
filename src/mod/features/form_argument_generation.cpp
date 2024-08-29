#include "exlaunch.hpp"

#include "data/species.h"
#include "data/utils.h"

#include "externals/DPData/Form_Enums.h"
#include "externals/PlayerWork.h"
#include "externals/Pml/Personal/GrowTableExtensions.h"
#include "externals/Pml/Personal/PersonalSystem.h"
#include "externals/Pml/PmlUse.h"
#include "externals/Pml/PokePara/Accessor.h"
#include "externals/Pml/PokePara/CalcTool.h"
#include "externals/Pml/PokePara/InitialSpec.h"
#include "logger/logger.h"
#include "romdata/romdata.h"

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
        accessor->SetTokuseiNo(Pml::PokePara::CalcTool::GetTokuseiNo(spec->fields.tokuseiIndex, spec->fields.tokuseiIndex, spec->fields.tokuseiIndex));
        accessor->SetTokusei1Flag(spec->fields.tokuseiIndex == 0);
        accessor->SetTokusei2Flag(spec->fields.tokuseiIndex == 1);
        accessor->SetTokusei3Flag(spec->fields.tokuseiIndex == 2);
        accessor->SetFavoriteFlag(false);
        accessor->SetExp(Pml::Personal::GrowTableExtensions::GetMinExp(Pml::Personal::PersonalSystem::GetGrowTable(spec->fields.monsno, spec->fields.formno), spec->fields.level));
        accessor->SetTalentHeight(spec->fields.height);
        accessor->SetTalentWeight(spec->fields.weight);

        // Form Argument
        switch (spec->fields.monsno)
        {
            case array_index(SPECIES, "Magikarp"):
            case array_index(SPECIES, "Scatterbug"):
            case array_index(SPECIES, "Spewpa"):
            case array_index(SPECIES, "Alcremie"):
            {
                accessor->SetMultiPurposeWork(RollForVariant(spec->fields.monsno, spec->fields.formno, PlayerWork::get_zoneID()));
            }
            break;

            default:
            {
                accessor->SetMultiPurposeWork(0);
            }
            break;
        }
    }
};

void exl_form_arg_generation_main() {
    Factory$$InitCoreData::InstallAtOffset(0x02054140);
}