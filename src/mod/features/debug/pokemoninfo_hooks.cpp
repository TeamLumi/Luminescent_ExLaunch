#include "exlaunch.hpp"

#include "externals/XLSXContent/PokemonInfo.h"

#include "logger/logger.h"

HOOK_DEFINE_TRAMPOLINE(PokemonInfo_Catalog_Checked) {
    static XLSXContent::PokemonInfo::SheetCatalog::Object* Callback(int32_t uniqueId) {
        Logger::log("Checking Catalog of %d\n", uniqueId);
        auto catalog = Orig(uniqueId);
        return catalog;
    }
};

void exl_pokemoninfo_hooks_main() {
    PokemonInfo_Catalog_Checked::InstallAtOffset(game::Address::GameData_DataManager_GetPokemonCatalog_Int32);
}
