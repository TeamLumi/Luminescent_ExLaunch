#include "exlaunch.hpp"

#include "externals/Dpr/Message/MessageManager.h"
#include "externals/GameData/DataManager.h"
#include "externals/MonsLv.h"
#include "externals/Pml/Personal/PersonalSystem.h"
#include "externals/Pml/Personal/SexVector.h"
#include "externals/Pml/Sex.h"
#include "externals/System/String.h"
#include "externals/TairyouHasseiPokeManager.h"
#include "externals/XLSXContent/PersonalTable.h"
#include "externals/XLSXContent/PokemonInfo.h"

HOOK_DEFINE_INLINE(TairyouHasseiPokeManager_Loading) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto pokeManager = (TairyouHasseiPokeManager::Object*)ctx->X[19];
        auto monsLv = (MonsLv::Array*)ctx->X[20];

        int32_t monsNo = (monsLv->m_Items[0].fields.monsNo) & 0x0000FFFF;
        int32_t formNo = ((monsLv->m_Items[0].fields.monsNo) & 0xFFFF0000) >> 16;

        XLSXContent::PersonalTable::SheetPersonal::Object* personal = Pml::Personal::PersonalSystem::GetPersonalData(monsNo, formNo);
        uint8_t sexRate = personal->fields.sex;
        Pml::Sex sex;
        switch (sexRate)
        {
            case (uint8_t)Pml::Personal::SexVector::ONLY_FEMALE:
                sex = Pml::Sex::FEMALE;
                break;

            case (uint8_t)Pml::Personal::SexVector::UNKNOWN:
                sex = Pml::Sex::UNKNOWN;
                break;

            default:
                sex = Pml::Sex::MALE;
                break;
        }

        bool isRare = false;
        bool isEgg = false;

        XLSXContent::PokemonInfo::SheetCatalog::Object* catalog = GameData::DataManager::GetPokemonCatalog(monsNo, formNo, sex, isRare, isEgg);
        System::String::Object* assetBundleName = catalog->fields.AssetBundleName;

        System::String::Object* fullPath = System::String::Concat(System::String::Create("pokemons/field/"), assetBundleName);

        ctx->X[0] = (uint64_t)fullPath;
    }
};

HOOK_DEFINE_INLINE(Dpr_Message_MessageWordSetHelper_SetMonsNameWord) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto messageManager = (Dpr::Message::MessageManager::Object*)ctx->X[0];
        auto fileName = (System::String::Object*)ctx->X[1];
        auto fullMonsNo = (int32_t)ctx->W[2];

        uint16_t monsNo = fullMonsNo & 0x0000FFFF;
        uint16_t formNo = (fullMonsNo & 0xFFFF0000) >> 16;

        ctx->X[0] = (uint64_t)messageManager->GetNameMessageDataModel(fileName, monsNo);
    }
};

HOOK_DEFINE_TRAMPOLINE(Fix_Swarm_Zone) {
    static int32_t Callback() {
        Orig();
        return 385;
    }
};

void exl_swarm_forms_main() {
    TairyouHasseiPokeManager_Loading::InstallAtOffset(0x02caee94);
    Dpr_Message_MessageWordSetHelper_SetMonsNameWord::InstallAtOffset(0x01f9a0e0);
    Fix_Swarm_Zone::InstallAtOffset(0x017da6f0);
}
