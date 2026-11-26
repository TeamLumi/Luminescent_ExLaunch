#pragma once

#include "externals/il2cpp-api.h"

#include "externals/DPData/GET_STATUS.h"
#include "externals/Dpr/Message/MessageEnumData.h"
#include "externals/Pml/PokePara/PokemonParam.h"

struct ZukanWork : ILClass<ZukanWork, game::Address::ZukanWork_TypeInfo> {
    static inline bool GetZenkokuFlag() {
        return external<bool>(game::Address::ZukanWork_GetZenkokuFlag);
    }

    static inline void SetPoke(uint32_t monsno, int32_t get, uint8_t sex, int32_t form, bool color) {
        external<void>(game::Address::ZukanWork_SetPoke_UInt32_Int32_UInt8_Int32_Bool, monsno, get, sex, form, color);
    }

    static inline void SetPoke(Pml::PokePara::PokemonParam::Object* mons, int32_t get) {
        external<void>(game::Address::ZukanWork_SetPoke_PokemonParam_Int32, mons, get);
    }

    static inline void AddLangFlag(uint32_t monsno, Dpr::Message::MessageEnumData::MsgLangId msglang) {
        external<void>(game::Address::ZukanWork_AddLangFlag, monsno, msglang);
    }

    static inline void DebugSet(uint32_t monsno, DPData::GET_STATUS get, Pml::Sex sex, int32_t form, bool color, bool isGet) {
        external<void>(game::Address::ZukanWork_DebugSet, monsno, get, sex, form, color, isGet);
    }

    static inline void ZukanON() {
        external<void>(game::Address::ZukanWork_ZukanON);
    }

    static inline void ZenkokuON() {
        external<void>(game::Address::ZukanWork_ZenkokuON);
    }

    static inline DPData::GET_STATUS GetStatus(uint32_t monsno) {
        return external<DPData::GET_STATUS>(game::Address::ZukanWork_GetStatus, monsno);
    }
};
