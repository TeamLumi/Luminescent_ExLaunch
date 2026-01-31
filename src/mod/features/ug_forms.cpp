#include "exlaunch.hpp"

#include "externals/Dpr/UnderGround/UgPokeLottery.h"
#include "externals/Pml/Local/Random.h"
#include "externals/System/_Object.h"
#include "externals/System/Primitives.h"
#include "externals/System/Text/StringBuilder.h"
#include "externals/XLSXContent/UgEncount.h"

#include "logger/logger.h"

HOOK_DEFINE_REPLACE(Dpr_UnderGround_UgPokeLottery_LotteryPoke) {
    static void Callback(Dpr::UnderGround::UgPokeLottery::Object* __this, XLSXContent::UgEncount::Sheettable::Array* origList, System::Collections::Generic::List$$UgPokeLottery_PokeSlot::Object* slots, uint8_t rareTryCount) {
        system_load_typeinfo(0xa0b3);
        
        for (int i=0; i<slots->fields._size; i++)
        {
            uint32_t origListIdx = Pml::Local::Random::GetValue() % origList->max_length;

            int32_t inMonsNo = origList->m_Items[origListIdx]->fields.monsno;
            int32_t monsNo = inMonsNo & 0x0000FFFF;
            int32_t formNo = (inMonsNo & 0xFFFF0000) >> 16;

            Pml::PokePara::PokemonParam::Object* poke_param = __this->CreatePokemonParam_by_Tokusei(monsNo, rareTryCount);

            if (formNo != 0)
            {
                ((Pml::PokePara::CoreParam::Object*)poke_param)->ChangeFormNo((uint16_t)formNo, nullptr);
            }
            slots->fields._items->m_Items[i]->fields.param = poke_param;
        }
    }
};

HOOK_DEFINE_TRAMPOLINE(GetUgPokeData) {
    static void* Callback(int32_t monsNo) {
        return Orig(monsNo & 0x0000FFFF);
    }
};

void exl_ug_forms_main() {
    Dpr_UnderGround_UgPokeLottery_LotteryPoke::InstallAtOffset(0x018bfa90);
    GetUgPokeData::InstallAtOffset(0x01b1b540);
}
