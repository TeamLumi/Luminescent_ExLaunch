#pragma once

#include "externals/il2cpp.h"
#include "externals/UnityEngine/Vector2.h"
#include "externals/System/Primitives.h"

enum class GameController_Button : int32_t {
    BUTTON_A = 0x1,
    BUTTON_B = 0x2,
    BUTTON_X = 0x4,
    BUTTON_Y = 0x8,
    BUTTON_L3 = 0x10,
    BUTTON_R3 = 0x20,

    BUTTON_L = 0x40,
    BUTTON_R = 0x80,
    BUTTON_ZL = 0x100,
    BUTTON_ZR = 0x200,

    BUTTON_PLUS = 0x400,
    BUTTON_MINUS = 0x800,

    DPAD_LEFT = 0x1000,
    DPAD_UP = 0x2000,
    DPAD_RIGHT = 0x4000,
    DPAD_DOWN = 0x8000,

    LSTICK_LEFT = 0x10000,
    LSTICK_UP = 0x20000,
    LSTICK_RIGHT = 0x40000,
    LSTICK_DOWN = 0x80000,

    RSTICK_LEFT = 0x100000,
    RSTICK_UP = 0x200000,
    RSTICK_RIGHT = 0x400000,
    RSTICK_DOWN = 0x800000,
};

struct GameController : ILClass<GameController, 0x04c5a618> {
    struct StaticFields {
        UnityEngine::Vector2::Object analogStickL;
        UnityEngine::Vector2::Object analogStickR;
        UnityEngine::Vector2::Object digitalPad;
        int32_t on;
        int32_t push;
        int32_t release;
        int32_t accel;
        System::Int64_array* times;
        int32_t repeat;
        int64_t start;
        int64_t interval;
        int64_t limit_intarval;
        int64_t required_time;
        int64_t prevTicks;
        void* logicalInputs;
        void* npadIds;
        u8 npadState[0x30];
        int32_t npadId;
        int32_t npadStyle;
        System::Int32_array* _analogStickLButtonMasks;
        System::Int32_array* _analogStickRButtonMasks;
    };

    static inline bool IsOn(GameController_Button button) {
        return getClass()->static_fields->on & (int32_t)button;
    }

    static inline bool IsPush(GameController_Button button) {
        return getClass()->static_fields->push & (int32_t)button;
    }

    static inline bool IsRelease(GameController_Button button) {
        return getClass()->static_fields->release & (int32_t)button;
    }

    static inline bool IsAccel(GameController_Button button) {
        return getClass()->static_fields->accel & (int32_t)button;
    }

    static inline bool IsRepeat(GameController_Button button) {
        return getClass()->static_fields->repeat & (int32_t)button;
    }

    static inline int32_t GetAnalogStickButtonIndex(UnityEngine::Vector2::Object analogStick) {
        return external<int32_t>(0x02ccc470, analogStick);
    }
};
