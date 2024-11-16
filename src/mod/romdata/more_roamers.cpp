#include "exlaunch.hpp"
#include "memory/json.h"
#include "helpers/fsHelper.h"
#include "data/RoamerData.h"
#include "externals/GameManager.h"
#include "externals/RandomGroupWork.h"
#include "externals/EncountDataWork.h"
#include "externals/Dpr/EncountTools.h"
#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/DPData/SWAY_GRASS_REC.h"
#include "features/commands/utils/utils.h"
#include "data/species.h"
#include "save/save.h"

const char* roamersFilePath = "rom:/Data/ExtraData/Roamers/roamers.json";
constexpr int32_t HoneyTreeCount = 21;

nn::vector<RomData::RoamerData> GetRoamerData() {
    nn::string filePath(roamersFilePath);

    nn::json j = FsHelper::loadJsonFileFromPath(filePath.c_str());
    if (j != nullptr && !j.is_discarded()) {
        return j["roamers"].get<nn::vector<RomData::RoamerData>>();

    }
    else {
        Logger::log("Error when parsing Roamer data!\n");
    }
}

int32_t GetRoamerIndex(int32_t monsNo) {
    auto encSVData = PlayerWork::get_Enc_SV_Data();
    for (int32_t i = 0; i < encSVData.fields.MovePokeData->max_length; i++) {
        bool foundMonsNo = encSVData.fields.MovePokeData->m_Items[i].fields.MonsNo == monsNo;
        if (foundMonsNo) {
            return i;
        }
    }

    return 0;
}

HOOK_DEFINE_REPLACE(EncountDataWork$$Create) {
    static void Callback() {
        system_load_typeinfo(0x3f32);

        auto honeyArray = DPData::HONEY_DATA::newArray(HoneyTreeCount);
        for (size_t i = 0; i < honeyArray->max_length; i++) {
            honeyArray->m_Items[i].fields.TblMonsNo = 0;
            honeyArray->m_Items[i].fields.Spreaded = false;
            honeyArray->m_Items[i].fields.Minutes = 0;
            honeyArray->m_Items[i].fields.RareLv = 0;
            honeyArray->m_Items[i].fields.SwayLv = 0;
        }

        auto swayGrassRec = DPData::SWAY_GRASS_REC::newArray(3);
        for (size_t i = 0; i < swayGrassRec->max_length; i++) {
            swayGrassRec->m_Items[i].fields.MonsNo = 0;
            swayGrassRec->m_Items[i].fields.Chain = 0;
        }

        DPData::PLAYER_ZONE_HIST::Object playerZoneHist {};
        playerZoneHist.fields.BeforeZone = 0x161;
        playerZoneHist.fields.OldZone = 0x161;

        auto mvPokeData = DPData::MV_POKE_DATA::newArray(RoamerCount);
        auto roamers = GetRoamerData();
        for (size_t i = 0; i < mvPokeData->max_length; i++) {
            mvPokeData->m_Items[i].fields.MonsNo = roamers[i].monsNo;
            Logger::log("[Roamers] Adding %s to roamers.\n", SPECIES[roamers[i].monsNo]);
            mvPokeData->m_Items[i].fields.EncountStatus = 0;
        }

        DPData::ENC_SV_DATA::Object encSvData {};
        encSvData.fields.HoneyTree.fields.HoneyData = honeyArray;
        encSvData.fields.SwayGrassHist.fields.SwayGrassRec = swayGrassRec;
        encSvData.fields.PlayerZoneHist = playerZoneHist;
        encSvData.fields.MovePokeData = mvPokeData;

        PlayerWork::set_Enc_SV_Data(&encSvData);

    }
};

HOOK_DEFINE_REPLACE(MovePoke_RandomZone) {
    static void Callback(int32_t monsno) {
        system_load_typeinfo(0x3f3e);
        GameManager::getClass()->initIfNeeded();
        auto encountTable = GameManager::get_fieldEncountTable();
        auto rand = RandomGroupWork::RandomValue(encountTable->fields.mvpoke->max_length);
        auto zoneID = encountTable->fields.mvpoke->m_Items[rand]->fields.zoneID;

        EncountDataWork::SetMovePokeData_ZoneID(GetRoamerIndex(monsno), zoneID);
    }
};


HOOK_DEFINE_REPLACE(SetMovePokeData) {
    static void Callback(Pml::PokePara::PokemonParam::Object* param, bool init) {
        system_load_typeinfo(0x3f43);
        auto coreParam = param->cast<Pml::PokePara::CoreParam>();
        int32_t index = GetRoamerIndex(coreParam->GetMonsNo());

        auto encSVData = PlayerWork::get_Enc_SV_Data();
        encSVData.fields.MovePokeData->m_Items[index].fields.RndSeed = coreParam->GetPersonalRnd();
        encSVData.fields.MovePokeData->m_Items[index].fields.Hp = !init ? coreParam->GetHp() : 999;
        encSVData.fields.MovePokeData->m_Items[index].fields.Lv = coreParam->GetLevel();
        encSVData.fields.MovePokeData->m_Items[index].fields.Sick = coreParam->GetSick();

        PlayerWork::set_Enc_SV_Data(&encSVData);
    }
};

HOOK_DEFINE_REPLACE(EvCmdAddMovePoke) {
    static bool Callback(Dpr::EvScript::EvDataManager::Object* manager) {
        system_load_typeinfo(0x43bb);
        Logger::log("_ADD_MOVE_POKE\n");
        EvData::Aregment::Array* args = manager->fields._evArg;

        if (args->max_length >= 2) {
            int32_t index = GetWorkOrIntValue(args->m_Items[1]);
            if (EncountDataWork::IsMovePokeData_ReActive(index)) {
                uint32_t monsNo = PlayerWork::get_Enc_SV_Data().fields.MovePokeData->m_Items[index].fields.MonsNo;
                RandomGroupWork::GetRandom(0);
                auto party = Dpr::EncountTools::CreateSimpleParty(monsNo, 50, 0,
                                                                  1, nullptr, 255, -1, 255,
                                                                  -1, 0, 0, 3);

                auto roamerData = GetRoamerData();
                party->GetMemberPointer(0)->fields.m_accessor->SetFormNo(roamerData[index].formNo);
                for (int32_t i = 0; i < 4; i++) {
                    if (roamerData[index].moves[i] != 0) {
                        party->GetMemberPointer(0)->cast<Pml::PokePara::CoreParam>()->SetWaza(i, roamerData[index].moves[i]);
                    }
                }

                EncountDataWork::SetMovePokeData_Status(index, 1);
                EncountDataWork::SetMovePokeData(party->GetMemberPointer(0), true);
                EncountDataWork::MovePoke_RandomZone(monsNo);
            }
        }

        return true;
    }
};



using namespace exl::armv8::inst;
using namespace exl::armv8::reg;

void EncountDataWork_ASM(exl::patch::CodePatcher p) {
    auto inst = nn::vector<exl::patch::Instruction> {
            {0x01f089a4, Movz(W1, RoamerCount)}, // $$Create
    };
    p.WriteInst(inst);
}

void exl_more_roamers_main() {
    exl::patch::CodePatcher p(0);

    /* Assembly Patch */
    EncountDataWork_ASM(p);

    /* Hooks */
    MovePoke_RandomZone::InstallAtOffset(0x01f09270);
    SetMovePokeData::InstallAtOffset(0x01f08b60);
    EvCmdAddMovePoke::InstallAtOffset(0x02c7e290);
    EncountDataWork$$Create::InstallAtOffset(0x01f08790);

    //Todo  EncountDataWork$$FukkatuMvPoke
}
