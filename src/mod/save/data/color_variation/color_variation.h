#pragma once

#include "externals/PlayerWork.h"
#include "externals/System/Primitives.h"
#include "externals/UnityEngine/Color.h"

#include "logger/logger.h"

struct ColorVariationSaveData {
    static constexpr const char* fileName = "SaveData:/Lumi_PlayerColorVariation.bin";
    static constexpr const char* backupFileName = "SaveData:/Lumi_PlayerColorVariation_BK.bin";

    System::Int32 playerColorID;

    UnityEngine::Color::Object fSkinFace;
    UnityEngine::Color::Object fSkinMouth;
    UnityEngine::Color::Object fEyes;
    UnityEngine::Color::Object fEyebrows;
    UnityEngine::Color::Object fSkinBody;
    UnityEngine::Color::Object fHair;

    UnityEngine::Color::Object bSkinFace;
    UnityEngine::Color::Object bHairExtra;
    UnityEngine::Color::Object bEyeLeft;
    UnityEngine::Color::Object bEyeRight;
    UnityEngine::Color::Object bSkinBody;
    UnityEngine::Color::Object bHair;

    void Initialize() {
        playerColorID = 0;

        fSkinFace.fields =    { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
        fSkinMouth.fields =   { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
        fEyes.fields =        { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
        fEyebrows.fields =    { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
        fSkinBody.fields =    { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
        fHair.fields =        { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };

        bSkinFace.fields =    { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
        bHairExtra.fields =   { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
        bEyeLeft.fields =     { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
        bEyeRight.fields =    { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
        bSkinBody.fields =    { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
        bHair.fields =        { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
    }

    long GetByteCount() {
        long count = 0;
        count += sizeof(System::Int32);
        count += sizeof(UnityEngine::Color::Object) * 12;
        return count;
    }

    long ToBytes(char* buffer, long index) {
        memcpy((void*)(buffer+index), &playerColorID, sizeof(System::Int32));
        index += sizeof(System::Int32);

        memcpy((void*)(buffer+index), &fSkinFace, sizeof(UnityEngine::Color::Object));
        index += sizeof(UnityEngine::Color::Object);
        memcpy((void*)(buffer+index), &fSkinMouth, sizeof(UnityEngine::Color::Object));
        index += sizeof(UnityEngine::Color::Object);
        memcpy((void*)(buffer+index), &fEyes, sizeof(UnityEngine::Color::Object));
        index += sizeof(UnityEngine::Color::Object);
        memcpy((void*)(buffer+index), &fEyebrows, sizeof(UnityEngine::Color::Object));
        index += sizeof(UnityEngine::Color::Object);
        memcpy((void*)(buffer+index), &fSkinBody, sizeof(UnityEngine::Color::Object));
        index += sizeof(UnityEngine::Color::Object);
        memcpy((void*)(buffer+index), &fHair, sizeof(UnityEngine::Color::Object));
        index += sizeof(UnityEngine::Color::Object);

        memcpy((void*)(buffer+index), &bSkinFace, sizeof(UnityEngine::Color::Object));
        index += sizeof(UnityEngine::Color::Object);
        memcpy((void*)(buffer+index), &bHairExtra, sizeof(UnityEngine::Color::Object));
        index += sizeof(UnityEngine::Color::Object);
        memcpy((void*)(buffer+index), &bEyeLeft, sizeof(UnityEngine::Color::Object));
        index += sizeof(UnityEngine::Color::Object);
        memcpy((void*)(buffer+index), &bEyeRight, sizeof(UnityEngine::Color::Object));
        index += sizeof(UnityEngine::Color::Object);
        memcpy((void*)(buffer+index), &bSkinBody, sizeof(UnityEngine::Color::Object));
        index += sizeof(UnityEngine::Color::Object);
        memcpy((void*)(buffer+index), &bHair, sizeof(UnityEngine::Color::Object));
        index += sizeof(UnityEngine::Color::Object);

        return index;
    }

    long FromBytes(char* buffer, long buffer_size, long index) {
        Initialize();

        if (buffer_size < index + (long)sizeof(System::Int32))
            return index;
        memcpy(&playerColorID, (void*)(buffer+index), sizeof(System::Int32));
        index += sizeof(System::Int32);


        if (buffer_size < index + (long)sizeof(UnityEngine::Color::Object))
            return index;
        memcpy(&fSkinFace, (void*)(buffer + index), sizeof(UnityEngine::Color::Object));
        index += sizeof(UnityEngine::Color::Object);

        if (buffer_size < index + (long)sizeof(UnityEngine::Color::Object))
            return index;
        memcpy(&fSkinMouth, (void*)(buffer + index), sizeof(UnityEngine::Color::Object));
        index += sizeof(UnityEngine::Color::Object);

        if (buffer_size < index + (long)sizeof(UnityEngine::Color::Object))
            return index;
        memcpy(&fEyes, (void*)(buffer + index), sizeof(UnityEngine::Color::Object));
        index += sizeof(UnityEngine::Color::Object);

        if (buffer_size < index + (long)sizeof(UnityEngine::Color::Object))
            return index;
        memcpy(&fEyebrows, (void*)(buffer + index), sizeof(UnityEngine::Color::Object));
        index += sizeof(UnityEngine::Color::Object);

        if (buffer_size < index + (long)sizeof(UnityEngine::Color::Object))
            return index;
        memcpy(&fSkinBody, (void*)(buffer + index), sizeof(UnityEngine::Color::Object));
        index += sizeof(UnityEngine::Color::Object);

        if (buffer_size < index + (long)sizeof(UnityEngine::Color::Object))
            return index;
        memcpy(&fHair, (void*)(buffer + index), sizeof(UnityEngine::Color::Object));
        index += sizeof(UnityEngine::Color::Object);


        if (buffer_size < index + (long)sizeof(UnityEngine::Color::Object))
            return index;
        memcpy(&bSkinFace, (void*)(buffer + index), sizeof(UnityEngine::Color::Object));
        index += sizeof(UnityEngine::Color::Object);

        if (buffer_size < index + (long)sizeof(UnityEngine::Color::Object))
            return index;
        memcpy(&bHairExtra, (void*)(buffer + index), sizeof(UnityEngine::Color::Object));
        index += sizeof(UnityEngine::Color::Object);

        if (buffer_size < index + (long)sizeof(UnityEngine::Color::Object))
            return index;
        memcpy(&bEyeLeft, (void*)(buffer + index), sizeof(UnityEngine::Color::Object));
        index += sizeof(UnityEngine::Color::Object);

        if (buffer_size < index + (long)sizeof(UnityEngine::Color::Object))
            return index;
        memcpy(&bEyeRight, (void*)(buffer + index), sizeof(UnityEngine::Color::Object));
        index += sizeof(UnityEngine::Color::Object);

        if (buffer_size < index + (long)sizeof(UnityEngine::Color::Object))
            return index;
        memcpy(&bSkinBody, (void*)(buffer + index), sizeof(UnityEngine::Color::Object));
        index += sizeof(UnityEngine::Color::Object);

        if (buffer_size < index + (long)sizeof(UnityEngine::Color::Object))
            return index;
        memcpy(&bHair, (void*)(buffer + index), sizeof(UnityEngine::Color::Object));
        index += sizeof(UnityEngine::Color::Object);

        return index;
    }
};

void loadColorVariations(bool isBackup);
void linkColorVariations(PlayerWork::Object* playerWork);
void unlinkColorVariations(PlayerWork::Object* playerWork);
void saveColorVariations(bool isMain, bool isBackup);
void relinkColorVariations(PlayerWork::Object* playerWork);