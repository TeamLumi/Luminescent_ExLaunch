#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/calc.h"
#include "externals/Dpr/Battle/Logic/Exp.h"
#include "externals/Dpr/Battle/Logic/ExpCalculator.h"
#include "externals/Dpr/Battle/Logic/MyStatus.h"
#include "externals/Dpr/Battle/Logic/MainModule.h"
#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "data/items.h"
#include "data/utils.h"

using namespace Dpr::Battle::Logic;
HOOK_DEFINE_REPLACE(CalcExp) {
    static void Callback(ExpCalculator::CalcExpWork::Object* result, MyStatus::Object* myStatus, MainModule::Object** mainModule, BTL_POKEPARAM::Object** bpp, BTL_POKEPARAM::Object** deadPoke) {
        system_load_typeinfo(0x479c);

        if ((*bpp)->IsFightEnable()) {
            auto param = (*bpp)->GetSrcDataConst()->cast<Pml::PokePara::CoreParam>();
            auto calc = Exp::CalcParam::newInstance();
            calc->fields.competitor = (*mainModule)->GetCompetitor(true);
            calc->fields.playerLanguageId = myStatus->fields.lang;
            calc->fields.levelCap = (*mainModule)->GetExpLevelCap();

            ExpCalculator::getClass()->initIfNeeded();
            system_load_typeinfo(0x479f);

            calc->fields.deadPokeExp = calc::PERSONAL_GetParam((*deadPoke)->GetMonsNo(), (*deadPoke)->fields.m_formNo, 9);
            calc->fields.deadPokeLevel = (*deadPoke)->GetValue(BTL_POKEPARAM::ValueID::BPP_LEVEL);
            calc->fields.getPokeLevel = (*bpp)->GetValue(BTL_POKEPARAM::ValueID::BPP_LEVEL);
            calc->fields.getPokeFriendship = (*mainModule)->GetPokeFriendship(*bpp);
            calc->fields.getPokeLanguageId = param->GetLangId();

            calc->fields.isMatch = (*deadPoke)->CONFRONT_REC_IsMatch((*bpp)->GetID()) || (*bpp)->GetItem() == array_index(ITEMS, "Exp. Share");
            calc->fields.isGakusyuSoutiOn = false;
            calc->fields.haveSiawasetamago = (*bpp)->GetItem() == array_index(ITEMS, "Lucky Egg");

            system_load_typeinfo(0x47a0);
            calc::getClass()->initIfNeeded();

            calc->fields.isEvoCanceledPoke = calc::PERSONAL_IsEvoCancelPokemon((*bpp)->GetMonsNo(), (*bpp)->fields.m_formNo, (*bpp)->GetValue(BTL_POKEPARAM::ValueID::BPP_LEVEL));
            calc->fields.isMyPoke = myStatus->IsMyPokemon(param);

            auto cResult = Exp::CalcResult::newInstance();

            Exp::CalcExp(reinterpret_cast<Exp::CalcParam **>(&calc), cResult);
            result->fields.exp = cResult->fields.exp;
            result->fields.bGakusyuSouti = cResult->fields.byGakusyuSouti;
            result->fields.fBonus = cResult->fields.isContainBonus;
        }
    }
};

void exl_exp_share_main() {
    CalcExp::InstallAtOffset(0x018f8c10);
}
