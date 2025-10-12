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
#include "externals/XLSXContent/FieldCommonParam.h"
#include "features/commands/utils/utils.h"
#include "externals/GameData/DataManager.h"
#include "externals/Dpr/UI/PoketchAppMapMarking.h"
#include "externals/FieldObjectEntity.h"
#include "externals/FieldManager.h"
#include "externals/UnityEngine/Time.h"
#include "externals/UnityEngine/UI/Image.h"
#include "data/species.h"
#include "save/save.h"
#include "data/zones.h"

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

UnityEngine::Vector3::Object CalculateCursorPosition(UnityEngine::Vector2::Fields basePosition, UnityEngine::Vector2::Fields mapPos, int32_t gridSize) {
    UnityEngine::Vector3::Object position {};
    position.fields.x = basePosition.x + (float)(gridSize * mapPos.x);
    position.fields.y = basePosition.y - (float)(gridSize * mapPos.y);
    position.fields.z = 0.0f;
    return position;
}


void UpdateWanderingPokemonImages(Dpr::UI::PoketchAppMapMarking::Object* __this) {
    for (size_t i = 0; i < __this->fields._pokeZoneIDs->max_length; i++) {
        Logger::log("[UpdateWander] Loop i: %d.\n", i);
        int32_t zoneID = EncountDataWork::GetMovePoke_ZoneID(i);
        __this->fields._pokeZoneIDs->m_Items[i] = zoneID;
        auto wanderingImage = __this->fields._wanderingPokemonImages->m_Items[i];
        if (wanderingImage != nullptr) {

            for (size_t j = 0; i < __this->fields._mapDatas->max_length; j++) {
                Logger::log("[UpdateWander] Loop i: %d, j: %d.\n", i, j);
                auto mapData = __this->fields._mapDatas->m_Items[j];

                if (mapData->fields.zoneID == zoneID) {
                    Logger::log("[UpdateWander] Found at index: %d.\n", i);
                    UnityEngine::Vector3::Object cursorPos = CalculateCursorPosition(
                            __this->fields._grigBasePosition.fields, mapData->fields.MarkingMapPosXZ.fields,
                            __this->fields._gridSize);
                    wanderingImage->cast<UnityEngine::Component>()->get_transform()->set_localPosition(cursorPos);

                    bool isVisible = (__this->fields._blinkCount < __this->fields._blinkSpeed * 2);
                    wanderingImage->cast<UnityEngine::Behaviour>()->set_enabled(isVisible);
                    break;
                }
            }
        }
    }
}

bool IsPlayerOnMap(XLSXContent::TownMapTable::Sheetdata::Object* mapData, UnityEngine::Vector2Int::Object gridCoordinates, int32_t gridSize) {
    int32_t adjustedX = gridCoordinates.fields.m_X / gridSize;
    int32_t adjustedY = gridCoordinates.fields.m_Y / gridSize;

    int32_t xIndex = (adjustedX + 31) >> 5;
    int32_t yIndex = (adjustedY + 31) >> 5;

    int32_t mapIndex = mapData->fields.Width * yIndex + xIndex;

    return (mapIndex == mapData->fields.Index1 || mapIndex == mapData->fields.Index2 || mapIndex == mapData->fields.Index3);
}

HOOK_DEFINE_REPLACE(FieldEncount$$CheckMovePokeEnc) {
    static int32_t Callback() {
        system_load_typeinfo(0x48bf);
        int32_t moveZoneID;
        int32_t playerZoneID = PlayerWork::get_zoneID();
        Logger::log("[CheckFieldEnc] PlayerZone: %d.\n", playerZoneID);
        auto canEncounter = System::Boolean_array::newArray(RoamerCount);

        for (size_t i=0; i < canEncounter->max_length; i++) {
            moveZoneID = EncountDataWork::GetMovePoke_ZoneID(i);
            if (moveZoneID == playerZoneID) {
                DPData::MV_POKE_DATA::Object mvData {};
                bool getMoveData = EncountDataWork::GetMovePokeData(i, &mvData);
                canEncounter->m_Items[i] = getMoveData && mvData.fields.EncountStatus == 1;
                Logger::log("[CheckFieldEnc] Index: %d | getMoveData: %s | EncountStatus: %hhu | canEncounter: %s.\n", i,
                            getMoveData ? "True" : "False", mvData.fields.EncountStatus, canEncounter->m_Items[i] ? "True" : "False");
            }
        }

        int32_t randomValue = RandomGroupWork::RandomValue(100);
        Logger::log("[CheckFieldEnc] Random Value: %d.\n", randomValue);
        auto fieldParam = GameData::DataManager::getClass()->static_fields->FieldCommonParam->get_Item(0x2d);
        if (randomValue < static_cast<int32_t>(fieldParam->fields.param)) {
            return -1;
        }

        uint32_t randomRange = RandomGroupWork::RandomRange(0, RoamerCount);

        while (randomRange < canEncounter->max_length) {
            Logger::log("[CheckFieldEnc] Random Range: %d.\n", randomRange);
            if (canEncounter->m_Items[randomRange]) {
                Logger::log("[CheckFieldEnc] Returning %d.\n", randomRange);
                return randomRange;
            }

            uint32_t nextIndex = ((randomRange + 1) & 1) ? randomRange + 2 : randomRange + 1;
            randomRange = nextIndex;
        }
        return -1;
    }
};

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
        Logger::log("[MovePoke] %s moved to %s.\n", SPECIES[monsno], ZONES[zoneID]);
    }
};

HOOK_DEFINE_TRAMPOLINE(MovePoke_RandomZone2) {
    static void Callback(int32_t index) {
        Orig(index);
        int32_t monsno = PlayerWork::get_Enc_SV_Data().fields.MovePokeData->m_Items[index].fields.MonsNo;
        int32_t zoneIDIndex = PlayerWork::get_Enc_SV_Data().fields.MovePokeData->m_Items[index].fields.ZoneIDIndex;
        Logger::log("[MovePoke] %s moved to %s.\n", SPECIES[monsno], ZONES[zoneIDIndex]);
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
                auto roamerData = GetRoamerData();
                auto party = Dpr::EncountTools::CreateSimpleParty(monsNo, roamerData[index].lvl, 0,
                                                                  1, nullptr, 255, -1, 255,
                                                                  -1, 0, 0, 3);

                party->GetMemberPointer(0)->fields.m_accessor->SetFormNo(roamerData[index].formNo);
                for (int32_t i = 0; i < 4; i++) {
                    if (roamerData[index].moves[i] != 0) {
                        party->GetMemberPointer(0)->cast<Pml::PokePara::CoreParam>()->SetWaza(i, roamerData[index].moves[i]);
                    }
                }

                EncountDataWork::SetMovePokeData_Status(index, '\x01');
                EncountDataWork::SetMovePokeData(party->GetMemberPointer(0), true);
                EncountDataWork::MovePoke_RandomZone(monsNo);
            }
        }

        return true;
    }
};

HOOK_DEFINE_REPLACE(OnUpdateDraw) {
    static void Callback(Dpr::UI::PoketchAppMapMarking::Object* __this) {
        auto pos = (UnityEngine::Vector3::Object) {
            .fields = {
                    .x = 0.0,
                    .y = 0.0,
                    .z = 0.0
            }
        };

        float blinkCount = __this->fields._blinkCount;
        float blindSpeed = __this->fields._blinkSpeed;
        int32_t zoneId = 0;

        FieldManager::getClass()->initIfNeeded();
        FieldManager::getClass()->static_fields->_Instance_k__BackingField->GetTownMapPos(&zoneId, &pos, false);

        auto gridPos = FieldObjectEntity::PositionToGrid(pos);

        blinkCount -= UnityEngine::Time::get_deltaTime();
        if (blinkCount < 0.0f) {
            blinkCount = blindSpeed * 3.0f;
        }
        __this->fields._blinkCount = blinkCount;
        __this->fields._playerCursorImage->cast<UnityEngine::Behaviour>()->set_enabled(blinkCount < blindSpeed);


        UpdateWanderingPokemonImages(__this);

        for (size_t i = 0; i < __this->fields._mapDatas->max_length; i++) {
            auto mapData = __this->fields._mapDatas->m_Items[i];
            if (mapData != nullptr && IsPlayerOnMap(mapData, gridPos, __this->fields._gridSize)) {
                Logger::log("[UpdateDraw] Player is on Map.\n");
                auto cursorPos = (UnityEngine::Vector3::Object) {
                    .fields {
                        .x = (__this->fields._grigBasePosition.fields.x + static_cast<float>(__this->fields._gridSize * mapData->fields.MarkingMapPosXZ.fields.x)),
                        .y = (__this->fields._grigBasePosition.fields.y + static_cast<float>(__this->fields._gridSize * mapData->fields.MarkingMapPosXZ.fields.y)),
                        .z = 0.0f
                    }
                };
                __this->fields._playerCursorRoot->cast<UnityEngine::Transform>()->set_localPosition(cursorPos);
                return;
            }
            else {
                Logger::log("[UpdateDraw] Player not on Map or nullptr.\n");
            }
        }

        FieldManager::getClass()->static_fields->_Instance_k__BackingField->GetTownMapPos(&zoneId, &pos, false);
        gridPos = FieldObjectEntity::PositionToGrid(pos);

        for (size_t i = 0; i < __this->fields._mapDatas->max_length; i++) {
            auto mapData = __this->fields._mapDatas->m_Items[i];

            if (mapData != nullptr && IsPlayerOnMap(mapData, gridPos, __this->fields._gridSize)) {
                auto cursorPos = (UnityEngine::Vector3::Object) {
                        .fields {
                                .x = (__this->fields._grigBasePosition.fields.x + static_cast<float>(__this->fields._gridSize * mapData->fields.MarkingMapPosXZ.fields.x)),
                                .y = (__this->fields._grigBasePosition.fields.y + static_cast<float>(__this->fields._gridSize * mapData->fields.MarkingMapPosXZ.fields.y)),
                                .z = 0.0f
                        }
                };
                __this->fields._playerCursorRoot->cast<UnityEngine::Transform>()->set_localPosition(cursorPos);
                return;
            }
        }

    }
};

HOOK_DEFINE_INLINE(PoketchAppMapMarking$$ctor) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        ctx->X[0] = reinterpret_cast<u64>(System::Int32_array::newArray(RoamerCount));
    }
};

HOOK_DEFINE_TRAMPOLINE(OnOpen) {
    static void Callback(Dpr::UI::PoketchAppMapMarking::Object* __this) {
        auto imageArray = UnityEngine::UI::Image::newArray(RoamerCount);
        auto root = reinterpret_cast<UnityEngine::Transform::Object*>(__this->fields._wanderingPokemonImages);
        for (size_t i = 0; i < RoamerCount; i++) {

            if (i < 2) {
                imageArray->m_Items[i] = root->GetChild(i)->GetComponent(UnityEngine::Component::Method$$Image$$GetComponent);
            }

            else {
                auto child = root->Find(System::String::Create("Image_Pokemon_02"));
                auto newTransform = UnityEngine::_Object::Instantiate<UnityEngine::Transform>(child);
                newTransform->SetParent(root, false);
                newTransform->SetSiblingIndex(i);
                imageArray->m_Items[i] = newTransform->GetComponent(UnityEngine::Component::Method$$Image$$GetComponent);
            }

        }

        __this->fields._wanderingPokemonImages = imageArray;

        Orig(__this);

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
    MovePoke_RandomZone2::InstallAtOffset(0x01f09110);
    SetMovePokeData::InstallAtOffset(0x01f08b60);
    EvCmdAddMovePoke::InstallAtOffset(0x02c7e290);
    EncountDataWork$$Create::InstallAtOffset(0x01f08790);
    FieldEncount$$CheckMovePokeEnc::InstallAtOffset(0x019b4500);

    PoketchAppMapMarking$$ctor::InstallAtOffset(0x01a0ac60);
    OnUpdateDraw::InstallAtOffset(0x01a0a1b0);
    //OnOpen::InstallAtOffset(0x01a09cd0);

    //Todo  EncountDataWork$$FukkatuMvPoke
}
