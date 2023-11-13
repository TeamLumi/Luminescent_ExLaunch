#include "utils.h"

#include "externals/Dpr/Battle/Logic/Section_AddSick.h"
#include "externals/Dpr/Battle/Logic/Section_CureSick.h"
#include "externals/Dpr/Battle/Logic/Section_FromEvent_PlayWazaEffect.h"
#include "externals/Dpr/Battle/Logic/Section_FromEvent_RankEffect.h"
#include "externals/Dpr/Battle/Logic/Section_FromEvent_Shrink.h"

#include "logger/logger.h"

void HandlerAddSick(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID, uint8_t targetPokeID, Pml::WazaData::WazaSick sickID, int64_t sickCont)
{
    system_load_typeinfo(0xa9aa);
    auto addSickDesc = Section_AddSick::Description::newInstance();
    addSickDesc->fields.pokeID = pokeID;
    addSickDesc->fields.targetPokeID = targetPokeID;
    addSickDesc->fields.sickID = sickID;
    addSickDesc->fields.sickCont.fields.raw = sickCont;
    addSickDesc->fields.sickCause = SickCause::OTHER;
    addSickDesc->fields.isDisplayTokuseiWindow = false;
    addSickDesc->fields.isFailResultDisplay = false;
    Common::AddSick(args, &addSickDesc);
}

void HandlerCureSick(EventFactor::EventHandlerArgs::Object** args, uint8_t causePokeID, Pml::WazaData::WazaSick sickID, uint8_t targetPokeID)
{
    system_load_typeinfo(0x893f);
    auto cureSickDesc = Section_CureSick::Description::newInstance();
    cureSickDesc->fields.pokeID = causePokeID;
    cureSickDesc->fields.sick = (Dpr::Battle::Logic::WazaSickEx)sickID;
    cureSickDesc->fields.targetPokeID->m_Items[0] = targetPokeID;
    cureSickDesc->fields.targetPokeCount = 1;
    cureSickDesc->fields.isDisplayTokuseiWindow = false;
    cureSickDesc->fields.isStandardMessageDisable = false;
    Common::CureSick(args, &cureSickDesc);
}

void HandlerRankEffect(EventFactor::EventHandlerArgs::Object** args, uint8_t causePokeID, uint8_t targetPokeID, Pml::WazaData::WazaRankEffect rankType, int8_t rankVolume, bool displayAbility, bool ignoreSubstitute, bool messageOnFail, bool byWazaEffect)
{
    system_load_typeinfo(0x89b2);
    auto rankEffectDesc = Section_FromEvent_RankEffect::Description::newInstance();
    rankEffectDesc->fields.pokeID = causePokeID;
    rankEffectDesc->fields.targetPokeCount = 1;
    rankEffectDesc->fields.targetPokeID->m_Items[0] = targetPokeID;
    rankEffectDesc->fields.rankType = rankType;
    rankEffectDesc->fields.rankVolume = rankVolume;
    rankEffectDesc->fields.cause = RankEffectCause::OTHER;
    rankEffectDesc->fields.isDisplayTokuseiWindow = displayAbility;
    rankEffectDesc->fields.isMigawariThrew = ignoreSubstitute;
    rankEffectDesc->fields.isSpFailMessageDisplay = messageOnFail;
    rankEffectDesc->fields.byWazaEffect = byWazaEffect;
    Common::RankEffect(args, &rankEffectDesc);
}

void HandlerShrink(EventFactor::EventHandlerArgs::Object** args, uint8_t targetPokeID, uint8_t percentage)
{
    system_load_typeinfo(0x58b8);
    auto shrinkDesc = Section_FromEvent_Shrink::Description::newInstance();
    shrinkDesc->ctor();
    shrinkDesc->fields.pokeID = targetPokeID;
    shrinkDesc->fields.percentage = percentage;
    Common::Shrink(args, &shrinkDesc);
}

void HandlerPlayEffect(EventFactor::EventHandlerArgs::Object** args, uint8_t pos, int32_t waza, uint8_t wazaType)
{
    Logger::log("HandlerPlayEffect\n");
    system_load_typeinfo(0xa922);
    auto playEffectDesc = Section_FromEvent_PlayWazaEffect::Description::newInstance();
    playEffectDesc->ctor();
    playEffectDesc->fields.atkPos = pos;
    playEffectDesc->fields.defPos = pos;
    playEffectDesc->fields.waza = waza;
    playEffectDesc->fields.wazaType = wazaType;
    playEffectDesc->fields.turnType = 0;
    playEffectDesc->fields.isSyncEffect = false;
    Common::PlayWazaEffect(args, &playEffectDesc);
}