#include "exlaunch.hpp"

#include "externals/GameData/DataManager.h"

#include "data/utils.h"
#include "data/species.h"

#include "save/save.h"

HOOK_DEFINE_REPLACE(UniqueID) {
    static int32_t Callback(int32_t monsNo, int32_t formNo, uint8_t sex, uint8_t rareType, bool isEgg) {
        system_load_typeinfo(0x3436);

        GameData::DataManager::getClass()->initIfNeeded();

        if (isEgg) {
            if (monsNo == array_index(SPECIES, "Manaphy"))
                return rareType && getCustomSaveData()->settings.shinyEggsEnabled ? 121 : 120;
            else
                return rareType && getCustomSaveData()->settings.shinyEggsEnabled ? 21 : 20;
        }

        auto notEggID = static_cast<int32_t>(
                (monsNo * 10000 + formNo * 100 + static_cast<uint>(sex) * 10) | static_cast<uint>(rareType != 0));

        return notEggID;
    }
};

void exl_visible_shiny_egg_main() {
    UniqueID::InstallAtOffset(0x02ccd170);
}