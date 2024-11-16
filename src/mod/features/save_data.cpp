#include "data/features.h"
#include "data/utils.h"

#include "features/activated_features.h"
#include "features/save_data/save_data.h"

void exl_save_data_expansion_main() {
    if (IsActivatedSaveFeature(array_index(SAVE_FEATURES, "Badge Expansion")))
        exl_save_badges_expansion_main();
    if (IsActivatedSaveFeature(array_index(SAVE_FEATURES, "Box Expansion")))
        exl_save_box_expansion_main();
    if (IsActivatedSaveFeature(array_index(SAVE_FEATURES, "Dex Expansion")))
        exl_save_dex_expansion_main();
    if (IsActivatedSaveFeature(array_index(SAVE_FEATURES, "Roamer Expansion")))
        exl_more_roamers_main();
};