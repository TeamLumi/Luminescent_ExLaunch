#pragma once

#include "data/balls.h"
#include "data/debug.h"
#include "data/moves.h"
#include "data/species.h"
#include "data/utils.h"
#include "externals/PlayerWork.h"
#include "externals/poketool/poke_memo/poketool_poke_memo.h"
#include "ui/base/collapsing_header.h"
#include "ui/base/element.h"

#include "logger/logger.h"

namespace ui {
    ELEMENT(PokemonTool) {
        PokemonTool() {
            auto header = CollapsingHeader::create([this](CollapsingHeader &_) {
                _.label = "Pokemon Tool";

                auto* species = _.ComboSimple([](ComboSimple &_) {
                    _.label = "Species";
                    _.items = SPECIES;
                    _.items_count = SPECIES_COUNT;
                    _.selected =  array_index(SPECIES, "Bulbasaur");
                });
                auto *form = _.InputInt([](InputInt &_) {
                    _.label = "Form";
                    _.min = 0;
                    _.max = 100;
                    _.value = 0;
                });
                auto *level = _.InputInt([](InputInt &_) {
                    _.label = "Level";
                    _.min = 1;
                    _.max = 100;
                    _.value = 5;
                });
                auto *ball = _.ComboSimple([](ComboSimple &_) {
                    _.label = "Caught Ball";
                    _.items = BALLS;
                    _.items_count = BALL_COUNT;
                    _.selected = array_index(BALLS, "--BALL ZERO--");
                });
                auto *sex = _.ComboSimple([](ComboSimple &_) {
                    _.label = "Sex";
                    _.items = DEBUG_SEXES;
                    _.items_count = DEBUG_SEX_COUNT;
                    _.selected = array_index(DEBUG_SEXES, "-");
                });
                auto *shiny = _.ComboSimple([](ComboSimple &_) {
                    _.label = "Shininess";
                    _.items = DEBUG_SHINIES;
                    _.items_count = DEBUG_SHINY_COUNT;
                    _.selected = array_index(DEBUG_SHINIES, "-");
                });
                auto *friendship = _.InputInt([](InputInt &_) {
                    _.label = "Friendship";
                    _.min = 0;
                    _.max = 255;
                    _.value = 70;
                });
                auto* customMoves = _.Checkbox([](Checkbox &_) {
                    _.label = "Custom moves";
                    _.enabled = false;
                });
                auto *move1 = _.ComboSimple([](ComboSimple &_) {
                    _.label = "Move 1";
                    _.items = MOVES;
                    _.items_count = MOVE_COUNT;
                    _.selected = array_index(MOVES, "Pound");
                });
                auto *move2 = _.ComboSimple([](ComboSimple &_) {
                    _.label = "Move 2";
                    _.items = MOVES;
                    _.items_count = MOVE_COUNT;
                    _.selected = array_index(MOVES, "Karate Chop");
                });
                auto *move3 = _.ComboSimple([](ComboSimple &_) {
                    _.label = "Move 3";
                    _.items = MOVES;
                    _.items_count = MOVE_COUNT;
                    _.selected = array_index(MOVES, "Double Slap");
                });
                auto *move4 = _.ComboSimple([](ComboSimple &_) {
                    _.label = "Move 4";
                    _.items = MOVES;
                    _.items_count = MOVE_COUNT;
                    _.selected = array_index(MOVES, "Comet Punch");
                });

                _.Button([species, form, level, ball, sex, shiny, friendship, customMoves, move1, move2, move3, move4](Button &_) {
                    _.label = "Give Pokemon";
                    _.onClick = [species, form, level, ball, sex, shiny, friendship, customMoves, move1, move2, move3, move4]() {
                        system_load_typeinfo(0x43be);
                        auto party = PlayerWork::get_playerParty();

                        auto initialSpec = Pml::PokePara::InitialSpec::newInstance();
                        initialSpec->fields.monsno = species->selected;
                        initialSpec->fields.formno = form->value;
                        initialSpec->fields.level = level->value;
                        if (sex->selected == array_index(DEBUG_SEXES, "Male"))
                            initialSpec->fields.sex = 0;
                        else if (sex->selected == array_index(DEBUG_SEXES, "Female"))
                            initialSpec->fields.sex = 1;
                        else if (sex->selected == array_index(DEBUG_SEXES, "Genderless"))
                            initialSpec->fields.sex = 2;

                        auto param = Pml::PokePara::PokemonParam::newInstance(initialSpec);
                        auto core = param->cast<Pml::PokePara::CoreParam>();

                        core->SetFriendship(friendship->value);

                        if (customMoves->enabled)
                        {
                            core->SetWaza(0, move1->selected);
                            core->SetWaza(1, move2->selected);
                            core->SetWaza(2, move3->selected);
                            core->SetWaza(3, move4->selected);
                        }
                        core->SetGetBall(ball->selected);
                        if (shiny->selected == array_index(DEBUG_SHINIES, "Non-shiny"))
                            core->SetRareType(Pml::PokePara::RareType::NOT_RARE);
                        else if (shiny->selected == array_index(DEBUG_SHINIES, "Shiny"))
                            core->SetRareType(Pml::PokePara::RareType::CAPTURED);
                        else if (shiny->selected == array_index(DEBUG_SHINIES, "Square Shiny"))
                            core->SetRareType(Pml::PokePara::RareType::DISTRIBUTED);

                        auto myStatus = PlayerWork::get_playerStatus();
                        auto zoneID = PlayerWork::get_zoneID();
                        poketool::poke_memo::poketool_poke_memo::SetFromCapture((Pml::PokePara::CoreParam::Object*)param, myStatus, zoneID);
                        if (!party->AddMember(param))
                            Logger::log("Could not give Pokémon!\n");
                    };
                });
            });

            addChild(header);
        }
    };
}
