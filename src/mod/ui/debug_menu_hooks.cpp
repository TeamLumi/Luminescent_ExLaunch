#include "exlaunch.hpp"

#include "logger/logger.h"
#include "helpers/InputHelper.h"

#include "data/species.h"
#include "data/utils.h"
#include "externals/DPData/Form_Enums.h"
#include "externals/Dpr/Message/MessageEnumData.h"
#include "externals/FieldCanvas.h"
#include "externals/Pml/PokeParty.h"
#include "externals/ZukanWork.h"
#include "save/save.h"

static bool enabled = false;
static int id = 1;

HOOK_DEFINE_TRAMPOLINE(ArenaHook) {
    static void Callback(void *battleSetupParam, Pml::PokeParty *iPtrEnemyParty,int32_t arenaID,int32_t mapAttrib,int32_t weatherType,bool isSwim,bool isFishing,int32_t partnerID,bool isCaptureDemo,int32_t safariBallNum,bool isSymbol,bool isMitu,void *overlapBgm,int32_t overlapSetupEffectId,bool isCantUseBall,MethodInfo *method) {
        if (enabled) {
            arenaID = id;
        }
        return Orig(battleSetupParam, iPtrEnemyParty, arenaID, mapAttrib, weatherType, isSwim, isFishing, partnerID, isCaptureDemo, safariBallNum, isSymbol, isMitu, overlapBgm, overlapSetupEffectId, isCantUseBall, method);
    }
};

void setArenaSettings(bool e, int i) {
    enabled = e;
    id = i;
}

HOOK_DEFINE_TRAMPOLINE(CollissionOverride) {
    static uint32_t Callback(void* outVelocity, void* outHitNormal, float* outHitAngle, void* worldPosition, void* worldVelocity, float entityRadius, void* checkGridFunc, uint32_t ignoreDir) {
        if (InputHelper::isHoldL()) {
            return 0;
        }
        return Orig(outVelocity, outHitNormal, outHitAngle, worldPosition, worldVelocity, entityRadius, checkGridFunc, ignoreDir);
    }
};

HOOK_DEFINE_TRAMPOLINE(CollissionOverride2) {
    static bool Callback(void* origin, float radius, void* direction, void* hitInfo, float maxDistance, int32_t layerMask) {
        if (InputHelper::isHoldL()) {
            return false;
        }
        return Orig(origin, radius, direction, hitInfo, maxDistance, layerMask);
    }
};

static System::String* lastString = nullptr;

HOOK_DEFINE_TRAMPOLINE(LastAreaName) {
    static void* Callback(System::String* string) {
        lastString = string;
        return Orig(string);
    }
};

void showAreaName() {
    if (lastString != nullptr) {
        FieldCanvas::ResetAreaNameWindow();
        FieldCanvas::OpenAreaNameWindow(lastString);
    }
}

static bool flyOverride = false;

void setFlyOverride(bool b) {
    flyOverride = b;
}

static bool dexGetStatusOverride = false;

void setDexGetStatusOverride(bool b) {
    dexGetStatusOverride = b;
}

void setFullDex(int32_t getStatus) {
    for (int i=1; i<=DexSize; i++) {
        ZukanWork::SetPoke(i, getStatus, 0, 0, true);
        ZukanWork::SetPoke(i, getStatus, 1, 0, true);
        ZukanWork::SetPoke(i, getStatus, 0, 0, false);
        ZukanWork::SetPoke(i, getStatus, 1, 0, false);

        ZukanWork::AddLangFlag(i, Dpr::Message::MessageEnumData::MsgLangId::JPN);
        ZukanWork::AddLangFlag(i, Dpr::Message::MessageEnumData::MsgLangId::USA);
        ZukanWork::AddLangFlag(i, Dpr::Message::MessageEnumData::MsgLangId::FRA);
        ZukanWork::AddLangFlag(i, Dpr::Message::MessageEnumData::MsgLangId::ITA);
        ZukanWork::AddLangFlag(i, Dpr::Message::MessageEnumData::MsgLangId::DEU);
        ZukanWork::AddLangFlag(i, Dpr::Message::MessageEnumData::MsgLangId::ESP);
        ZukanWork::AddLangFlag(i, Dpr::Message::MessageEnumData::MsgLangId::KOR);
        ZukanWork::AddLangFlag(i, Dpr::Message::MessageEnumData::MsgLangId::SCH);
        ZukanWork::AddLangFlag(i, Dpr::Message::MessageEnumData::MsgLangId::TCH);

        if (i == array_index(SPECIES, "Unown")) {
            for (int j=1; j<=(int)UnownForm::MAX; j++) {
                ZukanWork::SetPoke(i, getStatus, 0, j, true);
                ZukanWork::SetPoke(i, getStatus, 1, j, true);
                ZukanWork::SetPoke(i, getStatus, 0, j, false);
                ZukanWork::SetPoke(i, getStatus, 1, j, false);
            }
        }
        else if (i == array_index(SPECIES, "Castform")) {
            for (int j=1; j<=(int)CastformForm::MAX; j++) {
                ZukanWork::SetPoke(i, getStatus, 0, j, true);
                ZukanWork::SetPoke(i, getStatus, 1, j, true);
                ZukanWork::SetPoke(i, getStatus, 0, j, false);
                ZukanWork::SetPoke(i, getStatus, 1, j, false);
            }
        }
        else if (i == array_index(SPECIES, "Deoxys")) {
            for (int j=1; j<=(int)DeoxysForm::MAX; j++) {
                ZukanWork::SetPoke(i, getStatus, 0, j, true);
                ZukanWork::SetPoke(i, getStatus, 1, j, true);
                ZukanWork::SetPoke(i, getStatus, 0, j, false);
                ZukanWork::SetPoke(i, getStatus, 1, j, false);
            }
        }
        else if (i == array_index(SPECIES, "Burmy")) {
            for (int j=1; j<=(int)BurmyForm::MAX; j++) {
                ZukanWork::SetPoke(i, getStatus, 0, j, true);
                ZukanWork::SetPoke(i, getStatus, 1, j, true);
                ZukanWork::SetPoke(i, getStatus, 0, j, false);
                ZukanWork::SetPoke(i, getStatus, 1, j, false);
            }
        }
        else if (i == array_index(SPECIES, "Wormadam")) {
            for (int j=1; j<=(int)WormadamForm::MAX; j++) {
                ZukanWork::SetPoke(i, getStatus, 0, j, true);
                ZukanWork::SetPoke(i, getStatus, 1, j, true);
                ZukanWork::SetPoke(i, getStatus, 0, j, false);
                ZukanWork::SetPoke(i, getStatus, 1, j, false);
            }
        }
        else if (i == array_index(SPECIES, "Mothim")) {
            for (int j=1; j<=(int)MothimForm::MAX; j++) {
                ZukanWork::SetPoke(i, getStatus, 0, j, true);
                ZukanWork::SetPoke(i, getStatus, 1, j, true);
                ZukanWork::SetPoke(i, getStatus, 0, j, false);
                ZukanWork::SetPoke(i, getStatus, 1, j, false);
            }
        }
        else if (i == array_index(SPECIES, "Cherrim")) {
            for (int j=1; j<=(int)CherrimForm::MAX; j++) {
                ZukanWork::SetPoke(i, getStatus, 0, j, true);
                ZukanWork::SetPoke(i, getStatus, 1, j, true);
                ZukanWork::SetPoke(i, getStatus, 0, j, false);
                ZukanWork::SetPoke(i, getStatus, 1, j, false);
            }
        }
        else if (i == array_index(SPECIES, "Shellos")) {
            for (int j=1; j<=(int)ShellosForm::MAX; j++) {
                ZukanWork::SetPoke(i, getStatus, 0, j, true);
                ZukanWork::SetPoke(i, getStatus, 1, j, true);
                ZukanWork::SetPoke(i, getStatus, 0, j, false);
                ZukanWork::SetPoke(i, getStatus, 1, j, false);
            }
        }
        else if (i == array_index(SPECIES, "Gastrodon")) {
            for (int j=1; j<=(int)GastrodonForm::MAX; j++) {
                ZukanWork::SetPoke(i, getStatus, 0, j, true);
                ZukanWork::SetPoke(i, getStatus, 1, j, true);
                ZukanWork::SetPoke(i, getStatus, 0, j, false);
                ZukanWork::SetPoke(i, getStatus, 1, j, false);
            }
        }
        else if (i == array_index(SPECIES, "Rotom")) {
            for (int j=1; j<=(int)RotomForm::MAX; j++) {
                ZukanWork::SetPoke(i, getStatus, 0, j, true);
                ZukanWork::SetPoke(i, getStatus, 1, j, true);
                ZukanWork::SetPoke(i, getStatus, 0, j, false);
                ZukanWork::SetPoke(i, getStatus, 1, j, false);
            }
        }
        else if (i == array_index(SPECIES, "Giratina")) {
            for (int j=1; j<=(int)GiratinaForm::MAX; j++) {
                ZukanWork::SetPoke(i, getStatus, 0, j, true);
                ZukanWork::SetPoke(i, getStatus, 1, j, true);
                ZukanWork::SetPoke(i, getStatus, 0, j, false);
                ZukanWork::SetPoke(i, getStatus, 1, j, false);
            }
        }
        else if (i == array_index(SPECIES, "Shaymin")) {
            for (int j=1; j<=(int)ShayminForm::MAX; j++) {
                ZukanWork::SetPoke(i, getStatus, 0, j, true);
                ZukanWork::SetPoke(i, getStatus, 1, j, true);
                ZukanWork::SetPoke(i, getStatus, 0, j, false);
                ZukanWork::SetPoke(i, getStatus, 1, j, false);
            }
        }
        else if (i == array_index(SPECIES, "Arceus")) {
            for (int j=1; j<=(int)ArceusForm::MAX; j++) {
                ZukanWork::SetPoke(i, getStatus, 0, j, true);
                ZukanWork::SetPoke(i, getStatus, 1, j, true);
                ZukanWork::SetPoke(i, getStatus, 0, j, false);
                ZukanWork::SetPoke(i, getStatus, 1, j, false);
            }
        }
    }

    ZukanWork::SetPoke(25, 3, 0, 0, true);
    ZukanWork::SetPoke(25, 3, 1, 0, true);
    ZukanWork::SetPoke(25, 3, 0, 0, false);
    ZukanWork::SetPoke(25, 3, 1, 0, false);
}

HOOK_DEFINE_TRAMPOLINE(FlyOverride) {
    static bool Callback(void* __this, bool useArrive) {
        if (flyOverride) {
            return true;
        }
        return Orig(__this, useArrive);
    }
};

HOOK_DEFINE_TRAMPOLINE(FlyOverride2) {
    static bool Callback(void* __this, void* cell) {
        if (flyOverride) {
            return true;
        }
        return Orig(__this, cell);
    }
};

HOOK_DEFINE_INLINE(ZukanWork$$SetPokeOverride) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto getStatusPtr = (DPData::GET_STATUS*)ctx->X[9];
        auto maxValue = (DPData::GET_STATUS)ctx->W[10];
        auto newValue = (DPData::GET_STATUS)ctx->W[23];

        if (dexGetStatusOverride)
            *getStatusPtr = newValue;
        else
            *getStatusPtr = maxValue;
    }
};

void exl_debug_menu_main() {
    ArenaHook::InstallAtOffset(0x02c3abc0);
    CollissionOverride::InstallAtOffset(0x01a763a0);
    CollissionOverride2::InstallAtOffset(0x02c1dd80);
    LastAreaName::InstallAtOffset(0x01786970);
    FlyOverride::InstallAtOffset(0x0184e780);
    FlyOverride2::InstallAtOffset(0x01850250);
    ZukanWork$$SetPokeOverride::InstallAtOffset(0x017dbe30);
}
