#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/TouchType.h"
#include "externals/UnityEngine/Vector2.h"

namespace UnityEngine {
    struct Touch : ILStruct<Touch> {
        struct Fields {
            int32_t m_FingerId;
            UnityEngine::Vector2::Object m_Position;
            UnityEngine::Vector2::Object m_RawPosition;
            UnityEngine::Vector2::Object m_PositionDelta;
            float m_TimeDelta;
            int32_t m_TapCount;
            int32_t m_Phase;
            UnityEngine::TouchType m_Type;
            float m_Pressure;
            float m_maximumPossiblePressure;
            float m_Radius;
            float m_RadiusVariance;
            float m_AltitudeAngle;
            float m_AzimuthAngle;
        };

        inline UnityEngine::Vector2::Object get_position() {
            return {
                .fields = external<UnityEngine::Vector2::Fields>(game::Address::UnityEngine_Touch_get_position, this)
            };
        }
    };
}

static_assert(sizeof(UnityEngine::Touch::Object) == 0x44);
