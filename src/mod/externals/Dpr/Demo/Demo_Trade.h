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
            void* timeLine;                                                // 0xC0 TimeLineBinder
            Pml::PokePara::PokemonParam::Object* MyPokeParam_Copy;        // 0xC8
            Pml::PokePara::PokemonParam::Object* FriendPokeParam_Copy;    // 0xD0
            // BoxPokeData FriendPokeParam at 0xD8 (struct, not pointer)
            void* _friendPokeParam_pokeParam;                              // 0xD8
            bool _friendPokeParam_isTeam;                                  // 0xE0
            int32_t _friendPokeParam_teamIndex;                            // 0xE4
            int32_t _friendPokeParam_teamPos;                              // 0xE8
            int32_t nowState;                                              // 0xF0 TradeState enum
            Param::Object* _param;                                         // 0xF8
            void* receiver;                                                // 0x100 MarkerReceiver
            bool isTimelineComplete;                                       // 0x108
            int32_t Debug_RemoveEffectsNum;                                // 0x10C
            bool UsePreGetCheck;                                           // 0x110
            bool IsGetMonsNo;                                              // 0x111
            bool IsGetEvolvedMonsNo;                                       // 0x112
            void* displayTime;                                             // 0x118 float[]
        };

        inline void ctor() {
            external<void>(0x0197C800, this);
        }

        inline void SetParam(int32_t myId, int32_t friendId,
                             System::String::Object* friendName,
                             bool isMiracle, int32_t msgLangId) {
            external<void>(0x0197C950, this, myId, friendId, friendName, isMiracle, msgLangId);
        }
    };
}
