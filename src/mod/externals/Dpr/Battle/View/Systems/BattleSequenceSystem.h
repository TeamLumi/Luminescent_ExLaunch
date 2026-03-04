#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Sequence/SequenceSystem.h"
#include "externals/Dpr/SequenceEditor/CommandParam.h"
#include "externals/Dpr/SequenceEditor/ISequenceViewSystem.h"

namespace Dpr::Battle::View::Systems {
    struct BattleSequenceSystem : ILClass<BattleSequenceSystem> {
        struct Fields : Dpr::Sequence::SequenceSystem::Fields {
            Dpr::SequenceEditor::ISequenceViewSystem::Object* _ISequenceViewSystem;
            bool _isPlayWaitCamera;
            void* _uPtrModelHash; // Dpr_Battle_View_VectorHash_int__ObjectEntity__o*
            void* _uPtrParticleVectorHash; // Dpr_Battle_View_VectorHash_int__BtlvEffectInstance__o*
            void* _uPtrSoundHash; // Dpr_Battle_View_VectorHash_int__BtlvSound__o*
            void* _removeCommands; // System_Collections_Generic_List_CommandParam__o*
            void* _effectLoadParams; // System_Collections_Generic_List_EffectManager_LoadParam__o*
        };

        static_assert(offsetof(Fields, _effectLoadParams) == 0xC8);

        inline void PreloadParticle(/*Dpr_SequenceEditor_SequenceFile_o**/ void* pSeqFile, Dpr::SequenceEditor::ISequenceViewSystem::Object* pViewSystem, Dpr::SequenceEditor::CommandParam::Object* param) {
            external<void>(0x01776520, this, pSeqFile, pViewSystem, param);
        }
    };
}
