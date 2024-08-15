#pragma once

#include "externals/il2cpp-api.h"
#include "externals/System/String.h"
#include "externals/UnityEngine/ScriptableObject.h"

namespace XLSXContent {
    struct BattleSetupEffectLots : ILClass<BattleSetupEffectLots, 0x04c5aad8> {
        struct SheetAttEffTable : ILClass<SheetAttEffTable> {
            struct Fields {
                int32_t AttributeEx;
                int32_t ArenaID;
                int32_t EffectID;
                System::String::Object* SoundEventName;
            };
        };

        struct SheetRuleEffTable : ILClass<SheetRuleEffTable> {
            struct Fields {
                int32_t Rule;
                int32_t BattleID;
            };
        };

        struct SheetArenaEffTable : ILClass<SheetArenaEffTable> {
            struct Fields {
                int32_t ArenaID;
                System::Int32_array* EffectID;
            };
        };

        struct Fields : UnityEngine::ScriptableObject::Fields {
            SheetAttEffTable::Array* AttEffTable;
            SheetRuleEffTable::Array* RuleEffTable;
            SheetArenaEffTable::Array* ArenaEffTable;
        };
    };
}
