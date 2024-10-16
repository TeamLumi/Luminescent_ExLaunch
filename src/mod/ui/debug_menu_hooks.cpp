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

void setFullDex(int getStatus) {
    for (int i=1; i<=DexSize; i++) {
        ZukanWork::DebugSet(i, (DPData::GET_STATUS)getStatus, Pml::Sex::UNKNOWN, 0, true, (DPData::GET_STATUS)getStatus >= DPData::GET_STATUS::GET);
        ZukanWork::DebugSet(i, (DPData::GET_STATUS)getStatus, Pml::Sex::UNKNOWN, 0, false, (DPData::GET_STATUS)getStatus >= DPData::GET_STATUS::GET);

        ZukanWork::AddLangFlag(i, Dpr::Message::MessageEnumData::MsgLangId::JPN);
        ZukanWork::AddLangFlag(i, Dpr::Message::MessageEnumData::MsgLangId::USA);
        ZukanWork::AddLangFlag(i, Dpr::Message::MessageEnumData::MsgLangId::FRA);
        ZukanWork::AddLangFlag(i, Dpr::Message::MessageEnumData::MsgLangId::ITA);
        ZukanWork::AddLangFlag(i, Dpr::Message::MessageEnumData::MsgLangId::DEU);
        ZukanWork::AddLangFlag(i, Dpr::Message::MessageEnumData::MsgLangId::ESP);
        ZukanWork::AddLangFlag(i, Dpr::Message::MessageEnumData::MsgLangId::KOR);
        ZukanWork::AddLangFlag(i, Dpr::Message::MessageEnumData::MsgLangId::SCH);
        ZukanWork::AddLangFlag(i, Dpr::Message::MessageEnumData::MsgLangId::TCH);
    }

    ZukanWork::DebugSet(25, DPData::GET_STATUS::GET, Pml::Sex::UNKNOWN, 0, true, true);
    ZukanWork::DebugSet(25, DPData::GET_STATUS::GET, Pml::Sex::UNKNOWN, 0, false, true);

    ZukanWork::DebugSet(25, DPData::GET_STATUS::GET, Pml::Sex::UNKNOWN, 3, true, true);
    ZukanWork::DebugSet(25, DPData::GET_STATUS::GET, Pml::Sex::UNKNOWN, 3, false, true);
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

void exl_debug_menu_main() {
    ArenaHook::InstallAtOffset(0x02c3abc0);
    CollissionOverride::InstallAtOffset(0x01a763a0);
    CollissionOverride2::InstallAtOffset(0x02c1dd80);
    LastAreaName::InstallAtOffset(0x01786970);
    FlyOverride::InstallAtOffset(0x0184e780);
    FlyOverride2::InstallAtOffset(0x01850250);
}
