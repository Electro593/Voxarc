/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define GL_TRIANGLES        0x0004
#define GL_FRONT            0x0404
#define GL_CULL_FACE        0x0B44
#define GL_DEPTH_TEST       0x0B71
#define GL_SCISSOR_TEST     0x0C11
#define GL_DONT_CARE        0x1100
#define GL_UNSIGNED_BYTE    0x1401
#define GL_FLOAT            0x1406
#define GL_DEBUG_SOURCE_API 0x8246
#define GL_DEBUG_TYPE_OTHER 0x8251
#define GL_ARRAY_BUFFER     0x8892
#define GL_STATIC_DRAW      0x88E4
#define GL_FRAGMENT_SHADER  0x8B30
#define GL_VERTEX_SHADER    0x8B31
#define GL_COMPILE_STATUS   0x8B81
#define GL_LINK_STATUS      0x8B82
#define GL_INFO_LOG_LENGTH  0x8B84
#define GL_DEBUG_OUTPUT     0x92E0

#define GL_DEPTH_BUFFER_BIT   0x00000100
#define GL_STENICL_BUFFER_BIT 0x00000400
#define GL_COLOR_BUFFER_BIT   0x00004000





typedef void (API_ENTRY func_OpenGL_DebugProc)(u32 Source, u32 Type, u32 ID, u32 Severity, s32 Length, c08 *Message, vptr UserParam);





#define OPENGL_FUNCS_TYPE_1 \
    IMPORT(void, Clear,                   u32 Mask) \
    IMPORT(void, ClearColor,              r32 Red, r32 Green, r32 Blue, r32 Alpha) \
    IMPORT(void, CullFace,                u32 Mode) \
    IMPORT(void, DrawArrays,              u32 Mode, s32 First, s32 Count) \
    IMPORT(void, Enable,                  u32 Capability) \
    IMPORT(void, Scissor,                 s32 X, s32 Y, s32 Width, s32 Height) \
    IMPORT(void, Viewport,                s32 X, s32 Y, s32 Width, s32 Height) \

#define OPENGL_FUNCS_TYPE_2 \
    IMPORT(void, AttachShader,            u32 Program, u32 Shader) \
    IMPORT(void, BindBuffer,              u32 Target, u32 Buffer) \
    IMPORT(void, BindVertexArray,         u32 ArrayID) \
    IMPORT(void, BufferData,              u32 Target, s64 Size, vptr Data, u32 Usage) \
    IMPORT(void, CompileShader,           u32 Shader) \
    IMPORT(u32,  CreateProgram,           void) \
    IMPORT(u32,  CreateShader,            u32 Type) \
    IMPORT(void, DebugMessageCallback,    func_OpenGL_DebugProc Callback, vptr UserParam) \
    IMPORT(void, DebugMessageControl,     u32 Source, u32 Type, u32 Severity, s32 Count, u32 *IDs, u08 Enabled) \
    IMPORT(void, DeleteShader,            u32 Shader) \
    IMPORT(void, DetachShader,            u32 Program, u32 Shader) \
    IMPORT(void, EnableVertexAttribArray, u32 Index) \
    IMPORT(void, GenBuffers,              s32 Count, u32 *BufferIDs) \
    IMPORT(void, GenVertexArrays,         s32 Count, u32 *ArrayIDs) \
    IMPORT(void, GetProgramiv,            u32 Program, u32 ParamName, s32 *Param) \
    IMPORT(void, GetProgramInfoLog,       u32 Program, s32 BufferSize, s32 *Length, c08 *InfoLog) \
    IMPORT(void, GetShaderiv,             u32 Shader, u32 ParamName, s32 *Param) \
    IMPORT(void, GetShaderInfoLog,        u32 Shader, s32 BufferSize, s32 *Length, c08 *InfoLog) \
    IMPORT(void, LinkProgram,             u32 Program) \
    IMPORT(void, ShaderSource,            u32 Shader, s32 Count, c08 **String, s32 *Length) \
    IMPORT(void, UseProgram,              u32 Program) \
    IMPORT(void, VertexAttribPointer,     u32 Index, s32 Size, u32 Type, b08 Normalized, s32 Stride, vptr Offset) \

#define OPENGL_FUNCS OPENGL_FUNCS_TYPE_1 OPENGL_FUNCS_TYPE_2

#define IMPORT(ReturnType, Name, ...) \
    typedef ReturnType func_OpenGL_##Name(__VA_ARGS__);
#define X OPENGL_FUNCS
#include <x.h>

struct opengl_funcs {
    #define IMPORT(ReturnType, Name, ...) \
        func_OpenGL_##Name *OpenGL_##Name;
    #define X OPENGL_FUNCS
    #include <x.h>
};

#if defined(_RENDERER_MODULE)
    #define IMPORT(ReturnType, Name, ...) \
        global func_OpenGL_##Name *OpenGL_##Name;
    #define X OPENGL_FUNCS
    #include <x.h>
#endif