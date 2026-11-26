#include "exlaunch.hpp"

#include "data/species.h"
#include "data/utils.h"
#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventFactor.h"

HOOK_DEFINE_REPLACE(Dpr_Battle_Logic_Handler_Item_handler_Leek) {
    static void Callback(Dpr::Battle::Logic::EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
        using namespace Dpr::Battle::Logic;

        system_load_typeinfo(game::TypeInfo::Dpr_Battle_Logic_Handler_Item_handler_Naganegi);
        system_load_typeinfo(game::TypeInfo::Dpr_Battle_Logic_Handler_Item_common_Juel_Decide);

        Common::getClass()->initIfNeeded();

        uint32_t attackingMon = Common::GetEventVar(args, EventVar::Label::POKEID_ATK);
        if (attackingMon == pokeID)
        {
            BTL_POKEPARAM::Object* __this = Common::GetPokeParam(args, pokeID);
            uint16_t monsNo = __this->GetMonsNo();
            if ((int32_t)monsNo == array_index(SPECIES, "Farfetch'd") ||
                (int32_t)monsNo == array_index(SPECIES, "Sirfetch'd"))
            {
                int32_t critStage = Common::GetEventVar(args, EventVar::Label::CRITICAL_RANK);
                Common::RewriteEventVar(args, EventVar::Label::CRITICAL_RANK, critStage + 2);
            }
        }
    }
};

void exl_items_leek_main() {
    Dpr_Battle_Logic_Handler_Item_handler_Leek::InstallAtOffset(game::Address::Dpr_Battle_Logic_Handler_Item_handler_Naganegi);
};