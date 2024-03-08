#include "externals/il2cpp.h"
#include "exlaunch.hpp"
#include "externals/Dpr/EncountTools.h"
#include "externals/PlayerWork.h"
#include "externals/Pml/PokePara/CoreParam.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/Pml/PokeParty.h"
#include "externals/UnityEngine/Random.h"
#include "externals/Pml/PokePara/EggCheckType.h"
#include "externals/XLSXContent/PersonalTable.h"
#include "externals/Pml/Personal/PersonalSystem.h"

#include "util.h"
#include "Logger/logger.h"



using namespace Pml;

HOOK_DEFINE_REPLACE(SetWildPokemonItem) {
    static uint16_t Callback(PokePara::PokemonParam::Object* pp, int32_t itemRnd, int32_t rnd1, int32_t rnd2) {

        const int32_t SUPERLUCK_ABILITYNO = 105;
        const int32_t COMPOUNDEYES_ABILITYNO = 14;

        const int32_t ITEM1_BASERATE = 50;
        const int32_t ITEM1_BOOSTEDRATE = 60;
        const int32_t ITEM2_BASERATE = 20;
        const int32_t ITEM2_BOOSTEDRATE = 20;

        system_load_typeinfo(0x3f8c);
        int32_t item1Rate = ITEM1_BASERATE;
        int32_t item2Rate = ITEM2_BASERATE;
        PokeParty::Object *party = PlayerWork::get_playerParty();
        auto firstPoke = (PokePara::CoreParam *) party->GetMemberPointer(0);

        if (!firstPoke->IsEgg(PokePara::EggCheckType::BOTH_EGG)) {
            int32_t ability = firstPoke->GetTokuseiNo();
            switch (ability) {
                case SUPERLUCK_ABILITYNO:
                case COMPOUNDEYES_ABILITYNO:
                    item1Rate = ITEM1_BOOSTEDRATE;
                    item2Rate = ITEM2_BOOSTEDRATE;
                    break;
                default:
                    item1Rate = ITEM1_BASERATE;
                    item2Rate = ITEM2_BASERATE;
                    break;
            }
        }

        system_load_typeinfo(0x3f54);
        auto ppCore = (PokePara::CoreParam::Object*) pp;
        uint16_t setItem = 0;
        uint16_t getItem = ppCore->GetItem();

        if ((-1 < itemRnd) && (getItem == 0)) {

            int32_t monsno = ppCore->GetMonsNo();
            uint16_t formno = ppCore->GetFormNo();


            XLSXContent::PersonalTable::SheetPersonal::Object* personalData = Personal::PersonalSystem::GetPersonalData(monsno, formno);

            if (itemRnd < rnd1) {
                setItem = personalData->fields.item1;
            }

            else if (itemRnd < rnd2 + rnd1) {
                setItem = personalData->fields.item2;
            }

            else {
                setItem = personalData->fields.item3;
            }

            ppCore->SetItem(setItem);

        }
        return setItem;
    }
};


