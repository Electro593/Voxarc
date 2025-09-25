/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
*                                                                             *
*  Author: Aria Seiler                                                        *
*                                                                             *
*  This program is in the public domain. There is no implied warranty, so     *
*  use it at your own risk.                                                   *
*                                                                             *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef INCLUDE_HEADER

#define GL_ZERO                   0x0000
#define GL_ONE                    0x0001
#define GL_LINES                  0x0001
#define GL_LINE_LOOP              0x0002
#define GL_TRIANGLES              0x0004
#define GL_SRC_COLOR              0x0300
#define GL_SRC_ALPHA              0x0302
#define GL_ONE_MINUS_SRC_ALPHA    0x0303
#define GL_DST_COLOR              0x0306
#define GL_ONE_MINUS_DST_COLOR    0x0307
#define GL_FRONT                  0x0404
#define GL_BACK                   0x0405
#define GL_FRONT_AND_BACK         0x0408

typedef enum opengl_error {
	OPENGL_ERROR_INVALID_ENUM	   = 0x0500,
	OPENGL_ERROR_INVALID_VALUE	   = 0x0501,
	OPENGL_ERROR_INVALID_OPERATION = 0x0502,
	OPENGL_ERROR_STACK_OVERFLOW	   = 0x0503,
	OPENGL_ERROR_STACK_UNDERFLOW   = 0x0504,
	OPENGL_ERROR_OUT_OF_MEMORY	   = 0x0505,
} opengl_error;

#define GL_LINE_SMOOTH            0x0B20
#define GL_SMOOTH_LINE_WIDTH_RANGE 0x0B22
#define GL_CULL_FACE              0x0B44
#define GL_DEPTH_TEST             0x0B71
#define GL_BLEND                  0x0BE2
#define GL_COLOR_LOGIC_OP         0x0BF2
#define GL_SCISSOR_TEST           0x0C11
#define GL_DONT_CARE              0x1100
#define GL_UNSIGNED_BYTE          0x1401
#define GL_SHORT                  0x1402
#define GL_UNSIGNED_SHORT         0x1403
#define GL_UNSIGNED_INT           0x1405
#define GL_FLOAT                  0x1406
#define GL_COPY                   0x1503
#define GL_INVERT                 0x150A
#define GL_RGBA                   0x1908
#define GL_LINE                   0x1B01
#define GL_FILL                   0x1B02
#define GL_NEAREST                0x2600
#define GL_LINEAR                 0x2601
#define GL_TEXTURE_MAG_FILTER     0x2800
#define GL_TEXTURE_MIN_FILTER     0x2801
#define GL_TEXTURE_WRAP_S         0x2802
#define GL_TEXTURE_WRAP_T         0x2803
#define GL_UNSIGNED_INT_8_8_8_8   0x8035
#define GL_CLAMP_TO_EDGE          0x812F
#define GL_R32UI                  0x8236

typedef enum opengl_debug_source {
	OPENGL_DEBUG_SOURCE_API				= 0x8246,
	OPENGL_DEBUG_SOURCE_WINDOW_SYSTEM	= 0x8247,
	OPENGL_DEBUG_SOURCE_SHADER_COMPILER = 0x8248,
	OPENGL_DEBUG_SOURCE_THIRD_PARTY		= 0x8249,
	OPENGL_DEBUG_SOURCE_APPLICATION		= 0x824A,
	OPENGL_DEBUG_SOURCE_OTHER			= 0x824B,
} opengl_debug_source;

typedef enum opengl_debug_type {
	OPENGL_DEBUG_TYPE_ERROR				  = 0x824C,
	OPENGL_DEBUG_TYPE_DEPRECATED_BEHAVIOR = 0x824D,
	OPENGL_DEBUG_TYPE_UNDEFINED_BEHAVIOR  = 0x824E,
	OPENGL_DEBUG_TYPE_PORTABILITY		  = 0x824F,
	OPENGL_DEBUG_TYPE_PERFORMANCE		  = 0x8250,
	OPENGL_DEBUG_TYPE_OTHER				  = 0x8251,
	OPENGL_DEBUG_TYPE_MARKER			  = 0x8268,
	OPENGL_DEBUG_TYPE_PUSH_GROUP		  = 0x8269,
	OPENGL_DEBUG_TYPE_POP_GROUP			  = 0x826A,
} opengl_debug_type;

typedef enum opengl_debug_severity {
	OPENGL_DEBUG_SEVERITY_NOTIFICATION = 0x826B,
	OPENGL_DEBUG_SEVERITY_HIGH		   = 0x9146,
	OPENGL_DEBUG_SEVERITY_MEDIUM	   = 0x9147,
	OPENGL_DEBUG_SEVERITY_LOW		   = 0x9148,
} opengl_debug_severity;

#define GL_ALIASED_LINE_WIDTH_RANGE 0x846E
#define GL_TEXTURE0               0x84C0
#define GL_BUFFER_SIZE            0x8764
#define GL_ARRAY_BUFFER           0x8892
#define GL_ELEMENT_ARRAY_BUFFER   0x8893
#define GL_STATIC_DRAW            0x88E4
#define GL_STATIC_READ            0x88E5
#define GL_DYNAMIC_DRAW           0x88E8
#define GL_FRAGMENT_SHADER        0x8B30
#define GL_VERTEX_SHADER          0x8B31
#define GL_COMPILE_STATUS         0x8B81
#define GL_LINK_STATUS            0x8B82
#define GL_INFO_LOG_LENGTH        0x8B84
#define GL_TEXTURE_2D_ARRAY       0x8C1A
#define GL_TEXTURE_BUFFER         0x8C2A
#define GL_RGBA32UI               0x8D70
#define GL_INT_2_10_10_10_REV     0x8D9F
#define GL_SHADER_STORAGE_BUFFER  0x90D2
#define GL_DEBUG_OUTPUT           0x92E0

#define GL_DEPTH_BUFFER_BIT   0x00000100
#define GL_STENICL_BUFFER_BIT 0x00000400
#define GL_COLOR_BUFFER_BIT   0x00004000

typedef void(API_ENTRY func_OpenGL_DebugProc)(
	u32	 Source,
	u32	 Type,
	u32	 ID,
	u32	 Severity,
	s32	 Length,
	c08 *Message,
	vptr UserParam
);

#define OPENGL_FUNCS_TYPE_1 \
   IMPORT(void, BindTexture,   u32 Target, u32 Texture) \
   IMPORT(void, BlendFunc,     u32 SFactor, u32 DFactor) \
   IMPORT(void, Clear,         u32 Mask) \
   IMPORT(void, ClearColor,    r32 Red, r32 Green, r32 Blue, r32 Alpha) \
   IMPORT(void, CullFace,      u32 Mode) \
   IMPORT(void, Disable,       u32 Capability) \
   IMPORT(void, DrawArrays,    u32 Mode, s32 First, s32 Count) \
   IMPORT(void, DrawElements,  u32 Mode, s32 Count, u32 Type, vptr Indices) \
   IMPORT(void, Enable,        u32 Capability) \
   IMPORT(void, GetFloatv,     u32 Name, r32 *Data) \
   IMPORT(void, GenTextures,   s32 Count, u32 *Textures) \
   IMPORT(void, LineWidth,     r32 Width) \
   IMPORT(void, LogicOp,       u32 OpCode) \
   IMPORT(void, PolygonMode,   u32 Face, u32 Mode) \
   IMPORT(void, Scissor,       s32 X, s32 Y, s32 Width, s32 Height) \
   IMPORT(void, TexParameteri, u32 Target, u32 Name, s32 Param) \
   IMPORT(void, Viewport,      s32 X, s32 Y, s32 Width, s32 Height)

#define OPENGL_FUNCS_TYPE_2 \
   IMPORT(void, ActiveTexture,               u32 Texture) \
   IMPORT(void, AttachShader,                u32 Program, u32 Shader) \
   IMPORT(void, BindBuffer,                  u32 Target, u32 Buffer) \
   IMPORT(void, BindBufferBase,              u32 Target, u32 Index, u32 Buffer) \
   IMPORT(void, BindSampler,                 u32 Unit, u32 Sampler) \
   IMPORT(void, BindVertexArray,             u32 ArrayID) \
   IMPORT(void, BufferData,                  u32 Target, s64 Size, vptr Data, u32 Usage) \
   IMPORT(void, BufferSubData,               u32 Target, s64 Offset, s64 Size, vptr Data) \
   IMPORT(void, CompileShader,               u32 Shader) \
   IMPORT(u32,  CreateProgram,               void) \
   IMPORT(u32,  CreateShader,                u32 Type) \
   IMPORT(void, DebugMessageCallback,        func_OpenGL_DebugProc Callback, vptr UserParam) \
   IMPORT(void, DebugMessageControl,         u32 Source, u32 Type, u32 Severity, s32 Count, u32 *IDs, u08 Enabled) \
   IMPORT(void, DeleteBuffers,               u32 Count, u32 *Buffers) \
   IMPORT(void, DeleteProgram,               u32 Program) \
   IMPORT(void, DeleteShader,                u32 Shader) \
   IMPORT(void, DetachShader,                u32 Program, u32 Shader) \
   IMPORT(void, DrawElementsBaseVertex,      u32 Mode, s32 Count, u32 Type, vptr *Indices, s32 BaseVertex) \
   IMPORT(void, EnableVertexAttribArray,     u32 Index) \
   IMPORT(void, GenBuffers,                  s32 Count, u32 *BufferIDs) \
   IMPORT(void, GenSamplers,                 s32 Count, u32 *SamplerIDs) \
   IMPORT(void, GenVertexArrays,             s32 Count, u32 *ArrayIDs) \
   IMPORT(void, GetBufferParameteriv,        u32 Target, u32 Value, s32 *Data) \
   IMPORT(void, GetProgramiv,                u32 Program, u32 ParamName, s32 *Param) \
   IMPORT(void, GetProgramInfoLog,           u32 Program, s32 BufferSize, s32 *Length, c08 *InfoLog) \
   IMPORT(void, GetShaderiv,                 u32 Shader, u32 ParamName, s32 *Param) \
   IMPORT(void, GetShaderInfoLog,            u32 Shader, s32 BufferSize, s32 *Length, c08 *InfoLog) \
   IMPORT(s32,  GetUniformLocation,          u32 Program, c08 *Name) \
   IMPORT(void, LinkProgram,                 u32 Program) \
   IMPORT(void, MultiDrawArrays,             u32 Mode, s32 *First, s32 *Count, s32 PrimitiveCount) \
   IMPORT(void, MultiDrawElementsBaseVertex, u32 Mode, s32 *Count, u32 Type, vptr *Indices, s32 PrimitiveCount, s32 *BaseVertex) \
   IMPORT(void, TexBuffer,                   u32 Target, u32 InternalFormat, u32 Buffer) \
   IMPORT(void, TexImage3D,                  u32 Target, s32 Level, s32 InternalFormat, s32 Width, s32 Height, s32 Depth, s32 Border, u32 Format, u32 Type, vptr Pixels) \
   IMPORT(void, SamplerParameteri,           u32 Sampler, u32 Name, s32 Param) \
   IMPORT(void, ShaderSource,                u32 Shader, s32 Count, c08 **String, s32 *Length) \
   IMPORT(void, Uniform1f,                   s32 Location, r32 V0) \
   IMPORT(void, Uniform1i,                   s32 Location, s32 V0) \
   IMPORT(void, Uniform2ui,                  s32 Location, u32 V0, u32 V1) \
   IMPORT(void, Uniform3f,                   s32 Location, r32 V0, r32 V1, r32 V2) \
   IMPORT(void, Uniform4f,                   s32 Location, r32 V0, r32 V1, r32 V2, r32 V3) \
   IMPORT(void, UniformMatrix4fv,            s32 Location, u32 Count, b08 Transpose, m4x4r32 Matrix) \
   IMPORT(void, UseProgram,                  u32 Program) \
   IMPORT(void, VertexAttribPointer,         u32 Index, s32 Size, u32 Type, b08 Normalized, s32 Stride, vptr Offset) \
   IMPORT(void, VertexAttribIPointer,        u32 Index, s32 Size, u32 Type, s32 Stride, vptr Offset)

#define OPENGL_FUNCS OPENGL_FUNCS_TYPE_1 OPENGL_FUNCS_TYPE_2

#define IMPORT(ReturnType, Name, ...) \
   typedef ReturnType func_OpenGL_##Name(__VA_ARGS__);
#define X OPENGL_FUNCS
#include <x.h>

typedef struct opengl_funcs {
#define IMPORT(ReturnType, Name, ...) \
      func_OpenGL_##Name *OpenGL_##Name;
#define X OPENGL_FUNCS
#include <x.h>

	b08 Initialized;
} opengl_funcs;

#if defined(_RENDERER_MODULE)
#define IMPORT(ReturnType, Name, ...) \
      global func_OpenGL_##Name *OpenGL_##Name;
#define X OPENGL_FUNCS
#include <x.h>
#endif

#endif
