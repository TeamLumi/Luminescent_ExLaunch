#include "exlaunch.hpp"
#include "externals/il2cpp-api.h"

#include "data/species.h"
#include "externals/Dpr/Contest/ContestDataModel.h"
#include "externals/Dpr/Contest/EntryPlayerData.h"

#include "logger/logger.h"

HOOK_DEFINE_REPLACE(Dpr_Contest_ContestDataModel_CreateNPCPlayerData_Predicate) {
    static bool Callback(Dpr::Contest::ContestDataModel::DisplayClass72_0::Object* __this, XLSXContent::ContestMasterDatas::SheetNPCLevelData::Object* x) {
        int32_t npcLevel = __this->fields.npcMDDatas->fields.monsNo & (0b111);
        return x->fields.npcLevel == npcLevel;
    }
};

HOOK_DEFINE_INLINE(Dpr_Contest_ContestDataModel_CreateNPCPlayerData_MonsNo) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto entryPlayerData = (Dpr::Contest::EntryPlayerData::Object*)ctx->X[22];
        auto displayClass = (Dpr::Contest::ContestDataModel::DisplayClass72_0::Object*)ctx->X[21];

        uint16_t isShiny = displayClass->fields.npcMDDatas->fields.monsNo & (0b1000);
        uint16_t monsNo = displayClass->fields.npcMDDatas->fields.npcLevel & (0x0000FFFF);
        uint16_t formNo = (displayClass->fields.npcMDDatas->fields.npcLevel & (0xFFFF0000)) >> 16;

        Logger::log("Set up NPC with: %s of form %d, %d shiny\n", SPECIES[monsNo], formNo, isShiny != 0);

        entryPlayerData->fields.monsNo = (int32_t)monsNo;
        entryPlayerData->fields.formNo = (uint32_t)formNo;
        entryPlayerData->fields.isRare = isShiny != 0;
        entryPlayerData->fields.rareType = isShiny != 0 ? 1 : 0;
    }
};

void exl_contest_npc_forms_main() {
    // Repoint npcLevel predicate check to monsno
    Dpr_Contest_ContestDataModel_CreateNPCPlayerData_Predicate::InstallAtOffset(0x01c71710);

    // Repoint monsno, formno, and shininess to proper data formatting
    Dpr_Contest_ContestDataModel_CreateNPCPlayerData_MonsNo::InstallAtOffset(0x01c6d974);

    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p(0x01c6e37c);
    p.WriteInst(Movz(W8, 1)); // Hardcode debug log to always use Bulbasaur in the enum
};