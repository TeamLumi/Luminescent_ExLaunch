#pragma once

#include "logger/logger.h"
#include "memory/json.h"

static constexpr int MAX_PERSISTENT_SHINIES = 10;

struct PersistentShiny {
    int32_t monsNo;
    int16_t formNo;
    int32_t level;
    int8_t sex;
    int32_t zoneID;
    int32_t gridX;
    int32_t gridZ;
    bool occupied;
};

struct SymbolEncountersSaveData {
    PersistentShiny shinies[MAX_PERSISTENT_SHINIES];
    int32_t count;

    void Initialize() {
        count = 0;
        for (int i = 0; i < MAX_PERSISTENT_SHINIES; i++) {
            shinies[i] = {};
            shinies[i].occupied = false;
        }
    }

    // Add a shiny. FIFO: if full, remove oldest (index 0) and shift.
    void AddShiny(int32_t monsNo, int16_t formNo, int32_t level, int8_t sex,
                  int32_t zoneID, int32_t gridX, int32_t gridZ) {
        if (count >= MAX_PERSISTENT_SHINIES) {
            // Shift everything left, dropping index 0
            for (int i = 0; i < MAX_PERSISTENT_SHINIES - 1; i++) {
                shinies[i] = shinies[i + 1];
            }
            count = MAX_PERSISTENT_SHINIES - 1;
        }
        auto& s = shinies[count];
        s.monsNo = monsNo;
        s.formNo = formNo;
        s.level = level;
        s.sex = sex;
        s.zoneID = zoneID;
        s.gridX = gridX;
        s.gridZ = gridZ;
        s.occupied = true;
        count++;
        Logger::log("[SymbolEnc] Persisted shiny: monsNo=%d zone=%d grid(%d,%d) [%d/%d]\n",
                    monsNo, zoneID, gridX, gridZ, count, MAX_PERSISTENT_SHINIES);
    }

    // Remove a shiny by index (e.g. after battle)
    void RemoveShiny(int index) {
        if (index < 0 || index >= count) return;
        for (int i = index; i < count - 1; i++) {
            shinies[i] = shinies[i + 1];
        }
        count--;
        shinies[count] = {};
        shinies[count].occupied = false;
    }

    [[nodiscard]] nn::json ToJson() const {
        nn::json shinyArray = nn::json::array();
        for (int i = 0; i < count; i++) {
            shinyArray.push_back({
                {"monsNo", shinies[i].monsNo},
                {"formNo", shinies[i].formNo},
                {"level", shinies[i].level},
                {"sex", shinies[i].sex},
                {"zoneID", shinies[i].zoneID},
                {"gridX", shinies[i].gridX},
                {"gridZ", shinies[i].gridZ},
            });
        }
        return {{"symbolEncounters", {{"shinies", shinyArray}}}};
    }

    void FromJson(const nn::json& data) {
        Initialize();
        if (!data.contains("shinies")) return;
        auto& arr = data["shinies"];
        for (int i = 0; i < (int)arr.size() && i < MAX_PERSISTENT_SHINIES; i++) {
            shinies[i].monsNo = arr[i]["monsNo"].get<int32_t>();
            shinies[i].formNo = arr[i]["formNo"].get<int16_t>();
            shinies[i].level = arr[i]["level"].get<int32_t>();
            shinies[i].sex = arr[i]["sex"].get<int8_t>();
            shinies[i].zoneID = arr[i]["zoneID"].get<int32_t>();
            shinies[i].gridX = arr[i]["gridX"].get<int32_t>();
            shinies[i].gridZ = arr[i]["gridZ"].get<int32_t>();
            shinies[i].occupied = true;
            count++;
        }
        Logger::log("[SymbolEnc] Loaded %d persistent shinies from save\n", count);
    }
};

void loadSymbolEncountersFromJson(const nn::json& saveFile);
nn::json getSymbolEncountersAsJson();
