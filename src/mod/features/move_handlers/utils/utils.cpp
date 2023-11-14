#include "utils.h"

#include "externals/Dpr/Battle/Logic/Section_AddSick.h"
#include "externals/Dpr/Battle/Logic/Section_CureSick.h"
#include "externals/Dpr/Battle/Logic/Section_FromEvent_PlayWazaEffect.h"
#include "externals/Dpr/Battle/Logic/Section_FromEvent_RankEffect.h"
#include "externals/Dpr/Battle/Logic/Section_FromEvent_SetWazaEffectEnable.h"
#include "externals/Dpr/Battle/Logic/Section_FromEvent_SetWazaEffectIndex.h"
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

void HandlerPlayEffect(EventFactor::EventHandlerArgs::Object** args, uint8_t atkPos, uint8_t defPos, int32_t waza, uint8_t wazaType)
{
    system_load_typeinfo(0xa922);
    auto playEffectDesc = Section_FromEvent_PlayWazaEffect::Description::newInstance();
    playEffectDesc->fields.atkPos = atkPos;
    playEffectDesc->fields.defPos = defPos;
    playEffectDesc->fields.waza = waza;
    playEffectDesc->fields.wazaType = wazaType;
    playEffectDesc->fields.turnType = 0;
    playEffectDesc->fields.isSyncEffect = false;
    Common::PlayWazaEffect(args, &playEffectDesc);
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

void HandlerSetEffectEnable(EventFactor::EventHandlerArgs::Object** args)
{
    system_load_typeinfo(0xaa0c);
    auto setEffectEnableDesc = Section_FromEvent_SetWazaEffectEnable::Description::newInstance();
    Common::SetWazaEffectEnable(args, &setEffectEnableDesc);
}

void HandlerSetEffectIndex(EventFactor::EventHandlerArgs::Object** args, uint8_t effectIndex)
{
    system_load_typeinfo(0xa922);
    auto setEffectIndexDesc = Section_FromEvent_SetWazaEffectIndex::Description::newInstance();
    setEffectIndexDesc->fields.effectIndex = effectIndex;
    Common::SetWazaEffectIndex(args, &setEffectIndexDesc);
}

void HandlerShrink(EventFactor::EventHandlerArgs::Object** args, uint8_t targetPokeID, uint8_t percentage)
{
    system_load_typeinfo(0x58b8);
    auto shrinkDesc = Section_FromEvent_Shrink::Description::newInstance();
    shrinkDesc->fields.pokeID = targetPokeID;
    shrinkDesc->fields.percentage = percentage;
    Common::Shrink(args, &shrinkDesc);
}