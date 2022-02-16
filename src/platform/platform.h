/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

typedef enum platform_updates {
    WINDOW_RESIZED = 0x01,
} platform_updates;

typedef enum execution_state {
    EXECUTION_UNINITIALIZED,
    EXECUTION_RUNNING,
    EXECUTION_PAUSED,
    EXECUTION_ENDED,
} execution_state;

typedef enum key_state {
    KEY_RELEASED,
    KEY_PRESSED,
    KEY_HELD,
} key_state;

typedef enum file_mode {
    FILE_READ,
    FILE_WRITE,
    FILE_APPEND,
} file_mode;

typedef struct platform_state {
    v2u32 WindowSize; 
    execution_state ExecutionState;
    platform_updates Updates;
} platform_state;

internal void _Assert(c08 *File, u32 Line, c08 *Expression, c08 *Message);
internal b08 Platform_OpenFile(file_handle *FileHandle, c08 *FileName, file_mode OpenMode);
internal u64 Platform_GetFileLength(file_handle FileHandle);
internal u64 Platform_ReadFile(file_handle FileHandle, vptr Dest, u64 Length, u64 Offset);
internal u64 Platform_WriteFile(file_handle FileHandle, vptr Src, u64 Length, u64 Offset);
internal void Platform_CloseFile(file_handle FileHandle);
internal vptr Platform_AllocateMemory(u64 Size);
internal void Platform_FreeMemory(vptr Base);