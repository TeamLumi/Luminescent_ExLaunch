#pragma once

#include <common.hpp>
#include "sys/mem_layout.hpp"
#include "offsets.hpp"

namespace exl::util::modules {
    
    inline uintptr_t GetSelfStart() {
        return GetSelfModuleInfo().m_Total.m_Start;
    }
    
    inline uintptr_t GetTargetOffset(game::Location offset) {
        return GetMainModuleInfo().m_Total.m_Start + offsets::Resolve(offset);
    }

    inline uintptr_t GetTargetStart() {
        return GetMainModuleInfo().m_Total.m_Start;
    }

}
