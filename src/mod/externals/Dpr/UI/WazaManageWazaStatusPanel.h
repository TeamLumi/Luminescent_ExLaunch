#pragma once

#include "externals/il2cpp.h"

#include "externals/Dpr/UI/PokemonStatusPanel.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/UnityEngine/MonoBehaviour.h"

namespace Dpr::UI {
    struct WazaManageWazaStatusPanel : ILClass<WazaManageWazaStatusPanel> {
        struct Fields : Dpr::UI::PokemonStatusPanel::Fields {
            // TODO
        };

        inline void Initialize() {
            external<void>(0x01a42550, this);
        }

        inline void Setup(Pml::PokePara::PokemonParam::Object* pokemonParam, Pml::WazaNo_array* wazaNos, int32_t newWazaIndex) {
            external<void>(0x01a42640, this, pokemonParam, wazaNos, newWazaIndex);
        }

        inline void UpdateSelect(float deltaTime, bool isPlaySound) {
            external<void>(0x01a426a0, this, deltaTime, isPlaySound);
        }

        inline int32_t GetSelectedWazaNo() {
            return external<int32_t>(0x01a432a0, this);
        }
    };
}
