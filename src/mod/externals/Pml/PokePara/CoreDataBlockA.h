#pragma once

#include "externals/il2cpp-api.h"

namespace Pml::PokePara {
    struct CoreDataBlockA : ILStruct<CoreDataBlockA> {
        struct Fields {
            uint16_t monsno;
            uint16_t itemno;
            uint32_t id;
            uint32_t exp;
            uint16_t tokuseino;
            uint16_t _bitsA;
            uint16_t boxMark;
            uint32_t colorRnd;
            uint8_t seikaku;
            uint8_t seikakuHosei;
            uint8_t _bitsB;
            uint16_t formNo;
            uint8_t effortHp;
            uint8_t effortAtk;
            uint8_t effortDef;
            uint8_t effortAgi;
            uint8_t effortSpatk;
            uint8_t effortSpdef;
            uint8_t style;
            uint8_t beautiful;
            uint8_t cute;
            uint8_t clever;
            uint8_t strong;
            uint8_t fur;
            uint8_t pokerus;
            uint32_t ribbonA;
            uint32_t ribbonB;
            uint8_t lumpingRibbonA;
            uint8_t lumpingRibbonB;
            uint32_t ribbonC;
            uint32_t ribbonD;
            uint32_t _bitsC;
            uint32_t camp_reserved;
            uint8_t talentHeight;
            uint8_t talentWeight;
            uint8_t _bitsD;
            uint8_t padding[5];
        };

        static_assert(sizeof(Fields) == 0x50);
    };
}
