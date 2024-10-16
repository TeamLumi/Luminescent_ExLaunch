#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Message/MessageEnumData.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/XLSXContent/LocalKoukanData.h"

struct LocalKoukan : ILClass<LocalKoukan> {
    struct Operation : ILClass<Operation, 0x04c643b0> {
        struct Fields {
            bool IsTemotiMode;
            int32_t TrayNo;
            int32_t Pos;
            Pml::PokePara::PokemonParam::Object* from;
            Pml::PokePara::PokemonParam::Object* other;
        };

        inline void ctor() {
            external<void>(0x01af3940, this);
        }
    };

    struct Fields {};

    inline static XLSXContent::LocalKoukanData::Sheetdata::Object* GetTargetData(int32_t npcindex, Dpr::Message::MessageEnumData::MsgLangId lang) {
        return external<XLSXContent::LocalKoukanData::Sheetdata::Object*>(0x01af32a0, npcindex, lang);
    }
};
