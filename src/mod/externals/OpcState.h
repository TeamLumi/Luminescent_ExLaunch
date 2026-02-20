#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/MonoBehaviour.h"

struct OpcState : ILClass<OpcState> {
    enum class OnlineState : int32_t {
        NONE = 0,
        DIG_FOSILL = 1,
        SECRETBASE_ACTION = 2,
        RECRUITMENT_BATTLE = 3,
        RECRUITMENT_TRADE = 4,
        RECRUITMENT_RECORD = 5,
        RECRUITMENT_GREETINGS = 6,
        RECRUITMENT_BALL_DECORATION = 7,
        COMMUNICATE = 8,
        LIKES = 9,
        CROSS = 10,
        EXCLAMATION = 11,
        TOGETHER = 12,
        GET = 13,
        NOW_DIG_FOSILE = 14,
        NOW_MENU = 15,
        NOW_BATTLE = 16,
        NOW_BATTLE_UNION = 17,
        NOW_TRADE = 18,
        NOW_RECORD = 19,
        NOW_GREETINGS = 20,
        NOW_BALL_DECORATION = 21,
        NULL_STATE = 22,  // Originally "_NULL" in IL2CPP — renamed to avoid macro clash
    };

    struct Fields : UnityEngine::MonoBehaviour::Fields {
        OnlineState _curretOnlineState;
        void* _OnChangeState; // Action<OnlineState>
    };

    // Access fields via Object*: opcState->fields._curretOnlineState
    // Methods that access fields directly can't be defined on the outer struct —
    // the 'fields' member only exists on Object, not on T in ILClass<T>.
};
