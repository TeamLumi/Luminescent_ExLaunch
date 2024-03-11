#include "exlaunch.hpp"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/XLSXContent/PersonalTable.h"
#include "externals/Pml/Personal/PersonalSystem.h"
#include "externals/Pml/PokeParty.h"
#include "externals/PlayerWork.h"
#include "logger/logger.h"
#include "data/abilities.h"
#include "data/utils.h"


HOOK_DEFINE_REPLACE(WildHeldItems) {
    static uint16_t Callback(Pml::PokePara::PokemonParam::Object *pp,int32_t itemRnd,int32_t rnd1,int32_t rnd2) {
        system_load_typeinfo(0x3f54);
        system_load_typeinfo(0xa0a8);

        auto coreParam = reinterpret_cast<Pml::PokePara::CoreParam::Object *>(pp);

        uint16_t initItem = coreParam->GetItem();
        if ((-1 < itemRnd) && (initItem == 0)) {
            int32_t monsno = coreParam->GetMonsNo();
            uint16_t formNo = coreParam->GetFormNo();

            Pml::Personal::PersonalSystem::getClass()->initIfNeeded();

            XLSXContent::PersonalTable::SheetPersonal::Object *personalData = Pml::Personal::PersonalSystem::GetPersonalData(monsno, formNo);

            auto pokeParty = PlayerWork::get_playerParty();

            if (pokeParty->fields.m_memberCount == 0)
            {
                return 0;
            }

            auto firstPartyPokemon = pokeParty->GetMemberPointer(0);

            auto firstPokemonCoreParam = reinterpret_cast<Pml::PokePara::CoreParam::Object *>(firstPartyPokemon);


            int32_t firstRate = 0;
            int32_t secondRate = 0;

            if (!firstPokemonCoreParam->IsEgg(Pml::PokePara::EggCheckType::BOTH_EGG)) {
                auto abilityNo = firstPokemonCoreParam->GetTokuseiNo();

                if (abilityNo == array_index(ABILITIES, "Frisk")
                    || abilityNo == array_index(ABILITIES, "Compound Eyes")
                    || abilityNo == array_index(ABILITIES, "Super Luck"))
                {
                    firstRate = 60;
                    secondRate = 20;
                }
            }

            else
            {
                firstRate = 50;
                secondRate = 5;
            }

            int32_t item = 0;

            if (itemRnd < firstRate) {
                item = (personalData->fields).item1;
            }
            else if (itemRnd < secondRate + firstRate) {
                item = (personalData->fields).item2;
            }
            else {
                item = (personalData->fields).item3;
            }

            coreParam->SetItem(item);
            return item;
        }
        return 0;
    }
};

void exl_wild_held_items_main(){
    WildHeldItems::InstallAtOffset(0x02c3aab0);
};