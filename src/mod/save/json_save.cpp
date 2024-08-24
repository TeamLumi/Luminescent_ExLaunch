#include "exlaunch.hpp"
#include "externals/il2cpp-api.h"
#include "memory/json.h"
#include "externals/PlayerWork.h"
#include "externals/DPData/PLAYER_DATA.h"
#include "helpers/fsHelper.h"

nn::json GetPlayerData() {
    PlayerWork::getClass()->initIfNeeded();
    auto config = (PlayerWork::get_config())->fields;
    auto myStatus = (PlayerWork::get_playerStatus())->fields;
    auto playTime = (PlayerWork::get_playerData())->fields.playtime.fields;
    auto contest_data = (PlayerWork::get_playerData())->fields.contestdata.fields;

    nn::json playerDataJson = {
            {"PlayerData", {
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
                            {"photoDatas", 0}, // need to change
                            {"contestRankPoint", contest_data.contestRankPoint}
                    }}
            }}
    };

    return playerDataJson;
}

void JsonSaveTest() {

    nn::json testJson = GetPlayerData();

    nn::string writeString = testJson.dump(4);
    nn::string filePath = "SaveData:/JSON_PlayerData.bin";
    Logger::log("[JSONSaveTest] Dumping JSON to %s\n", filePath.c_str());
    FsHelper::writeFileToPath((void*) writeString.data(), writeString.size(), filePath.c_str());
    Logger::log("[JSONSaveTest] File written successfully.\n");

}
