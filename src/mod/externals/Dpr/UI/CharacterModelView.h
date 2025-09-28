#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/UI/ModelViewBase.h"
#include "externals/System/String.h"
#include "externals/UnityEngine/Transform.h"
#include "externals/XLSXContent/CharacterDressData.h"

namespace Dpr::UI {
    struct CharacterModelView : ILClass<CharacterModelView> {
        struct Param : ILClass<Param> {
            struct Fields {
                int32_t type;
                XLSXContent::CharacterDressData::SheetData::Object* characterDressData;
                int32_t modelType;
                void* canvas;
                float offsetZ;
                bool isAutoUpdateTexture;
            };

            static_assert(offsetof(Fields, isAutoUpdateTexture) == 0x24);
        };

        struct Fields : Dpr::UI::ModelViewBase::Fields {
            UnityEngine::Transform::Object* _transModel;
            float _fov;
            float _rotateSpeed;
            int32_t _saveModelViewCacheNum;
            Param::Object* _param;
            System::String::Object* _clipName;
        };

        static_assert(offsetof(Fields, _clipName) == 0x40);
    };

}