#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Pml/Personal/PersonalSystem.h"
#include "data/species.h"
#include "data/utils.h"
#include "move_handlers.h"
#include "utils/utils.h"

using namespace Dpr::Battle::Logic;

uint8_t HandlerRelicSongGetFormID(uint8_t formNo, uint8_t targetFormNo) {
    return HighestMultiple(formNo, 2) + targetFormNo;
}

HOOK_DEFINE_REPLACE(Dpr_Battle_Logic_Handler_Waza_handler_InisieNoUta) {
    static void Callback(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
        if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
            return;

        BTL_POKEPARAM::Object* bpp = Common::GetPokeParam(args, pokeID);
        uint16_t dexID = bpp->GetMonsNo();
        if (dexID != array_index(SPECIES, "Meloetta"))
            return;

        uint8_t formNo = bpp->fields.m_formNo;

        // Swaps between 0-1 or 2-3 or 4-5 or...
        uint8_t ariaForm = HandlerRelicSongGetFormID(formNo, 0);
        uint8_t pirouetteForm = HandlerRelicSongGetFormID(formNo, 1);

        uint8_t nextForm = (formNo == ariaForm) ? pirouetteForm : ariaForm;
        if (nextForm >= Pml::Personal::PersonalSystem::GetPersonalData(dexID, 0)->fields.form_max)
            return;

        HandlerFormChange(args, pokeID, nextForm, false, false, true);
    }
};

void InstallHooks_Move_RelicSong() {
    Dpr_Battle_Logic_Handler_Waza_handler_InisieNoUta::InstallAtOffset(0x0181df60);
}