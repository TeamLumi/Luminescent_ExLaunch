#include "activated_features.h"
#include "data/features.h"
#include "data/utils.h"
#include "frontier/frontier_hooks.h"

void exl_battle_frontier_main() {
    if (IsActivatedFrontierFeature(array_index(FRONTIER_FEATURES, "Battle Hall")))
        exl_frontier_hall_main();
}