#include "data/features.h"
#include "data/utils.h"

#include "features/activated_features.h"
#include "features/battle/battle.h"

void exl_battle_features_main() {
    if (IsActivatedBattleFeature(array_index(BATTLE_FEATURES, "Move Handlers")))
        exl_move_handlers_main();
    /*if (IsActivatedBattleFeature(array_index(BATTLE_FEATURES, "Ability Handlers")))
        exl_ability_handlers_main();*/
    if (IsActivatedBattleFeature(array_index(BATTLE_FEATURES, "Field Handlers")))
        exl_field_handlers_main();
    if (IsActivatedBattleFeature(array_index(BATTLE_FEATURES, "Side Handlers")))
        exl_side_handlers_main();
    if (IsActivatedBattleFeature(array_index(BATTLE_FEATURES, "Battle Situation")))
        exl_battle_situation_main();
}