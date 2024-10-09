#pragma once

#include "externals/PlayerWork.h"

#include "logger/logger.h"
#include "memory/json.h"

enum class ModVersion : int32_t {
    // Base game
    Vanilla,

    // Mod releases (in order!)
    Release_3_0,

    // Development version, always after the latest release
    Dev,
};

struct MainSaveData {
    ModVersion version;

    void Initialize() {
        version = ModVersion::Vanilla;
    }

    [[nodiscard]] nn::json ToJson() const {
        return {
                {"main", {
                        {"version", static_cast<int32_t>(version)}
                }}
        };
    }

    void FromJson(const nn::json& main) {
        Initialize();
        version = static_cast<ModVersion>(main["version"].get<int32_t>());
    }

};

void loadMain(bool isBackup);
nn::json saveMain();
