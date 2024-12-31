#pragma once

#include "externals/il2cpp-api.h"
#include "externals/System/String.h"

struct FieldManager : ILClass<FieldManager, 0x04c5a638> {
    struct Fields {
        // TODO
    };

    struct StaticFields {
        FieldManager* _Instance_k__BackingField;
        void* fwMng; // Dpr_Field_Walking_FieldWalkingManager_o*
        void* abUnloader; // Dpr_SubContents_Utils_AssetUnloader_o*
        bool _IsResume_k__BackingField;
        bool SealPrevFlag;
    };

    inline uint16_t GetFormNo(int32_t mons, int32_t karana, int32_t anno) {
        return external<uint16_t>(0x0179f560, this, mons, karana, anno);
    }

    inline void EventWildBattle(int32_t mons, int32_t level, bool isCaptureDemo, bool isSymbol, bool isMitu, uint8_t talentVNum, bool isCantUseBall, int32_t formNo, bool tokusei3rd) {
        external<void>(0x0179f720, this, mons, level, isCaptureDemo, isSymbol, isMitu, talentVNum, isCantUseBall, formNo, tokusei3rd);
    }

    inline void EncountStart(int32_t type, int32_t trainerid1, int32_t trainerid2) {
        external<void>(0x0179f330, this, type, trainerid1, trainerid2);
    }

    inline void PreLoadEncEffect(System::String::Object* assetname) {
        external<void>(0x0179e6a0, this, assetname);
    }
};
