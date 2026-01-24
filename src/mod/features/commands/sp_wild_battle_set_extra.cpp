#include <data/abilities.h>
#include <data/utils.h>

#include <externals/RandomGroupWork.h>
#include <externals/Dpr/EncountTools.h>
#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/FieldManager.h"
#include "externals/PlayerWork.h"
#include "externals/Pml/PokePara/CoreParam.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"
#include "exlaunch.hpp"

bool SpWildBtlSetExtra(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_SP_WILD_BTL_SET_EXTRA\n");
    system_load_typeinfo(0x44d1);

    EvData::Aregment::Array* args = manager->fields._evArg;

    if (args->max_length >= 4)
    {
        auto monsNo = GetWorkOrIntValue(args->m_Items[1]);
        auto formNo = GetWorkOrIntValue(args->m_Items[2]);
        auto level = GetWorkOrIntValue(args->m_Items[3]);
        auto maxIVs = 0; // Doesn't enforce any maxIVs in case arg not given
        auto shiny = -1; // Defaults to regular shiny calculation in case arg not given
        auto gender = 255; // Defaults to regular gender calculation in case arg not given
        auto formArg = -1; // Defaults to no variant in case arg not given
        auto nature = 65535; // Defaults to regular nature calculation in case arg not given
        auto ability = -1; // Defaults to regular ability calculation in case arg not given
        auto isCantUseBall = 0; // Can catch statics by default

        if (args->max_length >= 5) {
            maxIVs = GetWorkOrIntValue(args->m_Items[4]);
        }
        if (args->max_length >= 6) {
            shiny = GetWorkOrIntValue(args->m_Items[5]);
        }
        if (args->max_length >= 7) {
            gender = GetWorkOrIntValue(args->m_Items[6]);
        }
        if (args->max_length >= 8) {
            formArg = GetWorkOrIntValue(args->m_Items[7]);
        }
        if (args->max_length >= 9) {
            nature = GetWorkOrIntValue(args->m_Items[8]);
        }
        if (args->max_length >= 10) {
            ability = GetWorkOrIntValue(args->m_Items[9]);
        }
        if (args->max_length >= 11) {
            isCantUseBall = GetWorkOrIntValue(args->m_Items[10]);
        }

        manager->SetBattleReturn();
        FieldManager::Object* fieldManager = FieldManager::getClass()->static_fields->_Instance_k__BackingField->instance();

        system_load_typeinfo(0x4931);
        PlayerWork::getClass()->initIfNeeded();

        auto playerparty = PlayerWork::get_playerParty();
        auto lead = playerparty->GetMemberPointer(0);

        if (!lead->IsEgg(EggType::BOTH)) {
            switch (lead->GetTokuseiNo()) {
                case array_index(ABILITIES, "Synchronize"): {
                        nature = lead->GetSeikaku();
                        break;
                    }
                case array_index(ABILITIES, "Cute Charm"): {
                        auto rndSex = RandomGroupWork::RandomValue(3);
                        if (rndSex < 1)
                            break;
                        auto paramSex = lead->GetSex();
                        if (paramSex == 0)
                            gender = 1;
                        else if (paramSex == 1)
                            gender = 0;
                        break;
                }
            }
        }

        Dpr::EncountTools::getClass(0x4c59e90)->initIfNeeded();

        // Creates a party
        auto enemyparty = Dpr::EncountTools::CreateSimpleParty(monsNo, level, 0, 1, nullptr, gender, nature, 255, 65535, formNo, 0, maxIVs);
        // Modifies the first Pokemon in the party
        auto p0 = enemyparty->GetMemberPointer(0);
        auto coreParam = reinterpret_cast<Pml::PokePara::CoreParam::Object*>(p0);
        if (shiny == 0) coreParam->SetRareType(Pml::PokePara::RareType::NOT_RARE); // Never shiny
        if (shiny == 1) coreParam->SetRareType(Pml::PokePara::RareType::CAPTURED); // Shiny
        if (shiny == 2) coreParam->SetRareType(Pml::PokePara::RareType::DISTRIBUTED); // Square shiny
        if (formArg >= 0) coreParam->SetMultiPurposeWork(formArg);
        if (ability >= 0) coreParam->SetTokuseiIndex(ability);

        fieldManager->EventWildBattleParty(enemyparty, false, true, false, isCantUseBall);

        return true;
    }
}