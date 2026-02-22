#pragma once

#include "externals/il2cpp-api.h"

namespace TMPro {
    struct TMP_Text;

    struct TMP_TextInfo : ILClass<TMP_TextInfo> {
        struct Fields {
            TMP_Text* textComponent;
            int32_t characterCount;
            int32_t spriteCount;
            int32_t spaceCount;
            int32_t wordCount;
            int32_t linkCount;
            int32_t lineCount;
            int32_t pageCount;
            int32_t materialCount;
            void* characterInfo; // TMP_CharacterInfo::Array*
            void* wordInfo; // TMP_WordInfo::Array*
            void* linkInfo; // TMP_LinkInfo::Array*
            void* lineInfo; // TMP_LineInfo::Array*
            void* pageInfo; // TMP_PageInfo::Array*
            void* meshInfo; // TMP_MeshInfo::Array*
            void* m_CachedMeshInfo; // TMP_MeshInfo::Array*
        };

        static_assert(offsetof(Fields, m_CachedMeshInfo) == 0x58);
    };
}
