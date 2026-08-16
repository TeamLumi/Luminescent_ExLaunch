#include "features/commands/utils/cmd_utils.h"

#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/FieldPoketch.h"
#include "externals/PlayerWork.h"
#include "externals/poketool/poke_memo/poketool_poke_memo.h"
#include "externals/ZukanWork.h"

#include "logger/logger.h"

bool AddPokemonExtra(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_ADD_POKEMON_EXTRA\n");
    system_load_typeinfo(0x43be);

    EvData::Aregment::Array* args = manager->fields._evArg;

    if (args->max_length >= 4) {

        auto monsNo = GetWorkOrIntValue(args->m_Items[1]);
        auto formNo = GetWorkOrIntValue(args->m_Items[2]);
        auto level = GetWorkOrIntValue(args->m_Items[3]);
        auto item = -1; // Defaults to none in case arg not given
        auto maxIVs = -1; // Defaults to none in case arg not given
        auto ball = -1; // Defaults to regular poke ball in case arg not given
        auto shiny = -1; // Defaults to regular shiny calculation in case arg not given
        auto gender = -1; // Defaults to regular gender calculation in case arg not given
        auto formArg = -1; // Defaults to no variant in case arg not given
        auto nature = -1; // Defaults to regular nature calculation in case arg not given
        auto ability = -1; // Defaults to regular ability calculation in case arg not given

        if (args->max_length >= 5) {
            item = GetWorkOrIntValue(args->m_Items[4]);
            if (item == -1) {
                item = 0;
            }
        }
        if (args->max_length >= 6) {
            maxIVs = GetWorkOrIntValue(args->m_Items[5]);
        }
        if (args->max_length >= 7) {
            ball = GetWorkOrIntValue(args->m_Items[6]);
        }
        if (args->max_length >= 8) {
            shiny = GetWorkOrIntValue(args->m_Items[7]);
        }
        if (args->max_length >= 9) {
            gender = GetWorkOrIntValue(args->m_Items[8]);
        }
        if (args->max_length >= 10) {
            formArg = GetWorkOrIntValue(args->m_Items[9]);
        }
        if (args->max_length >= 11) {
            nature = GetWorkOrIntValue(args->m_Items[10]);
        }
        if (args->max_length >= 12) {
            ability = GetWorkOrIntValue(args->m_Items[11]);
        }

        Pml::PokePara::InitialSpec::Object* initialSpec = Pml::PokePara::InitialSpec::newInstance();
        initialSpec->fields.monsno = monsNo;
        initialSpec->fields.formno = formNo;
        initialSpec->fields.level = level;
        if (maxIVs >= 0) initialSpec->fields.talentVNum = maxIVs;
        if (gender >= 0) initialSpec->fields.sex = gender;
        if (nature >= 0) initialSpec->fields.seikaku = nature;
        if (ability >= 0) initialSpec->fields.tokuseiIndex = ability;
        auto coreParam = Pml::PokePara::PokemonParam::newInstance(initialSpec)->cast<Pml::PokePara::CoreParam>();
        if (item > 0) coreParam->SetItem(item);

        if (ball == -1) {
            ball = 4;
        }
        coreParam->SetGetBall(ball);

        if (shiny == 0) coreParam->SetRareType(Pml::PokePara::RareType::NOT_RARE); // Never shiny
        if (shiny == 1) coreParam->SetRareType(Pml::PokePara::RareType::CAPTURED); // Shiny
        if (shiny == 2) coreParam->SetRareType(Pml::PokePara::RareType::DISTRIBUTED); // Square shiny
        if (formArg >= 0) coreParam->SetMultiPurposeWork(formArg);

        PlayerWork::getClass()->initIfNeeded();
        auto pMyStatus = PlayerWork::get_playerStatus();
        auto placeNo = PlayerWork::get_zoneID();
        poketool::poke_memo::poketool_poke_memo::SetFromCapture(coreParam, pMyStatus, placeNo);

        auto party = PlayerWork::get_playerParty();
        auto member = reinterpret_cast<Pml::PokePara::PokemonParam::Object*>(coreParam);
        auto result = party->AddMember(member);

        //Resolves a vanilla bug with _ADD_POKEMON setting the dex entry even if there was no space in the party.
        if (result == true) {
            ZukanWork::SetPoke(member, 3);
            FieldPoketch::AddPokemonHistory(member);
        }
        else {
            Logger::log("No space in the party. Member not added and dex entry not set.\n");
        }

    }
    return true;
}