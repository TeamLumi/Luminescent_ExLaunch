#pragma once

#include <cstring>

#include "logger/logger.h"
#include "memory/json.h"

static constexpr int MAX_PERSISTENT_SHINIES = 10;

// Core 328 + calc 16, must match Accessor::Serialize_FullData.
static constexpr int PERSIST_POKE_DATA_SIZE = 344;

struct PersistentShiny {
    int32_t monsNo;
    int16_t formNo;
    int32_t level;
    int8_t sex;
    int32_t zoneID;
    int32_t gridX;
    int32_t gridZ;
    bool occupied;
    // The exact individual comes back after save/reload, not just the species.
    bool hasFullData;
    uint8_t fullData[PERSIST_POKE_DATA_SIZE];
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

    // FIFO: if full, remove oldest (index 0) and shift. fullData nullable.
    void AddShiny(int32_t monsNo, int16_t formNo, int32_t level, int8_t sex,
                  int32_t zoneID, int32_t gridX, int32_t gridZ,
                  const uint8_t* fullData = nullptr) {
        if (count >= MAX_PERSISTENT_SHINIES) {
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
        s.hasFullData = (fullData != nullptr);
        if (fullData != nullptr) {
            memcpy(s.fullData, fullData, PERSIST_POKE_DATA_SIZE);
        } else {
            memset(s.fullData, 0, PERSIST_POKE_DATA_SIZE);
        }
        count++;
        Logger::log("[SymbolEnc] Persisted shiny: monsNo=%d zone=%d grid(%d,%d) fullData=%d [%d/%d]\n",
                    monsNo, zoneID, gridX, gridZ, (int)s.hasFullData, count, MAX_PERSISTENT_SHINIES);
    }

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
            nn::json entry = {
                {"monsNo", shinies[i].monsNo},
                {"formNo", shinies[i].formNo},
                {"level", shinies[i].level},
                {"sex", shinies[i].sex},
                {"zoneID", shinies[i].zoneID},
                {"gridX", shinies[i].gridX},
                {"gridZ", shinies[i].gridZ},
            };
            if (shinies[i].hasFullData) {
                char hex[PERSIST_POKE_DATA_SIZE * 2 + 1];
                static const char* digits = "0123456789abcdef";
                for (int b = 0; b < PERSIST_POKE_DATA_SIZE; b++) {
                    hex[b * 2]     = digits[shinies[i].fullData[b] >> 4];
                    hex[b * 2 + 1] = digits[shinies[i].fullData[b] & 0xF];
                }
                hex[PERSIST_POKE_DATA_SIZE * 2] = '\0';
                entry["pokeData"] = hex;
            }
            shinyArray.push_back(entry);
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
            shinies[i].hasFullData = false;
            // Older saves have no pokeData; those entries regenerate at battle.
            if (arr[i].contains("pokeData") && arr[i]["pokeData"].is_string()) {
                const auto hexStr = arr[i]["pokeData"].get<nn::string>();
                if (hexStr.length() == PERSIST_POKE_DATA_SIZE * 2) {
                    auto nib = [](char c) -> int {
                        if (c >= '0' && c <= '9') return c - '0';
                        if (c >= 'a' && c <= 'f') return c - 'a' + 10;
                        if (c >= 'A' && c <= 'F') return c - 'A' + 10;
                        return -1;
                    };
                    bool ok = true;
                    for (int b = 0; b < PERSIST_POKE_DATA_SIZE && ok; b++) {
                        int hi = nib(hexStr[b * 2]), lo = nib(hexStr[b * 2 + 1]);
                        if (hi < 0 || lo < 0) { ok = false; break; }
                        shinies[i].fullData[b] = (uint8_t)((hi << 4) | lo);
                    }
                    shinies[i].hasFullData = ok;
                }
            }
            count++;
        }
        Logger::log("[SymbolEnc] Loaded %d persistent shinies from save\n", count);
    }
};

void loadSymbolEncountersFromJson(const nn::json& saveFile);
nn::json getSymbolEncountersAsJson();
