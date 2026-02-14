#pragma once

#include "externals/il2cpp-api.h"

#include "externals/OpcController.h"

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
        void* _OpcControllers;                      // List<OpcController>
        void* _CopyOpcControllers;                  // List<OpcController>
        void* _DataTable;                           // UnionCharacterTable.SheetSheet1[]
        void* _ColorDataTable;                      // UnionNpcColorTable.SheetSheet1[]
        int32_t characterCreateCount;
        void* _RequestNetDataCallback;              // Action<byte>
        int32_t _CreateMaxCharacterCount;
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

    inline void DestroyCharacterObject(Il2CppObject* gameObj) {
        external<void>(0x01e11600, this, gameObj);
    }

    inline void RemoveAllCharacter() {
        external<void>(0x01e116a0, this);
    }
};
