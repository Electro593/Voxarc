#ifndef VOX_PLATFORM_SHARED_H_

#include "util/vox_defines.h"
#include "util/vox_memory.h"

#define VK_LBUTTON        0x01
#define VK_RBUTTON        0x02
#define VK_CANCEL         0x03
#define VK_MBUTTON        0x04
#define VK_XBUTTON1       0x05
#define VK_XBUTTON2       0x06
//                        0x07
#define VK_BACK           0x08
#define VK_TAB            0x09
//                        0x0A
//                        0x0B
#define VK_CLEAR          0x0C
#define VK_RETURN         0x0D
//                        0x0E
//                        0x0F
#define VK_SHIFT          0x10
#define VK_CONTROL        0x11
#define VK_MENU           0x12
#define VK_PAUSE          0x13
#define VK_CAPITAL        0x14
#define VK_KANA           0x15
#define VK_HANGEUL        0x15
#define VK_HANGUL         0x15
//                        0x16
#define VK_JUNJA          0x17
#define VK_FINAL          0x18
#define VK_HANJA          0x19
#define VK_KANJI          0x19
//                        0x1A
#define VK_ESCAPE         0x1B
#define VK_CONVERT        0x1C
#define VK_NONCONVERT     0x1D
#define VK_ACCEPT         0x1E
#define VK_MODECHANGE     0x1F
#define VK_SPACE          0x20
#define VK_PRIOR          0x21
#define VK_NEXT           0x22
#define VK_END            0x23
#define VK_HOME           0x24
#define VK_LEFT           0x25
#define VK_UP             0x26
#define VK_RIGHT          0x27
#define VK_DOWN           0x28
#define VK_SELECT         0x29
#define VK_PRINT          0x2A
#define VK_EXECUTE        0x2B
#define VK_SNAPSHOT       0x2C
#define VK_INSERT         0x2D
#define VK_DELETE         0x2E
#define VK_HELP           0x2F
#define VK_0              0x30
#define VK_1              0x31
#define VK_2              0x32
#define VK_3              0x33
#define VK_4              0x34
#define VK_5              0x35
#define VK_6              0x36
#define VK_7              0x37
#define VK_8              0x38
#define VK_9              0x39
//                        0x3A
//                        0x3B
//                        0x3C
//                        0x3D
//                        0x3E
//                        0x3F
//                        0x40
#define VK_A              0x41
#define VK_B              0x42
#define VK_C              0x43
#define VK_D              0x44
#define VK_E              0x45
#define VK_F              0x46
#define VK_G              0x47
#define VK_H              0x48
#define VK_I              0x49
#define VK_J              0x4A
#define VK_K              0x4B
#define VK_L              0x4C
#define VK_M              0x4D
#define VK_N              0x4E
#define VK_O              0x4F
#define VK_P              0x50
#define VK_Q              0x51
#define VK_R              0x52
#define VK_S              0x53
#define VK_T              0x54
#define VK_U              0x55
#define VK_V              0x56
#define VK_W              0x57
#define VK_X              0x58
#define VK_Y              0x59
#define VK_Z              0x5A
#define VK_LWIN           0x5B
#define VK_RWIN           0x5C
#define VK_APPS           0x5D
//                        0x5E
#define VK_SLEEP          0x5F
#define VK_NUMPAD0        0x60
#define VK_NUMPAD1        0x61
#define VK_NUMPAD2        0x62
#define VK_NUMPAD3        0x63
#define VK_NUMPAD4        0x64
#define VK_NUMPAD5        0x65
#define VK_NUMPAD6        0x66
#define VK_NUMPAD7        0x67
#define VK_NUMPAD8        0x68
#define VK_NUMPAD9        0x69
#define VK_MULTIPLY       0x6A
#define VK_ADD            0x6B
#define VK_SEPARATOR      0x6C
#define VK_SUBTRACT       0x6D
#define VK_DECIMAL        0x6E
#define VK_DIVIDE         0x6F
#define VK_F1             0x70
#define VK_F2             0x71
#define VK_F3             0x72
#define VK_F4             0x73
#define VK_F5             0x74
#define VK_F6             0x75
#define VK_F7             0x76
#define VK_F8             0x77
#define VK_F9             0x78
#define VK_F10            0x79
#define VK_F11            0x7A
#define VK_F12            0x7B
#define VK_F13            0x7C
#define VK_F14            0x7D
#define VK_F15            0x7E
#define VK_F16            0x7F
#define VK_F17            0x80
#define VK_F18            0x81
#define VK_F19            0x82
#define VK_F20            0x83
#define VK_F21            0x84
#define VK_F22            0x85
#define VK_F23            0x86
#define VK_F24            0x87
#define VK_NAVIGATION_VIEW       0x88
#define VK_NAVIGATION_MENU       0x89
#define VK_NAVIGATION_UP         0x8A
#define VK_NAVIGATION_DOWN       0x8B
#define VK_NAVIGATION_LEFT       0x8C
#define VK_NAVIGATION_RIGHT      0x8D
#define VK_NAVIGATION_ACCEPT     0x8E
#define VK_NAVIGATION_CANCEL     0x8F
#define VK_NUMLOCK        0x90
#define VK_SCROLL         0x91
#define VK_OEM_NEC_EQUAL  0x92
#define VK_OEM_FJ_JISHO   0x92
#define VK_OEM_FJ_MASSHOU 0x93
#define VK_OEM_FJ_TOUROKU 0x94
#define VK_OEM_FJ_LOYA    0x95
#define VK_OEM_FJ_ROYA    0x96
//                        0x97
//                        0x98
//                        0x99
//                        0x9A
//                        0x9B
//                        0x9C
//                        0x9D
//                        0x9E
//                        0x9F
#define VK_LSHIFT         0xA0
#define VK_RSHIFT         0xA1
#define VK_LCONTROL       0xA2
#define VK_RCONTROL       0xA3
#define VK_LMENU          0xA4
#define VK_RMENU          0xA5
#define VK_BROWSER_BACK        0xA6
#define VK_BROWSER_FORWARD     0xA7
#define VK_BROWSER_REFRESH     0xA8
#define VK_BROWSER_STOP        0xA9
#define VK_BROWSER_SEARCH      0xAA
#define VK_BROWSER_FAVORITES   0xAB
#define VK_BROWSER_HOME        0xAC
#define VK_VOLUME_MUTE         0xAD
#define VK_VOLUME_DOWN         0xAE
#define VK_VOLUME_UP           0xAF
#define VK_MEDIA_NEXT_TRACK    0xB0
#define VK_MEDIA_PREV_TRACK    0xB1
#define VK_MEDIA_STOP          0xB2
#define VK_MEDIA_PLAY_PAUSE    0xB3
#define VK_LAUNCH_MAIL         0xB4
#define VK_LAUNCH_MEDIA_SELECT 0xB5
#define VK_LAUNCH_APP1         0xB6
#define VK_LAUNCH_APP2         0xB7
//                        0xB8
//                        0xB9
#define VK_SEMICOLON      0xBA
#define VK_EQUAL          0xBB
#define VK_COMMA          0xBC
#define VK_MINUS          0xBD
#define VK_PERIOD         0xBE
#define VK_FRONT_SLASH    0xBF
#define VK_GRAVE          0xC0
//                        0xC1
//                        0xC2
#define VK_GAMEPAD_A                         0xC3
#define VK_GAMEPAD_B                         0xC4
#define VK_GAMEPAD_X                         0xC5
#define VK_GAMEPAD_Y                         0xC6
#define VK_GAMEPAD_RIGHT_SHOULDER            0xC7
#define VK_GAMEPAD_LEFT_SHOULDER             0xC8
#define VK_GAMEPAD_LEFT_TRIGGER              0xC9
#define VK_GAMEPAD_RIGHT_TRIGGER             0xCA
#define VK_GAMEPAD_DPAD_UP                   0xCB
#define VK_GAMEPAD_DPAD_DOWN                 0xCC
#define VK_GAMEPAD_DPAD_LEFT                 0xCD
#define VK_GAMEPAD_DPAD_RIGHT                0xCE
#define VK_GAMEPAD_MENU                      0xCF
#define VK_GAMEPAD_VIEW                      0xD0
#define VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON    0xD1
#define VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON   0xD2
#define VK_GAMEPAD_LEFT_THUMBSTICK_UP        0xD3
#define VK_GAMEPAD_LEFT_THUMBSTICK_DOWN      0xD4
#define VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT     0xD5
#define VK_GAMEPAD_LEFT_THUMBSTICK_LEFT      0xD6
#define VK_GAMEPAD_RIGHT_THUMBSTICK_UP       0xD7
#define VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN     0xD8
#define VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT    0xD9
#define VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT     0xDA
#define VK_OPEN_BRACKET   0xDB
#define VK_BACK_SLASH     0xDC
#define VK_CLOSE_BRACKET  0xDD
#define VK_QUOTE          0xDE
#define VK_OEM_8          0xDF
//                        0xE0
#define VK_OEM_AX         0xE1
#define VK_OEM_102        0xE2
#define VK_ICO_HELP       0xE3
#define VK_ICO_00         0xE4
#define VK_PROCESSKEY     0xE5
#define VK_ICO_CLEAR      0xE6
#define VK_PACKET         0xE7
//                        0xE8
#define VK_OEM_RESET      0xE9
#define VK_OEM_JUMP       0xEA
#define VK_OEM_PA1        0xEB
#define VK_OEM_PA2        0xEC
#define VK_OEM_PA3        0xED
#define VK_OEM_WSCTRL     0xEE
#define VK_OEM_CUSEL      0xEF
#define VK_OEM_ATTN       0xF0
#define VK_OEM_FINISH     0xF1
#define VK_OEM_COPY       0xF2
#define VK_OEM_AUTO       0xF3
#define VK_OEM_ENLW       0xF4
#define VK_OEM_BACKTAB    0xF5
#define VK_ATTN           0xF6
#define VK_CRSEL          0xF7
#define VK_EXSEL          0xF8
#define VK_EREOF          0xF9
#define VK_PLAY           0xFA
#define VK_ZOOM           0xFB
#define VK_NONAME         0xFC
#define VK_PA1            0xFD
#define VK_OEM_CLEAR      0xFE
//                        0xFF

#define GL_TRIANGLES                0x0004
#define GL_DEPTH_BUFFER_BIT     0x00000100
#define GL_LESS                     0x0201
#define GL_CULL_FACE                0x0B44
#define GL_DEPTH_TEST               0x0B71
#define GL_TEXTURE_2D               0x0DE1
#define GL_UNSIGNED_SHORT           0x1403
#define GL_FLOAT                    0x1406
#define GL_UNSIGNED_BYTE            0x1401
#define GL_RGBA                     0x1908
#define GL_NEAREST                  0x2600
#define GL_TEXTURE_MAG_FILTER       0x2800
#define GL_TEXTURE_MIN_FILTER       0x2801
#define GL_TEXTURE_WRAP_S           0x2802
#define GL_TEXTURE_WRAP_T           0x2803
#define GL_CLAMP                    0x2900
#define GL_COLOR_BUFFER_BIT     0x00004000
#define GL_BGRA                     0x80E1
#define GL_NUM_EXTENSIONS           0x821D
#define GL_ARRAY_BUFFER             0x8892
#define GL_STATIC_DRAW              0x88E4
#define GL_COMPILE_STATUS           0x8B81
#define GL_ELEMENT_ARRAY_BUFFER     0x8893
#define GL_FRAGMENT_SHADER          0x8B30
#define GL_VERTEX_SHADER            0x8B31
#define GL_LINK_STATUS              0x8B82
#define GL_INFO_LOG_LENGTH          0x8B84

#define GL_PROCS \
    PROC(s32,  glGetUniformLocation,       u32 Program, const c08 *Name) \
    PROC(u32,  glCreateShader,             u32 ShaderType) \
    PROC(u32,  glCreateProgram)            \
    PROC(void, glAttachShader,             u32 Program, u32 Shader) \
    PROC(void, glBindBuffer,               u32 Target, u32 Buffer) \
    PROC(void, glBindTexture,              u32 Target, u32 Texture) \
    PROC(void, glBindVertexArray,          u32 Array) \
    PROC(void, glBufferData,               u32 Target, u64 Size, vptr Data, u32 Usage) \
    PROC(void, glCompileShader,            u32 Shader) \
    PROC(void, glDeleteShader,             u32 Shader) \
    PROC(void, glDisableVertexAttribArray, u32 Index) \
    PROC(void, glDrawElements,             u32 Mode, s32 Count, u32 Type, const vptr Indices) \
    PROC(void, glEnableVertexAttribArray,  u32 Index) \
    PROC(void, glGenBuffers,               s32 N, u32 *Buffers) \
    PROC(void, glGenTextures,              s32 N, u32 *Textures) \
    PROC(void, glGenVertexArrays,          s32 N, u32 *Arrays) \
    PROC(void, glGetProgramInfoLog,        u32 Program, s32 MaxLength, s32 *Length, c08 *InfoLog) \
    PROC(void, glGetProgramiv,             u32 Program, u32 Name, s32 *Params) \
    PROC(void, glGetShaderInfoLog,         u32 Shader, s32 MaxLength, s32 *Length, c08 *InfoLog) \
    PROC(void, glGetShaderiv,              u32 Shader, u32 Name, s32 *Params) \
    PROC(void, glLinkProgram,              u32 Program) \
    PROC(void, glShaderSource,             u32 Shader, s32 Count, const c08 **String, const s32 *Length) \
    PROC(void, glUniformMatrix4fv,         s32 Location, s32 Count, s08 Transpose, const r32 *Value) \
    PROC(void, glUseProgram,               u32 Program) \
    PROC(void, glUniform1ui,               s32 Location, u32 V0) \
    PROC(void, glUniform3f,                s32 Location, r32 V0, r32 V1, r32 V2) \
    PROC(void, glUniform4ui,               s32 Location, u32 V0, u32 V1, u32 V2, u32 V3) \
    PROC(void, glVertexAttribPointer,      u32 Index, s32 Size, u32 Type, b08 Normalized, s32 Stride, vptr Pointer) \

#define PLATFORM_PROCS \
    PROC(size, GetFileSize,      memory_handle *FileHandle) \
    PROC(void, CloseFile,        memory_handle *FileHandle) \
    PROC(void, OpenFile,         memory_handle *FileHandle, c08 *FileName) \
    PROC(void, ReadDataFromFile, memory_handle *Dest, memory_handle *FileHandle, size Offset) \
    PROC(vptr, ReserveMemory,    size Size) \
    PROC(void, CaptureCursor,    void) \
    PROC(void, ReleaseCursor,    void) \

#define PROC(ReturnType, Name, ...) \
    typedef VOXAPI ReturnType VOXAPIENTRY Name##proc(__VA_ARGS__); \
    global_var Name##proc *Name;
GL_PROCS
PLATFORM_PROCS
#undef PROC

#define VOX_PLATFORM_SHARED_H_
#endif