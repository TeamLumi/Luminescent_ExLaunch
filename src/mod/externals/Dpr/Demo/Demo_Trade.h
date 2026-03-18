#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Demo/DemoBase.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/System/String.h"

namespace Dpr::Demo {
    struct Demo_Trade : ILClass<Demo_Trade, 0x04C5E608> {
        struct Param : ILClass<Param> {
            struct Fields {
                int32_t myTranerId;                        // 0x10
                int32_t friendTranerId;                    // 0x14
                System::String::Object* friendTranerName;  // 0x18
                bool isMiracle;                            // 0x20
                int32_t langId;                            // 0x24 (MsgLangId enum)
            };

            inline void ctor() {
                external<void>(0x0197CA70, this);
            }
        };

        struct Fields : Dpr::Demo::DemoBase::Fields {
            void* timeLine;                                        // abs 0xC0 (TimeLineBinder*)
            Pml::PokePara::PokemonParam::Object* MyPokeParam;     // abs 0xC8
            Pml::PokePara::PokemonParam::Object* FriendPokeParam; // abs 0xD0
            void* _field_d8;                                       // abs 0xD8
            void* _field_e0;                                       // abs 0xE0
            void* _field_e8;                                       // abs 0xE8
            int32_t nowState;                                      // abs 0xF0
            int32_t _pad_f4;                                       // abs 0xF4
            Param::Object* _param;                                 // abs 0xF8
            void* _field_100;                                      // abs 0x100
            bool _isReady;                                         // abs 0x108
        };

        inline void ctor() {
            external<void>(0x0197C800, this);
        }

        inline void SetParam(int32_t myId, int32_t friendId,
                             System::String::Object* friendName,
                             bool isMiracle, int32_t langId) {
            external<void>(0x0197C950, this, myId, friendId, friendName, isMiracle, langId);
        }
    };
}
