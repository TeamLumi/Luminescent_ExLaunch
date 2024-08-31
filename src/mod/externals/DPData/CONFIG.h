#pragma once

#include <cstdint>
#include "externals/il2cpp-api.h"

namespace DPData {
    struct CONFIG : ILStruct<CONFIG> {
        struct Fields {
            int32_t msg_speed;
            int32_t msg_lang_id;
            bool is_kanji;
            int32_t window_type;
            int32_t wazaeff_mode;
            int32_t battle_rule;
            int32_t party_and_box;
            bool regist_nickname;
            bool gyrosensor;
            bool camerashake_of_fossil;
            int32_t camera_updown;
            int32_t camera_leftright;
            bool autoreport;
            int32_t input_mode;
            bool show_nickname;
            uint8_t bgm_volume;
            uint8_t se_volume;
            uint8_t voice_volume;
        };

        inline int32_t GetValue(int32_t key) {
            return external<int32_t>(0x02299720, this, key);
        }

        inline bool IsEqualValue(int32_t configId, DPData::CONFIG::Object* t) {
            return external<bool>(0x022998a0, this, configId, t);
        }
    };
}
