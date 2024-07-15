#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Message/MessageEnumData.h"
#include "externals/Pml/PokePara/PokemonParam.h"

struct ZukanWork : ILClass<ZukanWork> {
    static inline bool GetZenkokuFlag() {
        return external<bool>(0x017db490);
    }

    static inline void SetPoke(uint32_t monsno, int32_t get, uint8_t sex, int32_t form, bool color) {
        return external<void>(0x017dbce0, monsno, get, sex, form, color);
    }

    static inline void SetPoke(Pml::PokePara::PokemonParam::Object* mons, int32_t get) {
        return external<void>(0x017dc560, mons, get);
    }

    static inline void AddLangFlag(uint32_t monsno, Dpr::Message::MessageEnumData::MsgLangId msglang) {
        return external<void>(0x017dc870, monsno, msglang);
    }
};
