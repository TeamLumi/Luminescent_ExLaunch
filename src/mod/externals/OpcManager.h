#pragma once

#include "externals/il2cpp-api.h"

#include "externals/OpcController.h"
#include "externals/UnityEngine/GameObject.h"

struct OpcManager : ILClass<OpcManager> {
    struct CharaData : ILStruct<CharaData> {
        struct Fields {
            int32_t stationIndex;
            System::String::Object* assetName;
            int32_t colorId;
            int32_t avatarId;
            int32_t sexId;
            int32_t cassetVersion;
        };
    };

    struct Fields {
        void* _OpcControllers;                      // 0x10 List<OpcController>
        void* _CopyOpcControllers;                  // 0x18 List<OpcController>
        void* _DataTable;                           // 0x20 UnionCharacterTable.SheetSheet1[]
        void* _ColorDataTable;                      // 0x28 UnionNpcColorTable.SheetSheet1[]
        int32_t characterCreateCount;               // 0x30
        void* _RequestNetDataCallback;              // 0x38 Action<byte>
        int32_t _CreateMaxCharacterCount;           // 0x40
    };

    inline OpcController::Object* GetOpc(int32_t id) {
        return external<OpcController::Object*>(0x01e10bf0, this, id);
    }

    inline void AddOpc(OpcController::Object* opc) {
        external<void>(0x01e10ee0, this, opc);
    }

    inline void RemoveOpc(OpcController::Object* opc) {
        external<void>(0x01e11090, this, opc);
    }

    inline void Init() {
        external<void>(0x01e10890, this);
    }

    inline void Destroy() {
        external<void>(0x01e10900, this);
    }

    inline void RemoveCharacter(int32_t stationIndex, bool isGameObjectDestroy = true) {
        external<void>(0x01e11460, this, stationIndex, isGameObjectDestroy);
    }

    inline void DestroyCharacterObject(UnityEngine::GameObject::Object* gameObj) {
        external<void>(0x01e11600, this, gameObj);
    }

    inline void RemoveAllCharacter() {
        external<void>(0x01e116a0, this);
    }
};
