#pragma once

#include "externals/il2cpp-api.h"

#include "externals/MonsLv.h"

struct ZoneWork : ILClass<ZoneWork> {
    static inline int32_t TairyouHassei_ZoneID() {
        return external<int32_t>(game::Address::ZoneWork_TairyouHassei_ZoneID);
    }

    static inline int32_t SafariZonePosID(int32_t zone_id) {
        return external<int32_t>(game::Address::ZoneWork_SafariZonePosID, zone_id);
    }
    
    static inline bool IsSpFishingZone(int32_t zone_id) {
        return external<bool>(game::Address::ZoneWork_IsSpFishingZone, zone_id);
    }
    
    static inline bool IsHillBackZone(int32_t zone_id) {
        return external<bool>(game::Address::ZoneWork_IsHillBackZone, zone_id);
    }

    static inline MonsLv::Array* TairyouHassei_MonsLv(int32_t zoneid) {
        return external<MonsLv::Array*>(game::Address::ZoneWork_TairyouHassei_MonsLv, zoneid);
    }
};
