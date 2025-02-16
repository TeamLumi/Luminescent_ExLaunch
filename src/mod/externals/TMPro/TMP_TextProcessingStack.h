#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/Primitives.h"
#include "externals/TMPro/HighlightState.h"
#include "externals/UnityEngine/Color32.h"

namespace TMPro {
    // Not making this generic bc I don't wanna deal with all that
    struct TMP_TextProcessingStack_int32_t : ILStruct<TMP_TextProcessingStack_int32_t> {
        struct Fields {
            System::Int32_array* itemStack;
            int32_t index;
            int32_t m_DefaultItem;
            int32_t m_Capacity;
            int32_t m_RolloverSize;
            int32_t m_Count;
        };

        static_assert(sizeof(Fields) == 0x20);
    };

    struct TMP_TextProcessingStack_float : ILStruct<TMP_TextProcessingStack_float> {
        struct Fields {
            System::Single_array* itemStack;
            int32_t	index;
            float m_DefaultItem;
            int32_t	m_Capacity;
            int32_t	m_RolloverSize;
            int32_t m_Count;
        };

        static_assert(sizeof(Fields) == 0x20);
    };

    struct TMP_TextProcessingStack_Color32 : ILStruct<TMP_TextProcessingStack_Color32> {
        struct Fields {
            UnityEngine::Color32::Array* itemStack;
            int32_t	index;
            UnityEngine::Color32::Object m_DefaultItem;
            int32_t	m_Capacity;
            int32_t	m_RolloverSize;
            int32_t m_Count;
        };

        static_assert(sizeof(Fields) == 0x20);
    };

    struct TMP_TextProcessingStack_HighlightState : ILStruct<TMP_TextProcessingStack_HighlightState> {
        struct Fields {
            HighlightState::Array* itemStack;
            int32_t	index;
            HighlightState::Object m_DefaultItem;
            int32_t	m_Capacity;
            int32_t	m_RolloverSize;
            int32_t m_Count;
        };

        static_assert(sizeof(Fields) == 0x30);
    };

    struct TMP_TextProcessingStack_TMP_ColorGradient : ILStruct<TMP_TextProcessingStack_TMP_ColorGradient> {
        struct Fields {
            TMP_ColorGradient::Array* itemStack;
            int32_t	index;
            TMP_ColorGradient::Object* m_DefaultItem;
            int32_t	m_Capacity;
            int32_t	m_RolloverSize;
            int32_t m_Count;
        };

        static_assert(sizeof(Fields) == 0x28);
    };
}
