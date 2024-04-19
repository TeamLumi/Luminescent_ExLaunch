#pragma once

#include "externals/il2cpp-api.h"
#include "externals/UnityEngine/UI/MaskableGraphic.h"
#include "externals/System/String.h"
#include "externals/TMPro/TextMeshProUGUI.h"
#include "externals/System/Collections/Generic/List.h"

namespace System::Collections::Generic {
    struct List$$UIText;
}

namespace Dpr::UI {
    struct UIText : ILClass<UIText> {
        struct Fields : TMPro::TextMeshProUGUI::Fields {
            // TODO: The class this inherits from has misaligned fields because of missing classes/structs.
            int32_t _sizeId;
            bool _useMessage;
            System::String::Object* _messageFile;
            System::String::Object* _messageId;
            bool _useTag;
            bool _isManual;
            int32_t _fontMaterialDataIndex;
            bool _uiTextEnable;
            void* _msgFile;
            int32_t _messageIndex;
        };

        inline void SetupMessage(System::String::Object* messageFile, System::String::Object* messageId) {
            external<void>(0x01dd18d0, this, messageFile, messageId);
        }

        inline void SetupMessage(System::String::Object* messageFile, int32_t messageIndex) {
            external<void>(0x01dd1940, this, messageFile, messageIndex);
        }
    };
}

namespace System::Collections::Generic {
    struct List$$UIText : List<List$$UIText, Dpr::UI::UIText> {
        struct Fields {
            Dpr::UI::UIText::Array* _items;
            int32_t _size;
            int32_t _version;
            Il2CppObject* _syncRoot;
        };
        static inline StaticILMethod<0x04c89ac0, Dpr::UI::UIText> Method$$UIText$$Add {};
        static inline StaticILMethod<0x04c89ac8, Dpr::UI::UIText> Method$$UIText$$Clear {};
    };

}
