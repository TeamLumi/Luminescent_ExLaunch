#pragma once
#define INSERT_PADDING_BYTES(size) char _padding_##__COUNTER__[size]

#include <nn/nn_common.hpp>
#include <exlaunch/nx/types.h>

namespace nn::fs {

    /* Handle representing an opened file. */
    struct FileHandle {
        u64 _internal;
    };

    /* Handle representing an opened directory. */
    struct DirectoryHandle {
        u64 _internal;
    };

    /* Kinds of entries within a directory. */
    enum DirectoryEntryType {
        DirectoryEntryType_Directory,
        DirectoryEntryType_File,
    };

    /* Bitfield to define the kinds of entries to open from a directory. */
    enum OpenDirectoryMode : u8 {
        OpenDirectoryMode_Directory = BIT(0),
        OpenDirectoryMode_File = BIT(1),

        OpenDirectoryMode_All = OpenDirectoryMode_Directory | OpenDirectoryMode_File,
    };

    /* Maximum length a directory name can be. */
    constexpr inline size_t MaxDirectoryEntryNameSize = 0x300;

    /* Information about an entry within a directory. */
    struct DirectoryEntry {
        char m_Name[MaxDirectoryEntryNameSize + 1];
        OpenDirectoryMode m_Type;
        u32 field_304;
        long m_FileSize;
    };

    /* Mode for opening files. */
    enum OpenMode {
        OpenMode_Read = BIT(0),
        OpenMode_Write = BIT(1),
        OpenMode_Append = BIT(2),

        OpenMode_ReadWrite = OpenMode_Read | OpenMode_Write,
    };

    /* Options for reading. TODO: is this an enum? what for? */
    struct ReadOption {
        int _field_0;

        static inline ReadOption MakeOption(int value) { return {value}; }
    };

    enum WriteOptionFlag {
        WriteOptionFlag_None = 0 << 0,
        WriteOptionFlag_Flush = 1 << 0,
    };

    /* Options for writing. TODO: What else can this do? */
    struct WriteOption {
        int flags;
        static WriteOption CreateOption(int v) {
            return {
                .flags = v,
            };
        }
    };

    struct SaveDataExtraData {
        char attr[0x40];
        u64 owner_id;
        s64 timestamp;
        u32 flags;
        char padding1[4];
        s64 available_size;
        s64 journal_size;
        s64 commit_id;
        char padding2[0x190];
    };
    static_assert(sizeof(SaveDataExtraData) == 0x200, "SaveDataExtraData has invalid size.");
    static_assert(std::is_trivially_copyable_v<SaveDataExtraData>,
                  "Data type must be trivially copyable.");
}; // namespace nn::fs
