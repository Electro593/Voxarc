#include <intrin.h>

#include "util/vox_defines.h"

#include "voxarc.h"
#include "platform/vox_platform_win32.h"
#include "platform/vox_platform_shared.h"
#include "voxarc.c"

global_var win32_data Data = {0};

typedef vptr WINAPI GetProcAddressproc(vptr Module, cc08 *ProcName);
global_var GetProcAddressproc *GetProcAddress;

typedef vptr WINAPI LoadLibraryAproc(c08 *FileName);
global_var LoadLibraryAproc *LoadLibraryA;

#define PE_GET_OFFSET(Module, Offset) ((u08*)(Module) + (Offset))

local_func void
Win32InitWindowsFunctions()
{
    vptr Kernel32 = 0;
    
    c16 Kernel32NameLower[] = L"kernel32.dll";
    c16 Kernel32NameUpper[] = L"KERNEL32.DLL";
    win32_teb *TEB = (win32_teb*)__readgsqword(0x30);
    win32_ldr_data_entry* LoaderDataEntry = TEB->PEB->LoaderData->LoaderDataEntry;
    while(LoaderDataEntry->DllBase)
    {
        if(CompareMemory(LoaderDataEntry->DllNameBuffer, Kernel32NameLower, MIN(LoaderDataEntry->DllNameLength, sizeof(Kernel32NameLower))) == 0 ||
           CompareMemory(LoaderDataEntry->DllNameBuffer, Kernel32NameUpper, MIN(LoaderDataEntry->DllNameLength, sizeof(Kernel32NameUpper))) == 0)
        {
            Kernel32 = LoaderDataEntry->BaseAddress;
            break;
        }
        LoaderDataEntry = (win32_ldr_data_entry*)(LoaderDataEntry->LinkedList.Next);
    }
    
    win32_msdos *MSDOSHeader = (win32_msdos*)PE_GET_OFFSET(Kernel32, 0);
    win32_pe *PEHeader = (win32_pe*)PE_GET_OFFSET(Kernel32, MSDOSHeader->PEOffset);
    win32_pe_export_table *ExportTable = (win32_pe_export_table*)PE_GET_OFFSET(Kernel32, PEHeader->ExportTable.VirtualAddress);
    u32 *NamePointerTable = (u32*)PE_GET_OFFSET(Kernel32, ExportTable->NamePointerRVA);
    
    s32 Low = 0;
    s32 High = ExportTable->NumberofNamePointers - 1;
    s32 Index = (High + Low) / 2;
    c08 *ProcName = (c08*)PE_GET_OFFSET(Kernel32, NamePointerTable[Index]);
    s32 CompareResult = 0;
    while((CompareResult = CompareArrays("GetProcAddress", ProcName)) != 0)
    {
        if (CompareResult > 0)
        {
            Low = Index;
        }
        else if (CompareResult < 0)
        {
            High = Index;
        }
        Index = (High + Low) / 2;
        ProcName = (c08*)PE_GET_OFFSET(Kernel32, NamePointerTable[Index]);
    }
    
    u16* OrdinalTable = (u16*)PE_GET_OFFSET(Kernel32, ExportTable->OrdinalTableRVA);
    u16 GetProcAddressOrdinal = OrdinalTable[Index];
    
    u32* ExportAddressTable = (u32*)PE_GET_OFFSET(Kernel32, ExportTable->ExportAddressTableRVA);
    u32 GetProcAddressRVA = ExportAddressTable[GetProcAddressOrdinal];
    GetProcAddress = (GetProcAddressproc*)PE_GET_OFFSET(Kernel32, GetProcAddressRVA);
    
    LoadLibraryA = (LoadLibraryAproc*)GetProcAddress(Kernel32, "LoadLibraryA");
    vptr User32 = LoadLibraryA("user32.dll");
    vptr Gdi32 = LoadLibraryA("gdi32.dll");
    vptr Opengl32 = LoadLibraryA("opengl32.dll");
    
    #define PROCM(Module, ReturnType, Name, ...) \
        Name = (Name##proc*)GetProcAddress(Module, #Name); \
        if(!Name) \
        { \
            OutputDebugStringA("Function " #Name " was not found.\n"); \
        }
    WIN32_PROCS
    #undef PROCM
    
    win32_window_class DummyWindowClass;
    SetMemory(&DummyWindowClass, 0, sizeof(DummyWindowClass));
    DummyWindowClass.Style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
    DummyWindowClass.WindowCallback = DefWindowProcA;
    DummyWindowClass.Instance = GetModuleHandleA(0);
    DummyWindowClass.ClassName = "VoxarcOGLDummyWindowClass";
    
    RegisterClassA(&DummyWindowClass);
    vptr DummyWindow = CreateWindowExA(0, DummyWindowClass.ClassName, "VoxarcOGLDummy", 0,
                                       CW_USEDEFAULT, CW_USEDEFAULT,
                                       CW_USEDEFAULT, CW_USEDEFAULT,
                                       0, 0, DummyWindowClass.Instance, 0);
    
    vptr DummyDeviceContext = GetDC(DummyWindow);
    
    win32_pixel_format_descriptor PixelFormatDescriptor;
    SetMemory(&PixelFormatDescriptor, 0, sizeof(PixelFormatDescriptor));
    PixelFormatDescriptor.Size = sizeof(PixelFormatDescriptor);
    PixelFormatDescriptor.Version = 1;
    PixelFormatDescriptor.PixelType = PFD_TYPE_RGBA;
    PixelFormatDescriptor.Flags = PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;
    PixelFormatDescriptor.ColorBits = 32;
    PixelFormatDescriptor.AlphaBits = 8;
    PixelFormatDescriptor.LayerType = PFD_MAIN_PLANE;
    PixelFormatDescriptor.DepthBits = 24;
    PixelFormatDescriptor.StencilBits = 8;
    
    s32 PixelFormat = ChoosePixelFormat(DummyDeviceContext, &PixelFormatDescriptor);
    SetPixelFormat(DummyDeviceContext, PixelFormat, &PixelFormatDescriptor);
    vptr DummyRenderContext = wglCreateContext(DummyDeviceContext);
    wglMakeCurrent(DummyDeviceContext, DummyRenderContext);
    
    #define PROC(ReturnType, Name, ...) \
        Name = (Name##proc*)wglGetProcAddress(#Name); \
        if(!Name) \
        { \
            OutputDebugStringA("Function " #Name " was not found.\n"); \
        }
        WGL_PROCS
    #undef PROC
    
    wglMakeCurrent(DummyDeviceContext, 0);
    wglDeleteContext(DummyRenderContext);
    ReleaseDC(DummyWindow, DummyDeviceContext);
    DestroyWindow(DummyWindow);
}

local_func vptr
Win32InitOpenGLFunctions(vptr DeviceContext)
{
    s32 PixelFormatAttribs[] = {WGL_DRAW_TO_WINDOW_ARB, TRUE,
                                WGL_SUPPORT_OPENGL_ARB, TRUE,
                                WGL_DOUBLE_BUFFER_ARB,  TRUE,
                                WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
                                WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
                                WGL_COLOR_BITS_ARB,     32,
                                WGL_DEPTH_BITS_ARB,     24,
                                WGL_STENCIL_BITS_ARB,   8,
                                WGL_SAMPLE_BUFFERS_ARB, 1,
                                WGL_SAMPLES_ARB, 4,
                                0};
    
    win32_pixel_format_descriptor PixelFormatDescriptor;
    s32 PixelFormat;
    u32 FormatCount;
    wglChoosePixelFormatARB(DeviceContext, PixelFormatAttribs, 0, 1, &PixelFormat, &FormatCount);
    DescribePixelFormat(DeviceContext, PixelFormat,
                        sizeof(PixelFormatDescriptor), &PixelFormatDescriptor);
    
    SetPixelFormat(DeviceContext, PixelFormat, &PixelFormatDescriptor);
    s32 AttribList[] = {WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
                        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
                        WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
                        WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                        0};
    
    vptr RenderContext = wglCreateContextAttribsARB(DeviceContext, 0, AttribList);
    wglMakeCurrent(DeviceContext, RenderContext);
    
    #define PROC(ReturnType, Name, ...) \
        Name = (Name##proc*)wglGetProcAddress(#Name); \
        if(!Name) \
        { \
            OutputDebugStringA("Function " #Name " was not found.\n"); \
        }
        GL_PROCS
    #undef PROC
    
    return RenderContext;
}

local_func size
Win32GetFileSize(memory_handle *FileHandle)
{
    win32_large_integer Size;
    Size.QuadPart = 0;
    if(GetFileSizeEx(*(vptr*)FileHandle->Base, &Size) == 0)
    {
        //TODO: Logging
    }
    return Size.QuadPart + 1;
}

local_func void
Win32CloseFile(memory_handle *FileHandle)
{
    CloseHandle(*(vptr*)FileHandle->Base);
}

local_func void
Win32CloseFileType(memory_handle *FindHandle)
{
    FindClose(*(vptr*)FindHandle);
}

local_func void
Win32OpenFile(memory_handle *FileHandle, c08 *FileName)
{
    vptr GivenHandle = CreateFileA(FileName, GENERIC_READ, FILE_SHARE_READ,
                                   0, OPEN_EXISTING, 0, 0);
    if(GivenHandle != INVALID_HANDLE_VALUE)
    {
        *(vptr*)FileHandle->Base = *(vptr*)(&GivenHandle);
    }
    else
    {
        *(vptr*)FileHandle->Base = NULL;
    }
}

local_func void
Win32OpenFileType(memory_handle *FileHandle, memory_handle *FindHandle, file_type FileType)
{
    c08 *Wildcard = "*.*";
    
    switch(FileType)
    {
        case FileType_Bitmap:
        {
            Wildcard = "assets/*.bmp";
        } break;
        default:
        {
            Assert(FALSE);
        }
    }
    
    win32_find_data FindData;
    vptr GivenFindHandle = FindFirstFileA(Wildcard, &FindData);
    if(GivenFindHandle != INVALID_HANDLE_VALUE)
    {
        *(vptr*)FindHandle->Base = *(vptr*)(&GivenFindHandle);
        Win32OpenFile(FileHandle, FindData.FileName);
    }
    else
    {
        *(vptr*)FileHandle->Base = NULL;
        *(vptr*)FindHandle->Base = NULL;
    }
}

local_func void
Win32OpenNextFile(memory_handle *FileHandle, memory_handle *FindHandle)
{
    win32_find_data FindData;
    if(FindNextFileA(*(vptr*)FindHandle->Base, &FindData))
    {
        Win32OpenFile(FileHandle, FindData.FileName);
    }
    else
    {
        *(vptr*)FileHandle->Base = NULL;
    }
}

local_func void
Win32ReadDataFromFile(memory_handle *Dest, memory_handle *FileHandle, size Offset)
{
    win32_overlapped Overlapped;
    SetMemory(&Overlapped, 0, sizeof(Overlapped));
    Overlapped.Offset = (u32)((Offset >> 0) & 0xFFFFFFFF);
    Overlapped.OffsetHigh = (u32)((Offset >> 32) & 0xFFFFFFFF);
    u32 BytesRead;
    if(!ReadFile(*(vptr*)FileHandle->Base, Dest->Base, (u32)Dest->Size - 1, &BytesRead, &Overlapped))
    {
        //TODO: Logging
    }
    u08 *NullTerminator = (u08*)Dest->Base + Dest->Size - 1;
    NullTerminator = '\0';
}

local_func void
WIn32ReleaseMemory(vptr Base)
{
    VirtualFree(Base, 0, MEM_RELEASE);
}

local_func vptr
Win32ReserveMemory(size Size)
{
    vptr MemoryBlock = VirtualAlloc(0, Size,
                                    MEM_COMMIT|MEM_RESERVE,
                                    PAGE_READWRITE);
    return MemoryBlock;
}

local_func void
Win32CaptureCursor(void)
{
    SetCursor(0);
    
    win32_rect ClipRect;
    GetClientRect(Data.Window, &ClipRect);
    ClientToScreen(Data.Window, (win32_point*)&ClipRect.Left);
    ClientToScreen(Data.Window, (win32_point*)&ClipRect.Right);
    ClipCursor(&ClipRect);
}

local_func void
Win32ReleaseCursor(void)
{
    SetCursor(LoadCursorA(0, IDC_ARROW));
    ClipCursor(0);
}

local_func s64 WINAPIENTRY
Win32WindowCallback(vptr Window,
                    u32 Message,
                    u64 WParam,
                    s64 LParam)
{
    switch(Message)
    {
        case WM_SETFOCUS:
        {
            if(Data.Flags.CursorIsCaptured)
            {
                Win32CaptureCursor();
            }
            Data.Flags.GameIsFocused = TRUE;
            return 0;
        } break;
        case WM_KILLFOCUS:
        {
            if(Data.Flags.CursorIsCaptured)
            {
                Win32ReleaseCursor();
            }
            Data.Flags.GameIsFocused = FALSE;
        } break;
        case WM_SYSCOMMAND:
        {
            if(WParam == SC_KEYMENU)
            {
                return 0;
            }
        } break;
        case WM_CLOSE:
        {
            Data.Flags.GameIsRunning = FALSE;
        } break;
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP:
        {
            b08 WasPressed = (LParam & (1 << 30)) != 0;
            b08 IsPressed = (LParam & (1 << 31)) == 0;
            
            if(WasPressed != IsPressed)
            {
                Data.Input.Keys[WParam] = IsPressed;
            }
            
            if(Data.Input.Keys[VK_MENU] && Data.Input.Keys[VK_F4])
            {
                Data.Flags.GameIsRunning = FALSE;
            }
        } break;
        case WM_LBUTTONDOWN:
        {
            Data.Input.Keys[VK_LBUTTON] = TRUE;
        } break;
        case WM_LBUTTONUP:
        {
            Data.Input.Keys[VK_LBUTTON] = FALSE;
        } break;
        case WM_RBUTTONDOWN:
        {
            Data.Input.Keys[VK_RBUTTON] = TRUE;
        } break;
        case WM_RBUTTONUP:
        {
            Data.Input.Keys[VK_RBUTTON] = FALSE;
        } break;
        case WM_MOUSEMOVE:
        {
            v2s32 CursorPos = V2s32_1_1(LOW16(LParam), HIGH16(LParam));
            
            if(Data.Flags.CursorIsCaptured)
            {
                Data.Input.CursorPos = V2s32_Add(Data.Input.CursorPos, V2s32_Subtract(CursorPos, Data.PrevCursorPos));
            }
            else
            {
                Data.Input.CursorPos = CursorPos;
            }
            
            Data.PrevCursorPos = CursorPos;
        } break;
        case WM_MOVE:
        {
            if(Data.Flags.CursorIsCaptured)
            {
                win32_rect ClipRect;
                GetClientRect(Data.Window, &ClipRect);
                ClientToScreen(Data.Window, (win32_point*)&ClipRect.Left);
                ClientToScreen(Data.Window, (win32_point*)&ClipRect.Right);
                ClipCursor(&ClipRect);
            }
        } break;
        case WM_SIZING:
        {
            Data.Flags.AspectRatioChanged = TRUE;
            Data.WindowRect = *(win32_rect*)LParam;
        } break;
        case WM_ERASEBKGND:
        {
            return 1;
        } break;
        case WM_DESTROY:
        {
            Data.Flags.GameIsRunning = FALSE;
        } break;
        case WM_QUIT:
        {
            Data.Flags.GameIsRunning = FALSE;
        } break;
    }
    
    return DefWindowProcA(Window, Message, WParam, LParam);
}

global_func s32 WINAPIENTRY
WinMain(vptr Instance,
        vptr PrevInstance,
        c08 *CmdLine,
        s32 ShowCmd)
{
    win32_window_class WindowClass = {0};
    WindowClass.Style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
    WindowClass.Instance = Instance;
    WindowClass.Cursor = LoadCursorA(Instance, IDC_ARROW);
    WindowClass.WindowCallback = Win32WindowCallback;
    WindowClass.ClassName = "VoxarcWindowClass";
    
    GetFileSize = Win32GetFileSize;
    CloseFileType = Win32CloseFileType;
    CloseFile = Win32CloseFile;
    OpenFile = Win32OpenFile;
    OpenFileType = Win32OpenFileType;
    OpenNextFile = Win32OpenNextFile;
    ReadDataFromFile = Win32ReadDataFromFile;
    ReleaseMemory = WIn32ReleaseMemory;
    ReserveMemory = Win32ReserveMemory;
    
    if(RegisterClassA(&WindowClass))
    {
        vptr Window = CreateWindowExA(0, WindowClass.ClassName, "Voxarc",
                                      WS_OVERLAPPED|WS_MAXIMIZEBOX|WS_MINIMIZEBOX|
                                      WS_THICKFRAME|WS_SYSMENU|WS_CAPTION|WS_VISIBLE,
                                      CW_USEDEFAULT, CW_USEDEFAULT,
                                      1280, 800,
                                      0, 0, Instance, 0);
        
        if(Window)
        {
            vptr DeviceContext = GetDC(Window);
            vptr RenderContext = Win32InitOpenGLFunctions(DeviceContext);
            
            Data.Window = Window;
            
            win32_large_integer PerformanceCount;
            QueryPerformanceCounter(&PerformanceCount);
            Data.State.Random = InitRand(PerformanceCount.LowPart);
            
            InitGame(&Data.State);
            
            Data.Flags.GameIsRunning = TRUE;
            Data.Flags.GameIsFocused = TRUE;
            b08 WasCursorCaptured = FALSE;
            
            while(Data.Flags.GameIsRunning)
            {
                //TODO: This hogs the CPU, put it to sleep
                
                win32_message Message;
                while(PeekMessageA(&Message, Window, 0, 0, PM_REMOVE))
                {
                    TranslateMessage(&Message);
                    DispatchMessageA(&Message);
                }
                
                if(Data.Flags.CursorIsCaptured != WasCursorCaptured)
                {
                    if(Data.Flags.CursorIsCaptured)
                    {
                        Win32CaptureCursor();
                    }
                    else
                    {
                        Win32ReleaseCursor();
                    }
                    WasCursorCaptured = Data.Flags.CursorIsCaptured;
                }                
                
                if(Data.Flags.GameIsFocused)
                {
                    if(Data.Flags.AspectRatioChanged)
                    {
                        s32 WindowWidth = Data.WindowRect.Right - Data.WindowRect.Left;
                        s32 WindowHeight = Data.WindowRect.Bottom - Data.WindowRect.Top;
                        Data.State.WindowAspectRatio = (r32)WindowWidth / (r32)WindowHeight;
                    }
                    
                    UpdateGame(&Data.Input, &Data.State, &Data.Flags);
                    
                    SwapBuffers(DeviceContext);
                }
            }
            wglMakeCurrent(DeviceContext, 0);
            wglDeleteContext(RenderContext);
            ReleaseDC(Window, DeviceContext);
            DestroyWindow(Window);
        }
        else
        {
            //TODO: Logging
        }
    }
    else
    {
        //TODO: Logging
    }
    
    return 0;
}

global_func void
WinMainCRTStartup()
{
    Win32InitWindowsFunctions();
    
    s32 Result = WinMain(GetModuleHandleA(0), 0, 0, 0);
    ExitProcess(Result);
}