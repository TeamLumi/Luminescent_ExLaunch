#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/View/BTLV_ENVIRONMENT_PARAM.h"
#include "externals/UnityEngine/Coroutine.h"

namespace Dpr::Sequence {
    struct SequenceSystem : ILClass<SequenceSystem> {
        struct Fields {
            Dpr::Battle::View::BTLV_ENVIRONMENT_PARAM::Object _environmentParam;
            UnityEngine::Coroutine::Object* _seqFileLoadCoroutine;
            void* _preLoadCoroutines; // System_Collections_Generic_List_Coroutine__o*
            void* _cachedSoundBank; // System_Collections_Generic_Dictionary_string__AudioData__o*
            int32_t _step;
            float _sequenceElapsedTime;
            float _sequenceInterpolationTime;
            void* _SequenceFile_k__BackingField; // Dpr_SequenceEditor_SequenceFile_o*
            int32_t _CurrentFrame_k__BackingField;
            int32_t _MaxFrame_k__BackingField;
            void* _Commands_k__BackingField; // System_Collections_Generic_List_CommandParam__o*
            bool _IsCommandSkip_k__BackingField;
            bool _IsPause_k__BackingField;
            bool _IsFinishSequence_k__BackingField;
            bool _IsInterpolationTime_k__BackingField;
        };

        static_assert(offsetof(Fields, _IsInterpolationTime_k__BackingField) == 0x93);
    };
}
