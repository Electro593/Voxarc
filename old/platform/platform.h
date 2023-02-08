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
    CURSOR_DISABLED = 0x02,
} platform_updates;

typedef enum execution_state {
    EXECUTION_UNINITIALIZED,
    EXECUTION_RUNNING,
    EXECUTION_PAUSED,
    EXECUTION_ENDED,
} execution_state;

typedef enum focus_state {
    FOCUS_CLIENT,
    FOCUS_FRAME,
    FOCUS_NONE
} focus_state;

typedef enum scancode {
    ScanCode_Unknown          = 0x00,
    
    ScanCode_Escape           = 0x01,
    ScanCode_1                = 0x02,
    ScanCode_2                = 0x03,
    ScanCode_3                = 0x04,
    ScanCode_4                = 0x05,
    ScanCode_5                = 0x06,
    ScanCode_6                = 0x07,
    ScanCode_7                = 0x08,
    ScanCode_8                = 0x09,
    ScanCode_9                = 0x0A,
    ScanCode_0                = 0x0B,
    ScanCode_Hyphen           = 0x0C,
    ScanCode_Equals           = 0x0D,
    ScanCode_Backspace        = 0x0E,
    ScanCode_Tab              = 0x0F,
    ScanCode_Q                = 0x10,
    ScanCode_W                = 0x11,
    ScanCode_E                = 0x12,
    ScanCode_R                = 0x13,
    ScanCode_T                = 0x14,
    ScanCode_Y                = 0x15,
    ScanCode_U                = 0x16,
    ScanCode_I                = 0x17,
    ScanCode_O                = 0x18,
    ScanCode_P                = 0x19,
    ScanCode_BracketLeft      = 0x1A,
    ScanCode_BracketRight     = 0x1B,
    ScanCode_Enter            = 0x1C,
    ScanCode_ControlLeft      = 0x1D,
    ScanCode_A                = 0x1E,
    ScanCode_S                = 0x1F,
    ScanCode_D                = 0x20,
    ScanCode_F                = 0x21,
    ScanCode_G                = 0x22,
    ScanCode_H                = 0x23,
    ScanCode_K                = 0x25,
    ScanCode_J                = 0x24,
    ScanCode_L                = 0x26,
    ScanCode_Semicolon        = 0x27,
    ScanCode_Apostrophe       = 0x28,
    ScanCode_Grave            = 0x29,
    ScanCode_ShiftLeft        = 0x2A,
    ScanCode_Backslash        = 0x2B,
    ScanCode_Z                = 0x2C,
    ScanCode_X                = 0x2D,
    ScanCode_C                = 0x2E,
    ScanCode_V                = 0x2F,
    ScanCode_B                = 0x30,
    ScanCode_N                = 0x31,
    ScanCode_M                = 0x32,
    ScanCode_Comma            = 0x33,
    ScanCode_Period           = 0x34,
    ScanCode_Slash            = 0x35,
    ScanCode_ShiftRight       = 0x36,
    ScanCode_Multiply         = 0x37,
    ScanCode_AltLeft          = 0x38,
    ScanCode_Space            = 0x39,
    ScanCode_CapsLock         = 0x3A,
    ScanCode_F1               = 0x3B,
    ScanCode_F2               = 0x3C,
    ScanCode_F3               = 0x3D,
    ScanCode_F4               = 0x3E,
    ScanCode_F5               = 0x3F,
    ScanCode_F6               = 0x40,
    ScanCode_F7               = 0x41,
    ScanCode_F8               = 0x42,
    ScanCode_F9               = 0x43,
    ScanCode_F10              = 0x44,
    ScanCode_NumLock          = 0x45,
    ScanCode_ScrollLock       = 0x46,
    ScanCode_Numpad7          = 0x47,
    ScanCode_Numpad8          = 0x48,
    ScanCode_Numpad9          = 0x49,
    ScanCode_NumpadSubtract   = 0x4A,
    ScanCode_Numpad4          = 0x4B,
    ScanCode_Numpad5          = 0x4C,
    ScanCode_Numpad6          = 0x4D,
    ScanCode_NumpadAdd        = 0x4E,
    ScanCode_Numpad1          = 0x4F,
    ScanCode_Numpad2          = 0x50,
    ScanCode_Numpad3          = 0x51,
    ScanCode_Numpad0          = 0x52,
    ScanCode_NumpadDecimal    = 0x53,
    ScanCode_AltPrintScreen   = 0x54,
    ScanCode_AngleBracket     = 0x59,
    ScanCode_F11              = 0x57,
    ScanCode_F12              = 0x58,
    ScanCode_OEM1             = 0x5A,
    ScanCode_OEM2             = 0x5B,
    ScanCode_OEM3             = 0x5C,
    ScanCode_EraseEOF         = 0x5D,
    ScanCode_OEM4             = 0x5E,
    ScanCode_OEM5             = 0x5F,
    ScanCode_Zoom             = 0x62,
    ScanCode_Help             = 0x63,
    ScanCode_F13              = 0x64,
    ScanCode_F14              = 0x65,
    ScanCode_F15              = 0x66,
    ScanCode_F16              = 0x67,
    ScanCode_F17              = 0x68,
    ScanCode_F18              = 0x69,
    ScanCode_F19              = 0x6A,
    ScanCode_F20              = 0x6B,
    ScanCode_F21              = 0x6C,
    ScanCode_F22              = 0x6D,
    ScanCode_F23              = 0x6E,
    ScanCode_OEM6             = 0x6F,
    ScanCode_Katakana         = 0x70,
    ScanCode_OEM7             = 0x71,
    ScanCode_F24              = 0x76,
    ScanCode_SBCS             = 0x77,
    ScanCode_Convert          = 0x79,
    ScanCode_NonConvert       = 0x7B,
    
    ScanCode_MediaPrev        = 0x80 | 0x10,
    ScanCode_MediaNext        = 0x80 | 0x19,
    ScanCode_NumpadEnter      = 0x80 | 0x1C,
    ScanCode_ControlRight     = 0x80 | 0x1D,
    ScanCode_VolumeMute       = 0x80 | 0x20,
    ScanCode_LaunchApp2       = 0x80 | 0x21,
    ScanCode_MediaPlay        = 0x80 | 0x22,
    ScanCode_MediaStop        = 0x80 | 0x24,
    ScanCode_VolumeDown       = 0x80 | 0x2E,
    ScanCode_VolumeUp         = 0x80 | 0x30,
    ScanCode_BrowserHome      = 0x80 | 0x32,
    ScanCode_NumpadDivide     = 0x80 | 0x35,
    ScanCode_PrintScreen      = 0x80 | 0x37,
    ScanCode_AltRight         = 0x80 | 0x38,
    ScanCode_Cancel           = 0x80 | 0x46,
    ScanCode_Home             = 0x80 | 0x47,
    ScanCode_ArrowUp          = 0x80 | 0x48,
    ScanCode_PageUp           = 0x80 | 0x49,
    ScanCode_ArrowLeft        = 0x80 | 0x4B,
    ScanCode_ArrowRight       = 0x80 | 0x4D,
    ScanCode_End              = 0x80 | 0x4F,
    ScanCode_ArrowDown        = 0x80 | 0x50,
    ScanCode_PageDown         = 0x80 | 0x51,
    ScanCode_Insert           = 0x80 | 0x52,
    ScanCode_Delete           = 0x80 | 0x53,
    ScanCode_MetaLeft         = 0x80 | 0x5B,
    ScanCode_MetaRight        = 0x80 | 0x5C,
    ScanCode_Application      = 0x80 | 0x5D,
    ScanCode_Power            = 0x80 | 0x5E,
    ScanCode_Sleep            = 0x80 | 0x5F,
    ScanCode_Wake             = 0x80 | 0x63,
    ScanCode_BrowserSearch    = 0x80 | 0x65,
    ScanCode_BrowserFavorites = 0x80 | 0x66,
    ScanCode_BrowserRefresh   = 0x80 | 0x67,
    ScanCode_BrowserStop      = 0x80 | 0x68,
    ScanCode_BrowserForward   = 0x80 | 0x69,
    ScanCode_BrowserBack      = 0x80 | 0x6A,
    ScanCode_LaunchApp1       = 0x80 | 0x6B,
    ScanCode_LaunchEmail      = 0x80 | 0x6C,
    ScanCode_LaunchMedia      = 0x80 | 0x6D,
    
    ScanCode_Pause            = 0xFF, //0xE11D45,
} scancode;

typedef enum button {
    Button_Left   = 0,
    Button_Right  = 1,
    Button_Middle = 2
} button;

typedef enum key_state {
    RELEASED,
    PRESSED,
    HELD,
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
    b08 CursorIsDisabled;
    v2s32 RestoreCursorPos;
    v2s32 CursorPos;
    
    v2u32 WindowSize; 
    execution_state ExecutionState;
    focus_state FocusState;
    platform_updates Updates;
    u08 Buttons[5];
    u08 Keys[256];
    
    r32 FPS;
};