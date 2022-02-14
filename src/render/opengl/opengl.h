/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define GL_TRIANGLES        0x0004
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





#define OPENGL_FUNCS \
    FUNC_TYPE2(void, AttachShader,            u32 Program, u32 Shader) \
    FUNC_TYPE2(void, BindBuffer,              u32 Target, u32 Buffer) \
    FUNC_TYPE2(void, BindVertexArray,         u32 ArrayID) \
    FUNC_TYPE2(void, BufferData,              u32 Target, s64 Size, vptr Data, u32 Usage) \
    FUNC_TYPE1(void, Clear,                   u32 Mask) \
    FUNC_TYPE1(void, ClearColor,              r32 Red, r32 Green, r32 Blue, r32 Alpha) \
    FUNC_TYPE2(void, CompileShader,           u32 Shader) \
    FUNC_TYPE2(u32,  CreateProgram,           void) \
    FUNC_TYPE2(u32,  CreateShader,            u32 Type) \
    FUNC_TYPE1(void, DrawArrays,              u32 Mode, s32 First, s32 Count) \
    FUNC_TYPE2(void, DebugMessageCallback,    func_OpenGL_DebugProc Callback, vptr UserParam) \
    FUNC_TYPE2(void, DebugMessageControl,     u32 Source, u32 Type, u32 Severity, s32 Count, u32 *IDs, u08 Enabled) \
    FUNC_TYPE2(void, DeleteShader,            u32 Shader) \
    FUNC_TYPE2(void, DetachShader,            u32 Program, u32 Shader) \
    FUNC_TYPE1(void, Enable,                  u32 Capability) \
    FUNC_TYPE2(void, EnableVertexAttribArray, u32 Index) \
    FUNC_TYPE2(void, GenBuffers,              s32 Count, u32 *BufferIDs) \
    FUNC_TYPE2(void, GenVertexArrays,         s32 Count, u32 *ArrayIDs) \
    FUNC_TYPE2(void, GetProgramiv,            u32 Program, u32 ParamName, s32 *Param) \
    FUNC_TYPE2(void, GetProgramInfoLog,       u32 Program, s32 BufferSize, s32 *Length, c08 *InfoLog) \
    FUNC_TYPE2(void, GetShaderiv,             u32 Shader, u32 ParamName, s32 *Param) \
    FUNC_TYPE2(void, GetShaderInfoLog,        u32 Shader, s32 BufferSize, s32 *Length, c08 *InfoLog) \
    FUNC_TYPE2(void, LinkProgram,             u32 Program) \
    FUNC_TYPE2(void, ShaderSource,            u32 Shader, s32 Count, c08 **String, s32 *Length) \
    FUNC_TYPE2(void, UseProgram,              u32 Program) \
    FUNC_TYPE2(void, VertexAttribPointer,     u32 Index, s32 Size, u32 Type, b08 Normalized, s32 Stride, vptr Offset) \

#define FUNC_TYPE1(ReturnType, Name, ...) \
    typedef ReturnType func_OpenGL_##Name(__VA_ARGS__); \
    global func_OpenGL_##Name *OpenGL_##Name;
#define FUNC_TYPE2 FUNC_TYPE1
OPENGL_FUNCS
#undef FUNC_TYPE1
#undef FUNC_TYPE2