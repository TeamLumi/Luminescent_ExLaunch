#include "exlaunch.hpp"
#include "externals/il2cpp-api.h"

#include "externals/Dpr/Field/SwayGrass.h"

#include "logger/logger.h"

HOOK_DEFINE_REPLACE(Dpr_Field_SwayGrass_RensaTalent) {
    static uint8_t Callback() {
        system_load_typeinfo(game::TypeInfo::Dpr_Field_SwayGrass_RensaTalent);
        Dpr::Field::SwayGrass::getClass()->initIfNeeded();

        bool isSwayGrass = Dpr::Field::SwayGrass::getClass()->static_fields->is_swaygrass_flag;
        if (isSwayGrass)
        {
            // Cap chain to 99,999,999
            uint32_t chainLength = 99999999;
            if (Dpr::Field::SwayGrass::getClass()->static_fields->rensa_count + 1 < 99999999)
            {
                chainLength = Dpr::Field::SwayGrass::getClass()->static_fields->rensa_count + 1;
            }

            if (chainLength < 20) // 0-19
            {
                Logger::log("0 IVs\n");
                return 0;
            }
            else if (chainLength < 40) // 20-39
            {
                Logger::log("1 IVs\n");
                return 1;
            }
            else if (chainLength < 60) // 40-59
            {
                Logger::log("2 IVs\n");
                return 2;
            }
            else if (chainLength < 80) // 60-79
            {
                Logger::log("3 IVs\n");
                return 3;
            }
            else if (chainLength < 100) // 80-99
            {
                Logger::log("4 IVs\n");
                return 4;
            }
            else // 100+
            {
                Logger::log("5 IVs\n");
                return 5;
            }
        }

        return 0;
    }
};

void exl_poke_radar_fixes_main() {
    Dpr_Field_SwayGrass_RensaTalent::InstallAtOffset(game::Address::Dpr_Field_SwayGrass_RensaTalent);

    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p;
    auto inst = nn::vector<exl::patch::Instruction> {
        { game::Address::Dpr_Field_SwayGrass_BtlResultRensa + 0x2F4, Movz(W8, 100) },    // Catching on ring 4 is 100%
        { game::Address::Dpr_Field_SwayGrass_BtlResultRensa + 0x2E8, Movz(W8, 97) },     // Defeating on ring 4 is 97%
    };
    p.WriteInst(inst);
};