#pragma once
#include "externals/il2cpp-api.h"
#include "externals/System/Primitives.h"
#include "externals/System/Action.h"

namespace Dpr::NX {
    struct SaveSystem : ILClass<SaveSystem, game::Address::Dpr_NX_SaveSystem_TypeInfo> {
        struct Fields {
            bool _isMounted;
            bool _isBusy;
            System::Byte_array* _buffer;
            void* _saveLoadThread;
            bool _threadEnable;
            bool _saveRequest;
            System::Byte_array* _saveDataPtr;
            bool _writeMain;
            bool _writeBackup;
            bool _loadRequest;
            bool _fromBackup;
            bool _loadResult;
            System::Action::Object* _postLoadFunction;
            void* _mainThread;
        };

        struct StaticFields {
            int32_t SAVELOAD_BUFFER_SIZE;
            Dpr::NX::SaveSystem::Object* _Instance;
        };

        static_assert(offsetof(Fields, _mainThread) == 0x38);

        inline void MountSaveData() {
            external<void>(game::Address::Dpr_NX_SaveSystem_MountSaveData, this);
        }

        static inline bool SaveDataExists() {
            return external<bool>(game::Address::Dpr_NX_SaveSystem_SaveDataExists);
        }

        static inline bool Save(System::Byte_array* data, bool writeMain, bool writeBackup) {
            return external<bool>(game::Address::Dpr_NX_SaveSystem_Save, data, writeMain, writeBackup);
        }

        static inline void SaveAsync(System::Byte_array* data, bool writeMain, bool writeBackup) {
            external<void>(game::Address::Dpr_NX_SaveSystem_SaveAsync, data, writeMain, writeBackup);
        }

        inline void StartThread() {
            external<void>(game::Address::Dpr_NX_SaveSystem_StartThread, this);
        }
    };
}
