#include "exlaunch.hpp"
#include "externals/il2cpp.h"
#include "logger/logger.h"
#include "externals/System/_Object.h"

void do_log(const char* prefix, Il2CppObject* message) {
    // Game seems to freeze when this is enabled?
//    auto string = System::_Object::ToString(message)->asCString();
//    Logger::log("IL2CPP %s: %s\n", prefix, string.c_str());
}

/*HOOK_DEFINE_REPLACE(DebugLog) {
    static void Callback(Il2CppObject* message) {
        do_log("Debug", message);
    }
};

HOOK_DEFINE_REPLACE(DebugLogWarning) {
    static void Callback(Il2CppObject* message) {
        do_log("Warn", message);
    }
};

HOOK_DEFINE_REPLACE(DebugLogError) {
    static void Callback(Il2CppObject* message) {
        do_log("Error", message);
    }
};

HOOK_DEFINE_REPLACE(DebugLogException) {
    static void Callback(Il2CppObject* message) {
        do_log("Exception", message);
    }
};

HOOK_DEFINE_REPLACE(DebugLogAssertion) {
    static void Callback(Il2CppObject* message) {
        do_log("Assertion", message);
    }
};*/

HOOK_DEFINE_TRAMPOLINE(FUN_710025c3d0_Il2CppException) {
    static void* Callback(void* param_1, char* param_2, char* param_3, void* param_4) {
        Logger::log("[EXCEPTION OCCURRED] %s.%s\n", param_2, param_3);
        return Orig(param_1, param_2, param_3, param_4);
    }
};

void exl_il2cpp_log_main() {
    /*DebugLog::InstallAtOffset(0x026a9c10);
    DebugLog::InstallAtOffset(0x026a9d50);
    DebugLog::InstallAtOffset(0x026a9ea0);
    DebugLogWarning::InstallAtOffset(0x026aa660);
    DebugLogWarning::InstallAtOffset(0x026aa7a0);
    DebugLogWarning::InstallAtOffset(0x026aa8f0);
    DebugLogWarning::InstallAtOffset(0x026aaa40);
    DebugLogError::InstallAtOffset(0x026a9ff0);
    DebugLogError::InstallAtOffset(0x026aa130);
    DebugLogError::InstallAtOffset(0x026aa280);
    DebugLogError::InstallAtOffset(0x026aa3d0);
    DebugLogException::InstallAtOffset(0x026a16d0);
    DebugLogException::InstallAtOffset(0x026aa520);
    DebugLogAssertion::InstallAtOffset(0x02c378d0);
    DebugLogAssertion::InstallAtOffset(0x02c378e0);
    DebugLogAssertion::InstallAtOffset(0x02c378f0);
    DebugLogAssertion::InstallAtOffset(0x02c37900);*/

    FUN_710025c3d0_Il2CppException::InstallAtOffset(0x025c3d0);
}
