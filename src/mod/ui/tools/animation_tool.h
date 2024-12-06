#pragma once

#include "data/clip_names.h"
#include "data/utils.h"
#include "externals/FlagWork.h"
#include "ui/base/collapsing_header.h"
#include "ui/base/element.h"

namespace ui {
    ELEMENT(AnimationTool) {
        AnimationTool() {
            auto header = CollapsingHeader::create([this](CollapsingHeader &_) {
                _.label = "Animation Tool";

                auto* loopAnim = _.Checkbox([](Checkbox &_) {
                    _.label = "Loop Animation";
                    _.enabled = true;
                });

                _.Checkbox([](Checkbox &_) {
                    _.label = "Force Battle Bundles in UI";
                    _.enabled = false;
                    _.onChange = [](bool value) {
                        FlagWork::SetSysFlag(FlagWork_SysFlag::SYSFLAG_999, value);
                    };
                });

                auto *fieldAnimationId = _.ComboSimple([](ComboSimple &_) {
                    _.label = "Field Animation ID";
                    _.items = FIELD_MON_CLIPS;
                    _.items_count = FIELD_MON_CLIP_COUNT;
                    _.selected = array_index(FIELD_MON_CLIPS, "Idle");
                });

                auto *battleAnimationId = _.ComboSimple([](ComboSimple &_) {
                    _.label = "Battle Animation ID";
                    _.items = BATTLE_MON_CLIPS;
                    _.items_count = BATTLE_MON_CLIP_COUNT;
                    _.selected = array_index(BATTLE_MON_CLIPS, "WaitA01");
                });

                _.Button([fieldAnimationId, loopAnim](Button &_) {
                    _.label = "Play Field Animation";
                    _.onClick = [fieldAnimationId, loopAnim]() {
                        auto uiManager = Dpr::UI::UIManager::get_Instance();
                        Logger::log("Playing Field Animation %s (%d)\n", FIELD_MON_CLIPS[fieldAnimationId->selected], fieldAnimationId->selected);
                        uiManager->fields._modelView->PlayAnimation(fieldAnimationId->selected, loopAnim->enabled);
                    };
                });

                _.Button([battleAnimationId, loopAnim](Button &_) {
                    _.label = "Play Battle Animation";
                    _.onClick = [battleAnimationId, loopAnim]() {
                        auto uiManager = Dpr::UI::UIManager::get_Instance();
                        Logger::log("Playing Battle Animation %s (%d)\n", BATTLE_MON_CLIPS[battleAnimationId->selected], battleAnimationId->selected);
                        uiManager->fields._modelView->PlayAnimation(battleAnimationId->selected, loopAnim->enabled);
                    };
                });
            });

            addChild(header);
        }
    };
}
