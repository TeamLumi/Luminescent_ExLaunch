#pragma once

#include <common.hpp>
#include "sys/mem_layout.hpp"

namespace exl::util::modules {
    
    inline uintptr_t GetSelfStart() {
        return GetSelfModuleInfo().m_Total.m_Start;
    }
    
    inline uintptr_t GetTargetOffset(uintptr_t offset) {
        return GetMainModuleInfo().m_Total.m_Start + offset;
    }

    inline uintptr_t GetTargetStart() {
        return GetTargetOffset(0);
    }

    inline uintptr_t GetSDKOffset(uintptr_t offset) {
        return GetSdkModuleInfo().m_Total.m_Start + offset;
    }

    inline uintptr_t GetSDKStart() {
        return GetSDKOffset(0);
    }

}