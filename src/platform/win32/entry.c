/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <shared.h>

global execution_state ExecutionState;
// global u32 MainThreadID;
global u32 GlobalAllocCounter;

internal void
Platform_LoadWin32Funcs(void)
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
    
    #define FUNC(Module, ReturnType, Name, ...) \
        Win32_##Name = (func_Win32_##Name*)Win32_GetProcAddress(Module, #Name); \
        Assert(Win32_##Name);
    WIN32_FUNCS
    #undef FUNC
}

internal void
Platform_LoadWGLFuncs(void)
{
    win32_module OpenGL32 = Win32_LoadLibraryA("opengl32.dll");
    #define FUNC_TYPE1(ReturnType, Name, ...) \
        WGL_##Name = (func_WGL_##Name*)Win32_GetProcAddress(OpenGL32, "wgl" #Name); \
        Assert(WGL_##Name);
    #define FUNC_TYPE2(ReturnType, Name, ...)
    WGL_FUNCS
    #undef FUNC_TYPE1
    #undef FUNC_TYPE2
    
    win32_window_class_a DummyWindowClass = {0};
    DummyWindowClass.Style = Win32_ClassStyle_HRedraw |
                             Win32_ClassStyle_VRedraw |
                             Win32_ClassStyle_OwnDC;
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
    PixelFormatDescriptor.PixelType = Win32_PixelFormatDescriptor_TypeRGBA;
    PixelFormatDescriptor.Flags = Win32_PixelFormatDescriptor_DrawToWindow |
                                  Win32_PixelFormatDescriptor_SupportOpenGL |
                                  Win32_PixelFormatDescriptor_DoubleBuffer;
    PixelFormatDescriptor.ColorBits = 32;
    PixelFormatDescriptor.AlphaBits = 8;
    PixelFormatDescriptor.LayerType = Win32_PixelFormatDescriptor_MainPlane;
    PixelFormatDescriptor.DepthBits = 24;
    PixelFormatDescriptor.StencilBits = 8;
    
    s32 PixelFormat = Win32_ChoosePixelFormat(DummyDeviceContext, &PixelFormatDescriptor);
    Win32_SetPixelFormat(DummyDeviceContext, PixelFormat, &PixelFormatDescriptor);
    vptr DummyRenderContext = WGL_CreateContext(DummyDeviceContext);
    WGL_MakeCurrent(DummyDeviceContext, DummyRenderContext);
    
    #define FUNC_TYPE1(ReturnType, Name, ...)
    #define FUNC_TYPE2(ReturnType, Name, ...) \
        WGL_##Name = (func_WGL_##Name*)WGL_GetProcAddress("wgl" #Name); \
        Assert(WGL_##Name);
    WGL_FUNCS
    #undef FUNC_TYPE1
    #undef FUNC_TYPE2
    
    WGL_MakeCurrent(DummyDeviceContext, 0);
    WGL_DeleteContext(DummyRenderContext);
    Win32_ReleaseDC(DummyWindow, DummyDeviceContext);
    Win32_DestroyWindow(DummyWindow);
}

internal void
Platform_LoadOpenGLFuncs(win32_device_context DeviceContext)
{
    s32 PixelFormatAttribs[] = {
        WGL_ARB_DrawToWindow,  TRUE,
        WGL_ARB_SupportOpenGL, TRUE,
        WGL_ARB_DoubleBuffer,  TRUE,
        WGL_ARB_Acceleration,  WGL_ARB_FullAcceleration,
        WGL_ARB_PixelType,     WGL_ARB_TypeRGBA,
        WGL_ARB_ColorBits,     32,
        WGL_ARB_DepthBits,     24,
        WGL_ARB_StencilBits,   8,
        WGL_ARB_SampleBuffers, 1,
        WGL_ARB_Samples,       4,
        0
    };
    
    win32_pixel_format_descriptor PixelFormatDescriptor;
    s32 PixelFormat;
    u32 FormatCount;
    WGL_ChoosePixelFormatARB(DeviceContext, PixelFormatAttribs, 0, 1, &PixelFormat, &FormatCount);
    Win32_DescribePixelFormat(DeviceContext, PixelFormat, sizeof(win32_pixel_format_descriptor), &PixelFormatDescriptor);
    
    #if defined(_DEBUG)
        u32 DebugBit = WGL_ARB_Context_Debug;
    #else
        u32 DebugBit = 0;
    #endif
    
    Win32_SetPixelFormat(DeviceContext, PixelFormat, &PixelFormatDescriptor);
    s32 AttribList[] = {
        WGL_ARB_Context_MajorVersion, 4,
        WGL_ARB_Context_MinorVersion, 6,
        WGL_ARB_Context_Flags,        DebugBit | WGL_ARB_Context_ForwardCompatible,
        WGL_ARB_Context_ProfileMask,  WGL_ARB_Context_CoreProfile,
        0
    };
    
    win32_opengl_render_context RenderContext = WGL_CreateContextAttribsARB(DeviceContext, 0, AttribList);
    WGL_MakeCurrent(DeviceContext, RenderContext);
    
    win32_handle OpenGL32 = Win32_GetModuleHandleA("opengl32.dll");
    Assert(OpenGL32);
    
    #define FUNC_TYPE1(ReturnType, Name, ...) \
        OpenGL_##Name = (func_OpenGL_##Name*)Win32_GetProcAddress(OpenGL32, "gl" #Name); \
        Assert(OpenGL_##Name);
    #define FUNC_TYPE2(ReturnType, Name, ...) \
        OpenGL_##Name = (func_OpenGL_##Name*)WGL_GetProcAddress("gl" #Name); \
        Assert(OpenGL_##Name);
    OPENGL_FUNCS
    #undef FUNC_TYPE1
    #undef FUNC_TYPE2
}

internal void
_Assert(c08 *File,
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
    ++GlobalAllocCounter;
    
    vptr MemoryBlock = Win32_VirtualAlloc(0, Size, Win32_Alloc_Commit|Win32_Alloc_Reserve, Win32_Page_ReadWrite);
    Assert(MemoryBlock);
    
    return MemoryBlock;
}

internal void
Platform_FreeMemory(vptr Base)
{
    --GlobalAllocCounter;
    
    Win32_VirtualFree(Base, 0, Win32_Alloc_Release);
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

internal s64 API_ENTRY
Platform_WindowCallback(win32_window Window,
                        u32 Message,
                        s64 WParam,
                        s64 LParam)
{
    if(ExecutionState == EXECUTION_UNINITIALIZED) {
        return Win32_DefWindowProcA(Window, Message, WParam, LParam);
    }
    
    Assert(Message != Win32_WindowMessage_Destroy);

    switch(Message) {
        case Win32_WindowMessage_Close: {
            ExecutionState = EXECUTION_ENDED;
        } return 0;
    }
    
    return Win32_DefWindowProcA(Window, Message, WParam, LParam);
}

external void API_ENTRY
Platform_Entry(void)
{
    Context.PrevContext = &Context;
    
    Platform_LoadWin32Funcs();
    Platform_LoadWGLFuncs();
    
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
        Win32_WindowStyle_Overlapped|Win32_WindowStyle_SysMenu|Win32_WindowStyle_Caption|Win32_WindowStyle_Visible,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, WindowClass.Instance, NULL
    );
    
    Assert(Window);
    
    u64 StackSize = 32*1024*1024;
    vptr MemBase = Platform_AllocateMemory(StackSize);
    Context.Stack = Stack_Init(MemBase, StackSize);
    
    vptr FileHandle;
    Platform_OpenFile(&FileHandle, "assets\\fonts\\cour.ttf", FILE_READ);
    u64 FileLength = Platform_GetFileLength(FileHandle);
    u08 *TTFData = Stack_Allocate(FileLength);
    Platform_ReadFile(FileHandle, TTFData, FileLength, 0);
    Platform_CloseFile(FileHandle);
    
    vptr FontFile, FontBitmap;
    u64 FontFileSize, FontBitmapSize;
    MakeFont(TTFData, 40, &FontFile, &FontFileSize, &FontBitmap, &FontBitmapSize);
    Platform_OpenFile(&FileHandle, "assets\\fonts\\cour.font", FILE_WRITE);
    Platform_WriteFile(FileHandle, FontFile, FontFileSize, 0);
    Platform_CloseFile(FileHandle);
    Platform_OpenFile(&FileHandle, "assets\\textures\\cour.bmp", FILE_WRITE);
    Platform_WriteFile(FileHandle, FontBitmap, FontBitmapSize, 0);
    Platform_CloseFile(FileHandle);
    
    win32_device_context DeviceContext = Win32_GetDC(Window);
    Platform_LoadOpenGLFuncs(DeviceContext);
    
    game_state Game = {0};
    renderer_state Renderer = {0};
    Game_Init(&Game, &Renderer);
    
    s08 Time = 0;
    ExecutionState = EXECUTION_RUNNING;
    while(ExecutionState == EXECUTION_RUNNING)
    {
        win32_message Message;
        while(Win32_PeekMessageA(&Message, Window, 0, 0, Win32_PeekMessage_Remove)) {
            if(Message.Message == Win32_WindowMessage_Quit) {
                ExecutionState = EXECUTION_ENDED;
                break;
            }
            
            Win32_TranslateMessage(&Message);
            Win32_DispatchMessageA(&Message);
        }
        
        Game_Update(&Game, &Renderer);
        
        Win32_SwapBuffers(DeviceContext);
    }
    
    Platform_FreeMemory(MemBase);
    Assert(GlobalAllocCounter == 0, "Not everything was freed!");
    
    Win32_ExitProcess(0);
}