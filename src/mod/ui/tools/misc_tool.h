#pragma once

#include "data/species.h"
#include "externals/DPData/Form_Enums.h"
#include "externals/Dpr/Message/MessageEnumData.h"
#include "externals/ZukanWork.h"
#include "ui/base/collapsing_header.h"
#include "ui/base/element.h"
#include "ui/ui.h"

namespace ui {
    ELEMENT(MiscTool) {
        MiscTool() {
            auto header = CollapsingHeader::create([this](CollapsingHeader &_) {
                _.label = "Misc Tool";

                _.Button([](Button &_) {
                    _.label = "Billionaire!";
                    _.onClick = []() {
                        PlayerWork::SetMoney(999999);
                    };
                });

                _.Button([](Button &_) {
                    _.label = "Play last area name";
                    _.onClick = []() {
                        showAreaName();
                    };
                });

                _.Checkbox([](Checkbox &_) {
                    _.label = "Fly anywhere";
                    _.enabled = false;
                    _.onChange = [](bool value) {
                        setFlyOverride(value);
                    };
                });

                _.Button([](Button &_) {
                    _.label = "Register all Pokémon to Dex";
                    _.onClick = []() {
                        for (int i=1; i<=DexSize; i++) {
                            ZukanWork::SetPoke(i, 3, 0, 0, true);
                            ZukanWork::SetPoke(i, 3, 1, 0, true);
                            ZukanWork::SetPoke(i, 3, 0, 0, false);
                            ZukanWork::SetPoke(i, 3, 1, 0, false);

                            ZukanWork::AddLangFlag(i, Dpr::Message::MessageEnumData::MsgLangId::JPN);
                            ZukanWork::AddLangFlag(i, Dpr::Message::MessageEnumData::MsgLangId::USA);
                            ZukanWork::AddLangFlag(i, Dpr::Message::MessageEnumData::MsgLangId::FRA);
                            ZukanWork::AddLangFlag(i, Dpr::Message::MessageEnumData::MsgLangId::ITA);
                            ZukanWork::AddLangFlag(i, Dpr::Message::MessageEnumData::MsgLangId::DEU);
                            ZukanWork::AddLangFlag(i, Dpr::Message::MessageEnumData::MsgLangId::ESP);
                            ZukanWork::AddLangFlag(i, Dpr::Message::MessageEnumData::MsgLangId::KOR);
                            ZukanWork::AddLangFlag(i, Dpr::Message::MessageEnumData::MsgLangId::SCH);
                            ZukanWork::AddLangFlag(i, Dpr::Message::MessageEnumData::MsgLangId::TCH);

                            if (i == array_index(SPECIES, "Unown")) {
                                for (int j=1; j<=(int)UnownForm::MAX; j++) {
                                    ZukanWork::SetPoke(i, 3, 0, j, true);
                                    ZukanWork::SetPoke(i, 3, 1, j, true);
                                    ZukanWork::SetPoke(i, 3, 0, j, false);
                                    ZukanWork::SetPoke(i, 3, 1, j, false);
                                }
                            }
                            else if (i == array_index(SPECIES, "Castform")) {
                                for (int j=1; j<=(int)CastformForm::MAX; j++) {
                                    ZukanWork::SetPoke(i, 3, 0, j, true);
                                    ZukanWork::SetPoke(i, 3, 1, j, true);
                                    ZukanWork::SetPoke(i, 3, 0, j, false);
                                    ZukanWork::SetPoke(i, 3, 1, j, false);
                                }
                            }
                            else if (i == array_index(SPECIES, "Deoxys")) {
                                for (int j=1; j<=(int)DeoxysForm::MAX; j++) {
                                    ZukanWork::SetPoke(i, 3, 0, j, true);
                                    ZukanWork::SetPoke(i, 3, 1, j, true);
                                    ZukanWork::SetPoke(i, 3, 0, j, false);
                                    ZukanWork::SetPoke(i, 3, 1, j, false);
                                }
                            }
                            else if (i == array_index(SPECIES, "Burmy")) {
                                for (int j=1; j<=(int)BurmyForm::MAX; j++) {
                                    ZukanWork::SetPoke(i, 3, 0, j, true);
                                    ZukanWork::SetPoke(i, 3, 1, j, true);
                                    ZukanWork::SetPoke(i, 3, 0, j, false);
                                    ZukanWork::SetPoke(i, 3, 1, j, false);
                                }
                            }
                            else if (i == array_index(SPECIES, "Wormadam")) {
                                for (int j=1; j<=(int)WormadamForm::MAX; j++) {
                                    ZukanWork::SetPoke(i, 3, 0, j, true);
                                    ZukanWork::SetPoke(i, 3, 1, j, true);
                                    ZukanWork::SetPoke(i, 3, 0, j, false);
                                    ZukanWork::SetPoke(i, 3, 1, j, false);
                                }
                            }
                            else if (i == array_index(SPECIES, "Mothim")) {
                                for (int j=1; j<=(int)MothimForm::MAX; j++) {
                                    ZukanWork::SetPoke(i, 3, 0, j, true);
                                    ZukanWork::SetPoke(i, 3, 1, j, true);
                                    ZukanWork::SetPoke(i, 3, 0, j, false);
                                    ZukanWork::SetPoke(i, 3, 1, j, false);
                                }
                            }
                            else if (i == array_index(SPECIES, "Cherrim")) {
                                for (int j=1; j<=(int)CherrimForm::MAX; j++) {
                                    ZukanWork::SetPoke(i, 3, 0, j, true);
                                    ZukanWork::SetPoke(i, 3, 1, j, true);
                                    ZukanWork::SetPoke(i, 3, 0, j, false);
                                    ZukanWork::SetPoke(i, 3, 1, j, false);
                                }
                            }
                            else if (i == array_index(SPECIES, "Shellos")) {
                                for (int j=1; j<=(int)ShellosForm::MAX; j++) {
                                    ZukanWork::SetPoke(i, 3, 0, j, true);
                                    ZukanWork::SetPoke(i, 3, 1, j, true);
                                    ZukanWork::SetPoke(i, 3, 0, j, false);
                                    ZukanWork::SetPoke(i, 3, 1, j, false);
                                }
                            }
                            else if (i == array_index(SPECIES, "Gastrodon")) {
                                for (int j=1; j<=(int)GastrodonForm::MAX; j++) {
                                    ZukanWork::SetPoke(i, 3, 0, j, true);
                                    ZukanWork::SetPoke(i, 3, 1, j, true);
                                    ZukanWork::SetPoke(i, 3, 0, j, false);
                                    ZukanWork::SetPoke(i, 3, 1, j, false);
                                }
                            }
                            else if (i == array_index(SPECIES, "Rotom")) {
                                for (int j=1; j<=(int)RotomForm::MAX; j++) {
                                    ZukanWork::SetPoke(i, 3, 0, j, true);
                                    ZukanWork::SetPoke(i, 3, 1, j, true);
                                    ZukanWork::SetPoke(i, 3, 0, j, false);
                                    ZukanWork::SetPoke(i, 3, 1, j, false);
                                }
                            }
                            else if (i == array_index(SPECIES, "Giratina")) {
                                for (int j=1; j<=(int)GiratinaForm::MAX; j++) {
                                    ZukanWork::SetPoke(i, 3, 0, j, true);
                                    ZukanWork::SetPoke(i, 3, 1, j, true);
                                    ZukanWork::SetPoke(i, 3, 0, j, false);
                                    ZukanWork::SetPoke(i, 3, 1, j, false);
                                }
                            }
                            else if (i == array_index(SPECIES, "Shaymin")) {
                                for (int j=1; j<=(int)ShayminForm::MAX; j++) {
                                    ZukanWork::SetPoke(i, 3, 0, j, true);
                                    ZukanWork::SetPoke(i, 3, 1, j, true);
                                    ZukanWork::SetPoke(i, 3, 0, j, false);
                                    ZukanWork::SetPoke(i, 3, 1, j, false);
                                }
                            }
                            else if (i == array_index(SPECIES, "Arceus")) {
                                for (int j=1; j<=(int)ArceusForm::MAX; j++) {
                                    ZukanWork::SetPoke(i, 3, 0, j, true);
                                    ZukanWork::SetPoke(i, 3, 1, j, true);
                                    ZukanWork::SetPoke(i, 3, 0, j, false);
                                    ZukanWork::SetPoke(i, 3, 1, j, false);
                                }
                            }
                        }
                    };
                });
            });

            addChild(header);
        }
    };
}
