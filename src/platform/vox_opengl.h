/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                         * 
 *  Copyright (C) 2020 Andrew Seiler                                       * 
 *                                                                         * 
 *  This program is free software: you can redistribute it and/or modify   * 
 *  it under the terms of the GNU General Public License as published by   * 
 *  the Free Software Foundation, either version 3 of the License, or      * 
 *  (at your option) any later version.                                    * 
 *                                                                         * 
 *  This program is distributed in the hope that it will be useful,        * 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         * 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           * 
 *  GNU General Public License for more details.                           * 
 *                                                                         * 
 *  You should have received a copy of the GNU General Public License      * 
 *  along with this program. If not, see <https://www.gnu.org/licenses/>.  * 
 *                                                                         * 
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if !defined(PLATFORM_VOX_OPENGL_H_)
#define      PLATFORM_VOX_OPENGL_H_

#define GL_FALSE                                     0
#define GL_NO_ERROR                                  0
#define GL_TRIANGLES                            0x0004
#define GL_DEPTH_BUFFER_BIT                 0x00000100
#define GL_LESS                                 0x0201
#define GL_SRC_ALPHA                            0x0302
#define GL_ONE_MINUS_SRC_ALPHA                  0x0303
#define GL_INVALID_ENUM                         0x0500
#define GL_INVALID_VALUE                        0x0501
#define GL_INVALID_OPERATION                    0x0502
#define GL_STACK_OVERFLOW                       0x0503
#define GL_STACK_UNDERFLOW                      0x0504
#define GL_OUT_OF_MEMORY                        0x0505
#define GL_CULL_FACE                            0x0B44
#define GL_DEPTH_TEST                           0x0B71
#define GL_BLEND                                0x0BE2
#define GL_SCISSOR_TEST                         0x0C11
#define GL_TEXTURE_2D                           0x0DE1
#define GL_DONT_CARE                            0x1100
#define GL_UNSIGNED_SHORT                       0x1403
#define GL_FLOAT                                0x1406
#define GL_UNSIGNED_BYTE                        0x1401
#define GL_SHORT                                0x1402
#define GL_UNSIGNED_SHORT                       0x1403
#define GL_UNSIGNED_INT                         0x1405
#define GL_RGBA                                 0x1908
#define GL_EXTENSIONS                           0x1F03
#define GL_NEAREST                              0x2600
#define GL_LINEAR                               0x2601
#define GL_NEAREST_MIPMAP_NEAREST               0x2700
#define GL_LINEAR_MIPMAP_NEAREST                0x2701
#define GL_NEAREST_MIPMAP_LINEAR                0x2702
#define GL_LINEAR_MIPMAP_LINEAR                 0x2703
#define GL_TEXTURE_MAG_FILTER                   0x2800
#define GL_TEXTURE_MIN_FILTER                   0x2801
#define GL_TEXTURE_WRAP_S                       0x2802
#define GL_TEXTURE_WRAP_T                       0x2803
#define GL_CLAMP                                0x2900
#define GL_REPEAT                               0x2901
#define GL_COLOR_BUFFER_BIT                 0x00004000
#define GL_BGRA                                 0x80E1
#define GL_CLAMP_TO_EDGE                        0x812F
#define GL_NUM_EXTENSIONS                       0x821D
#define GL_DEBUG_OUTPUT_SYNCHRONOUS             0x8242
#define GL_DEBUG_NEXT_LOGGED_MESSAGE_LENGTH     0x8243
#define GL_DEBUG_SOURCE_API                     0x8246
#define GL_DEBUG_SOURCE_WINDOW_SYSTEM           0x8247
#define GL_DEBUG_SOURCE_SHADER_COMPILER         0x8248
#define GL_DEBUG_SOURCE_THIRD_PARTY             0x8249
#define GL_DEBUG_SOURCE_APPLICATION             0x824A
#define GL_DEBUG_SOURCE_OTHER                   0x824B
#define GL_DEBUG_TYPE_ERROR                     0x824C
#define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR       0x824D
#define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR        0x824E
#define GL_DEBUG_TYPE_PORTABILITY               0x824F
#define GL_DEBUG_TYPE_PERFORMANCE               0x8250
#define GL_DEBUG_TYPE_OTHER                     0x8251
#define GL_DEBUG_TYPE_MARKER                    0x8268
#define GL_DEBUG_TYPE_PUSH_GROUP                0x8269
#define GL_DEBUG_TYPE_POP_GROUP                 0x826A
#define GL_DEBUG_SEVERITY_NOTIFICATION          0x826B
#define GL_TEXTURE0                             0x84C0
#define GL_ACTIVE_TEXTURE                       0x84E0
#define GL_BUFFER_SIZE                          0x8764
#define GL_ARRAY_BUFFER                         0x8892
#define GL_ELEMENT_ARRAY_BUFFER                 0x8893
#define GL_ARRAY_BUFFER_BINDING                 0x8894
#define GL_STATIC_DRAW                          0x88E4
#define GL_DYNAMIC_DRAW                         0x88E8
#define GL_COMPILE_STATUS                       0x8B81
#define GL_FRAGMENT_SHADER                      0x8B30
#define GL_VERTEX_SHADER                        0x8B31
#define GL_LINK_STATUS                          0x8B82
#define GL_INFO_LOG_LENGTH                      0x8B84
#define GL_TEXTURE_2D_ARRAY                     0x8C1A
#define GL_SRGB8_ALPHA8                         0x8C43
#define GL_MAX_TEXTURE_SIZE                     0x0D33
#define GL_INT_2_10_10_10_REV                   0x8D9F
#define GL_SHADER_STORAGE_BUFFER                0x90D2
#define GL_DEBUG_SEVERITY_HIGH                  0x9146
#define GL_DEBUG_SEVERITY_MEDIUM                0x9147
#define GL_DEBUG_SEVERITY_LOW                   0x9148
#define GL_DEBUG_OUTPUT                         0x92E0


#if defined(_VOX_DEBUG)
#define GL_DEBUG_PROCS \
    PROC(void, DebugMessageCallback, _type__GL_DebugCallback *Callback, vptr UserParam) \
    PROC(void, DebugMessageControl, u32 Source, u32 Type, u32 Severity, s32 Count, u32 *IDs, b08 Enabled) \
    PROC(u32,  GetDebugMessageLog, u32 Count, s32 BufSize, u32 *Sources, u32 *Types, u32 *IDs, u32 *Severities, s32 *Lengths, chr *MessageLog) \

#define PLATFORM_FUNCS_GL_DEBUG \
    PROC(void _API_ENTRY, GL_DebugCallback, u32 Source, u32 Type, u32 ID, u32 Severity, s32 Length, chr *Message, vptr UserParam) \

#else
    #define GL_DEBUG_PROCS
    #define PLATFORM_FUNCS_GL_DEBUG
#endif

#define GL_PROCS \
    GL_DEBUG_PROCS \
    \
    PROC(s32, GetUniformLocation, u32 Program, chr *Name) \
    PROC(u32, CreateShader, u32 ShaderType) \
    PROC(u32, CreateProgram, void) \
    PROC(void, ActiveTexture, u32 Texture) \
    PROC(void, AttachShader, u32 Program, u32 Shader) \
    PROC(void, BindBuffer, u32 Target, u32 Buffer) \
    PROC(void, BindBufferBase, u32 Target, u32 Index, u32 Buffer) \
    PROC(void, BindVertexArray, u32 Array) \
    PROC(void, BufferData, u32 Target, s64 Size, vptr Data, u32 Usage) \
    PROC(void, BufferSubData, u32 Target, s64 Offset, s64 Size, vptr Data) \
    PROC(void, CompileShader, u32 Shader) \
    PROC(void, DeleteShader, u32 Shader) \
    PROC(void, DisableVertexAttribArray, u32 Index) \
    PROC(void, EnableVertexAttribArray, u32 Index) \
    PROC(void, GenBuffers, s32 N, u32 *Buffers) \
    PROC(void, GenerateMipmap, u32 Target) \
    PROC(void, GenVertexArrays, s32 N, u32 *Arrays) \
    PROC(void, GetBufferParameteriv, u32 Target, u32 Value, s32 *Data) \
    PROC(void, GetProgramInfoLog, u32 Program, s32 MaxLength, s32 *Length, chr *InfoLog) \
    PROC(void, GetProgramiv, u32 Program, u32 Name, s32 *Params) \
    PROC(void, GetShaderInfoLog, u32 Shader, s32 MaxLength, s32 *Length, chr *InfoLog) \
    PROC(void, GetShaderiv, u32 Shader, u32 Name, s32 *Params) \
    PROC(void, LinkProgram, u32 Program) \
    PROC(void, MultiDrawElementsBaseVertex, u32 Mode, s32 *Counts, u32 Type, vptr *IndexOffsets, s32 DrawCount, s32 *BaseVertices) \
    PROC(void, ShaderSource, u32 Shader, s32 Count, chr **String, s32 *Length) \
    PROC(void, UniformMatrix4fv, s32 Location, s32 Count, s08 Transpose, r32 *Value) \
    PROC(void, UseProgram, u32 Program) \
    PROC(void, Uniform1i, s32 Location, s32 V0) \
    PROC(void, Uniform1ui, s32 Location, u32 V0) \
    PROC(void, Uniform3f, s32 Location, r32 V0, r32 V1, r32 V2) \
    PROC(void, Uniform4ui, s32 Location, u32 V0, u32 V1, u32 V2, u32 V3) \
    PROC(void, VertexAttribPointer, u32 Index, s32 Size, u32 Type, b08 Normalized, s32 Stride, vptr Offset) \
    PROC(void, TexImage3D, u32 Target, s32 Level, s32 InternalFormat, s32 Width, s32 Height, s32 Depth, s32 Border, u32 Format, u32 Type, vptr Pixels) \

#define GL_PLATFORM_PROCS \
    PROC(b08, Viewport, s32 X, s32 Y, s32 Width, s32 Height) \
    PROC(u32, GetError, void) \
    PROC(void, BindTexture, u32 Target, u32 Texture) \
    PROC(void, BlendFunc, u32 SFactor, u32 DFactor) \
    PROC(void, Clear, u32 Mask) \
    PROC(void, ClearColor, r32 Red, r32 Green, r32 Blue, r32 Alpha) \
    PROC(void, Disable, u32 Cap) \
    PROC(void, DrawArrays, u32 Mode, s32 First, s32 Count) \
    PROC(void, DrawElements, u32 Mode, s32 Count, u32 Type, vptr IndexOffset) \
    PROC(void, Enable, u32 Cap) \
    PROC(void, GenTextures, s32 N, u32 *Textures) \
    PROC(void, GetIntegerv, u32 Name, s32 *Data) \
    PROC(void, Scissor, s32 x, s32 y, s32 width, s32 height) \
    PROC(void, TexImage2D, u32 Target, s32 Level, s32 InternalFormat, s32 Width, s32 Height, s32 Border, u32 Format, u32 Type, vptr Pixels) \
    PROC(void, TexParameteri, u32 Target, u32 Name, s32 Param) \

#define PLATFORM_FUNCS_GL \
    PLATFORM_FUNCS_GL_DEBUG \

#define PROC(ReturnType, Name, ...) \
    typedef ReturnType _type__##Name(__VA_ARGS__);
PLATFORM_FUNCS_GL
#undef PROC

#define PROC(ReturnType, Name, ...) \
    typedef ReturnType _type__GL_##Name(__VA_ARGS__); \
    global_var _type__GL_##Name *GL_##Name;
GL_PROCS
GL_PLATFORM_PROCS
#undef PROC

#if _VOX_MODULE != _VOX_MODULE_PLATFORM
    #define PROC(ReturnType, Name, ...) \
        global_var _type__##Name *Name;
    PLATFORM_FUNCS_GL
    #undef PROC
#endif

#endif