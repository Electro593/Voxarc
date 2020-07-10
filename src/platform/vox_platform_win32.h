#ifndef VOX_PLATFORM_WIN32_H_

#include "util/vox_defines.h"
#include "math/vox_v2s32.h"

#define WINAPIENTRY __stdcall
#define VOXAPIENTRY WINAPIENTRY
#define GLAPIENTRY WINAPIENTRY

#define WINAPI __declspec(dllimport)
#define VOXAPI WINAPI
#define GLAPI WINAPI

#define HIGH16(S) ((s32)(s16)((((u64)(S)) >> 16) & 0xffff))
#define LOW16(S) ((s32)(s16)(((u64)(S)) & 0xffff))

#define IDC_ARROW ((c08*)((u64)((u16)(32512))))

#define CW_USEDEFAULT ((s32)0x80000000)

#define INVALID_HANDLE_VALUE ((vptr)(s64)-1)

#define CS_VREDRAW              0x0001
#define CS_HREDRAW              0x0002
#define CS_OWNDC                0x0020

#define WM_DESTROY              0x0002
#define WM_MOVE                 0x0003
#define WM_ACTIVATE             0x0006
#define WM_SETFOCUS             0x0007
#define WM_KILLFOCUS            0x0008
#define WM_CLOSE                0x0010
#define WM_QUIT                 0x0012
#define WM_ERASEBKGND           0x0014
#define WM_MOUSEACTIVATE        0x0021
#define WM_KEYDOWN              0x0100
#define WM_KEYUP                0x0101
#define WM_SYSKEYDOWN           0x0104
#define WM_SYSKEYUP             0x0105
#define WM_SYSCOMMAND           0x0112
#define WM_MOUSEMOVE            0x0200
#define WM_LBUTTONDOWN          0x0201
#define WM_LBUTTONUP            0x0202
#define WM_RBUTTONDOWN          0x0204
#define WM_RBUTTONUP            0x0205
#define WM_MBUTTONDOWN          0x0207
#define WM_MBUTTONUP            0x0208
#define WM_XBUTTONDOWN          0x020B
#define WM_XBUTTONUP            0x020C
#define WM_SIZING               0x0214
#define WM_CAPTURECHANGED       0x0215

#define SC_KEYMENU              0xF100

#define WA_ACTIVE                    1

#define WS_OVERLAPPED       0x00000000L
#define WS_MAXIMIZEBOX      0x00010000L
#define WS_MINIMIZEBOX      0x00020000L
#define WS_THICKFRAME       0x00040000L
#define WS_SYSMENU          0x00080000L
#define WS_CAPTION          0x00C00000L
#define WS_VISIBLE          0x10000000L

#define MEM_COMMIT          0x00001000
#define MEM_RESERVE         0x00002000
#define MEM_RELEASE         0x00008000

#define PAGE_READWRITE            0x04

#define MAX_PATH                   260

#define OPEN_EXISTING                3

#define GENERIC_READ        0x80000000L

#define FILE_SHARE_READ     0x00000001

#define PFD_TYPE_RGBA                0
#define PFD_MAIN_PLANE               0
#define PFD_DOUBLEBUFFER    0x00000001
#define PFD_DRAW_TO_WINDOW  0x00000004
#define PFD_SUPPORT_OPENGL  0x00000020

#define PM_REMOVE               0x0001

#define HTCLIENT                     1

typedef s64 (WINAPIENTRY *window_callback)(vptr, u32, u64, s64);
typedef struct win32_window_class
{
    u32 Style;
    window_callback WindowCallback;
    s32 WindowClassExtraBytes;
    s32 WindowInstanceExtraBytes;
    vptr Instance;
    vptr Icon;
    vptr Cursor;
    vptr Background;
    cc08 *MenuName;
    cc08 *ClassName;
} win32_window_class;
typedef struct win32_rect
{
    s32 Left;
    s32 Top;
    s32 Right;
    s32 Bottom;
} win32_rect;
typedef struct win32_point
{
    s32 X;
    s32 Y;
} win32_point;
typedef struct win32_overlapped
{
    u64 Internal;
    u64 InternalHigh;
    union
    {
        struct
        {
            u32 Offset;
            u32 OffsetHigh;
        };
        vptr Pointer;
    };
    vptr Event;
} win32_overlapped;
typedef union win32_large_integer
{
    struct {
        u32 LowPart;
        s32 HighPart;
    };
    struct {
        u32 LowPart;
        s32 HighPart;
    } U;
    s64 QuadPart;
} win32_large_integer;
typedef struct win32_security_attributes
{
    u32 Length;
    vptr SecurityDescriptor;
    b08 InheritHandle;
} win32_security_attributes;
typedef struct win32_pixel_format_descriptor
{
    u16 Size;
    u16 Version;
    u32 Flags;
    u08 PixelType;
    u08 ColorBits;
    u08 RedBits;
    u08 RedShift;
    u08 GreenBits;
    u08 GreenShift;
    u08 BlueBits;
    u08 BlueShift;
    u08 AlphaBits;
    u08 AlphaShift;
    u08 AccumBits;
    u08 AccumRedBits;
    u08 AccumGreenBits;
    u08 AccumBlueBits;
    u08 AccumAlphaBits;
    u08 DepthBits;
    u08 StencilBits;
    u08 AuxBuffers;
    u08 LayerType;
    u08 Reserved;
    u32 LayerMask;
    u32 VisibleMask;
    u32 DamageMask;
} win32_pixel_format_descriptor;
typedef struct win32_list_entry
{
    struct win32_list_entry *Next;
    struct win32_list_entry *Prev;
} win32_list_entry;
typedef struct win32_message
{
    vptr Window;
    u32 Message;
    u64 WParam;
    s64 LParam;
    u32 Time;
    win32_point Point;
} win32_message;
typedef struct win32_filetime
{
    u32 LowDateTime;
    u32 HighDateTime;
} win32_filetime;
typedef struct win32_find_data
{
    u32 FileAttributes;
    win32_filetime CreationTime;
    win32_filetime LastAccessTime;
    win32_filetime LastWriteTime;
    u32 FileSizeHigh;
    u32 FileSizeLow;
    u32 Reserved0;
    u32 Reserved1;
    c08 FileName[MAX_PATH];
    c08 AlternateFileName[14];
} win32_find_data;

#define WIN32_PROCS \
    PROCM(Gdi32,    b08,  SetPixelFormat,             vptr DeviceContext, s32 Format, const win32_pixel_format_descriptor *PixelFormatDescriptor) \
    PROCM(Gdi32,    b08,  SwapBuffers,                vptr DeviceContext) \
    PROCM(Gdi32,    s32,  ChoosePixelFormat,          vptr DeviceContext, win32_pixel_format_descriptor *PixelFormatDescriptor) \
    PROCM(Gdi32,    s32,  DescribePixelFormat,        vptr DeviceContext, s32 PixelFormat, u32 BytesCount, win32_pixel_format_descriptor *PixelFormatDescriptor) \
    PROCM(Kernel32, b08,  CloseHandle,                vptr Object) \
    PROCM(Kernel32, b08,  FindClose,                  vptr FindFile) \
    PROCM(Kernel32, b08,  FindNextFileA,              vptr FindFile, win32_find_data *FindFileData) \
    PROCM(Kernel32, b08,  GetFileSizeEx,              vptr File, win32_large_integer *FileSize) \
    PROCM(Kernel32, b08,  QueryPerformanceCounter,    win32_large_integer *PerformanceCount) \
    PROCM(Kernel32, b08,  ReadFile,                   vptr File, vptr Buffer, u32 NumberOfBytesToRead, u32 *NumberOfBytesRead, win32_overlapped *OVerlapped) \
    PROCM(Kernel32, void, ExitProcess,                u32 ExitCode) \
    PROCM(Kernel32, void, OutputDebugStringA,         cc08 *OutputString) \
    PROCM(Kernel32, vptr, CreateFileA,                cc08 *FileName, u32 DesiredAccess, u32 ShareMode, win32_security_attributes *SecurityAttributes, u32 CreationDisposition, u32 FlagsAndAttributes, vptr TemplateFile) \
    PROCM(Kernel32, vptr, FindFirstFileA,             c08 *FileName, win32_find_data *FindFileData) \
    PROCM(Kernel32, vptr, GetModuleHandleA,           cc08 *ModuleName) \
    PROCM(Kernel32, vptr, VirtualAlloc,               vptr Address, size Size, u32 AllocationType, u32 Protect) \
    PROCM(Kernel32, b08,  VirtualFree,                vptr Address, size Size, u32 FreeType) \
    PROCM(Opengl32, b08,  wglDeleteContext,           vptr RenderContext) \
    PROCM(Opengl32, b08,  wglMakeCurrent,             vptr DeviceContext, vptr RenderContext) \
    PROCM(Opengl32, vptr, wglCreateContext,           vptr DeviceContext) \
    PROCM(Opengl32, vptr, wglGetProcAddress,          cc08 *Process) \
    PROCM(Opengl32, b08,  glViewport,                 s32 X, s32 Y, s32 Width, s32 Height) \
    PROCM(Opengl32, void, glTexParameteri,            u32 Target, u32 Name, s32 Param) \
    PROCM(Opengl32, void, glTexImage2D,               u32 Target, s32 Level, s32 InternalFormat, s32 Width, s32 Height, s32 Border, u32 Format, u32 Type, const vptr Data) \
    PROCM(Opengl32, void, glClearColor,               r32 Red, r32 Green, r32 Blue, r32 Alpha) \
    PROCM(Opengl32, void, glClear,                    u32 Mask) \
    PROCM(Opengl32, void, glEnable,                   u32 Cap) \
    PROCM(Opengl32, void, glGetIntegerv,              u32 Name, s32 *Data) \
    PROCM(User32,   b08,  ClientToScreen,             vptr Window, win32_point *Point) \
    PROCM(User32,   b08,  DestroyWindow,              vptr Window) \
    PROCM(User32,   b08,  GetClientRect,              vptr Window, win32_rect *Rect) \
    PROCM(User32,   b08,  GetCursorPos,               win32_point *Point) \
    PROCM(User32,   b08,  PeekMessageA,               win32_message *Message, vptr Window, u32 MessageFilterMin, u32 MessageFilterMax, u32 RemoveMessage) \
    PROCM(User32,   b08,  ScreenToClient,             vptr Window, win32_point *Point) \
    PROCM(User32,   b08,  SetCursorPos,               s32 X, s32 Y) \
    PROCM(User32,   b08,  TranslateMessage,           const win32_message *Message) \
    PROCM(User32,   s16,  GetKeyState,                s32 VirtualKey) \
    PROCM(User32,   s32,  ReleaseDC,                  vptr Window, vptr DeviceContext) \
    PROCM(User32,   s32,  ShowCursor,                 b08 Show) \
    PROCM(User32,   s64,  DefWindowProcA,             vptr Window, u32 Message, u64 WParam, s64 LParam) \
    PROCM(User32,   s64,  DispatchMessageA,           const win32_message *Message) \
    PROCM(User32,   u16,  RegisterClassA,             win32_window_class *WindowClass) \
    PROCM(User32,   vptr, CreateWindowExA,            u32 ExtendedStyle, cc08 *ClassName, cc08 *WindowName, u32 Style, s32 X, s32 Y, s32 Width, s32 Height, vptr Parent, vptr Menu, vptr Instance, vptr Param) \
    PROCM(User32,   vptr, GetDC,                      vptr Window) \
    PROCM(User32,   vptr, LoadCursorA,                vptr Instance, cc08 *CursorName) \
    PROCM(User32,   vptr, SetCursor,                  vptr Cursor) \
    PROCM(User32,   b08,  ClipCursor,                 const win32_rect *Rect) \
    PROCM(User32,   b08,  SetPropA,                   vptr Window, cc08 *String, vptr Data) \
    PROCM(User32,   vptr, GetPropA,                   vptr Window, cc08 *String) \

#define PROCM(Module, ReturnType, Name, ...) \
    typedef WINAPI ReturnType WINAPIENTRY Name##proc(__VA_ARGS__); \
    global_var Name##proc *Name;
WIN32_PROCS
#undef PROCM

#define WGL_PROCS \
    PROC(vptr, wglCreateContextAttribsARB, vptr DeviceContext, vptr ShareContext, const s32 *AttribList) \
    PROC(b08,  wglChoosePixelFormatARB,    vptr DeviceContext, const s32 *AttribListI, const r32 *AttribListF, u32 MaxFormats, s32 *Formats, u32 *NumFormats) \
    PROC(b08,  wglSwapIntervalEXT,         s32 Num) \

#define PROC(ReturnType, Name, ...) \
    typedef GLAPI ReturnType GLAPIENTRY Name##proc(__VA_ARGS__); \
    global_var Name##proc *Name;
WGL_PROCS
#undef PROC

#define WGL_DRAW_TO_WINDOW_ARB                  0x2001
#define WGL_ACCELERATION_ARB                    0x2003
#define WGL_SUPPORT_OPENGL_ARB                  0x2010
#define WGL_DOUBLE_BUFFER_ARB                   0x2011
#define WGL_PIXEL_TYPE_ARB                      0x2013
#define WGL_COLOR_BITS_ARB                      0x2014
#define WGL_DEPTH_BITS_ARB                      0x2022
#define WGL_STENCIL_BITS_ARB                    0x2023
#define WGL_FULL_ACCELERATION_ARB               0x2027
#define WGL_TYPE_RGBA_ARB                       0x202B
#define WGL_SAMPLE_BUFFERS_ARB                  0x2041
#define WGL_SAMPLES_ARB                         0x2042
#define WGL_CONTEXT_MAJOR_VERSION_ARB           0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB           0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB             0x2093
#define WGL_CONTEXT_FLAGS_ARB                   0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB            0x9126
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB  0x0002
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB        0x00000001

typedef struct win32_data
{
    vptr Window;
    win32_rect WindowRect;
    game_state State;
    game_input Input;
    platform_flags Flags;
    v2s32 PrevCursorPos;
} win32_data;

typedef struct win32_ldr_data_entry
{
    win32_list_entry LinkedList;
    win32_list_entry UnusedList;
    vptr BaseAddress;
    vptr Reserved2[1];
    vptr DllBase;
    vptr EntryPoint;
    vptr Reserved3;
    u16 DllNameLength;
    u16 DllNameMaximumLength;
    u16 *DllNameBuffer;
} win32_ldr_data_entry;

typedef struct win32_ldr_data
{
    u08 Padding1[0x20];
    win32_ldr_data_entry *LoaderDataEntry;
} win32_ldr_data;

typedef struct win32_peb
{
    u08 Padding1[0x18];
    win32_ldr_data *LoaderData;
} win32_peb;

typedef struct win32_teb
{
    u08 Padding1[0x60];
    win32_peb *PEB;
} win32_teb;

typedef struct win32_msdos
{
    u08 Padding1[0x3C];
    u32 PEOffset;
} win32_msdos;

typedef struct win32_pe_image_data
{
    u32 VirtualAddress;
    u32 Size;
} win32_pe_image_data;

typedef struct win32_pe
{
    u08 Signature[4];
    u16 Machine;
    u16 NumberOfSections;
    u32 TimeDateStamp;
    u32 PointerToSymbolTable;
    u32 NumberOfSymbols;
    u16 SizeOfOptionalHeader;
    u16 Characteristics;
    
    u16 Magic; 
    u08 MajorLinkerVersion;
    u08 MinorLinkerVersion;
    u32 SizeOfCode;
    u32 SizeOfInitializedData;
    u32 SizeOfUninitializedData;
    u32 AddressOfEntryPoint;
    u32 BaseOfCode;
    
    u64 ImageBase;
    u32 SectionAlignment;
    u32 FileAlignment;
    u16 MajorOperatingSystemVersion;
    u16 MinorOperatingSystemVersion;
    u16 MajorImageVersion;
    u16 MinorImageVersion;
    u16 MajorSubsystemVersion;
    u16 MinorSubsystemVersion;
    u32 Win32VersionValue;
    u32 SizeOfImage;
    u32 SizeOfHeaders;
    u32 CheckSum;
    u16 Subsystem;
    u16 DllCharacteristics;
    u64 SizeOfStackReserve;
    u64 SizeOfStackCommit;
    u64 SizeOfHeapReserve;
    u64 SizeOfHeapCommit;
    u32 LoaderFlags;
    u32 NumberOfRvaAndSizes;
    
    win32_pe_image_data ExportTable;
    win32_pe_image_data ImportTable;
    win32_pe_image_data ResourceTable;
    win32_pe_image_data ExceptionTable;
    win32_pe_image_data CertificateTable;
    win32_pe_image_data BaseRelocationTable;
    win32_pe_image_data Debug;
    win32_pe_image_data Architecture;
    win32_pe_image_data GlobalPtr;
    win32_pe_image_data TLSTable;
    win32_pe_image_data LoadConfigTable;
    win32_pe_image_data BoundImport;
    win32_pe_image_data IAT;
    win32_pe_image_data DelayImportDescriptor;
    win32_pe_image_data CLRRuntimeHeader;
    win32_pe_image_data ReservedTable;
} win32_pe;

typedef struct win32_pe_export_table
{
    u32 ExportFlags;
    u32 TimeDateStamp;
    u16 MajorVersion;
    u16 MinorVersion;
    u32 NameRVA;
    u32 OrdinalBase;
    u32 AddressTableEntries;
    u32 NumberofNamePointers;
    u32 ExportAddressTableRVA;
    u32 NamePointerRVA;
    u32 OrdinalTableRVA;
} win32_pe_export_table;

#define VOX_PLATFORM_WIN32_H_
#endif