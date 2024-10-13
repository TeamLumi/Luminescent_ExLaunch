#pragma once
#include "externals/il2cpp-api.h"
#include "externals/System/Primitives.h"
#include "externals/System/Action.h"

namespace Dpr::NX {
    struct SaveSystem : ILClass<SaveSystem, 0x04c600b8> {
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
            external<void>(0x01ddf5a0, this);
        }

        static inline bool SaveDataExists() {
            return external<bool>(0x01ddfcf0);
        }

        static inline bool Save(System::Byte_array* data, bool writeMain, bool writeBackup) {
            return external<bool>(0x01ddf3e0, data, writeMain, writeBackup);
        }

        static inline void SaveAsync(System::Byte_array* data, bool writeMain, bool writeBackup) {
            external<void>(0x01ddf680, data, writeMain, writeBackup);
        }

        inline void StartThread() {
            external<void>(0x01ddf830, this);
        }
    };
}
