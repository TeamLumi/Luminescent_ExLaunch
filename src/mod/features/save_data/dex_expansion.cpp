#include "exlaunch.hpp"

#include "externals/Dpr/UI/UIText.h"
#include "externals/Dpr/UI/ZukanInfo.h"
#include "save/save.h"

#include "logger/logger.h"

void ZukanInfo_SetupUITexts_SetupMessage(Dpr::UI::UIText::Object* __this, System::String::Object* messageFile, int32_t monsno, Dpr::UI::ZukanInfo::Object* zukanInfo)
{
    int32_t currentIndex = zukanInfo->fields.modelIndexSelector->fields._CurrentIndex_k__BackingField;
    Dpr::UI::ZukanInfo::ModelParam::Object * modelParam = zukanInfo->fields.modelParams->m_Items[currentIndex];

    int32_t messageIndex;
    if (modelParam->fields.FormNo == 0 || zukanInfo->fields.formIndex == 0)
    {
        messageIndex = monsno;
    }
    else
    {
        messageIndex = zukanInfo->fields.formIndex + modelParam->fields.FormNo - 1;
    }

    __this->SetupMessage(messageFile, messageIndex);
}

void exl_save_dex_expansion_main() {
    // Patch dex limit checks
    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p(0);
    auto inst = nn::vector<exl::patch::Instruction> {
        { 0x017dbd3c, CmpImmediate(W19, DexSize - 1) },
        { 0x017dc8b4, CmpImmediate(W20, DexSize - 1) },
        { 0x017dc9d0, CmpImmediate(W19, DexSize - 1) },
        { 0x017dca90, CmpImmediate(W19, DexSize - 1) },
        { 0x017dcb50, CmpImmediate(W19, DexSize - 1) },
        { 0x017dcc10, CmpImmediate(W19, DexSize - 1) },
        { 0x017dcd34, CmpImmediate(W20, DexSize - 1) },
        { 0x017dcf78, CmpImmediate(W19, DexSize - 1) },
        { 0x017dd030, CmpImmediate(W20, DexSize - 1) },
        { 0x017dd4b4, CmpImmediate(W20, DexSize - 1) },
        { 0x017ddee0, Movz(W1, DexSize) },
        { 0x018d372c, CmpImmediate(W0, 0) },
        { 0x0199ca48, CmpImmediate(W9, DexSize + 1) },
        { 0x0199ca54, SubImmediate(W8, W8, DexSize - 1) },
        { 0x0199ca88, AddImmediate(W8, W8, DexSize - 1) },
        { 0x01a39760, CmpImmediate(W22, DexSize + 1) },
        { 0x01cd637c, CmpImmediate(W8, DexSize - 1) },
        { 0x01cd6494, CmpImmediate(W8, DexSize) },
        { 0x01cd6628, CmpImmediate(W8, DexSize - 1) },
        { 0x01cd6ecc, CmpImmediate(W8, DexSize - 1) },
        { 0x01cd799c, CmpImmediate(W8, DexSize - 1) },
        { 0x01cd8b1c, CmpImmediate(W8, DexSize - 1) },
        { 0x01cd8cd8, CmpImmediate(W8, DexSize - 1) },
        { 0x01cd8f68, CmpImmediate(W8, DexSize - 1) },
        { 0x01f7b440, Movz(W8, DexSize) },
        { 0x0202faf0, Movz(W26, DexSize + 1) },
        { 0x020494ec, Movz(W0, DexSize + 1) },
        { 0x0204ea40, Movz(W0, DexSize + 1) },
        { 0x02051ba0, CmpImmediate(W21, DexSize + 1) },
        { 0x02052220, Movz(W8, DexSize + 1) },
        { 0x024a09b4, CmpImmediate(W8, DexSize + 1) },
        { 0x024a09c4, CmpImmediate(W0, DexSize) },
        { 0x024a0a5c, CmpImmediate(W8, DexSize + 1) },
        { 0x024a0a60, Movz(W8, DexSize) },
        { 0x024a0b64, CmpImmediate(W8, DexSize + 1) },
        { 0x024a0b68, Movz(W8, DexSize) },
        { 0x024a0eac, CmpImmediate(W19, DexSize) },
        { 0x024a0f54, CmpImmediate(W19, DexSize) },
        { 0x024a2acc, CmpImmediate(W19, DexSize) },
        { 0x024a2bc8, CmpImmediate(W19, DexSize) },
        { 0x024a3b48, Movz(W9, DexSize + 1) },
        { 0x024a6b38, Movz(W0, DexSize + 1) },
        { 0x017da928, Movz(W19, DexSize)}, // Dex Array Expansion

    };
    p.WriteInst(inst);

    // Patch dex description for new forms (?)
    p.Seek(0x01bb1364);
    p.WriteInst(MovRegister(X3, X21));
    p.BranchLinkInst((void*)&ZukanInfo_SetupUITexts_SetupMessage);
}
