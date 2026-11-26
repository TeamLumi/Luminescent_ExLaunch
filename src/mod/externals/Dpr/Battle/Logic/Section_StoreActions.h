#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BTL_ACTION_PARAM.h"
#include "externals/Dpr/Battle/Logic/Section.h"
#include "externals/Dpr/Battle/Logic/Section_CommonParam.h"

namespace Dpr::Battle::Logic {
    struct Section_StoreActions : ILClass<Section_StoreActions, game::Address::Dpr_Battle_Logic_Section_StoreActions_TypeInfo> {
        struct Fields : Dpr::Battle::Logic::Section::Fields {

        };
        inline void ctor(Section_CommonParam::Object** commonParam) {
            external<void>(game::Address::Dpr_Battle_Logic_Section_StoreActions_ctor, this, commonParam);
        }


        inline BTL_POKEPARAM::Object* getActionPoke(BTL_ACTION_PARAM::Object* clientInstruction, uint8_t clientID) {
            return external<BTL_POKEPARAM::Object*>(game::Address::Dpr_Battle_Logic_Section_StoreActions_getActionPoke, this, clientInstruction, clientID);
        }

        inline bool setupPokeAction_FromClientInstruction(Dpr::Battle::Logic::PokeAction::Object* pokeAction,
                                                          Dpr::Battle::Logic::BTL_ACTION_PARAM::Object* clientInstructions,
                                                          uint8_t clientID) {
            return external<bool>(game::Address::Dpr_Battle_Logic_Section_StoreActions_setupPokeAction_FromClientInstruction, this, pokeAction, clientInstructions, clientID);
        }
    };
}
