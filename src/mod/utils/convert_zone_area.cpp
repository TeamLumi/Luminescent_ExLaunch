#include "exlaunch.hpp"

#include "data/areas.h"
#include "data/utils.h"
#include "data/zones.h"
#include "externals/FlagWork_Enums.h"
#include "externals/System/String.h"

System::String::Object* Convert_AreaID(void* typeinfo, int32_t* areaId) {
    if (*areaId >= 0 && *areaId < AREA_COUNT)
    {
        return System::String::Create(AREA_CODES[*areaId]);
    }
    else
    {
        return System::String::Create("");
    }
}

System::String::Object* Convert_ZoneID(void* typeinfo, int32_t* zoneId) {
    if (*zoneId >= 0 && *zoneId < ZONE_COUNT)
    {
        return System::String::Create(ZONE_CODES[*zoneId]);
    }
    else
    {
        return System::String::Create("");
    }
}

int32_t ConvertZoneIDToArriveFlag(int32_t zoneId) {
    int32_t startRange1 = array_index(ZONE_CODES, "C01");
    int32_t startRange2 = array_index(ZONE_CODES, "D10R0902");
    int32_t startRange3 = array_index(ZONE_CODES, "D10R0601B") + 1;

    if (zoneId >= startRange1 && zoneId < startRange2)
    {
        // First Arrive Flag section
        int32_t adjustedZoneId = zoneId - startRange1;
        return ((int32_t)FlagWork_SysFlag::FLAG_ARRIVE_C01) + adjustedZoneId;
    }
    else if (zoneId >= startRange2 && zoneId < startRange3)
    {
        // Second Arrive Flag section
        int32_t adjustedZoneId = zoneId - startRange2;
        return ((int32_t)FlagWork_SysFlag::FLAG_ARRIVE_D10R0902) + adjustedZoneId;
    }
    else
    {
        // Third Arrive Flag section (Custom)
        int32_t adjustedZoneId = zoneId - startRange3;
        return ((int32_t)FlagWork_SysFlag::FLAG_ARRIVE_EXTRA_000) + adjustedZoneId;
    }
}