#include "exlaunch.hpp"

#include "externals/System/String.h"
#include "externals/UnityEngine/_Object.h"
#include "logger/logger.h"

void UnityLog(const char* prefix, Il2CppObject* message) {
    auto string = ((System::String::Object*) message)->asCString();
    Logger::log("[UNITY %s]: %s\n", prefix, string.c_str());
}

HOOK_DEFINE_TRAMPOLINE(InfoLog) {
    static void Callback(Il2CppObject* message) {
        UnityLog("INFO", message);
        Orig(message);
    }
};

HOOK_DEFINE_TRAMPOLINE(InfoLog2) {
    static void Callback(Il2CppObject* message, UnityEngine::_Object::Object* context) {
        UnityLog("INFO", message);
        Orig(message, context);
    }
};

HOOK_DEFINE_TRAMPOLINE(AssertionLog) {
    static void Callback(Il2CppObject* message) {
        UnityLog("ASSERT", message);
        Orig(message);
    }
};

HOOK_DEFINE_TRAMPOLINE(ErrorLog) {
    static void Callback(Il2CppObject* message) {
        UnityLog("ERROR", message);
        Orig(message);
    }
};

HOOK_DEFINE_TRAMPOLINE(ErrorLog2) {
    static void Callback(Il2CppObject* message, UnityEngine::_Object::Object* context) {
        UnityLog("ERROR", message);
        Orig(message, context);
    }
};

HOOK_DEFINE_TRAMPOLINE(ExceptionLog) {
    static void Callback(Il2CppObject* message) {
        UnityLog("EXCEPTION", message);
        Orig(message);
    }
};

HOOK_DEFINE_TRAMPOLINE(ExceptionLog2) {
    static void Callback(Il2CppObject* message, UnityEngine::_Object::Object* context) {
        UnityLog("EXCEPTION", message);
        Orig(message, context);
    }
};

HOOK_DEFINE_TRAMPOLINE(WarningLog) {
    static void Callback(Il2CppObject* message) {
        UnityLog("WARNING", message);
        Orig(message);
    }
};

HOOK_DEFINE_TRAMPOLINE(WarningLog2) {
    static void Callback(Il2CppObject* message, UnityEngine::_Object::Object* context) {
        UnityLog("WARNING", message);
        Orig(message, context);
    }
};


void exl_unity_log_main() {
    InfoLog::InstallAtOffset(game::Address::UnityEngine_Debug_Log_Object);
    InfoLog2::InstallAtOffset(game::Address::UnityEngine_Debug_Log_Object_UnityObject);
    AssertionLog::InstallAtOffset(game::Address::UnityEngine_Debug_LogAssertion);
    ErrorLog::InstallAtOffset(game::Address::UnityEngine_Debug_LogError_Object);
    ErrorLog2::InstallAtOffset(game::Address::UnityEngine_Debug_LogError_Object_UnityObject);
    ExceptionLog::InstallAtOffset(game::Address::UnityEngine_Debug_LogException_Exception);
    ExceptionLog2::InstallAtOffset(game::Address::UnityEngine_Debug_LogException_Exception_UnityObject);
    WarningLog::InstallAtOffset(game::Address::UnityEngine_Debug_LogWarning_Object);
    WarningLog2::InstallAtOffset(game::Address::UnityEngine_Debug_LogWarning_Object_UnityObject);
}