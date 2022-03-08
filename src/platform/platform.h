/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// typedef struct datetime {
struct datetime {
    u16 Year;
    u08 Month;
    u08 Day;
    u08 DayOfWeek;
    u08 Hour;
    u08 Minute;
    u08 Second;
    u16 Millisecond;
};
// } datetime;

#if defined(_WIN32)
    #include <platform/win32/win32.h>
    #include <platform/win32/entry.h>
    typedef win32_handle file_handle;
#endif

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

#define PLATFORM_FUNCS \
    EXPORT(vptr,         Platform, AllocateMemory, u64 Size) \
    EXPORT(void,         Platform, Assert,         c08 *File, u32 Line, c08 *Expression, c08 *Message) \
    EXPORT(void,         Platform, CloseFile,      file_handle FileHandle) \
    EXTERN(void,         Platform, Entry,          void) \
    EXPORT(void,         Platform, FreeMemory,     vptr Base) \
    EXPORT(u64,          Platform, GetFileLength,  file_handle FileHandle) \
    EXPORT(void,         Platform, GetFileTime,    c08 *FileName, datetime *CreationTime, datetime *LastAccessTime, datetime *LastWriteTime) \
    EXPORT(s08,          Platform, CmpFileTime,    datetime A, datetime B) \
    INTERN(void,         Platform, LoadGame,       module *Module, game_state *GameState, platform_exports *PlatformExports, opengl_funcs *OpenGLFuncs) \
    INTERN(opengl_funcs, Platform, LoadOpenGL,     win32_device_context DeviceContext) \
    INTERN(void,         Platform, LoadWGL,        void) \
    INTERN(void,         Platform, LoadWin32,      void) \
    EXPORT(b08,          Platform, OpenFile,       file_handle *FileHandle, c08 *FileName, file_mode OpenMode) \
    EXPORT(u64,          Platform, ReadFile,       file_handle FileHandle, vptr Dest, u64 Length, u64 Offset) \
    INTERN(s64,          Platform, WindowCallback, win32_window Window, u32 Message, s64 WParam, s64 LParam) \
    EXPORT(u64,          Platform, WriteFile,      file_handle FileHandle, vptr Src, u64 Length, u64 Offset) \

#define EXPORT(ReturnType, Namespace, Name, ...) \
    typedef ReturnType func_##Namespace##_##Name(__VA_ARGS__);
#define EXTERN EXPORT
#define X PLATFORM_FUNCS
#include <x.h>

struct platform_exports {
    #define EXPORT(ReturnType, Namespace, Name, ...) \
        func_##Namespace##_##Name *Namespace##_##Name;
    #define X PLATFORM_FUNCS
    #include <x.h>
};

#if defined(_PLATFORM_MODULE)
    #define DEFAULT(ReturnType, Namespace, Name, ...) \
        internal ReturnType Namespace##_##Name(__VA_ARGS__);
    #define EXTERN(ReturnType, Namespace, Name, ...) \
        external ReturnType Namespace##_##Name(__VA_ARGS__);
    #define X PLATFORM_FUNCS
    #include <x.h>
#else
    #define EXPORT(ReturnType, Namespace, Name, ...) \
        global func_##Namespace##_##Name *Namespace##_##Name;
    #define X PLATFORM_FUNCS
    #include <x.h>
#endif

struct platform_state {
    v2u32 WindowSize; 
    execution_state ExecutionState;
    platform_updates Updates;
};