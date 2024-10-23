#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BTL_ACTION.h"
#include "externals/Dpr/Battle/Logic/BtlPokePos.h"
#include "externals/Dpr/Battle/View/UI/BattleViewUICanvasBase.h"
#include "externals/UnityEngine/UI/Image.h"

namespace Dpr::Battle::View::UI {
    struct BUITargetSelect : ILClass<BUITargetSelect> {
        struct Fields : Dpr::Battle::View::UI::BattleViewUICanvasBase::Fields {
            void* _targetButtons;
            void* _targetCursor;
            UnityEngine::UI::Image::Array* _boardImage;
            Dpr::Battle::Logic::BtlPokePos _Result_k__BackingField;
            Dpr::Battle::Logic::BTL_ACTION::PARAM_OBJ::Object* _destActionParam;
            int32_t _actionVPos;
            int32_t _targetType;
            int32_t _overrideTargetType;
            void* _prevTargetPos;
            int32_t _pokeIndex;
            bool _isSingleTarget;
            void* currentFieldPokeParams;
        };

        static_assert(offsetof(Fields, currentFieldPokeParams) == 0xA0);
    };
}
