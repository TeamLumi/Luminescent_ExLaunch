#include "exlaunch.hpp"

#include "data/persons.h"
#include "externals/FlagWork.h"
#include "externals/System/String.h"

#include "logger/logger.h"

System::String::Object* GenerateFieldModelFileName()
{
    int32_t id = FlagWork::GetWork(FlagWork_Work::WORK_4000);
    if (id < FIELD_PERSON_PATH_COUNT)
        return System::String::Create(FIELD_PERSON_PATHS[id]);
    else
        return System::String::Create("");
}

System::String::Object* GenerateBattleModelFileName()
{
    int32_t id = FlagWork::GetWork(FlagWork_Work::WORK_4001);
    if (id < BATTLE_PERSON_PATH_COUNT)
        return System::String::Create(BATTLE_PERSON_PATHS[id]);
    else
        return System::String::Create("");
}

HOOK_DEFINE_INLINE(Boutique_ReplaceModel) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto strPart1 = (System::String::Object*)ctx->X[0];
        auto strPart2 = (System::String::Object*)ctx->X[1];

        if (FlagWork::GetSysFlag(FlagWork_SysFlag::SYSFLAG_998))
        {
            if (System::String::op_Equality(strPart1, System::String::Create("persons/field/")))
                ctx->X[0] = (uint64_t)System::String::Concat(strPart1, GenerateFieldModelFileName());
            else if (System::String::op_Equality(strPart1, System::String::Create("persons/battle/")))
                ctx->X[0] = (uint64_t)System::String::Concat(strPart1, GenerateBattleModelFileName());
        }
        else
        {
            ctx->X[0] = (uint64_t)System::String::Concat(strPart1, strPart2);
        }
    }
};

void exl_boutique_model_main() {
    Boutique_ReplaceModel::InstallAtOffset(game::Address::Dpr_UI_CharacterModelView_Setup + 0x480);
}
