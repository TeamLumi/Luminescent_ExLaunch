#include "exlaunch.hpp"

#include "data/balls.h"
#include "data/utils.h"

#include "externals/Dpr/Message/MessageEnumData.h"
#include "externals/Dpr/Message/MessageManager.h"
#include "externals/GameData/DataManager.h"
#include "externals/LocalKoukan.h"
#include "externals/PlayerWork.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/Pml/PokePara/PowerID.h"
#include "externals/poketool/poke_memo/poketool_poke_memo.h"
#include "externals/poketool/pokeplace/PlaceNo.h"
#include "externals/XLSXContent/LocalKoukanData.h"
#include "romdata/romdata.h"

Dpr::Message::MessageEnumData::MsgLangId GetUniformLanguage(Dpr::Message::MessageEnumData::MsgLangId langId)
{
    auto playerLangId = PlayerWork::get_msgLangID();

    if ((int32_t)langId == 0) // No language, so set to same language as player.
        return playerLangId;
    else if (langId != playerLangId) // Foreign trade, so set to the trade's language.
        return langId;
    else if (langId != Dpr::Message::MessageEnumData::MsgLangId::USA) // Foreign trade that matches player, so set to English.
        return Dpr::Message::MessageEnumData::MsgLangId::USA;
    else // Foreign trade that matches player and is English, so set to Japanese.
        return Dpr::Message::MessageEnumData::MsgLangId::JPN;
}

Pml::PokePara::PokemonParam::Object* CreateTradePokeParam(int32_t npcindex, Dpr::Message::MessageEnumData::MsgLangId lang)
{
    auto data = LocalKoukan::GetTargetData(npcindex, lang);
    auto extraData = GetExtraLocalTradeData(npcindex);

    system_load_typeinfo(0x5e90);

    auto spec = Pml::PokePara::InitialSpec::newInstance();

    int32_t formNo =      (data->fields.monsno & 0xFFFF0000) >> 16;  // Bits 16-31
    int32_t monsNo =       data->fields.monsno & 0x0000FFFF;         // Bits 0-15

    spec->fields.monsno = monsNo;
    spec->fields.formno = formNo;
    spec->fields.level = (uint16_t)data->fields.level;
    spec->fields.rareRnd = extraData.forceShiny ? 0x2FFFFFFFF : 0x3FFFFFFFF;
    spec->fields.id = (uint64_t)data->fields.trainerid;
    spec->fields.sex = (uint16_t)data->fields.sex;
    spec->fields.seikaku = (uint16_t)data->fields.seikaku;
    spec->fields.tokuseiIndex = (uint8_t)data->fields.tokusei;
    spec->fields.personalRnd = (uint64_t)data->fields.rand;
    spec->fields.randomSeed = (uint64_t)data->fields.rand;
    spec->fields.isRandomSeedEnable = true;

    if (!extraData.ivs.empty())
    {
        for (uint64_t i=0; i<spec->fields.talentPower->max_length; i++)
            spec->fields.talentPower->m_Items[i] = extraData.ivs[i];
    }

    auto pokeParam = Pml::PokePara::PokemonParam::newInstance(spec);
    auto coreParam = (Pml::PokePara::CoreParam::Object*)pokeParam;

    auto messageManager = Dpr::Message::MessageManager::get_Instance();

    coreParam->SetNickName(messageManager->GetNameMessage(System::String::Create("dp_scenario3"), data->fields.nickname_label));
    coreParam->SetParentName(messageManager->GetNameMessage(System::String::Create("dp_scenario3"), data->fields.name_label));
    coreParam->SetItem(data->fields.itemno);
    coreParam->SetLangId((uint32_t)GetUniformLanguage((Dpr::Message::MessageEnumData::MsgLangId)data->fields.language));
    coreParam->SetGetBall(extraData.ballId == array_index(BALLS, "--BALL ZERO--") ? array_index(BALLS, "Poké Ball") : extraData.ballId);
    coreParam->UpdateOwnerInfo(Pml::PokePara::OwnerInfo::newInstance(PlayerWork::get_playerStatus()));

    if (!extraData.evs.empty())
    {
        for (int32_t i=0; i<6; i++)
            coreParam->ChangeEffortPower((Pml::PokePara::PowerID)i, extraData.evs[i]);
    }

    if (!extraData.contestStats.empty())
    {
        for (int32_t i=0; i<5; i++)
            coreParam->SetCondition((Pml::PokePara::Condition)i, extraData.evs[i]);

        if (extraData.contestStats.size() >= 6)
            coreParam->SetCondition(Pml::PokePara::Condition::FUR, extraData.evs[5]);
    }

    for (uint64_t i=0; i<data->fields.waza->max_length && i<4; i++)
        coreParam->SetWaza(i, data->fields.waza->m_Items[i]);

    poketool::poke_memo::poketool_poke_memo::ClearPlaceTime(coreParam, poketool::poke_memo::DATA_TYPE_EGG_TAKEN);
    poketool::poke_memo::poketool_poke_memo::SetPlaceTime(coreParam, poketool::pokeplace::PlaceNo::SPECIAL_INNER_TRADE, poketool::poke_memo::DATA_TYPE_FIRST_CONTACT);
    poketool::poke_memo::poketool_poke_memo::SetGetLevel(coreParam);
    poketool::poke_memo::poketool_poke_memo::SetVersion(coreParam);

    coreParam->EndFastMode(coreParam->StartFastMode());

    return pokeParam;
}

HOOK_DEFINE_REPLACE(LocalKoukan_GetIndex) {
    static int32_t Callback(int32_t npcindex, Dpr::Message::MessageEnumData::MsgLangId lang) {
        return npcindex;
    }
};

HOOK_DEFINE_REPLACE(LocalKoukan_GetTargetData) {
    static XLSXContent::LocalKoukanData::Sheetdata::Object* Callback(int32_t npcindex, Dpr::Message::MessageEnumData::MsgLangId lang) {
        system_load_typeinfo(0x5e91);
        GameData::DataManager::getClass()->initIfNeeded();
        auto result = GameData::DataManager::getClass()->static_fields->LocalKoukanData->get_Item(npcindex);
        return result;
    }
};

HOOK_DEFINE_REPLACE(LocalKoukan_CreateOperation) {
    static LocalKoukan::Operation::Object* Callback(Pml::PokePara::PokemonParam::Object* myparam, int32_t npcindex,
            Dpr::Message::MessageEnumData::MsgLangId lang, bool istemoti, int32_t trayno, int32_t pos) {
        auto data = LocalKoukan::GetTargetData(npcindex, lang);

        int32_t targetFormNo =      (data->fields.target & 0xFFFF0000) >> 16;  // Bits 16-31
        int32_t targetMonsNo =       data->fields.target & 0x0000FFFF;         // Bits 0-15

        // Not the trade target monsno
        if (targetMonsNo != ((Pml::PokePara::CoreParam::Object*)myparam)->GetMonsNo())
            return nullptr;

        // Not the trade target formno (removed the check for now)
        /*if (targetFormNo != ((Pml::PokePara::CoreParam::Object*)myparam)->GetFormNo())
            return nullptr;*/

        system_load_typeinfo(0x5e8f);

        auto operation =  LocalKoukan::Operation::newInstance();
        operation->fields.from = myparam;
        operation->fields.other = CreateTradePokeParam(npcindex, lang);
        operation->fields.IsTemotiMode = istemoti;
        operation->fields.TrayNo = trayno;
        operation->fields.Pos = pos;

        return operation;
    }
};

void exl_local_trades_main() {
    LocalKoukan_GetIndex::InstallAtOffset(0x01af3390);
    LocalKoukan_GetTargetData::InstallAtOffset(0x01af32a0);
    LocalKoukan_CreateOperation::InstallAtOffset(0x01af3420);
}
