#pragma once

#include "externals/il2cpp.h"

#include "externals/UnityEngine/_Object.h"

namespace UnityEngine {
    struct AudioClip : ILClass<AudioClip> {
        struct Fields : UnityEngine::_Object::Fields {
            void* m_PCMReaderCallback;
	        void* m_PCMSetPositionCallback;
        };
    };
}
