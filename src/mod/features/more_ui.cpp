#include "exlaunch.hpp"
#include "logger/logger.h"
#include "externals/UIWindowID.h"
#include "externals/Dpr/UI/BattleTowerRecordWindow.h"
#include "externals/FlagWork.h"
#include "externals/FlagWork_Enums.h"
#include "externals/UnityEngine/_Object.h"
#include "externals/BtlTowerWork.h"
#include "externals/RecordWork.h"
#include "externals/Dpr/UI/BoxWindow.h"

//HOOK_DEFINE_INLINE(EvCmdBTowerAppSetProc) {
//    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
//        Logger::log("[EvCmdBTowerAppSetProc] Creating UI Window.\n");
//        auto manager = (Dpr::UI::UIManager::Object*) ctx->X[0];
//        ILMethod<Dpr::UI::BattleTowerRecordWindow>& method = Dpr::UI::UIManager::Method$$CreateUIWindow_BattleTowerRecordWindow_;
//        auto windowId = FlagWork::GetFlag(FlagWork_Flag::FLAG_UI_WINDOW_SWITCH) ? UIWindowID::BATTLEHALL_RECORD : UIWindowID::BATTLETOWER_RECORD;
//        Logger::log("[EvCmdBTowerAppSetProc] Running manager->CreateUIWindow.\n");
//
//        auto uiWindow = manager->CreateUIWindow(windowId, method);
//        Logger::log("[EvCmdBTowerAppSetProc] UI Window created.\n");
//        ctx->X[0] = (u64) uiWindow;
//    }
//};

HOOK_DEFINE_INLINE(EvCmdBoxSetProc) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        Logger::log("[EvCmdBoxSetProc] Creating UI Window.\n");
        auto manager = (Dpr::UI::UIManager::Object*) ctx->X[0];
        ILMethod<Dpr::UI::BoxWindow>& method = Dpr::UI::UIManager::Method$$CreateUIWindow_BoxWindow_;
        auto windowId = FlagWork::GetFlag(FlagWork_Flag::FLAG_UI_WINDOW_SWITCH) ? UIWindowID::BATTLEHALL_TYPE_SELECT : UIWindowID::BOX;
        Logger::log("[EvCmdBoxSetProc] Running manager->CreateUIWindow.\n");

        auto uiWindow = manager->CreateUIWindow(windowId, method);
        Logger::log("[EvCmdBoxSetProc] UI Window created.\n");
        ctx->X[0] = (u64) uiWindow;
    }
};

//HOOK_DEFINE_TRAMPOLINE(EvCmdBoxSetProc) {
//    static bool Callback(Dpr::EvScript::EvDataManager::Object* manager) {
//        Logger::log("_BOX_SET_PROC\n");
//        return Orig(manager);
//    }
//};


HOOK_DEFINE_INLINE(OpLoadWindows_b__136_0) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto assetName = (System::String::Object*) ctx->X[1];
        auto name = (System::String::Object*) ctx->X[0];
        bool res = System::String::op_Equality(name, assetName);

        if (strcmp(name->asCString().c_str(), "BattleTowerRecord") == 0 ||
            strcmp(name->asCString().c_str(), "BattleHallRecord") == 0 ||
            strcmp(name->asCString().c_str(), "BattleHallTypeSelector") == 0) {
            Logger::log("[OpLoadWindows_b__136_0] Name: %s, AssetName: %s.\n",
                        name->asCString().c_str(), assetName->asCString().c_str());
            Logger::log("[OpLoadWindows_b__136_0] Returning %s.\n", res ? "true" : "false");
        }

        ctx->X[0] = res;

    }
};

HOOK_DEFINE_REPLACE(EvCmdBTowerAppSetProc) {
    static bool Callback(Dpr::EvScript::EvDataManager::Object* manager) {

        system_load_typeinfo(0x43d6);

        Logger::log("_BHALL_APP_SET_PROC\n");

        EvData::Aregment::Array* args = manager->fields._evArg;


        if (args->max_length >= 3) {
            auto argType1 = args->m_Items[1].fields.argType;
            auto argData1 = (float)args->m_Items[1].fields.data;
            auto argType2 = args->m_Items[2].fields.argType;
            auto argData2 = (float)args->m_Items[2].fields.data;
            int32_t towerMode;
            int32_t work1;
            int32_t work2;

            switch (argType1) {
                case 2:
                    work1 = FlagWork::GetWork((int32_t)argData1);
                    break;
                case 1:
                    work1 = (int)argData1;
                    break;
                default:
                    work1 = 0;
                    break;
            }

            switch (argType2) {
                case 2:
                    work2 = FlagWork::GetWork((int32_t)argData2);
                    break;
                case 1:
                    work2 = (int)argData2;
                    break;
                default:
                    work2 = 0;
                    break;
            }

            manager->fields._isOpenBtlTowerRecode = true;

            SmartPoint::AssetAssistant::SingletonMonoBehaviour::getClass()->initIfNeeded();
            Dpr::UI::UIManager::Object* uiManager = Dpr::UI::UIManager::instance();
            auto recordWindow = (Dpr::UI::BattleTowerRecordWindow::Object*)
                    uiManager->CreateUIWindow(UIWindowID::BATTLEHALL_RECORD,
                                              Dpr::UI::UIManager::Method$$CreateUIWindow_BattleTowerRecordWindow_);
            MethodInfo* mi = *UnityEngine::Events::UnityAction::Method$$Dpr_EvScript_EvDataManager__EvCmdBTowerAppSetProc__b__1143_0;
            auto onClosed = UnityEngine::Events::UnityAction::getClass(UnityEngine::Events::UnityAction::UIWindow_TypeInfo)->newInstance(manager, mi);
            auto parentOnClosed = &(recordWindow->fields).onClosed;
            *parentOnClosed = onClosed;
            Dpr::UI::BattleTowerRecordWindow::Param::Object* recordParam = Dpr::UI::BattleTowerRecordWindow::Param::newInstance();
            recordParam->fields.isSuspended = false;


            if (work1 == 1) {
                if (work2 == 1) {
                    recordParam->fields.recordType = 0;
                    towerMode = 0;
                }

                else {
                    recordParam->fields.recordType = 2;
                    towerMode = 2;
                }
            }

            else if (work2 == 1) {
                recordParam->fields.recordType = 1;
                towerMode = 1;
            }

            else {
                recordParam->fields.recordType = 3;
                towerMode = 3;
            }
            BtlTowerWork::getClass()->initIfNeeded();

            uint32_t round = BtlTowerWork::GetRenshou(towerMode);
            recordParam->fields.isSuspended = (round != 0);

            int32_t index;
            switch (recordParam->fields.recordType) {
                case 0:
                    recordParam->fields.consecutiveWins = (int32_t) RecordWork::Get(7);
                    recordParam->fields.maxConsecutiveWins = (int32_t) RecordWork::Get(6);
                    index = 6;
                    break;
                case 1:
                    recordParam->fields.consecutiveWins = (int32_t) RecordWork::Get(9);
                    recordParam->fields.maxConsecutiveWins = (int32_t) RecordWork::Get(8);
                    index = 8;
                    break;
                case 2:
                    recordParam->fields.consecutiveWins = (int32_t) RecordWork::Get(11);
                    recordParam->fields.maxConsecutiveWins = (int32_t) RecordWork::Get(10);
                    index = 2;
                    break;
                case 3:
                    recordParam->fields.consecutiveWins = (int32_t) RecordWork::Get(13);
                    recordParam->fields.maxConsecutiveWins = (int32_t) RecordWork::Get(12);
                    index = 3;
                    break;
            }

            if (index == 2 || index == 3) {
                recordParam->fields.rank = BtlTowerWork::GetRank(index);
            }

            recordWindow->Open(recordParam, -2);
        }
        return true;
    }
};

void exl_more_ui_main() {
    //EvCmdBoxSetProc::InstallAtOffset(0x02c699b0);
    EvCmdBoxSetProc::InstallAtOffset(0x02c69a2c); // Inline Offset
    EvCmdBTowerAppSetProc::InstallAtOffset(0x02c7cc30);
    //EvCmdBTowerAppSetProc::InstallAtOffset(0x02c7cd64); // Inline Offset
    OpLoadWindows_b__136_0::InstallAtOffset(0x017c7ccc);
}