#pragma once

#include "externals/il2cpp-api.h"

#include "externals/DPData/GET_STATUS.h"
#include "externals/Dpr/Message/MessageEnumData.h"
#include "externals/Pml/PokePara/PokemonParam.h"

struct ZukanWork : ILClass<ZukanWork, 0x04c5b488> {
    static inline bool GetZenkokuFlag() {
        return external<bool>(0x017db490);
    }

    static inline void SetPoke(uint32_t monsno, int32_t get, uint8_t sex, int32_t form, bool color) {
        external<void>(0x017dbce0, monsno, get, sex, form, color);
    }

    static inline void SetPoke(Pml::PokePara::PokemonParam::Object* mons, int32_t get) {
        external<void>(0x017dc560, mons, get);
    }

    static inline void AddLangFlag(uint32_t monsno, Dpr::Message::MessageEnumData::MsgLangId msglang) {
        external<void>(0x017dc870, monsno, msglang);
    }

    static inline void DebugSet(uint32_t monsno, DPData::GET_STATUS get, Pml::Sex sex, int32_t form, bool color, bool isGet) {
        external<void>(0x017dd7e0, monsno, get, sex, form, color, isGet);
    }

    static inline void ZukanON() {
        external<void>(0x017db250);
    }

    static inline void ZenkokuON() {
        external<void>(0x017db310);
    }
};
