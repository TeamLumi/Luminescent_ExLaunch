#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/PlayerWork.h"
#include "externals/Pml/PokeParty.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"
#include "externals/poketool/poke_memo/poketool_poke_memo.h"

bool AddPokemonSilent(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_ADD_POKEMON_SILENT\n");

    EvData::Aregment::Array* args = manager->fields._evArg;

    if (args->max_length < 7) {
        Logger::log("[AddPokemonSilent] Not enough args\n");
        return true;
    }

    auto monsNo = GetWorkOrIntValue(args->m_Items[1]);
    auto formNo = GetWorkOrIntValue(args->m_Items[2]);
    auto level = GetWorkOrIntValue(args->m_Items[3]);
    auto item = GetWorkOrIntValue(args->m_Items[4]);
    auto maxIVs = GetWorkOrIntValue(args->m_Items[5]);
    auto ball = GetWorkOrIntValue(args->m_Items[6]);
    auto shiny = -1;
    auto gender = -1;
    auto formArg = -1;
    auto nature = -1;
    auto ability = -1;

    if (args->max_length >= 8)
        shiny = GetWorkOrIntValue(args->m_Items[7]);
    if (args->max_length >= 9)
        gender = GetWorkOrIntValue(args->m_Items[8]);
    if (args->max_length >= 10)
        formArg = GetWorkOrIntValue(args->m_Items[9]);
    if (args->max_length >= 11)
        nature = GetWorkOrIntValue(args->m_Items[10]);
    if (args->max_length >= 12)
        ability = GetWorkOrIntValue(args->m_Items[11]);

    Pml::PokePara::InitialSpec::Object* initialSpec = Pml::PokePara::InitialSpec::newInstance();
    initialSpec->fields.monsno = monsNo;
    initialSpec->fields.formno = formNo;
    initialSpec->fields.level = level;
    initialSpec->fields.talentVNum = maxIVs;
    if (gender >= 0) initialSpec->fields.sex = gender;
    if (nature >= 0) initialSpec->fields.seikaku = nature;
    if (ability >= 0) initialSpec->fields.tokuseiIndex = ability;

    auto coreParam = Pml::PokePara::PokemonParam::newInstance(initialSpec)->cast<Pml::PokePara::CoreParam>();
    if (item != 0) coreParam->SetItem(item);
    if (args->max_length >= 13) {
        auto nickname = GetStringText(manager, args->m_Items[12]);
        coreParam->SetNickName(nickname);
    }
    coreParam->SetGetBall(ball);
    if (shiny == 0) coreParam->SetRareType(Pml::PokePara::RareType::NOT_RARE);
    if (shiny == 1) coreParam->SetRareType(Pml::PokePara::RareType::CAPTURED);
    if (shiny == 2) coreParam->SetRareType(Pml::PokePara::RareType::DISTRIBUTED);
    if (formArg >= 0) coreParam->SetMultiPurposeWork(formArg);

    PlayerWork::getClass()->initIfNeeded();
    auto pMyStatus = PlayerWork::get_playerStatus();
    auto placeNo = PlayerWork::get_zoneID();
    poketool::poke_memo::poketool_poke_memo::SetFromCapture(coreParam, pMyStatus, placeNo);

    // Add directly to party without UI
    auto party = PlayerWork::get_playerParty();
    auto pokemonParam = reinterpret_cast<Pml::PokePara::PokemonParam::Object*>(coreParam);
    bool added = party->AddMember(pokemonParam);

    Logger::log("[AddPokemonSilent] monsNo=%d level=%d added=%d\n", monsNo, level, added);
    return true;
}
