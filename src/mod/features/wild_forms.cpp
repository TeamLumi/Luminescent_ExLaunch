#include "exlaunch.hpp"

#include "data/abilities.h"
#include "data/species.h"
#include "data/utils.h"

#include "externals/Dpr/Field/EncountResult.h"
#include "externals/Dpr/Field/FieldEncount.h"
#include "externals/FieldManager.h"

#define _SPECIES(monsno) (monsno & 0xFFFF)
#define _FORM(monsno) ((monsno >> 16) & 0xFFFF)

uint16_t GetFormSingle(FieldManager::Object *__this, int32_t mons, int32_t karana, int32_t anno)
{
    if (mons == array_index(SPECIES, "Shellos") || mons == array_index(SPECIES, "Gastrodon")) {
        return karana != 0;
    }

    if (mons == array_index(SPECIES, "Unown")) {
        return __this->GetFormNo(mons, karana, anno);
    }

    return karana & 0xFFFF;
}

uint16_t GetFormDouble(FieldManager::Object *__this, int32_t mons, int32_t karana, int32_t anno)
{
    if (mons == array_index(SPECIES, "Shellos") || mons == array_index(SPECIES, "Gastrodon")) {
        return karana != 0;
    }

    if (mons == array_index(SPECIES, "Unown")) {
        return __this->GetFormNo(mons, karana, anno);
    }

    return (karana & 0xFFFF0000) >> 16;
}

HOOK_DEFINE_REPLACE(PatchDataInit) {
    static void Callback(Dpr::Field::EncountResult::Object **result, Dpr::Field::FieldEncount::ENC_FLD_SPA::Object *spa) {
        auto encount = *result;
        if (encount->fields.HatudouTokusei == 0 && spa->fields.Egg == 0 &&
            (spa->fields.Spa == array_index(ABILITIES, "Compound Eyes") || spa->fields.Spa == array_index(ABILITIES, "Super Luck"))) {
            encount->fields.HatudouTokusei = spa->fields.Spa;
        }

        auto encSpecies = encount->fields.Enemy;

        if (encSpecies->max_length == 0) {
            return;
        }

        for (uint64_t i = 0; i < encSpecies->max_length; i++) {
            int species = _SPECIES(encSpecies->m_Items[i]);
            int form = _FORM(encSpecies->m_Items[i]);

            if (species == array_index(SPECIES, "Shellos") || species == array_index(SPECIES, "Gastrodon")) {
                form = spa->fields.FormProb->m_Items[0] != 0;
            }

            if (i == 0) {
                // single battle
                encount->fields.karanaForm = form;
                encSpecies->m_Items[i] = species;
            } else if (i == 1) {
                // double battle
                encount->fields.karanaForm |= form << 16;
                encSpecies->m_Items[i] = species;
            }

            if (species == array_index(SPECIES, "Unown")) {
                encount->fields.annoForm = spa->fields.AnnoonTblType;
            }
        }
    }
};

HOOK_DEFINE_INLINE(PatchGetFormSingle) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        ctx->W[0] = GetFormSingle((FieldManager::Object *)ctx->X[0], ctx->W[1], ctx->W[2], ctx->W[3]);
    }
};

HOOK_DEFINE_INLINE(PatchGetFormDouble) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        ctx->W[0] = GetFormDouble((FieldManager::Object *)ctx->X[0], ctx->W[1], ctx->W[2], ctx->W[3]);
    }
};

void exl_wild_forms_main() {
    PatchDataInit::InstallAtOffset(0x019b4990);
    PatchGetFormSingle::InstallAtOffset(0x0179ea88);
    PatchGetFormSingle::InstallAtOffset(0x01d4d834);
    PatchGetFormDouble::InstallAtOffset(0x0179ebec);
}
