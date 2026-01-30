#include <data/abilities.h>
#include <data/utils.h>

#include <externals/AttributeID.h>
#include <externals/EntityManager.h>
#include <externals/GameManager.h>
#include <externals/PLAYREPORT_DATA.h>
#include <externals/RandomGroupWork.h>
#include <externals/Dpr/EncountTools.h>
#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/FieldManager.h"
#include "externals/FieldObjectEntity.h"
#include "externals/PlayerWork.h"
#include "externals/Pml/PokePara/CoreParam.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"
#include "exlaunch.hpp"

bool SpWildBtlSetExtra(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_SP_WILD_BTL_SET_EXTRA\n");
    system_load_typeinfo(0x44d1);
    system_load_typeinfo(0x4931);
    system_load_typeinfo(0x497c);
    system_load_typeinfo(0x4932);
    system_load_typeinfo(0x3f8c);

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
        System::String::Object* overrideBGM = System::String::Create("");

        if (args->max_length >= 5) {
            maxIVs = GetWorkOrIntValue(args->m_Items[4]);
        }
        if (args->max_length >= 6) {
            shiny = GetWorkOrIntValue(args->m_Items[5]);
        }
        if (args->max_length >= 7) {
            gender = GetWorkOrIntValue(args->m_Items[6]);
            if (gender == -1) {
                gender = 255;
            }
        }
        if (args->max_length >= 8) {
            formArg = GetWorkOrIntValue(args->m_Items[7]);
        }
        if (args->max_length >= 9) {
            nature = GetWorkOrIntValue(args->m_Items[8]);
            if (nature == -1) {
                nature = 65535;
            }
        }
        if (args->max_length >= 10) {
            ability = GetWorkOrIntValue(args->m_Items[9]);
        }
        if (args->max_length >= 11) {
            isCantUseBall = GetWorkOrIntValue(args->m_Items[10]);
        }
        if (args->max_length >= 12) {
            overrideBGM = GetStringText(manager,args->m_Items[11]);
        }
        if (args->max_length >= 5)  maxIVs = GetWorkOrIntValue(args->m_Items[4]);
        if (args->max_length >= 6)  shiny  = GetWorkOrIntValue(args->m_Items[5]);
        if (args->max_length >= 7) {
            gender = GetWorkOrIntValue(args->m_Items[6]);
            if (gender == -1) gender = 255;
        }

        manager->SetBattleReturn();

        FieldManager::Object* fieldManager = FieldManager::getClass()->static_fields->_Instance_k__BackingField->instance();

        // Cute Charm and Synchronize
        PlayerWork::getClass()->initIfNeeded();
        auto playerparty = PlayerWork::get_playerParty();
        auto lead = playerparty->GetMemberPointer(0);
        auto leadCore = lead->cast<Pml::PokePara::CoreParam>();

        if (!leadCore->IsEgg(Pml::PokePara::EggCheckType::BOTH_EGG)) {
            switch (leadCore->GetTokuseiNo()) {
            case array_index(ABILITIES, "Synchronize"): {
                    nature = leadCore->GetSeikaku();
                    break;
            }
            case array_index(ABILITIES, "Cute Charm"): {
                    auto rndSex = RandomGroupWork::RandomValue(3);
                    if (rndSex < 1)
                        break;
                    auto paramSex = leadCore->GetSex();
                    if (paramSex == Pml::Sex::MALE)
                        gender = 1;
                    else if (paramSex == Pml::Sex::FEMALE)
                        gender = 0;
                    break;
                }
            }
        }
        // Creates a party
        Dpr::EncountTools::getClass()->initIfNeeded();
        auto enemyparty = Dpr::EncountTools::CreateSimpleParty(monsNo, level, 0, 1, nullptr, gender, nature, 255, 65535, formNo, 0, maxIVs);
        // Modifies the first Pokemon in the party
        auto p0 = enemyparty->GetMemberPointer(0);
        auto coreParam = reinterpret_cast<Pml::PokePara::CoreParam::Object*>(p0);
        if (shiny == 0) coreParam->SetRareType(Pml::PokePara::RareType::NOT_RARE); // Never shiny
        if (shiny == 1) coreParam->SetRareType(Pml::PokePara::RareType::CAPTURED); // Shiny
        if (shiny == 2) coreParam->SetRareType(Pml::PokePara::RareType::DISTRIBUTED); // Square shiny
        if (formArg >= 0) coreParam->SetMultiPurposeWork(formArg);
        if (ability >= 0) coreParam->SetTokuseiIndex(ability);

        // Player position and attributes stuff
        EntityManager::getClass()->initIfNeeded();
        auto playerEntity = reinterpret_cast<FieldObjectEntity::Object*>(EntityManager::getClass()->static_fields->_activeFieldPlayer_k__BackingField);
        auto playerGridPos = playerEntity->get_gridPosition();
        auto height = playerEntity->get_Height();

        GameManager::getClass()->initIfNeeded();
        auto code = 0;
        auto stop = 0;
        GameManager::GetAttribute(playerGridPos, &code, &stop, false);
        auto attrRow = GameManager::GetAttributeTable(code);
        auto exRow = GameManager::GetAttributeEx(playerGridPos, height, false);
        auto attributeEx = 0;
        if (exRow) {
            attributeEx = exRow->fields.AttributeEx;
        }

        // Checks if player is surfing
        bool water = false;
        if (attrRow) {
            water = AttributeID::MATR_IsWater(attrRow->fields.Code);
        }

        // Gets ArenaID and ZoneID and stuff
        auto zoneID  = PlayerWork::get_zoneID();
        auto mapInfo = GameManager::get_mapInfo();
        auto zoneData = mapInfo->get_Item(zoneID);
        auto battleBg = zoneData->fields.BattleBg;
        // Might need to do some kind of max length if statement here for battleBg (Line 113 in the dump). I'll come back to that if so because I am confusion. I just like getting variables and scary maths make me sad :c

        int32_t arenaID = battleBg->m_Items[(int)water];

        // Checks if Pokemon is a legendary
        System::String::Object* encSec = nullptr;
        System::String::Object* bgm = nullptr;
        auto setupEffect = 0;
        fieldManager->GetLegendPokeEncountInfo(p0, &encSec, &arenaID, &bgm, &setupEffect);

        if (!System::String::IsNullOrEmpty(encSec)) {
            fieldManager->PreLoadEncEffect(encSec);
        }

        // Battle BGM Override (The whole reason that I put myself through this lol)
        if (!System::String::IsNullOrEmpty(overrideBGM)) {
            bgm = overrideBGM;
        }

        // The original method checks if this is the CaptureDemo and does something but idk if we need to worry about that
        PLAYREPORT_DATA::StartWildBattle(1); // Double check that I put this in the right file

        // Battle setup stuff
        auto battleSetupParam = PlayerWork::get_battleSetupParam();
        auto weatherType = fieldManager->GetBatleWeather();

        Dpr::EncountTools::SetupBattleWild(battleSetupParam, enemyparty, arenaID, attributeEx, weatherType, water, false, 0, false, -1, true, false, bgm, setupEffect, isCantUseBall);

        fieldManager->EncountStart(0, 0, 0);
    }
    return true;
}
