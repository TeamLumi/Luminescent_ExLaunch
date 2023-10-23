#include "utils.h"

#include "externals/Dpr/Battle/Logic/Section_FromEvent_RankEffect.h"

#include "logger/logger.h"

void HandlerRankEffect(EventFactor::EventHandlerArgs::Object** args, uint8_t causePokeID, uint8_t targetPokeID, Pml::WazaData::WazaRankEffect rankType, int8_t rankVolume, bool displayAbility, bool ignoreSubstitute, bool messageOnFail)
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
    Common::RankEffect(args, &rankEffectDesc);
}