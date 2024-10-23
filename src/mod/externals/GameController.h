#pragma once

#include "externals/il2cpp.h"

#include "externals/System/Primitives.h"
#include "externals/UnityEngine/Vector2.h"

struct GameController : ILClass<GameController, 0x04c5a618> {
    struct ButtonIndex : ILClass<ButtonIndex> {
        static const int A = 0;
        static const int B = 1;
        static const int X = 2;
        static const int Y = 3;
        static const int StickL = 4;
        static const int StickR = 5;
        static const int L = 6;
        static const int R = 7;
        static const int ZL = 8;
        static const int ZR = 9;
        static const int Plus = 10;
        static const int Minus = 11;
        static const int Left = 12;
        static const int Up = 13;
        static const int Right = 14;
        static const int Down = 15;
        static const int StickLLeft = 16;
        static const int StickLUp = 17;
        static const int StickLRight = 18;
        static const int StickLDown = 19;
        static const int StickRLeft = 20;
        static const int StickRUp = 21;
        static const int StickRRight = 22;
        static const int StickRDown = 23;
        static const int LeftSL = 24;
        static const int LeftSR = 25;
        static const int RightSL = 26;
        static const int RightSR = 27;
        static const int Count = 28;
    };

    struct ButtonMask : ILClass<ButtonMask> {
        static const int None = 0x0;

        static const int A = 0x1;
        static const int B = 0x2;
        static const int X = 0x4;
        static const int Y = 0x8;

        static const int StickL = 0x10;
        static const int StickR = 0x20;
        static const int L = 0x40;
        static const int R = 0x80;

        static const int ZL = 0x100;
        static const int ZR = 0x200;
        static const int Plus = 0x400;
        static const int Minus = 0x800;

        static const int Left = 0x1000;
        static const int Up = 0x2000;
        static const int Right = 0x4000;
        static const int Down = 0x8000;

        static const int StickLLeft = 0x10000;
        static const int StickLUp = 0x20000;
        static const int StickLRight = 0x40000;
        static const int StickLDown = 0x80000;

        static const int StickRLeft = 0x100000;
        static const int StickRUp = 0x200000;
        static const int StickRRight = 0x400000;
        static const int StickRDown = 0x800000;

        static const int LeftSL = 0x1000000;
        static const int LeftSR = 0x2000000;
        static const int RightSL = 0x4000000;
        static const int RightSR = 0x8000000;
    };

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

    static inline int32_t GetAnalogStickButtonIndex(UnityEngine::Vector2::Object analogStick) {
        return external<int32_t>(0x02ccc470, analogStick);
    }
};
