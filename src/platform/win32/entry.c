/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <shared.h>

global platform_state *Platform;

internal void
Platform_LoadWin32(void)
{
    win32_teb *TEB = (win32_teb*)Asm_ReadGSQWord(48);
    win32_list_entry *Entry = TEB->PEB->Ldr->MemoryOrderList.Next;
    u32 Offset = OFFSET_OF(win32_ldr_data_table_entry, MemoryOrderLinks);
    Entry = Entry->Next->Next;
    win32_ldr_data_table_entry *TableEntry = (win32_ldr_data_table_entry*)((u08*)Entry - OFFSET_OF(win32_ldr_data_table_entry, MemoryOrderLinks));
    win32_module Kernel32 = TableEntry->DllBase;
    
    win32_image_dos_header *DOSHeader = (win32_image_dos_header*)Kernel32;
    win32_image_nt_headers *NTHeaders = (win32_image_nt_headers*)((u08*)DOSHeader + DOSHeader->e_lfanew);
    win32_image_export_directory *ExportDirectory = (win32_image_export_directory*)((u08*)DOSHeader + NTHeaders->OptionalHeader.ExportTable.VirtualAddress);
    u32 *ExportNameTable    = (u32*)((u08*)DOSHeader + ExportDirectory->AddressOfNames);
    u16 *ExportOrdinalTable = (u16*)((u08*)DOSHeader + ExportDirectory->AddressOfNameOrdinals);
    u32 *ExportAddressTable = (u32*)((u08*)DOSHeader + ExportDirectory->AddressOfFunctions);
    
    u32 Index;
    u32 Low = 0;
    u32 High = ExportDirectory->NumberOfNames - 1;
    while(TRUE) {
        Index = (High + Low) / 2;
        c08 *Goal = "GetProcAddress";
        c08 *Curr = (c08*)((u08*)DOSHeader + ExportNameTable[Index]);
        
        while(*Goal && *Curr && *Goal == *Curr) {
            Goal++;
            Curr++;
        }
        
        if(*Goal == *Curr) break;
        if(*Goal > *Curr) Low = Index;
        else High = Index;
    }
    
    u16 GetProcAddressOrdinal = ExportOrdinalTable[Index];
    u32 GetProcAddressRVA = ExportAddressTable[GetProcAddressOrdinal];
    Win32_GetProcAddress = (func_Win32_GetProcAddress*)((u08*)DOSHeader + GetProcAddressRVA);
    
    Win32_LoadLibraryA = (func_Win32_LoadLibraryA*)Win32_GetProcAddress(Kernel32, "LoadLibraryA");
    win32_module Gdi32 = Win32_LoadLibraryA("gdi32.dll");
    win32_module User32 = Win32_LoadLibraryA("user32.dll");
    
    #define IMPORT(Module, ReturnType, Name, ...) \
        Win32_##Name = (func_Win32_##Name*)Win32_GetProcAddress(Module, #Name); \
        Assert(Win32_##Name);
    #define X WIN32_FUNCS
    #include <x.h>
}

internal void
Platform_LoadWGL(void)
{
    win32_module OpenGL32 = Win32_LoadLibraryA("opengl32.dll");
    #define IMPORT(ReturnType, Name, ...) \
        WGL_##Name = (func_WGL_##Name*)Win32_GetProcAddress(OpenGL32, "wgl" #Name); \
        Assert(WGL_##Name);
    #define X WGL_FUNCS_TYPE_1
    #include <x.h>
    
    win32_window_class_a DummyWindowClass = {0};
    DummyWindowClass.Callback = (func_Win32_WindowCallback)Win32_DefWindowProcA;
    DummyWindowClass.Instance = Win32_GetModuleHandleA(NULL);
    DummyWindowClass.ClassName = "VoxarcDummyWindowClass";
    
    Win32_RegisterClassA(&DummyWindowClass);
    win32_window DummyWindow = Win32_CreateWindowExA(
        0, DummyWindowClass.ClassName, "VoxarcDummyWindow", 0,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, DummyWindowClass.Instance, NULL
    );
    
    vptr DummyDeviceContext = Win32_GetDC(DummyWindow);
    
    win32_pixel_format_descriptor PixelFormatDescriptor = {0};
    PixelFormatDescriptor.Size = sizeof(win32_pixel_format_descriptor);
    PixelFormatDescriptor.Version = 1;
    PixelFormatDescriptor.PixelType = PFD_TYPE_RGBA;
    PixelFormatDescriptor.Flags = PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLE_BUFFER;
    PixelFormatDescriptor.ColorBits = 32;
    PixelFormatDescriptor.AlphaBits = 8;
    PixelFormatDescriptor.LayerType = PFD_MAIN_PLANE;
    PixelFormatDescriptor.DepthBits = 24;
    PixelFormatDescriptor.StencilBits = 8;
    
    s32 PixelFormat = Win32_ChoosePixelFormat(DummyDeviceContext, &PixelFormatDescriptor);
    Win32_SetPixelFormat(DummyDeviceContext, PixelFormat, &PixelFormatDescriptor);
    vptr DummyRenderContext = WGL_CreateContext(DummyDeviceContext);
    WGL_MakeCurrent(DummyDeviceContext, DummyRenderContext);
    
    #define IMPORT(ReturnType, Name, ...) \
        WGL_##Name = (func_WGL_##Name*)WGL_GetProcAddress("wgl" #Name); \
        Assert(WGL_##Name);
    #define X WGL_FUNCS_TYPE_2
    #include <x.h>
    
    WGL_MakeCurrent(DummyDeviceContext, 0);
    WGL_DeleteContext(DummyRenderContext);
    Win32_ReleaseDC(DummyWindow, DummyDeviceContext);
    Win32_DestroyWindow(DummyWindow);
}

internal opengl_funcs
Platform_LoadOpenGL(win32_device_context DeviceContext)
{
    s32 PixelFormatAttribs[] = {
        WGL_DRAW_TO_WINDOW_ARB, TRUE,
        WGL_SUPPORT_OPENGL_ARB, TRUE,
        WGL_DOUBLE_BUFFER_ARB,  TRUE,
        WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
        WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB,     32,
        WGL_DEPTH_BITS_ARB,     24,
        WGL_STENCIL_BITS_ARB,   8,
        WGL_SAMPLE_BUFFERS_ARB, 1,
        WGL_SAMPLES_ARB,        4,
        0
    };
    
    win32_pixel_format_descriptor PixelFormatDescriptor;
    s32 PixelFormat;
    u32 FormatCount;
    WGL_ChoosePixelFormatARB(DeviceContext, PixelFormatAttribs, 0, 1, &PixelFormat, &FormatCount);
    Win32_DescribePixelFormat(DeviceContext, PixelFormat, sizeof(win32_pixel_format_descriptor), &PixelFormatDescriptor);
    
    #if defined(_DEBUG)
        u32 DebugBit = WGL_CONTEXT_DEBUG_BIT_ARB;
    #else
        u32 DebugBit = 0;
    #endif
    
    Win32_SetPixelFormat(DeviceContext, PixelFormat, &PixelFormatDescriptor);
    s32 AttribList[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 6,
        WGL_CONTEXT_FLAGS_ARB,         DebugBit | WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
        WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };
    
    win32_opengl_render_context RenderContext = WGL_CreateContextAttribsARB(DeviceContext, 0, AttribList);
    WGL_MakeCurrent(DeviceContext, RenderContext);
    
    win32_handle OpenGL32 = Win32_GetModuleHandleA("opengl32.dll");
    Assert(OpenGL32);
    
    opengl_funcs Funcs;
    
    #define IMPORT(ReturnType, Name, ...) \
        Funcs.OpenGL_##Name = (func_OpenGL_##Name*)Win32_GetProcAddress(OpenGL32, "gl" #Name); \
        Assert(Funcs.OpenGL_##Name);
    #define X OPENGL_FUNCS_TYPE_1
    #include <x.h>
    
    #define IMPORT(ReturnType, Name, ...) \
        Funcs.OpenGL_##Name = (func_OpenGL_##Name*)WGL_GetProcAddress("gl" #Name); \
        Assert(Funcs.OpenGL_##Name);
    #define X OPENGL_FUNCS_TYPE_2
    #include <x.h>
    
    return Funcs;
}

internal void
Platform_Assert(c08 *File,
                u32 Line,
                c08 *Expression,
                c08 *Message)
{
    c08 LineStr[11];
    u32 Index = sizeof(LineStr);
    LineStr[--Index] = 0;
    do {
        LineStr[--Index] = (Line % 10) + '0';
        Line /= 10;
    } while(Line);
    
    Win32_OutputDebugStringA(File);
    Win32_OutputDebugStringA(": ");
    Win32_OutputDebugStringA(LineStr + Index);
    if(Expression[0] != 0) {
        Win32_OutputDebugStringA(": Assertion hit!\n\t(");
        Win32_OutputDebugStringA(Expression);
        Win32_OutputDebugStringA(") was FALSE\n");
    } else {
        Win32_OutputDebugStringA(": \n");
    }
    if(Message[0] != 0) {
        Win32_OutputDebugStringA("\t");
        Win32_OutputDebugStringA(Message);
    }
    Win32_OutputDebugStringA("\n");
}

internal vptr
Platform_AllocateMemory(u64 Size)
{
    vptr MemoryBlock = Win32_VirtualAlloc(0, Size, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
    Assert(MemoryBlock);
    return MemoryBlock;
}

internal void
Platform_FreeMemory(vptr Base)
{
    Win32_VirtualFree(Base, 0, MEM_RELEASE);
}

internal u64
Platform_GetFileLength(file_handle FileHandle)
{
    win32_large_integer Size;
    Size.QuadPart = 0;
    
    Assert(Win32_GetFileSizeEx(FileHandle, &Size));
    
    return Size.QuadPart;
}

internal b08
Platform_OpenFile(file_handle *FileHandle,
                  c08 *FileName,
                  file_mode OpenMode)
{
    u32 DesiredAccess = 0;
    u32 CreationDisposition = 0;
    
    switch(OpenMode) {
        case FILE_READ: {
            DesiredAccess = GENERIC_READ;
            CreationDisposition = OPEN_EXISTING;
        } break;
        
        case FILE_WRITE: {
            DesiredAccess = GENERIC_WRITE;
            CreationDisposition = CREATE_ALWAYS;
        } break;
        
        case FILE_APPEND: {
            DesiredAccess = GENERIC_WRITE;
            CreationDisposition = CREATE_NEW;
        } break;
        
        default: {
            STOP;
        }
    }
    
    win32_handle GivenHandle = Win32_CreateFileA(FileName, DesiredAccess, FILE_SHARE_READ,
                                                 NULL, CreationDisposition, FILE_ATTRIBUTE_NORMAL, NULL);
    
    if(GivenHandle != INVALID_HANDLE_VALUE) {
        *(win32_handle*)FileHandle = GivenHandle;
        return TRUE;
    } else {
        *(win32_handle*)FileHandle = NULL;
    }
    
    return FALSE;
}

internal u64
Platform_ReadFile(file_handle FileHandle,
                  vptr Dest,
                  u64 Length,
                  u64 Offset)
{
    win32_overlapped Overlapped = {0};
    u64 *OverlappedOffset = (u64*)&Overlapped.Offset;
    *OverlappedOffset = Offset;
    
    u64 TotalBytesRead = 0;
    while(Length) {
        u32 BytesRead;
        u32 BytesToRead = Length % (U32_MAX+1ULL);
        b08 Success = Win32_ReadFile(FileHandle, Dest, BytesToRead, &BytesRead, &Overlapped);
        
        if(!Success) {
            u32 DEBUG_Err = Win32_GetLastError();
            Assert(Success, "Failed to read data from file");
            return 0;
        }
        
        TotalBytesRead += BytesRead;
        Length -= BytesRead;
        *OverlappedOffset += BytesRead;
    }
    
    return TotalBytesRead;
}

internal u64
Platform_WriteFile(file_handle FileHandle,
                   vptr Src,
                   u64 Length,
                   u64 Offset)
{
    win32_overlapped Overlapped = {0};
    u64 *OverlappedOffset = (u64*)&Overlapped.Offset;
    *OverlappedOffset = Offset;
    
    u32 TotalBytesWritten = 0;
    while(Length) {
        u32 BytesWritten;
        u32 BytesToWrite = Length % (U32_MAX+1ULL);
        b08 Success = Win32_WriteFile(FileHandle, Src, BytesToWrite, &BytesWritten, &Overlapped);
        
        if(!Success) {
            u32 DEBUG_Err = Win32_GetLastError();
            Assert(Success, "Failed to write data to file");
            return 0;
        }
        
        TotalBytesWritten += BytesWritten;
        Length -= BytesWritten;
        *OverlappedOffset += BytesWritten;
    }
    
    return TotalBytesWritten;
}

internal void
Platform_CloseFile(file_handle FileHandle)
{
    Win32_CloseHandle(FileHandle);
}

internal void
Platform_LoadGame(module *Module,
                  game_state *GameState,
                  platform_exports *PlatformExports,
                  opengl_funcs *OpenGLFuncs)
{
    file_handle FileHandle;
    win32_file_time LastWriteTime;
    Platform_OpenFile(&FileHandle, "build\\Voxarc_Game.dll", FILE_READ);
    Win32_GetFileTime(FileHandle, NULL, NULL, &LastWriteTime);
    Platform_CloseFile(FileHandle);
    if(Module->DLL) {
        Win32_GetFileTime(FileHandle, NULL, NULL, &Module->LastWriteTime);
        if(Win32_CompareFileTime(&Module->LastWriteTime, &LastWriteTime) >= 0)
            return;
        
        Game_Unload(GameState);
        Win32_FreeLibrary(Module->DLL);
    }
    Module->LastWriteTime = LastWriteTime;
    
    Win32_CopyFileA("build\\Voxarc_Game.dll", "build\\Voxarc_Game_Locked.dll", FALSE);
    Module->DLL = Win32_LoadLibraryA("build\\Voxarc_Game_Locked.dll");
    
    #define EXTERN(ReturnType, Namespace, Name, ...) \
        Namespace##_##Name = (func_##Namespace##_##Name*)Win32_GetProcAddress(Module->DLL, #Namespace "_" #Name);
    #define X GAME_FUNCS
    #include <x.h>
    
    game_exports GameExports;
    Game_Load(PlatformExports, &GameExports, GameState, OpenGLFuncs);
    
    #define EXPORT(ReturnType, Namespace, Name, ...) \
        Namespace##_##Name = GameExports.Namespace##_##Name;
    #define X GAME_FUNCS
    #include <x.h>
}

internal s64
Platform_WindowCallback(win32_window Window,
                        u32 Message,
                        s64 WParam,
                        s64 LParam)
{
    switch(Message) {
        case WM_DESTROY:
        case WM_CLOSE: {
            Platform->ExecutionState = EXECUTION_ENDED;
        } return 0;
        
        case WM_SIZE: {
            Platform->WindowSize.X = (s16)LParam;
            Platform->WindowSize.Y = (s16)(LParam >> 16);
            Platform->Updates |= WINDOW_RESIZED;
        } return 0;
    }
    
    return Win32_DefWindowProcA(Window, Message, WParam, LParam);
}

external void
Platform_Entry(void)
{
    platform_state _P = {0};
    game_state GameState = {0};
    renderer_state Renderer = {0};
    Platform = &_P;
    
    platform_exports PlatformExports;
    #define EXPORT(ReturnType, Namespace, Name, ...) \
        PlatformExports.Namespace##_##Name = Namespace##_##Name;
    #define X PLATFORM_FUNCS
    #include <x.h>
    
    Platform_LoadWin32();
    Platform_LoadWGL();
    
    win32_window_class_a WindowClass = {0};
    WindowClass.Callback = Platform_WindowCallback;
    WindowClass.Instance = Win32_GetModuleHandleA(NULL);
    WindowClass.Icon = Win32_LoadIconA(NULL, IDI_APPLICATION);
    WindowClass.Cursor = Win32_LoadCursorA(NULL, IDC_ARROW);
    WindowClass.Background = (win32_brush)Win32_GetStockObject(BRUSH_BLACK);
    WindowClass.ClassName = "VoxarcWindowClass";
    Win32_RegisterClassA(&WindowClass);
    
    win32_window Window = Win32_CreateWindowExA(
        0, WindowClass.ClassName, "Voxarc",
        WS_OVERLAPPED|WS_SYSMENU|WS_CAPTION|WS_VISIBLE|WS_THICKFRAME|WS_MINIMIZEBOX|WS_MAXIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, WindowClass.Instance, NULL
    );
    Assert(Window);
    
    win32_device_context DeviceContext = Win32_GetDC(Window);
    opengl_funcs OpenGLFuncs = Platform_LoadOpenGL(DeviceContext);
    
    module GameModule = {0};
    Platform_LoadGame(&GameModule, &GameState, &PlatformExports, &OpenGLFuncs);
    Game_Init(Platform, &GameState, &Renderer);
    
    Platform->ExecutionState = EXECUTION_RUNNING;
    while(Platform->ExecutionState == EXECUTION_RUNNING)
    {
        Platform_LoadGame(&GameModule, &GameState, &PlatformExports, &OpenGLFuncs);
        
        win32_message Message;
        while(Win32_PeekMessageA(&Message, Window, 0, 0, PM_REMOVE)) {
            if(Message.Message == WM_QUIT) {
                Platform->ExecutionState = EXECUTION_ENDED;
                break;
            }
            
            Win32_TranslateMessage(&Message);
            Win32_DispatchMessageA(&Message);
        }
        
        Game_Update(Platform, &GameState, &Renderer);
        
        Win32_SwapBuffers(DeviceContext);
    }
    
    Win32_ExitProcess(0);
}