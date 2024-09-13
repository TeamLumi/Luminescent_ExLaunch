#include "exlaunch.hpp"
#include "externals/il2cpp-api.h"
#include "memory/json.h"
#include "externals/PlayerWork.h"
#include "externals/DPData/PLAYER_DATA.h"
#include "helpers/fsHelper.h"
#include "externals/System/Convert.h"
#include "save/save.h"
#include "externals/UnityEngine/JsonUtility.h"

nn::string ByteArrayToBase64(System::Byte_array* inArray) {
    Logger::log("[ByteArrayToBase64] \n");
    return System::Convert::ToBase64String(inArray)->asCString();
}

nn::json GetTopMenuItem(DPData::TOPMENUITEM_WORK::Object menuItem) {
    return {
            {"index", menuItem.fields.index},
            {"isNew", menuItem.fields.isNew},
    };
}

nn::json GetArray(Dpr::Box::SaveBoxData::_STR17::Array* inArray) {
    Logger::log("[GetArray] \n");
    if (inArray != nullptr && inArray->max_length > 0) {
        auto stringArray = nn::vector<nn::string>();
        Logger::log("[GetArray] Pushing back %d items.\n", inArray->max_length);
        for (size_t i = 0; i < inArray->max_length; i++) {
            stringArray.push_back(inArray->m_Items[i].fields.str->asCString());
        }
        return stringArray;
    }
    return {};
}

nn::json GetArray(Dpr::Box::SaveBoxData::_STR11::Array* inArray) {
    Logger::log("[GetArray] \n");
    if (inArray != nullptr && inArray->max_length > 0) {
        auto stringArray = nn::vector<nn::string>();
        Logger::log("[GetArray] Pushing back %d items.\n", inArray->max_length);
        for (size_t i = 0; i < inArray->max_length; i++) {
            stringArray.push_back(inArray->m_Items[i].fields.str->asCString());
        }
        return stringArray;
    }
    return {};
}

nn::json GetArray(DPData::GET_STATUS_array* inArray) {
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

nn::json GetArray(DPData::TOPMENUITEM_WORK::Array* inArray) {
    Logger::log("[GetArray] \n");
    if (inArray != nullptr && inArray->max_length > 0) {
        auto int32Array = nn::vector<int32_t>();
        Logger::log("[GetArray] Pushing back %d items.\n", inArray->max_length);
        for (size_t i = 0; i < inArray->max_length; i++) {
            int32Array.push_back(GetTopMenuItem(inArray->m_Items[i]));
        }
        return int32Array;
    }
    return {};
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

nn::json GetArray(System::UInt16_array* inArray) {
    Logger::log("[GetArray] \n");
    if (inArray != nullptr && inArray->max_length > 0) {
        auto uint16Array = nn::vector<uint16_t>();
        Logger::log("[GetArray] Pushing back %d items.\n", inArray->max_length);
        for (size_t i = 0; i < inArray->max_length; i++) {
            uint16Array.push_back(inArray->m_Items[i]);
        }
        return uint16Array;
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
nn::json GetSaveItemFromArray(Dpr::Item::SaveItem::Object item) {
    auto saveItem = item.fields;
    return {
            {"Count", saveItem.Count},
            {"VanishNew", saveItem.VanishNew},
            {"FavouriteFlag", saveItem.FavoriteFlag},
            {"ShowWazaNameFlag", saveItem.ShowWazaNameFlag},
            {"Dummy1", saveItem.Dummy1},
            {"Dummy2", saveItem.Dummy2},
            {"SortNumber", saveItem.SortNumber}
    };
}

nn::json GetSaveItemArray(Dpr::Item::SaveItem::Array* inArray) {
    Logger::log("[GetArray] \n");
    if (inArray != nullptr && inArray->max_length > 0) {
        auto itemArray = nn::json::array();
        Logger::log("[GetArray] Pushing back %zu items.\n", inArray->max_length);
        for (uint64_t i = 0; i < inArray->max_length; i++) {
            itemArray.push_back(GetSaveItemFromArray(inArray->m_Items[i]));
        }
        return itemArray;
    }
    return {};
}
// TODO: saveUgItem
// TODO: saveItemShortcut

nn::json GetSerializedPokeFromArray(Pml::PokePara::SerializedPokemonFull::Array* inArray) {
    Logger::log("[GetArray] \n");
    if (inArray != nullptr && inArray->max_length > 0) {
        auto partyArray = nn::json::array();
        Logger::log("[GetArray] Pushing back %zu items.\n", inArray->max_length);
        for (uint64_t i = 0; i < inArray->max_length; i++) {
            partyArray.push_back(ByteArrayToBase64(inArray->m_Items[i].fields.buffer));
        }
        return partyArray;
    }
    return {};
}
nn::json GetSavePokeParty(Pml::PokePara::SavePokeParty::Object playerParty) {

    return {
            {"members", GetSerializedPokeFromArray(playerParty.fields.members)},
            {"memberCount", playerParty.fields.memberCount},
            {"markingIndex", playerParty.fields.markingIndex}
    };
}

nn::json GetBoxSaveData(Dpr::Box::SaveBoxData::Object saveBoxData) {
    auto boxData = saveBoxData.fields;
    return {
            {"trayName", GetArray(boxData.trayName)},
            {"teamName", GetArray(boxData.teamName)},
            {"teamPos", GetArray(boxData.teamPos)},
            {"teamLock", boxData.teamLock},
            {"trayMax", boxData.trayMax},
            {"currentTray", boxData.currentTray},
            {"isOpened", boxData.isOpened},
            {"wallPaper", ByteArrayToBase64(boxData.wallPaper)},
            {"statusPut", boxData.statusPut},
    };
}
// TODO: boxTray
nn::json GetPlayerData() {
    PlayerWork::getClass()->initIfNeeded();
    auto config = (PlayerWork::get_config())->fields;
    auto myStatus = (PlayerWork::get_playerStatus())->fields;
    auto playTime = (PlayerWork::get_playerData())->fields.playtime.fields;
    auto contest_data = (PlayerWork::get_playerData())->fields.contestdata.fields;
    Logger::log("[PlayerData] \n");
    return {
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
    };
}
nn::json GetZukan(DPData::ZUKAN_WORK::Object zukanWork) { //ToDo
    auto zukanData = zukanWork.fields;
    return {
            {"GET_STATUS", GetArray(zukanData.get_status)},
            {"male_color_flag", GetArray(zukanData.male_color_flag)},
            {"female_color_flag", GetArray(zukanData.male_color_flag)},
            {"male_flag", GetArray(zukanData.male_flag)},
            {"female_flag", GetArray(zukanData.female_flag)},
            {"UnknownTurn", GetArray(zukanData.UnknownTurn)},
            {"UnknownColor", GetArray(zukanData.UnknownColor)},
            {"PowarunTurn", GetArray(zukanData.PowarunTurn)},
            {"PowarunColor", GetArray(zukanData.PowarunColor)},
            {"DeokisisuTurn", GetArray(zukanData.DeokisisuTurn)},
            {"DeokisisuColor", GetArray(zukanData.DeokisisuColor)},
            {"MinomuttiTurn", GetArray(zukanData.MinomuttiTurn)},
            {"MinomuttiColor", GetArray(zukanData.MinomuttiColor)},
            {"MinomadamTurn", GetArray(zukanData.MinomadamTurn)},
            {"MinomadamColor", GetArray(zukanData.MinomadamColor)},
            {"GaameiruTurn", GetArray(zukanData.GaameiruTurn)},
            {"GaameiruColor", GetArray(zukanData.GaameiruColor)},
            {"TherimuTurn", GetArray(zukanData.TherimuTurn)},
            {"TherimuColor", GetArray(zukanData.TherimuColor)},
            {"KaranakusiTurn", GetArray(zukanData.KaranakusiTurn)},
            {"KaranakusiColor", GetArray(zukanData.KaranakusiColor)},
            {"ToritodonTurn", GetArray(zukanData.ToritodonTurn)},
            {"ToritodonColor", GetArray(zukanData.ToritodonColor)},
            {"RotomTurn", GetArray(zukanData.RotomTurn)},
            {"RotomColor", GetArray(zukanData.RotomColor)},
            {"GirathinaTurn", GetArray(zukanData.GirathinaTurn)},
            {"GirathinaColor", GetArray(zukanData.GirathinaColor)},
            {"SheimiTurn", GetArray(zukanData.SheimiTurn)},
            {"SheimiColor", GetArray(zukanData.SheimiColor)},
            {"AruseusuTurn", GetArray(zukanData.AruseusuTurn)},
            {"AruseusuColor", GetArray(zukanData.AruseusuColor)},
            {"TextVersionUp", GetArray(zukanData.TextVersionUp)},
            {"zukan_get", zukanData.zukan_get},
            {"zenkoku_flag", zukanData.zenkoku_flag}
    };
}
// TODO: tr_battleData

nn::json GetTopMenuData(DPData::TOPMENU_WORK::Object topMenu) {
    auto menu = topMenu.fields;
    return {
            {"gear_type", GetArray(menu.items)},
            {"selectType", menu.selectType}
    };
}
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

void JsonSaveTest(PlayerWork::SaveData::Object* obj) {

    auto saveData = obj->fields;

    nn::json wholeJson = {
            {"SaveData", {
                    {"version", saveData.version},
                    {"intValues", GetArray(saveData.intValues)}, //GetArray(saveData.intValues)
                    {"boolValues", GetArray(saveData.boolValues)}, //GetArray(saveData.boolValues)
                    {"systemFlags", GetArray(saveData.systemFlags)}, //GetArray(saveData.systemFlags)
                    {"rivalName", saveData.rivalName->asCString().c_str()},
                    {"zoneID", saveData.zoneID},
                    {"timeScale", saveData.timeScale},
                    {"saveItem", GetSaveItemArray(saveData.saveItem)},
                    {"saveUgItem", nullptr},
                    {"saveItemShortcut", nullptr},
                    {"playerParty", GetSavePokeParty(saveData.playerParty)},
                    {"boxData", GetBoxSaveData(saveData.boxData)},
                    {"boxTray", nullptr},
                    {"playerData", GetPlayerData()},
                    {"zukanData", GetZukan(saveData.zukanData)},
                    {"tr_battleData", nullptr},
                    {"topMenuData", GetTopMenuData(saveData.topMenuData)},
                    {"fieldObj_Save", nullptr},
                    {"record", nullptr},
                    {"enc_sv_data", nullptr},
                    {"player_save_data", GetPlayerSaveData(saveData)},
                    {"ballDecoData", nullptr},
                    {"saveSeal", nullptr},
                    {"randomGroup", nullptr},
                    {"fieldGimmickSaveData", nullptr},
                    {"kinomiGrowSaveData", nullptr},
                    {"poffinSaveData", nullptr},
                    {"btlTowerSave", nullptr},
                    {"systemData", GetSystemData(saveData)},
                    {"poketchData", nullptr},
                    {"azukariyaData", nullptr},
                    {"dendoudata", nullptr},
                    {"badgeSaveData", nullptr},
                    {"boukenNote", nullptr},
                    {"tvDataOld", nullptr},
                    {"ugSaveData", nullptr},
                    {"gmsdata", nullptr},
                    {"networkdata", nullptr},
                    {"unionSaveData", nullptr},
                    {"contstPhotoLangData", nullptr},
                    {"zukanPersonalRndData", nullptr},
                    {"contestPhotoExtData", nullptr},
                    {"gmsPointExtData", nullptr},
                    {"ugCountRecord", nullptr},
                    {"reBuffNameData", nullptr},
                    {"saveDataHash", nullptr},
                    {"recodeAddData", nullptr},
                    {"mysteryGiftSaveData", nullptr},
                    {"poketoreCountArray", nullptr},
                    {"playReportData", nullptr},
                    {"mtData", nullptr},
                    {"dendouSaveAdd", nullptr},
                    {"tvData", nullptr},
                    {"ballDecoExtraData", nullptr}
            }}
    };

//    auto extractedJson = UnityEngine::JsonUtility::ToJson(reinterpret_cast<Il2CppObject*>(obj), false);
//    Logger::log("[ToJson] Json extraction complete.\n");

    nn::string writeString = wholeJson.dump(4);
//    nn::string writeString = extractedJson->asCString();
    nn::string filePath = "SaveData:/JSON_PlayerData.bin";
    FsHelper::writeFileToPath((void*) writeString.data(), writeString.size(), filePath.c_str());
}

void JsonLoadTest(PlayerWork::Object* playerWork) {
    auto& saveData = playerWork->fields._saveData.fields;
    nn::string filePath = "SaveData:/JSON_PlayerData.bin";
    Logger::log("[JSONLoadTest] Loading file...\n");
    nn::json j = FsHelper::loadJsonFileFromPath(filePath.c_str());
    if (j != nullptr && !j.is_discarded()) {
        Logger::log("[JSONLoadTest] Loaded JSON\n");
        auto jData = j["SaveData"];

        // Version
        saveData.version = jData["version"].get<int32_t>();
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