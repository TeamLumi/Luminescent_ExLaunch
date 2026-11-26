#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Audio/AudioInstance.h"
#include "externals/Dpr/Field/FieldEncount.h"
#include "externals/UnityEngine/GameObject.h"
#include "externals/UnityEngine/Vector3.h"

namespace Dpr::Field {
    struct SwayGrass : ILClass<SwayGrass, game::Address::Dpr_Field_SwayGrass_TypeInfo> {
        struct GrassData : ILClass<GrassData, game::Address::Dpr_Field_SwayGrass_GrassData_TypeInfo> {
            struct Fields {
                bool enable;
                float effectTime;
                bool rensaMons;
                int32_t rank;
                int32_t random_iro;
                int32_t random_kakure;
                UnityEngine::Vector3::Object position;
                int32_t attricode;
                UnityEngine::GameObject::Object* transObject;
            };

            inline void ctor() {
                external<void>(game::Address::Dpr_Field_SwayGrass_GrassData_ctor, this);
            }
        };

        struct StaticFields {
            bool is_swaygrass_flag;
            int32_t swayZone;
            GrassData::Array* grass_data;
            Audio::AudioInstance::Array* _grassAudio;
            GrassData::Object* work_data;
            uint32_t rensa_count;
            int32_t rensa_mons;
            uint32_t rensa_lv;
            bool BattleEndRensaStart;
            UnityEngine::GameObject::Object* RootGrass;
            bool _callSwayBGM;
            bool _callStopSwayBGM;
        };

        static inline StaticILMethod<game::Address::Method_Dpr_Field_SwayGrass_OnAudioInstanceFinished, Audio::AudioInstance> Method$$Play$$OnAudioInstanceFinished {};

        static inline void StopSE() {
            external<void>(game::Address::Dpr_Field_SwayGrass_StopSE);
        }

        static inline bool SwayGrass_CheckSpEncount(Dpr::Field::FieldEncount::SWAY_ENC_INFO::Object *info, UnityEngine::Vector3::Object *pos, float size) {
            UnityEngine::Vector3::Fields posProxy = { .x = pos->fields.x, .y = pos->fields.y, .z = pos->fields.z };
            return external<bool>(game::Address::Dpr_Field_SwayGrass_SwayGrass_CheckSpEncount, info, &posProxy, size);
        }

        static inline int32_t RensaNum(uint32_t rensa, bool omamori) {
            return external<int32_t>(game::Address::Dpr_Field_SwayGrass_RensaNum, rensa, omamori);
        }
    };
}
