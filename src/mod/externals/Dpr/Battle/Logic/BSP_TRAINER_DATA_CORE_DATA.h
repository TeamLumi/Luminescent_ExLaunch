#pragma once
#include "externals/il2cpp-api.h"
#include "externals/System/String.h"
#include "externals/XLSXContent/TrainerTable.h"

namespace Dpr::Battle::Logic {
    struct BSP_TRAINER_DATA_CORE_DATA : ILClass<BSP_TRAINER_DATA_CORE_DATA> {
        struct Fields {
            int32_t tr_id;
            int32_t tr_type;
            void *tr_type_data; //XLSXContent::TrainerTable::SheetTrainerType::Object*
            void *trainer_data; //XLSXContent::TrainerTable::SheetTrainerData::Object*
            void *inst_trainer_data; //XLSXContent::TowerTrainerTable::SheetTrainerData::Object*
            int32_t btl_eff_id;
            uint8_t tr_group;
            uint8_t tr_sex;
            System::String::Object *model_id;
            int32_t color_id;
            uint32_t ai_bit;
            uint8_t gold;
            System::UInt16_array *use_item;
            System::String::Object *trtype_name_label;
            System::String::Object *name_label;
        };
    };
}
