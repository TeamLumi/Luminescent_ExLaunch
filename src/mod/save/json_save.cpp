#include "exlaunch.hpp"
#include "externals/il2cpp-api.h"
#include "memory/json.h"
#include "externals/PlayerWork.h"
#include "externals/DPData/PLAYER_DATA.h"
#include "helpers/fsHelper.h"
#include "externals/System/Convert.h"
#include "save/save.h"

nn::string ByteArrayToBase64(System::Byte_array* inArray) {
    Logger::log("[ByteArrayToBase64] \n");
    return System::Convert::ToBase64String(inArray)->asCString();
}


nn::json GetArray(System::Int32_array* inArray) {
    Logger::log("[GetArray] \n");
    if (inArray != nullptr && inArray->max_length > 0) {
        auto int32Array = nn::vector<int32_t>();
        Logger::log("[GetArray] Pushing back %d items.\n", inArray->max_length);
        for (size_t i = 0; i < inArray->max_length; i++) {
            int32Array.push_back(inArray->m_Items[i]);
        }
        return int32Array;
    }
    return {};
}

nn::json GetArray(System::Boolean_array* inArray) {
    Logger::log("[GetArray] \n");
    if (inArray != nullptr && inArray->max_length > 0) {
        auto boolArray = nn::vector<bool>();
        Logger::log("[GetArray] Pushing back %zu items.\n", inArray->max_length);
        for (uint64_t i = 0; i < inArray->max_length; i++) {
            boolArray.push_back(inArray->m_Items[i]);
        }
        return boolArray;
    }
    return {};
}

nn::json GetLocationWork(DPData::LOCATION_WORK::Fields locationWorkFields) {
    return {
            {"zoneID", locationWorkFields.zoneID},
            {"posX", locationWorkFields.posX},
            {"posY", locationWorkFields.posY},
            {"posZ", locationWorkFields.posZ},
            {"dir", locationWorkFields.dir}
    };
}

nn::json GetWorpPoint(DPData::WorpPoint::Fields worpPointFields) {
    return {
            {"teleport", GetLocationWork(worpPointFields.teleport.fields)},
            {"zenmetu", GetLocationWork(worpPointFields.zenmetu.fields)},
            {"ananuke", GetLocationWork(worpPointFields.ananuke.fields)}
    };
}

nn::json GetGMTime(DPData::GMTIME::Fields gmTimeFields) {
    return {
            {"saveCount", gmTimeFields.saveCount},
            {"timestmp", gmTimeFields.timestmp}
    };
}

// version
// intValues
// boolValues
// systemFlags
// rivalName
// zoneID
// timeScale
// TODO: saveItem
// TODO: saveUgItem
// TODO: saveItemShortcut
// TODO: playerParty
// TODO: boxData
// TODO: boxTray
nn::json GetPlayerData() {
    PlayerWork::getClass()->initIfNeeded();
    auto config = (PlayerWork::get_config())->fields;
    auto myStatus = (PlayerWork::get_playerStatus())->fields;
    auto playTime = (PlayerWork::get_playerData())->fields.playtime.fields;
    auto contest_data = (PlayerWork::get_playerData())->fields.contestdata.fields;
    Logger::log("[PlayerData] \n");
    return {
            {
                    {"Config", {
                            {"msg_speed", config.msg_speed},
                            {"msg_lang_id", config.msg_lang_id},
                            {"is_kanji", config.is_kanji},
                            {"window_type", config.window_type},
                            {"wazaeff_mode", config.wazaeff_mode},
                            {"battle_rule", config.battle_rule},
                            {"party_and_box", config.party_and_box},
                            {"regist_nickname", config.regist_nickname},
                            {"gyrosensor", config.gyrosensor},
                            {"camerashake_of_fossil", config.camerashake_of_fossil},
                            {"camera_updown", config.camera_updown},
                            {"camera_leftright", config.camera_leftright},
                            {"autoreport", config.autoreport},
                            {"input_mode", config.input_mode},
                            {"show_nickname", config.show_nickname},
                            {"bgm_volume", config.bgm_volume},
                            {"se_volume", config.se_volume},
                            {"voice_volume", config.voice_volume}
                    }},
                    {"MyStatus", {
                            {"name", myStatus.name->asCString().c_str()},
                            {"id", myStatus.id},
                            {"gold", myStatus.gold},
                            {"sex", myStatus.sex},
                            {"region_code", myStatus.region_code},
                            {"badge", myStatus.badge},
                            {"trainer_view", myStatus.trainer_view},
                            {"rom_code", myStatus.rom_code},
                            {"dp_clear", myStatus.dp_clear},
                            {"body_type", myStatus.body_type},
                            {"fashion", myStatus.fashion},
                            {"defaultPokeType", myStatus.defaultPokeType},
                            {"dsPlayerFlag", myStatus.dsPlayerFlag},
                            {"turewalkMemberIndex", myStatus.turewalkMemberIndex},
                            {"grid", {
                                    {"m_X", myStatus.grid.fields.m_X},
                                    {"m_Y", myStatus.grid.fields.m_Y}
                            }},
                            {"height", myStatus.height},
                            {"rotation", myStatus.rotation}
                    }},
                    {"PlayTime", {
                            {"hour", playTime.hour},
                            {"minute", playTime.minute},
                            {"second", playTime.second}
                    }},
                    {"Contest_Data", {
                            {"photoDatas", 0}, // ToDo
                            {"contestRankPoint", contest_data.contestRankPoint}
                    }}
            }
    };
}
nn::json GetZukan(PlayerWork::SaveData::Fields saveData) { //ToDo
    auto zukanData = saveData.zukanData.fields;
    return {
            {"GET_STATUS", 0},
            {"male_color_flag", 0},
            {"female_color_flag", 0},
            {"male_flag", 0},
            {"female_flag", 0},
            {"UnknownTurn", 0},
            {"UnknownColor", 0},
            {"PowarunTurn", 0},
            {"PowarunColor", 0},
            {"DeokisisuTurn", 0},
            {"DeokisisuColor", 0},
            {"MinomuttiTurn", 0},
            {"MinomuttiColor", 0},
            {"MinomadamTurn", 0},
            {"MinomadamColor", 0},
            {"GaameiruTurn", 0},
            {"GaameiruColor", 0},
            {"TherimuTurn", 0},
            {"TherimuColor", 0},
            {"KaranakusiTurn", 0},
            {"KaranakusiColor", 0},
            {"ToritodonTurn", 0},
            {"ToritodonColor", 0},
            {"RotomTurn", 0},
            {"RotomColor", 0},
            {"GirathinaTurn", 0},
            {"GirathinaColor", 0},
            {"SheimiTurn", 0},
            {"SheimiColor", 0},
            {"AruseusuTurn", 0},
            {"AruseusuColor", 0},
            {"TextVersionUp", 0},
            {"zukan_get", zukanData.zukan_get},
            {"zenkoku_flag", zukanData.zenkoku_flag}
    };
}
// TODO: tr_battleData
// TODO: topMenuData
// TODO: fieldObj_Save
// TODO: record
// TODO: enc_sv_data
nn::json GetPlayerSaveData(PlayerWork::SaveData::Fields saveDataFields) {
    auto saveData = saveDataFields.player_save_data.fields;
    Logger::log("[PlayerSaveData] \n");

    return {
            {"gear_type", saveData.gear_type},
            {"shoes_flag", saveData.shoes_flag},
            {"form", saveData.form},
            {"bic_color", saveData.bic_color},
            {"worpPoint", GetWorpPoint(saveData.worpPoint.fields)},
            {"walkcount", saveData.walkcount},
            {"natuki_walkcnt", saveData.natuki_walkcnt},
            {"TownMapLocation", GetLocationWork(saveData.TownMapLocation.fields)},
            {"isTokushuLocation", saveData.isTokushuLocation},
            {"TokushuLocation", GetLocationWork(saveData.TokushuLocation.fields)}
    };
}
// TODO: ballDecoData
// TODO: saveSeal
// TODO: randomGroup
// TODO: fieldGimmickSaveData
// TODO: kinomiGrowSaveData
// TODO: poffinSaveData
// TODO: btlTowerSave
nn::json GetSystemData(PlayerWork::SaveData::Fields saveDataFields) {
    auto systemData = saveDataFields.systemData.fields;
    Logger::log("[SystemData] \n");
    return {
            {"StartTime", GetGMTime(systemData.StartTime.fields)},
            {"SaveTime", GetGMTime(systemData.SaveTime.fields)},
            {"PenaltyTime", GetGMTime(systemData.PenaltyTime.fields)},
            {"DailyEventLastTime", GetGMTime(systemData.DailyEventLastTime.fields)},
            {"nxSnapshot", ByteArrayToBase64(systemData.nxSnapshot)},
            {"fd_bgmEvnet", systemData.fd_bgmEvnet},
            {"reserved_long0", systemData.reserved_long0},
            {"reserved_long1", systemData.reserved_long1},
            {"reserved_long2", systemData.reserved_long2},
            {"reserved_long3", systemData.reserved_long3},
            {"reserved_long4", systemData.reserved_long4},
            {"reserved_long5", systemData.reserved_long5}
    };
}
// TODO: poketchData
// TODO: azukariyaData
// TODO: dendoudata
// TODO: badgeSaveData
// TODO: boukenNote
// TODO: tvDataOld
// TODO: ugSaveData
// TODO: gmsdata
// TODO: networkdata
// TODO: unionSaveData
// TODO: contstPhotoLangData
// TODO: zukanPersonalRndData
// TODO: contestPhotoExtData
// TODO: gmsPointExtData
// TODO: ugCountRecord
// TODO: reBuffNameData
// TODO: saveDataHash
// TODO: recodeAddData
// TODO: mysteryGiftSaveData
// TODO: poketoreCountArray
// TODO: playReportData
// TODO: mtData
// TODO: dendouSaveAdd
// TODO: tvData
// TODO: ballDecoExtraData

void JsonSaveTest(PlayerWork::Object* playerWork) {

    auto saveData = playerWork->fields._saveData.fields;

    nn::json wholeJson = {
            {"SaveData", {
                    {"version", saveData.version},
                    {"intValues", 0}, //GetArray(saveData.intValues)
                    {"boolValues", 0}, //GetArray(saveData.boolValues)
                    {"systemFlags", 0}, //GetArray(saveData.systemFlags)
                    {"rivalName", saveData.rivalName->asCString().c_str()},
                    {"zoneID", saveData.zoneID},
                    {"timeScale", saveData.timeScale},
                    {"saveItem", 0},
                    {"saveUgItem", 0},
                    {"saveItemShortcut", 0},
                    {"playerParty", 0},
                    {"boxData", 0},
                    {"boxTray", 0},
                    {"playerData", GetPlayerData()},
                    {"zukanData", 0},
                    {"tr_battleData", 0},
                    {"topMenuData", 0},
                    {"fieldObj_Save", 0},
                    {"record", 0},
                    {"enc_sv_data", 0},
                    {"player_save_data", GetPlayerSaveData(saveData)},
                    {"ballDecoData", 0},
                    {"saveSeal", 0},
                    {"randomGroup", 0},
                    {"fieldGimmickSaveData", 0},
                    {"kinomiGrowSaveData", 0},
                    {"poffinSaveData", 0},
                    {"btlTowerSave", 0},
                    {"systemData", GetSystemData(saveData)},
                    {"poketchData", 0},
                    {"azukariyaData", 0},
                    {"dendoudata", 0},
                    {"badgeSaveData", 0},
                    {"boukenNote", 0},
                    {"tvDataOld", 0},
                    {"ugSaveData", 0},
                    {"gmsdata", 0},
                    {"networkdata", 0},
                    {"unionSaveData", 0},
                    {"contstPhotoLangData", 0},
                    {"zukanPersonalRndData", 0},
                    {"contestPhotoExtData", 0},
                    {"gmsPointExtData", 0},
                    {"ugCountRecord", 0},
                    {"reBuffNameData", 0},
                    {"saveDataHash", 0},
                    {"recodeAddData", 0},
                    {"mysteryGiftSaveData", 0},
                    {"poketoreCountArray", 0},
                    {"playReportData", 0},
                    {"mtData", 0},
                    {"dendouSaveAdd", 0},
                    {"tvData", 0},
                    {"ballDecoExtraData", 0}
            }}
    };



    nn::string writeString = wholeJson.dump(4);
    nn::string filePath = "SaveData:/JSON_PlayerData.bin";
    FsHelper::writeFileToPath((void*) writeString.data(), writeString.size(), filePath.c_str());
}

void JsonLoadTest(PlayerWork::Object* playerWork) {
    auto& saveData = playerWork->fields._saveData.fields;
    nn::string filePath = "SaveData:/JSON_PlayerData.json";
    Logger::log("[JSONLoadTest] Loading file...\n");
    nn::json j = FsHelper::loadJsonFileFromPath(filePath.c_str());
    if (j != nullptr && !j.is_discarded()) {
        Logger::log("[JSONLoadTest] Loaded JSON\n");
        auto jData = j.at("SaveData");

        // Version
        saveData.version = jData.at("version").get<int32_t>();
        Logger::log("[JSONLoadTest] Version set to %d\n", saveData.version);

        // IntValues
        auto storedArray = jData["intValues"];
        auto workArray = System::Int32_array::newArray(WorkCount);
        Logger::log("[JSONLoadTest] New Array created\n");
        for (uint64_t i = 0; i < workArray->max_length; i++) {
            memcpy(&workArray->m_Items[i], &storedArray[i], sizeof(int32_t));
        }
        Logger::log("[JSONLoadTest] All values copied\n");
        saveData.intValues = workArray;
    }

    else {
        Logger::log("[JSONLoadTest] Error parsing JSON\n");
    }
}