#include "exlaunch.hpp"
#include "externals/il2cpp.h"
#include "externals/il2cpp-api.h"
#include "logger/logger.h"
#include "util.h"
#include "externals/Dpr/Battle/Logic/BSP_TRAINER_DATA.h"
#include "externals/XLSXContent/TrainerTable.h"
#include "externals/PlayerWork.h"
#include "externals/BattleCharacterEntity.h"
#include "externals/Dpr/Battle/View/TrainerSimpleParam.h"
#include "externals/UnityEngine/_Object.h"
#include "externals/ColorVariation.h"
#include "externals/Dpr/Battle/View/BattleViewAssetManager.h"

using namespace Dpr::Battle::Logic;

HOOK_DEFINE_INLINE(SetupTrainerData) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto trData = reinterpret_cast<TrainerTable::SheetTrainerData::Object*>(ctx->X[19]);
        auto bspTrCoreData = reinterpret_cast<BSP_TRAINER_DATA_CORE_DATA::Object*>(ctx->X[8]);
        if (trData->fields.ColorID == 255) {
            Logger::log("[SetupTrainerData] ColorID is 255\n");
            bspTrCoreData->fields.color_id = PlayerWork::get_colorID();
            Logger::log("[SetupTrainerData] ColorID: %d\n", bspTrCoreData->fields.color_id);
        }
        else {
            Logger::log("[SetupTrainerData] ColorID is NOT 255\n");
            Logger::log("[SetupTrainerData] ColorID: %d\n", bspTrCoreData->fields.color_id);
            Logger::log("[SetupTrainerData] Testing Anyway\n");
            bspTrCoreData->fields.color_id = PlayerWork::get_colorID();;
//            bspTrCoreData->fields.color_id = ctx->W[9];
        }

        // bspTrCoreData->fields.color_id = (trData->fields.ColorID == 255) ? -1 : ctx->W[9]; <- Eventual Hook
    }
};

HOOK_DEFINE_TRAMPOLINE(GetColorID) {
    static int32_t Callback(BSP_TRAINER_DATA::Object* __this) {
        int32_t colorID = Orig(__this);
        Logger::log("[GetColorID] ColorID is %d.\n", colorID);
        return colorID;
    }
};

//HOOK_DEFINE_TRAMPOLINE(SetSkinColor) {
//    static void Callback(BattleCharacterEntity::Object* __this, int32_t colorId) {
//        if (colorId == -1) {
//            Logger::log("[SetSkinColor] Setting Skin Color\n");
//            system_load_typeinfo(0x20eb);
//            PlayerWork::getClass()->initIfNeeded();
//            auto colorVar = &(__this->fields)._colorVariation;
//            auto derefColorVar = *colorVar;
//            Logger::log("[SetSkinColor] Deref\n");
//
//
//            UnityEngine::_Object::getClass()->initIfNeeded();
//
//            if (UnityEngine::_Object::op_Equality(reinterpret_cast<UnityEngine::_Object::Object*>(derefColorVar), nullptr)) {
//                Logger::log("[SetSkinColor] Object not found\n");
//                derefColorVar = reinterpret_cast<ColorVariation::Object*>(
//                        reinterpret_cast<UnityEngine::Component::Object*>(__this)->GetComponent<ColorVariation>(
//                                UnityEngine::Component::Method$$ColorVariation$$GetComponent));
//                Logger::log("[SetSkinColor] Got component\n");
//                (__this->fields)._colorVariation = derefColorVar;
//                Logger::log("[SetSkinColor] Object set\n");
//            }
//
//            if (!UnityEngine::_Object::op_Equality(reinterpret_cast<UnityEngine::_Object::Object*>(derefColorVar), nullptr)) {
//                Logger::log("[SetSkinColor] Object found\n");
//                ((*colorVar)->fields).ColorIndex = PlayerWork::get_colorID();
//                Logger::log("[SetSkinColor] Value set\n");
//            }
//        }
//
//        else {
//            Orig(__this, colorId);
//        }
//    }
//};

HOOK_DEFINE_INLINE(SetSkinColorInline) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        Logger::log("[SetSkinColor] Original Color: %d\n", static_cast<int32_t>(ctx->W[19]));
        Logger::log("[SetSkinColor] Player Color ID: %d\n", PlayerWork::get_colorID());
        auto cVar = reinterpret_cast<ColorVariation::Object*>(ctx->X[8]);
        int32_t color = (static_cast<int32_t>(ctx->W[19]) == -1)
                ? PlayerWork::get_colorID()
                : ctx->W[19];

        cVar->fields.ColorIndex = color;
        Logger::log("[SetSkinColor] Color set to %d.\n", color);
    }
};

HOOK_DEFINE_TRAMPOLINE(LoadTrainer) {
    static void Callback(Dpr::Battle::View::BattleViewAssetManager::__c__DisplayClass24_0::Object* __this, UnityEngine::_Object::Object* x) {
        system_load_typeinfo(0x9675);
        Logger::log("[LoadTrainer] ColorID: %d\n", __this->fields.param.fields.colorID);
        if (__this->fields.param.fields.colorID == -1) {
            PlayerWork::getClass()->initIfNeeded();
            __this->fields.param.fields.colorID = PlayerWork::get_colorID();
            Logger::log("[LoadTrainer] New ColorID: %d\n", __this->fields.param.fields.colorID);
        }

        __this->fields.param.fields.colorID = -1;

        Orig(__this, x);
    }
};

HOOK_DEFINE_INLINE(LoadCharacterModel) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        Logger::log("[LoadCharacterModel] ctx->W[3] = %d. Setting to -1\n", ctx->W[3]);
        ctx->W[24] = -1;
    }
};

void exl_trainer_color_variations_main() {
    SetupTrainerData::InstallAtOffset(0x01ac48d8);
    GetColorID::InstallAtOffset(0x01ac4610);
    LoadTrainer::InstallAtOffset(0x01e4b9c0);
    SetSkinColorInline::InstallAtOffset(0x01d68454);
    LoadCharacterModel::InstallAtOffset(0x01be7ef8);
}

