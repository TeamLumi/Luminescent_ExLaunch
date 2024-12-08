#pragma once

#include "fs_types.hpp"
#include "types.h"
#include "nn/account.h"



namespace nn::fs {

    /// SaveDataSpaceId
    typedef enum {
        FsSaveDataSpaceId_System           = 0,    ///< System
        FsSaveDataSpaceId_User             = 1,    ///< User
        FsSaveDataSpaceId_SdSystem         = 2,    ///< SdSystem
        FsSaveDataSpaceId_Temporary        = 3,    ///< [3.0.0+] Temporary
        FsSaveDataSpaceId_SdUser           = 4,    ///< [4.0.0+] SdUser
        FsSaveDataSpaceId_ProperSystem     = 100,  ///< [3.0.0+] ProperSystem
        FsSaveDataSpaceId_SafeMode         = 101,  ///< [3.0.0+] SafeMode

        FsSaveDataSpaceId_All              = -1,   ///< Pseudo value for fsOpenSaveDataInfoReader().
    } SaveDataSpaceId;

    struct FileTimeStamp {
        u64 mTime1; // date created
        u64 mTime2; // sometimes is identical to above
        u64 mTime3; // looks like the date the file was created without exact time?
        bool unkBool;
    };

    /*
        Create a file.
        path: Path where to create the path.
        size: Size of the file to create.
    */
    Result CreateFile(char const* path, s64 size);

    /*
        Open a file.
        handleOut:   Output for handle representing opened file.
        path:        File path to open.
        mode:        Mode to open file. See OpenMode.
    */
    Result OpenFile(FileHandle* handleOut, char const* path, int mode);

    /*
        Close a file.
        handle:     Handle to file to be closed.
    */
    void CloseFile(FileHandle handle);

    /*
        Read file at a location.
        handle:     Handle representing file to be read.
        position:   Position within the file to be read.
        size:       How many bytes to read from file.
    */
    Result ReadFile(FileHandle handle, long position, void* buffer, ulong size);

    /*
        Read file at a location, with additional options.
        handle:     Handle representing file to be read.
        position:   Position within the file to be read.
        size:       How many bytes to read from file.
        option:     Additional options for reading, see ReadOption.
    */
    Result ReadFile(FileHandle handle, long position, void* buffer, const ReadOption& option);

    /*
        Read file at a location, with an output amount of bytes read.
        bytesRead:  How many bytes were actually read.
        handle:     Handle representing file to be read.
        position:   Position within the file to be read.
        size:       How many bytes to read from file.
    */
    Result ReadFile(ulong* bytesRead, FileHandle handle, long position, void* buffer);

    /*
        Read file at a location, with an output amount of bytes read, and additional options.
        bytesRead:  How many bytes were actually read.
        handle:     Handle representing file to be read.
        position:   Position within the file to be read.
        size:       How many bytes to read from file.
        option:     Additional options for reading, see ReadOption.
    */
    Result ReadFile(ulong* bytesRead, FileHandle handle, long position, void* buffer, const ReadOption& option);

    /*
        Gets the size of the file.
        size:   File size.
        handle: Handle representing file to check.
    */
    Result GetFileSize(long* size, nn::fs::FileHandle handle);

    /*
        Writes to a file.
        handle: Handle representing file to write to.
        position: Position within the file to write to.
        buffer: Pointer to the data to be written.
        size: Amount of data to write, from the pointer.
        option: Additional options for writing, like flushing.
    */
    Result WriteFile(FileHandle handle, s64 position, void const* buffer, u64 size, WriteOption const& option);

    /*
        Flush file.
        handle: Handle representing file to flush.
    */
    Result FlushFile(FileHandle handle);

    // Result GetSaveDataTimeStamp(nn::time::PosixTime *,ulong);
    // Result GetSaveDataTimeStamp(nn::time::PosixTime*, nn::fs::SaveDataSpaceId, ulong);
    Result GetFileTimeStampForDebug(nn::fs::FileTimeStamp*, char const*);

    Result DeleteFile(const char *path);

} // namespace nn::fs