#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/calc.h"
#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/Handler/Waza.h"
#include "externals/Dpr/Battle/Logic/SectionContainer.h"
#include "externals/Dpr/Battle/Logic/WAZADATA.h"
#include "data/moves.h"
#include "data/utils.h"
#include "move_handlers.h"
#include "romdata/romdata.h"
#include "utils/utils.h"

using namespace Dpr::Battle::Logic;

int32_t GetSecretPowerMove(EventFactor::EventHandlerArgs::Object** args) {
    switch (Common::GetGround(args))
    {
        case BtlGround::BTL_GROUND_GRASS:
            return array_index(MOVES, "Needle Arm");
        case BtlGround::BTL_GROUND_MIST:
            return array_index(MOVES, "Fairy Wind");
        case BtlGround::BTL_GROUND_ELEKI:
            return array_index(MOVES, "Thunder Shock");
        case BtlGround::BTL_GROUND_PHYCHO:
            return array_index(MOVES, "Confusion");
        default:
            break;
    }

    int32_t arenaId = Common::GetFieldSituation(args)->fields.bgComponent->fields.arenaIndex;
    return GetExtraArenaData(arenaId).secretPowerMove;
}

void HandlerSecretPowerAddSick(EventFactor::EventHandlerArgs::Object** args, uint8_t defender, uint32_t sickID, int32_t percent) {
    if (calc::IsOccurPer(percent)) {
        uint8_t causePokeID = Common::GetEventVar(args, EventVar::Label::POKEID_ATK);
        HandlerAddSick(args, causePokeID, defender, (Pml::WazaData::WazaSick)sickID, calc::MakeDefaultPokeSickCont((Pml::WazaData::WazaSick)sickID, causePokeID, false).fields.raw);
    }
}

void HandlerSecretPowerRankEffect(EventFactor::EventHandlerArgs::Object** args, uint8_t defender, uint32_t rankType,  int32_t percent) {
    if (calc::IsOccurPer(percent))
        HandlerRankEffect(args, Common::GetEventVar(args, EventVar::Label::POKEID_ATK), defender, (Pml::WazaData::WazaRankEffect)rankType, -1, false, false, false, true);
}

void HandlerSecretPowerShrink(EventFactor::EventHandlerArgs::Object** args, uint8_t defender, uint32_t arg, int32_t percent) {
    HandlerShrink(args, defender, percent);
}

void HandlerSecretPowerWazaExeStart(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    Logger::log("HandlerSecretPowerWazaExeStart\n");
    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    // Plays an additional move effect.
    // This plays before the actual move effect, which is not replaced.
    /*uint8_t atkPos = Common::GetExistFrontPokePos(args, pokeID);
    uint8_t defPos = Common::GetExistFrontPokePos(args, Common::GetEventVar(args, EventVar::Label::POKEID_TARGET1));
    int32_t waza = GetSecretPowerMove(args);
    HandlerPlayEffect(args, atkPos, defPos, waza, WAZADATA::GetType(waza));*/

    // Changes the effect waza to be the new move.
    // It just completely replaces the visuals with the new move, including the text.
    /*auto section = ((Section::Object*)(*args)->fields.pSectionContainer->instance()->fields.m_section_FromEvent_SetWazaEffectIndex);
    section->GetActionSharedData()->fields.wazaEffectParams->fields.effectWazaID = waza;*/
}

void HandlerSecretPowerDamageprocEndHitReal(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    if (Common::CheckShowDown(args))
        return;

    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    void (*effect) (EventFactor::EventHandlerArgs::Object**, uint8_t, uint32_t, int32_t);

    uint32_t arg = 0;
    switch (GetSecretPowerMove(args)) {
        case array_index(MOVES, "Spit Up"):
        case array_index(MOVES, "Thunder Shock"):
            effect = HandlerSecretPowerAddSick;
            arg = (uint32_t)Pml::PokePara::Sick::MAHI;
            break;

        case array_index(MOVES, "Rock Throw"):
        case array_index(MOVES, "Shadow Sneak"):
            effect = HandlerSecretPowerShrink;
            break;

        case array_index(MOVES, "Mud-Slap"):
            effect = HandlerSecretPowerRankEffect;
            arg = (uint32_t)Pml::WazaData::WazaRankEffect::HIT;
            break;

        case array_index(MOVES, "Water Pulse"):
            effect = HandlerSecretPowerRankEffect;
            arg = (uint32_t)Pml::WazaData::WazaRankEffect::ATTACK;
            break;

        case array_index(MOVES, "Ice Shard"):
            effect = HandlerSecretPowerAddSick;
            arg = (uint32_t)Pml::PokePara::Sick::KOORI;
            break;

        case array_index(MOVES, "Incinerate"):
            effect = HandlerSecretPowerAddSick;
            arg = (uint32_t)Pml::PokePara::Sick::YAKEDO;
            break;

        case array_index(MOVES, "Psywave"):
            effect = HandlerSecretPowerRankEffect;
            arg = (uint32_t)Pml::WazaData::WazaRankEffect::DEFENCE;
            break;

        case array_index(MOVES, "Needle Arm"):
            effect = HandlerSecretPowerAddSick;
            arg = (uint32_t)Pml::PokePara::Sick::NEMURI;
            break;

        case array_index(MOVES, "Fairy Wind"):
            effect = HandlerSecretPowerRankEffect;
            arg = (uint32_t)Pml::WazaData::WazaRankEffect::SP_ATTACK;
            break;

        case array_index(MOVES, "Confusion"):
            effect = HandlerSecretPowerRankEffect;
            arg = (uint32_t)Pml::WazaData::WazaRankEffect::AGILITY;
            break;

        default: // Default to Paralysis
            effect = HandlerSecretPowerAddSick;
            arg = (uint32_t)Pml::PokePara::Sick::MAHI;
            break;
    }

    uint8_t targetCount = Common::GetEventVar(args, EventVar::Label::TARGET_POKECNT);
    for (uint8_t i=0; i<targetCount; i++) {
        auto target = (EventVar::Label)(i + (uint16_t)EventVar::Label::POKEID_TARGET1);
        effect(args, Common::GetEventVar(args, target), arg, WAZADATA::GetSickPer(array_index(MOVES, "Secret Power")));
    }
}

EventFactor::EventHandlerTable::Array* ADD_SecretPower() {
    EventFactor::EventHandlerTable::Array* table = getExtraMoveHandlers()->HandlerTable_SecretPower;
    if (table == nullptr) {
        table = CreateEventHandlerTable(1);
        table->m_Items[0] = CreateMoveEventHandler(EventID::DAMAGEPROC_END_HIT_REAL, (Il2CppMethodPointer)&HandlerSecretPowerDamageprocEndHitReal);
        // FOR REPLACING THE MOVE EFFECT, DOESN'T WORK CURRENTLY
        //table->m_Items[1] = CreateMoveEventHandler(EventID::WAZA_EXE_START, (Il2CppMethodPointer)&HandlerSecretPowerWazaExeStart);
        getExtraMoveHandlers()->HandlerTable_SecretPower = table;
    }
    return table;
}

void Handlers_SecretPower(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Secret Power"), (Il2CppMethodPointer)&ADD_SecretPower);
    getExtraMoveHandlers()->currentIndex++;
}