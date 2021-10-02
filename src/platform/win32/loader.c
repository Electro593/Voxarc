/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                         * 
 *  Copyright (C) 2020 Aria Seiler                                       * 
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

global s32 GlobalAllocCounter;

internal win32_teb *
Platform_GetTEB(void)
{
    win32_teb *Result;
    
    Result = (win32_teb*)__readgsqword(OFFSET_OF(win32_nt_tib, Self));
    
    return Result;
}

internal win32_ldr_data_table_entry *
Platform_GetLoaderTableEntry(u32 Index)
{
    win32_ldr_data_table_entry *Result;
    
    win32_teb *TEB = Platform_GetTEB();
    win32_list_entry *Entry = TEB->ProcessEnvironmentBlock->Ldr->InMemoryOrderModuleList.Flink;
    while(Index--)
    {
        Entry = Entry->Flink;
    }
    Result = (win32_ldr_data_table_entry*)((u08*)Entry - sizeof(win32_list_entry));
    
    return Result;
}

internal win32_unicode_string
Platform_GetProcessPath(void)
{
    win32_unicode_string Result;
    
    win32_ldr_data_table_entry *VoxarcEntry = Platform_GetLoaderTableEntry(0);
    Result = VoxarcEntry->FullDllName;
    
    return Result;
}

internal void
Platform_InitFunctions(platform_exports *PlatformExports)
{
    win32_ldr_data_table_entry *KernelEntry = Platform_GetLoaderTableEntry(2);
    win32_module Kernel32 = KernelEntry->DllBase;
    
    win32_image_dos_header *ImageDOSHeader = (win32_image_dos_header*)Kernel32;
    win32_image_nt_headers *ImageNTHeaders = (win32_image_nt_headers*)PTR_JUMP(Kernel32, ImageDOSHeader->e_lfanew);
    win32_image_export_directory *ImageExportDirectory = (win32_image_export_directory*)PTR_JUMP(Kernel32, ImageNTHeaders->OptionalHeader.ExportTable.VirtualAddress);
    u32 *ExportNamePointerTable = (u32*)PTR_JUMP(Kernel32, ImageExportDirectory->AddressOfNames);
    
    s32 Low = 0;
    s32 High = ImageExportDirectory->NumberOfNames - 1;
    s32 Index = (High + Low) / 2;
    chr *CurrProcName = (chr*)PTR_JUMP(Kernel32, ExportNamePointerTable[Index]);
    s32 CompareResult = 0;
    chr *ProcName = "GetProcAddress";
    while(TRUE)
    {
        chr *ProcNameIter = ProcName;
        while(*ProcNameIter && *CurrProcName && *ProcNameIter == *CurrProcName)
        {
            ++ProcNameIter;
            ++CurrProcName;
        }
        
        CompareResult = *ProcNameIter - *CurrProcName;
        if(CompareResult == 0)
        {
            break;
        }
        
        if(CompareResult > 0)
        {
            Low = Index;
        }
        else if(CompareResult < 0)
        {
            High = Index;
        }
        
        Index = (High + Low) / 2;
        CurrProcName = (chr*)PTR_JUMP(Kernel32, ExportNamePointerTable[Index]);
    }
    
    u16 *ExportOrdinalTable = (u16*)PTR_JUMP(Kernel32, ImageExportDirectory->AddressOfNameOrdinals);
    u16 GetProcAddressOrdinal = ExportOrdinalTable[Index];
    
    u32 *ExportAddressTable = (u32*)PTR_JUMP(Kernel32, ImageExportDirectory->AddressOfFunctions);
    u32 GetProcAddressRVA = ExportAddressTable[GetProcAddressOrdinal];
    Win32_GetProcAddress = (_type__Win32_GetProcAddress*)PTR_JUMP(Kernel32, GetProcAddressRVA);
    
    Win32_LoadLibraryA = (_type__Win32_LoadLibraryA*)Win32_GetProcAddress(Kernel32, "LoadLibraryA");
    win32_module User32 = Win32_LoadLibraryA("user32.dll");
    win32_module Gdi32 = Win32_LoadLibraryA("gdi32.dll");
    win32_module Opengl32 = Win32_LoadLibraryA("opengl32.dll");
    
    Win32_OutputDebugStringA = (_type__Win32_OutputDebugStringA*)Win32_GetProcAddress(Kernel32, "OutputDebugStringA");
    if(!Win32_OutputDebugStringA)
    {
        ASSERT(FALSE);
    }
    
    #define FUNC(ReturnType, Name, ...) \
        PlatformExports->Name = Name;
    PLATFORM__EXPORTS
    #undef FUNC
    
    b08 WinReturnError = FALSE;
    #define PROC(Lib, ReturnType, LibName, Name, ...) \
        Name = (_type__##Name*)Win32_GetProcAddress(Lib, #LibName); \
        if(!Name) \
        { \
            Win32_OutputDebugStringA("Function " #Name " was not found.\n"); \
            WinReturnError = TRUE; \
        }
        WIN32__PROCS
    #undef PROC
    if(WinReturnError)
    {
        ASSERT(FALSE);
    }
    
    b08 GlReturnError = FALSE;
    #define PROC(Lib, ReturnType, LibName, Name, ...) \
        Name = PlatformExports->Name = (_type__##Name*)Win32_GetProcAddress(Lib, #LibName); \
        if(!Name) \
        { \
            Win32_OutputDebugStringA("Function " #Name " was not found.\n"); \
            GlReturnError = TRUE; \
        }
        OPENGL__BASE__PROCS
    #undef PROC
    if(GlReturnError)
    {
        ASSERT(FALSE);
    }
    
    win32_window_class_w DummyWindowClass = {0};
    DummyWindowClass.Style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
    DummyWindowClass.WindowCallback = Win32_DefWindowProcW;
    DummyWindowClass.Instance = Win32_GetModuleHandleW(0);
    DummyWindowClass.ClassName = L"VoxarcOGLDummyWindowClass";
    
    Win32_RegisterClassW(&DummyWindowClass);
    vptr DummyWindow = Win32_CreateWindowExW(0, DummyWindowClass.ClassName, L"VoxarcOGLDummy", 0,
                                       CW_USEDEFAULT, CW_USEDEFAULT,
                                       CW_USEDEFAULT, CW_USEDEFAULT,
                                       0, 0, DummyWindowClass.Instance, 0);
    
    vptr DummyDeviceContext = Win32_GetDC(DummyWindow);
    
    win32_pixel_format_descriptor PixelFormatDescriptor = {0};
    PixelFormatDescriptor.Size = sizeof(PixelFormatDescriptor);
    PixelFormatDescriptor.Version = 1;
    PixelFormatDescriptor.PixelType = PFD_TYPE_RGBA;
    PixelFormatDescriptor.Flags = PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;
    PixelFormatDescriptor.ColorBits = 32;
    PixelFormatDescriptor.AlphaBits = 8;
    PixelFormatDescriptor.LayerType = PFD_MAIN_PLANE;
    PixelFormatDescriptor.DepthBits = 24;
    PixelFormatDescriptor.StencilBits = 8;
    
    s32 PixelFormat = Win32_ChoosePixelFormat(DummyDeviceContext, &PixelFormatDescriptor);
    Win32_SetPixelFormat(DummyDeviceContext, PixelFormat, &PixelFormatDescriptor);
    vptr DummyRenderContext = WGL_CreateContext(DummyDeviceContext);
    WGL_MakeCurrent(DummyDeviceContext, DummyRenderContext);
    
        // Name = (_type__##Name*)Win32_GetProcAddress(Lib, #LibName); \
    
    b08 WglReturnError = FALSE;
    #define PROC(Lib, ReturnType, LibName, Name, ...) \
        Name = (_type__##Name*)WGL_GetProcAddress(#LibName); \
        if(!Name) \
        { \
            Win32_OutputDebugStringA("Function " #Name " was not found.\n"); \
            WglReturnError = TRUE; \
        }
        WGL__PROCS
    #undef PROC
    if(WglReturnError)
    {
        ASSERT(FALSE);
    }
    
    WGL_MakeCurrent(DummyDeviceContext, 0);
    WGL_DeleteContext(DummyRenderContext);
    Win32_ReleaseDC(DummyWindow, DummyDeviceContext);
    Win32_DestroyWindow(DummyWindow);
}

internal win32_opengl_render_context
Platform_InitOpenGLFunctions(platform_exports *PlatformExports,
                             win32_device_context DeviceContext)
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
    WGL_ChoosePixelFormatARB(DeviceContext, PixelFormatAttribs, 0, 1, &PixelFormat, &FormatCount);
    Win32_DescribePixelFormat(DeviceContext, PixelFormat, sizeof(PixelFormatDescriptor), &PixelFormatDescriptor);
    
    #if _MODE_DEBUG
    u32 DebugBit = WGL_CONTEXT_DEBUG_BIT_ARB;
    #else
    u32 DebugBit = 0;
    #endif
    
    Win32_SetPixelFormat(DeviceContext, PixelFormat, &PixelFormatDescriptor);
    s32 AttribList[] = {WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
                        WGL_CONTEXT_MINOR_VERSION_ARB, 6,
                        WGL_CONTEXT_FLAGS_ARB, DebugBit | WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
                        WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                        0};
    
    win32_opengl_render_context RenderContext = WGL_CreateContextAttribsARB(DeviceContext, 0, AttribList);
    WGL_MakeCurrent(DeviceContext, RenderContext);
    
    b08 GlReturnError = FALSE;
    #define PROC(Lib, ReturnType, LibName, Name, ...) \
        Name = PlatformExports->Name = (_type__##Name*)WGL_GetProcAddress(#LibName); \
        if(!Name) \
        { \
            Win32_OutputDebugStringA("Function " #Name " was not found.\n"); \
            GlReturnError = TRUE; \
        }
        OPENGL__EXT__PROCS
    #undef PROC
    if(GlReturnError)
    {
        ASSERT(FALSE);
    }
    
    return RenderContext;
}

internal void
Platform_InitKeyTables(platform_state *PlatformState)
{
    Mem_Set(PlatformState->Keycodes, KEYCODE_UNKNOWN, sizeof(PlatformState->Keycodes));
    Mem_Set(PlatformState->Scancodes, SCANCODE_UNKNOWN, sizeof(PlatformState->Scancodes));
    
    PlatformState->Keycodes[0x001] = KEYCODE_ESCAPE;
    PlatformState->Keycodes[0x039] = KEYCODE_SPACE;
    PlatformState->Keycodes[0x00F] = KEYCODE_TAB;
    PlatformState->Keycodes[0x01C] = KEYCODE_ENTER;
    PlatformState->Keycodes[0x152] = KEYCODE_INSERT;
    PlatformState->Keycodes[0x00E] = KEYCODE_BACKSPACE;
    PlatformState->Keycodes[0x153] = KEYCODE_DELETE;
    PlatformState->Keycodes[0x00B] = KEYCODE_0;
    PlatformState->Keycodes[0x002] = KEYCODE_1;
    PlatformState->Keycodes[0x003] = KEYCODE_2;
    PlatformState->Keycodes[0x004] = KEYCODE_3;
    PlatformState->Keycodes[0x005] = KEYCODE_4;
    PlatformState->Keycodes[0x006] = KEYCODE_5;
    PlatformState->Keycodes[0x007] = KEYCODE_6;
    PlatformState->Keycodes[0x008] = KEYCODE_7;
    PlatformState->Keycodes[0x009] = KEYCODE_8;
    PlatformState->Keycodes[0x00A] = KEYCODE_9;
    PlatformState->Keycodes[0x01E] = KEYCODE_A;
    PlatformState->Keycodes[0x030] = KEYCODE_B;
    PlatformState->Keycodes[0x02E] = KEYCODE_C;
    PlatformState->Keycodes[0x020] = KEYCODE_D;
    PlatformState->Keycodes[0x012] = KEYCODE_E;
    PlatformState->Keycodes[0x021] = KEYCODE_F;
    PlatformState->Keycodes[0x022] = KEYCODE_G;
    PlatformState->Keycodes[0x023] = KEYCODE_H;
    PlatformState->Keycodes[0x017] = KEYCODE_I;
    PlatformState->Keycodes[0x024] = KEYCODE_J;
    PlatformState->Keycodes[0x025] = KEYCODE_K;
    PlatformState->Keycodes[0x026] = KEYCODE_L;
    PlatformState->Keycodes[0x031] = KEYCODE_M;
    PlatformState->Keycodes[0x032] = KEYCODE_N;
    PlatformState->Keycodes[0x018] = KEYCODE_O;
    PlatformState->Keycodes[0x019] = KEYCODE_P;
    PlatformState->Keycodes[0x010] = KEYCODE_Q;
    PlatformState->Keycodes[0x013] = KEYCODE_R;
    PlatformState->Keycodes[0x01F] = KEYCODE_S;
    PlatformState->Keycodes[0x014] = KEYCODE_T;
    PlatformState->Keycodes[0x016] = KEYCODE_U;
    PlatformState->Keycodes[0x02F] = KEYCODE_V;
    PlatformState->Keycodes[0x011] = KEYCODE_W;
    PlatformState->Keycodes[0x02D] = KEYCODE_X;
    PlatformState->Keycodes[0x015] = KEYCODE_Y;
    PlatformState->Keycodes[0x02C] = KEYCODE_Z;
    PlatformState->Keycodes[0x029] = KEYCODE_GRAVE;
    PlatformState->Keycodes[0x00C] = KEYCODE_MINUS;
    PlatformState->Keycodes[0x00D] = KEYCODE_EQUAL;
    PlatformState->Keycodes[0x01A] = KEYCODE_LEFT_BRACKET;
    PlatformState->Keycodes[0x01B] = KEYCODE_RIGHT_BRACKET;
    PlatformState->Keycodes[0x02B] = KEYCODE_BACKSLASH;
    PlatformState->Keycodes[0x027] = KEYCODE_SEMICOLON;
    PlatformState->Keycodes[0x028] = KEYCODE_APOSTROPHE;
    PlatformState->Keycodes[0x033] = KEYCODE_COMMA;
    PlatformState->Keycodes[0x034] = KEYCODE_PERIOD;
    PlatformState->Keycodes[0x035] = KEYCODE_SLASH;
    PlatformState->Keycodes[0x056] = KEYCODE_WORLD_2;
    PlatformState->Keycodes[0x052] = KEYCODE_NUMPAD_0;
    PlatformState->Keycodes[0x04F] = KEYCODE_NUMPAD_1;
    PlatformState->Keycodes[0x050] = KEYCODE_NUMPAD_2;
    PlatformState->Keycodes[0x051] = KEYCODE_NUMPAD_3;
    PlatformState->Keycodes[0x04B] = KEYCODE_NUMPAD_4;
    PlatformState->Keycodes[0x04C] = KEYCODE_NUMPAD_5;
    PlatformState->Keycodes[0x04D] = KEYCODE_NUMPAD_6;
    PlatformState->Keycodes[0x047] = KEYCODE_NUMPAD_7;
    PlatformState->Keycodes[0x048] = KEYCODE_NUMPAD_8;
    PlatformState->Keycodes[0x049] = KEYCODE_NUMPAD_9;
    PlatformState->Keycodes[0x053] = KEYCODE_NUMPAD_DECIMAL;
    PlatformState->Keycodes[0x135] = KEYCODE_NUMPAD_DIVIDE;
    PlatformState->Keycodes[0x037] = KEYCODE_NUMPAD_MULTIPLY;
    PlatformState->Keycodes[0x04A] = KEYCODE_NUMPAD_SUBTRACT;
    PlatformState->Keycodes[0x04E] = KEYCODE_NUMPAD_ADD;
    PlatformState->Keycodes[0x11C] = KEYCODE_NUMPAD_ENTER;
    PlatformState->Keycodes[0x059] = KEYCODE_NUMPAD_EQUAL;
    PlatformState->Keycodes[0x03B] = KEYCODE_F1;
    PlatformState->Keycodes[0x03C] = KEYCODE_F2;
    PlatformState->Keycodes[0x03D] = KEYCODE_F3;
    PlatformState->Keycodes[0x03E] = KEYCODE_F4;
    PlatformState->Keycodes[0x03F] = KEYCODE_F5;
    PlatformState->Keycodes[0x040] = KEYCODE_F6;
    PlatformState->Keycodes[0x041] = KEYCODE_F7;
    PlatformState->Keycodes[0x042] = KEYCODE_F8;
    PlatformState->Keycodes[0x043] = KEYCODE_F9;
    PlatformState->Keycodes[0x044] = KEYCODE_F10;
    PlatformState->Keycodes[0x057] = KEYCODE_F11;
    PlatformState->Keycodes[0x058] = KEYCODE_F12;
    PlatformState->Keycodes[0x064] = KEYCODE_F13;
    PlatformState->Keycodes[0x065] = KEYCODE_F14;
    PlatformState->Keycodes[0x066] = KEYCODE_F15;
    PlatformState->Keycodes[0x067] = KEYCODE_F16;
    PlatformState->Keycodes[0x068] = KEYCODE_F17;
    PlatformState->Keycodes[0x069] = KEYCODE_F18;
    PlatformState->Keycodes[0x06A] = KEYCODE_F19;
    PlatformState->Keycodes[0x06B] = KEYCODE_F20;
    PlatformState->Keycodes[0x06C] = KEYCODE_F21;
    PlatformState->Keycodes[0x06D] = KEYCODE_F22;
    PlatformState->Keycodes[0x06E] = KEYCODE_F23;
    PlatformState->Keycodes[0x076] = KEYCODE_F24;
    PlatformState->Keycodes[0x137] = KEYCODE_PRINT_SCREEN;
    PlatformState->Keycodes[0x045] = KEYCODE_PAUSE;
    PlatformState->Keycodes[0x146] = KEYCODE_PAUSE;
    PlatformState->Keycodes[0x03A] = KEYCODE_CAPS_LOCK;
    PlatformState->Keycodes[0x046] = KEYCODE_SCROLL_LOCK;
    PlatformState->Keycodes[0x145] = KEYCODE_NUM_LOCK;
    PlatformState->Keycodes[0x02A] = KEYCODE_LEFT_SHIFT;
    PlatformState->Keycodes[0x01D] = KEYCODE_LEFT_CONTROL;
    PlatformState->Keycodes[0x038] = KEYCODE_LEFT_ALT;
    PlatformState->Keycodes[0x15B] = KEYCODE_LEFT_SUPER;
    PlatformState->Keycodes[0x036] = KEYCODE_RIGHT_SHIFT;
    PlatformState->Keycodes[0x11D] = KEYCODE_RIGHT_CONTROL;
    PlatformState->Keycodes[0x138] = KEYCODE_RIGHT_ALT;
    PlatformState->Keycodes[0x15C] = KEYCODE_RIGHT_SUPER;
    PlatformState->Keycodes[0x15D] = KEYCODE_MENU;
    PlatformState->Keycodes[0x14B] = KEYCODE_ARROW_LEFT;
    PlatformState->Keycodes[0x148] = KEYCODE_ARROW_UP;
    PlatformState->Keycodes[0x14D] = KEYCODE_ARROW_RIGHT;
    PlatformState->Keycodes[0x150] = KEYCODE_ARROW_DOWN;
    PlatformState->Keycodes[0x149] = KEYCODE_PAGE_UP;
    PlatformState->Keycodes[0x151] = KEYCODE_PAGE_DOWN;
    PlatformState->Keycodes[0x147] = KEYCODE_HOME;
    PlatformState->Keycodes[0x14F] = KEYCODE_END;
    
    for(u16 Scancode = 0;
        Scancode < 512;
        ++Scancode)
    {
        if(PlatformState->Keycodes[Scancode] != KEYCODE_UNKNOWN)
        {
            PlatformState->Scancodes[PlatformState->Keycodes[Scancode]] = Scancode;
        }
    }
}

// #if _MODE_DEBUG
internal void
Platform_ThrowError_DEBUG(chr *Message)
{
    Platform_Print_DEBUG(Message);
    STOP;
    Win32_ExitProcess(1);
}

internal void
Platform_Print_DEBUG(chr *Message)
{
    Win32_OutputDebugStringA(Message);
}
// #endif

internal void
Platform_LoadDll(loader_dll *DllData,
                 chr *Path,
                 chr *LockedPath,
                 chr *LoadFuncName)
{
    ASSERT(DllData->DLL == NULL);
    
    Win32_CopyFileA(Path, LockedPath, FALSE);
    
    DllData->DLL = Win32_LoadLibraryA(LockedPath);
    if(DllData->DLL)
    {
        DllData->LastWritten = Platform_GetFileDataA(LockedPath).LastWriteTime;
        DllData->LoadFunc = Win32_GetProcAddress(DllData->DLL, LoadFuncName);
    }
}

internal void
Platform_UnloadDll(loader_dll *DllData)
{
    if(DllData->DLL)
    {
        Win32_FreeLibrary(DllData->DLL);
        DllData->DLL = NULL;
    }
}

internal void
Platform_LoadGameDll(platform_exports *PlatformExports,
                     util_exports *UtilExports,
                     game_module *GameModule,
                     loader_dll *GameDll)
{
    Platform_LoadDll(GameDll, DLL_PATH("Game"), DLL_LOCKED_PATH("Game"), DLL_LOAD_FUNC_NAME("Game"));
    
    Game_Load = (_type__Game_Load*)GameDll->LoadFunc;
    Game_Load(PlatformExports, UtilExports, GameModule);
    
    #define FUNC(,Name,...) Name = GameModule->GameExports.Name;
    GAME__EXPORTS
    #undef FUNC
}

internal void
Platform_LoadUtilDll(platform_exports *PlatformExports,
                     util_module *UtilModule,
                     loader_dll *UtilDll)
{
    Platform_LoadDll(UtilDll, DLL_PATH("Util"), DLL_LOCKED_PATH("Util"), DLL_LOAD_FUNC_NAME("Util"));
    
    Util_Load = (_type__Util_Load*)UtilDll->LoadFunc;
    Util_Load(PlatformExports, UtilModule);
}

internal void
Platform_UnloadGameDll(game_module *GameModule,
                       loader_dll *GameDll)
{
    Mem_Zero(&GameModule->Exports, sizeof(game_exports));
    Platform_UnloadDll(GameDll);
    GameModule->Loaded = FALSE;
}

internal void
Platform_UnloadUtilDll(util_module *UtilModule,
                       loader_dll *UtilDll)
{
    Mem_Zero(&UtilModule->Exports, sizeof(util_exports));
    Platform_UnloadDll(UtilDll);
    UtilModule->Loaded = FALSE;
}

internal void
Platform_UpdateKeyNames(platform_state *PlatformState)
{
    u08 State[256] = {0};
    Mem_Set(PlatformState->KeyNames, 0, sizeof(PlatformState->KeyNames));
    
    for(u32 Keycode = KEYCODE_FIRST;
        Keycode < KEYCODE_LAST;
        ++Keycode)
    {
        wchr Buffer[16];
        
        u16 Scancode = PlatformState->Scancodes[Keycode];
        if(Scancode == SCANCODE_UNKNOWN)
        {
            continue;
        }
        
        u32 VirtualKey = Win32_MapVirtualKeyW(Scancode, MAPVK_VSC_TO_VK);
        u32 Length = Win32_ToUnicode(VirtualKey, Scancode, State, Buffer, ARRAY_COUNT(Buffer), 0);
        if(Length == -1)
        {
            Length = Win32_ToUnicode(VirtualKey, Scancode, State, Buffer, ARRAY_COUNT(Buffer), 0);    
        }
        
        if(Length < 1)
        {   
            continue;
        }
        
        Win32_WideCharToMultiByte(CP_UTF8, 0, Buffer, 1, PlatformState->KeyNames[Keycode], sizeof(PlatformState->KeyNames[Keycode]), NULL, NULL);
    }
}

internal void
Platform_CreateWorkQueueEntry(platform_work_queue *WorkQueue,
                              platform_work_queue_callback Function,
                              vptr Parameter)
{
    //TODO: InterlockedCompareExchange on this
    u32 NextCreateCursor = (WorkQueue->CreateCursor + 1) % MEM_COUNT(Heap_GetSize(WorkQueue->Entries), platform_work_queue_entry);
    ASSERT(NextCreateCursor != WorkQueue->ExecuteCursor); // Looped
    
    platform_work_queue_entry *WorkQueueEntry = MEM_MEMBER(WorkQueue->Entries, platform_work_queue_entry, WorkQueue->CreateCursor);
    WorkQueueEntry->Function = Function;
    WorkQueueEntry->Parameter = Parameter;
    
    ++WorkQueue->CreatedEntryCount;
    
    WriteBarrier();
    WriteFence();
    
    WorkQueue->CreateCursor = NextCreateCursor;
    Win32_ReleaseSemaphore(WorkQueue->Semaphore, 1, 0);
}

internal b08
Platform_ExecuteWorkQueueEntry(platform_work_queue *WorkQueue)
{
    u32 ExecuteCursor = WorkQueue->ExecuteCursor;
    u32 NextExecuteCursor = (ExecuteCursor + 1) % MEM_COUNT(Heap_GetSize(WorkQueue->Entries), platform_work_queue_entry);
    if(ExecuteCursor != WorkQueue->CreateCursor)
    {
        u32 EntryIndex = InterlockedCompareExchange((volatile s32*)&WorkQueue->ExecuteCursor, NextExecuteCursor, ExecuteCursor);
        if(EntryIndex == ExecuteCursor)
        {
            platform_work_queue_entry *WorkQueueEntry = MEM_MEMBER(WorkQueue->Entries, platform_work_queue_entry, EntryIndex);
            WorkQueueEntry->Function(WorkQueueEntry->Parameter);
            InterlockedIncrement((volatile s32*)&WorkQueue->ExecutedEntryCount);
        }
        
        return TRUE;
    }
    
    return FALSE;
}

internal void
Platform_FlushWorkQueue(platform_work_queue *WorkQueue)
{
    while(WorkQueue->ExecutedEntryCount < WorkQueue->CreatedEntryCount)
    {
        Platform_ExecuteWorkQueueEntry(WorkQueue);
    }
    
    WorkQueue->ExecutedEntryCount = 0;
    WorkQueue->CreatedEntryCount = 0;
}

// internal win32_find_data_w
// Platform_GetFileDataW(wstr FileName)
// {
//     win32_find_data_w Result;
    
//     win32_handle FindHandle = FindFirstFileW(FileName, &Result);
//     if(FindHandle != INVALID_HANDLE_VALUE)
//     {
//         FindClose(FindHandle);
//     }
//     else
//     {
//         Platform_ThrowError_DEBUG("Failed to find file\n");
//     }
    
//     return Result;
// }

internal win32_find_data_a
Platform_GetFileDataA(chr *FileName)
{
    win32_find_data_a Result;
    
    win32_handle FindHandle = Win32_FindFirstFileA(FileName, &Result);
    if(FindHandle != INVALID_HANDLE_VALUE)
    {
        Win32_FindClose(FindHandle);
    }
    else
    {
        Platform_ThrowError_DEBUG("Failed to find file\n");
    }
    
    return Result;
}

internal u64
Platform_GetFileLength(vptr FileHandle)
{
    win32_large_integer Size;
    Size.QuadPart = 0;
    
    if(Win32_GetFileSizeEx(*(win32_handle*)FileHandle, &Size) == 0)
    {
        Platform_ThrowError_DEBUG("Failed to get u64 of file\n");
    }
    
    return Size.QuadPart;
}

internal void
Platform_CloseFile(vptr FileHandle)
{
    Win32_CloseHandle(*(win32_handle*)FileHandle);
}

internal void
Platform_CloseFileType(vptr FindHandle,
                       file_data *FileData)
{
    Heap_Free(FileData->FileName);
    Win32_FindClose(*(win32_handle*)FindHandle);
}

internal b08
Platform_OpenFile(vptr FileHandle,
                  str FileName,
                  file_mode OpenMode)
{
    u32 DesiredAccess = 0;
    u32 CreationDisposition = 0;
    
    switch(OpenMode)
    {
        case FileMode_Read:
        {
            DesiredAccess = GENERIC_READ;
            CreationDisposition = OPEN_EXISTING;
        } break;
        
        case FileMode_Write:
        {
            DesiredAccess = GENERIC_WRITE;
            CreationDisposition = CREATE_ALWAYS;
        } break;
        
        case FileMode_Append:
        {
            DesiredAccess = GENERIC_WRITE;
            CreationDisposition = CREATE_NEW;
        } break;
        
        default:
        {
            STOP;
        }
    }
    
    win32_handle GivenHandle = Win32_CreateFileA(FileName, DesiredAccess, FILE_SHARE_READ,
                                                 NULL, CreationDisposition, FILE_ATTRIBUTE_NORMAL, NULL);
    
    u32 Error = Win32_GetLastError();
    if(Error == ERROR_PATH_NOT_FOUND)
    {
        u32 NestLevel = 0;
        u32 SlashIndex = 0;
        u32 FinalSlashIndex = Str_FindChar(FileName, '/', -1);
        
        do
        {
            SlashIndex = Str_FindChar(FileName, '/', NestLevel++);
            str DirStr = Str_Sub(NULL, FileName, 0, SlashIndex);
            
            Win32_CreateDirectoryA(DirStr, NULL);
            Str_Free(DirStr);
        }
        while(SlashIndex < FinalSlashIndex);
        
        GivenHandle = Win32_CreateFileA(FileName, DesiredAccess, FILE_SHARE_READ,
                                        NULL, CreationDisposition, FILE_ATTRIBUTE_NORMAL, NULL);
    }
    
    if(GivenHandle != INVALID_HANDLE_VALUE)
    {
        *(win32_handle*)FileHandle = *(win32_handle*)(&GivenHandle);
        
        return TRUE;
    }
    else
    {
        *(win32_handle*)FileHandle = NULL;
    }
    
    return FALSE;
}

internal u32
Platform_CountFilesOfTypeInDir(file_type FileType)
{
    u32 Result = 0;
    
    chr *Selector;
    switch(FileType)
    {
        case FileType_Any:
        {
            Selector = "*.*";
        } break;
        case FileType_Bitmap:
        {
            Selector = "*.bmp";
        } break;
        case FileType_AssetPack:
        {
            Selector = "*.vaa";
        } break;
        default:
        {
            Selector = "";
            STOP;
        }
    }
    
    win32_handle FindHandle = Win32_FindFirstFileA(Selector, NULL);
    if(FindHandle != INVALID_HANDLE_VALUE)
    {
        ++Result;
        
        while(Win32_FindNextFileA(FindHandle, NULL))
        {
            ++Result;
        }
        
        Win32_FindClose(FindHandle);
    }
    
    return Result;
}

internal b08
Platform_OpenFileType(vptr FileHandle,
                      vptr FindHandle,
                      file_data *FileData,
                      file_type FileType,
                      file_group FileGroup)
{
    b08 Result = FALSE;
    
    switch(FileGroup)
    {
        case FileGroup_Assets:
        {
            FileData->RelDir = FILE_GROUP__ASSETS;
        } break;
        case FileGroup_Shaders:
        {
            FileData->RelDir = FILE_GROUP__SHADERS;
        } break;
        default:
        {
            STOP;
        }
    }
    
    chr *ExtStr;
    switch(FileType)
    {
        case FileType_Any:
        {
            ExtStr = FILE_TYPE__ANY;
        } break;
        case FileType_Bitmap:
        {
            ExtStr = FILE_TYPE__BITMAP;
        } break;
        default:
        {
            ExtStr = "";
            STOP;
        }
    }
    
    str Selector = Str_Create(NULL, FileData->RelDir, 0);
    Str_Cat(&Selector, ExtStr);
    
    win32_find_data_a FindData = {0};
    win32_handle GivenFindHandle = Win32_FindFirstFileA(Selector, &FindData);
    Str_Free(Selector);
    
    if(GivenFindHandle != INVALID_HANDLE_VALUE)
    {
        ASSERT(sizeof(win32_handle) == sizeof(vptr));
        *(win32_handle*)FindHandle = *(win32_handle*)(&GivenFindHandle);
        
        str FileNameFull = Str_Create(NULL, FileData->RelDir, 0);
        Str_CatC(&FileNameFull, FindData.FileName);
        Result = Platform_OpenFile(FileHandle, FileNameFull, FileMode_Read);
        Str_Free(FileNameFull);
        
        FileData->FileSize = MERGE_32(FindData.FileSizeHigh, FindData.FileSizeLow);
        Str_Create(&FileData->FileName, FindData.FileName, 0);
    }
    else
    {
        *(win32_handle*)FileHandle = NULL;
        *(win32_handle*)FindHandle = NULL;
    }
    
    return Result;
}

internal b08
Platform_OpenNextFile(vptr FileHandle,
                      vptr FindHandle,
                      file_data *FileData)
{
    b08 Result = FALSE;
    win32_find_data_a FindData;
    
    ASSERT(FileData->FileName);
    
    if(Win32_FindNextFileA(*(win32_handle*)FindHandle, &FindData))
    {
        str FileNameFull = Str_Create(NULL, FileData->RelDir, 0);
        Str_CatC(&FileNameFull, FindData.FileName);
        Result = Platform_OpenFile(FileHandle, FileNameFull, FileMode_Read);
        Str_Free(FileNameFull);
        
        FileData->FileSize = MERGE_32(FindData.FileSizeHigh, FindData.FileSizeLow);
        Str_Free(FileData->FileName);
        Str_Create(&FileData->FileName, FindData.FileName, 0);
    }
    else
    {
        *(win32_handle*)FileHandle = NULL;
    }
    
    return Result;
}

internal void
Platform_ReadFile(vptr Dest,
                  vptr FileHandle,
                  u64 Length,
                  u64 Offset)
{
    win32_overlapped Overlapped = {0};
    Overlapped.Offset = (u32)((Offset >> 0) & U32_MAX);
    Overlapped.OffsetHigh = (u32)((Offset >> 32) & U32_MAX);
    u32 BytesRead;
    
    ASSERT(Dest);
    
    if(!Win32_ReadFile(*(vptr*)FileHandle, Dest, (u32)Length, &BytesRead, &Overlapped))
    {
        Platform_ThrowError_DEBUG("Failed to read data from file\n");
    }
    ASSERT(BytesRead == Length);
}

internal void
Platform_WriteFile(vptr Src,
                   vptr FileHandle,
                   u64 Length,
                   u64 Offset)
{
    win32_overlapped Overlapped = {0};
    Overlapped.Offset = (u32)((Offset >> 0) & U32_MAX);
    Overlapped.OffsetHigh = (u32)((Offset >> 32) & U32_MAX);
    u32 BytesWritten;
    
    if(!Win32_WriteFile(*(vptr*)FileHandle, Src, (u32)Length, &BytesWritten, &Overlapped))
    {
        Platform_ThrowError_DEBUG("Failed to write data to file\n");
    }
    ASSERT(BytesWritten == Length);
}

internal vptr
Platform_AllocateMemory(u64 Size)
{
    ++GlobalAllocCounter;
    
    vptr MemoryBlock = Win32_VirtualAlloc(0, Size, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
    ASSERT(MemoryBlock);
    // vptr MemoryBlock = HeapAlloc(GetProcessHeap(), 0, Size);
    return MemoryBlock;
}

internal void
Platform_FreeMemory(vptr Base)
{
    --GlobalAllocCounter;
    
    Win32_VirtualFree(Base, 0, MEM_RELEASE);
}

internal void
Platform_ClipCursor(win32_window Window)
{
    win32_rect ClipRect;
    Win32_GetClientRect(Window, &ClipRect);
    Win32_ClientToScreen(Window, (win32_point*)&ClipRect.Left);
    Win32_ClientToScreen(Window, (win32_point*)&ClipRect.Right);
    Win32_ClipCursor(&ClipRect);
}

internal void
Platform_DisableCursor(loader_state *LoaderState,
                       cursor_flags CursorFlags)
{
    LoaderState->CursorIsDisabled = TRUE;
    
    win32_point CursorPos;
    Win32_GetCursorPos(&CursorPos);
    Win32_ScreenToClient(LoaderState->Window, &CursorPos);
    LoaderState->RestoreCursorPos = FORCE_CAST(CursorPos, v2s32);
    
    Win32_SetCursor(NULL);
    
    win32_rect WindowRect;
    Win32_GetClientRect(LoaderState->Window, &WindowRect);
    win32_point CenterPos = (win32_point){WindowRect.Right / 2, WindowRect.Bottom / 2};
    LoaderState->PrevCursorPos = FORCE_CAST(CenterPos, v2s32);
    Win32_ClientToScreen(LoaderState->Window, &CenterPos);
    Win32_SetCursorPos(CenterPos.X, CenterPos.Y);
    
    Platform_ClipCursor(LoaderState->Window);
    
    if(CursorFlags & CURSOR_RAW_BIT)
    {
        win32_raw_input_device RawInputDevice = {0};
        RawInputDevice.UsagePage = HID_USAGE_PAGE_GENERIC;
        RawInputDevice.Usage = HID_USAGE_GENERIC_MOUSE;
        RawInputDevice.Target = LoaderState->Window;
        
        if(!Win32_RegisterRawInputDevices(&RawInputDevice, 1, sizeof(win32_raw_input_device)))
        {
            Platform_ThrowError_DEBUG("Failed to register raw input device\n");
        }
    }
}

internal void
Platform_EnableCursor(loader_state *LoaderState,
                      cursor_flags CursorFlags)
{
    if(CursorFlags & CURSOR_RAW_BIT)
    {
        win32_raw_input_device RawInputDevice = {0};
        RawInputDevice.UsagePage = HID_USAGE_PAGE_GENERIC;
        RawInputDevice.Usage = HID_USAGE_GENERIC_MOUSE;
        RawInputDevice.Flags = RIDEV_REMOVE;
        RawInputDevice.Target = NULL;
        
        if(!Win32_RegisterRawInputDevices(&RawInputDevice, 1, sizeof(RawInputDevice)))
        {
            Platform_ThrowError_DEBUG("Failed to remove raw input device\n");
        }
    }
    
    LoaderState->CursorIsDisabled = FALSE;
    Win32_ClipCursor(NULL);
    
    LoaderState->PrevCursorPos = LoaderState->RestoreCursorPos;
    win32_point RestorePoint = FORCE_CAST(LoaderState->RestoreCursorPos, win32_point);
    Win32_ClientToScreen(LoaderState->Window, &RestorePoint);
    Win32_SetCursorPos(RestorePoint.X, RestorePoint.Y);
    
    Win32_SetCursor(Win32_LoadCursorW(NULL, IDC_ARROW_W));
}

internal void
Platform_ProcessMessages(platform_state *PlatformState)
{
    win32_message Message;
    while(Win32_PeekMessageW(&Message, ((loader_state*)PlatformState->LoaderState)->Window, 0, 0, PM_REMOVE))
    {
        if(Message.Message == WM_QUIT)
        {
            PlatformState->ExecutionState = EXECUTION_ENDED;
            break;
        }
        
        Win32_TranslateMessage(&Message);
        Win32_DispatchMessageW(&Message);
    }
}

internal u32 _API_ENTRY
Platform_ThreadCallback(vptr Parameter)
{
    loader_thread_info *ThreadInfo = (loader_thread_info*)Parameter;
    
    while(TRUE)
    {
        if(!Platform_ExecuteWorkQueueEntry(ThreadInfo->Queue))
        {
            Win32_WaitForSingleObjectEx(ThreadInfo->Queue->Semaphore, INFINITE, FALSE);
        }
    }
}

internal s64 _API_ENTRY
Platform_WindowCallback(win32_window Window,
                        u32 Message,
                        u64 WParam,
                        s64 LParam)
{
    platform_module *PlatformModule = Win32_GetPropA(Window, "PlatformModule");
    game_module *GameModule = Win32_GetPropA(Window, "GameModule");
    if(!PlatformModule || !GameModule)
    {
        return Win32_DefWindowProcW(Window, Message, WParam, LParam);
    }
    
    platform_state *PlatformState = &PlatformModule->State;
    loader_state *LoaderState = (loader_state*)PlatformState->LoaderState;
    game_input *GameInput = &GameModule->Input;
    
    switch(Message)
    {
        case WM_MOUSEACTIVATE:
        {
            if(HIGH_16(LParam) == WM_LBUTTONDOWN)
            {
                if(LOW_16(LParam) != HTCLIENT)
                {
                    PlatformState->FocusState = FOCUS_FRAME;
                }
            }
        } break;
        
        case WM_CAPTURECHANGED:
        {
            if(LParam == 0 &&
               PlatformState->FocusState == FOCUS_FRAME)
            {
                if(PlatformState->CursorFlags & CURSOR_DISABLED_BIT)
                {
                    Platform_DisableCursor(LoaderState, PlatformState->CursorFlags);
                }
                
                PlatformState->FocusState = FOCUS_CLIENT;
            }
        } break;
        
        case WM_SETFOCUS:
        {
            if(PlatformState->FocusState == FOCUS_FRAME)
            {
                PlatformState->ExecutionState = EXECUTION_PAUSED;
                break;
            }
            PlatformState->FocusState = FOCUS_CLIENT;
            
            if(PlatformState->CursorFlags & CURSOR_DISABLED_BIT)
            {
                Platform_DisableCursor(LoaderState, PlatformState->CursorFlags);
            }
            
            PlatformState->ExecutionState = EXECUTION_RUNNING;
            
            return 0;
        } break;
        
        case WM_KILLFOCUS:
        {
            PlatformState->FocusState = FOCUS_NONE;
            
            if(PlatformState->CursorFlags & CURSOR_DISABLED_BIT)
            {
                Platform_EnableCursor(LoaderState, PlatformState->CursorFlags);
            }
            
            PlatformState->ExecutionState = EXECUTION_PAUSED;
            
            return 0;
        } break;
        
        case WM_SYSCOMMAND:
        {
            switch(WParam & 0xFFF0)
            {
                case SC_KEYMENU:
                {
                    return 0;
                } break;
            }
        } break;
        
        case WM_INPUTLANGCHANGE:
        {
            Platform_UpdateKeyNames(PlatformState);
        } break;
        
        case WM_CHAR:
        case WM_SYSCHAR:
        case WM_UNICHAR:
        {
            if(Message == WM_UNICHAR &&
               WParam == UNICODE_NOCHAR)
            {
                return TRUE;
            }
            
            return 0;
        } break;
        
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP:
        {
            key_state Action = (HIGH_16(LParam) & KF_UP) ? KEY_RELEASED : KEY_PRESSED;
            u32 Scancode = (HIGH_16(LParam) & (KF_EXTENDED | 0xFF));
            if(!Scancode)
            {
                Scancode = Win32_MapVirtualKeyW((u32)WParam, MAPVK_VK_TO_VSC);
            }
            
            u08 Keycode = PlatformState->Keycodes[Scancode];
            
            if(WParam == VK_CONTROL)
            {
                if(HIGH_16(LParam) & KF_EXTENDED)
                {
                    Keycode = KEYCODE_RIGHT_CONTROL;
                }
                else
                {
                    s64 MessageTime = Win32_GetMessageTime();
                    win32_message Next;
                    if(Win32_PeekMessageW(&Next, NULL, 0, 0, PM_NOREMOVE))
                    {
                        if(Next.Message == WM_SYSKEYDOWN ||
                           Next.Message == WM_SYSKEYUP ||
                           Next.Message == WM_KEYDOWN ||
                           Next.Message == WM_KEYUP)
                        {
                            if(Next.WParam == VK_MENU &&
                               (HIGH_16(Next.LParam) & KF_EXTENDED) &&
                               Next.Time == MessageTime)
                            {
                                break;
                            }
                        }
                    }
                    Keycode = KEYCODE_LEFT_CONTROL;
                }
            }
            else if(WParam == VK_PROCESSKEY)
            {
                break;
            }
            
            if(WParam == VK_SHIFT &&
               Action == KEY_RELEASED)
            {
                GameInput->Keys[KEYCODE_LEFT_SHIFT] = KEY_RELEASED;
                GameInput->Keys[KEYCODE_RIGHT_SHIFT] = KEY_RELEASED;
            }
            else if(WParam == KEYCODE_PRINT_SCREEN)
            {
                break;
            }
            else
            {
                GameInput->Keys[Keycode] = Action;
            }
        } break;
        
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_XBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
        case WM_XBUTTONUP:
        {
            u32 Button;
            if(Message == WM_LBUTTONDOWN ||
               Message == WM_LBUTTONUP)
            {
                Button = BUTTON_LEFT;
            }
            else if(Message == WM_RBUTTONDOWN ||
                    Message == WM_RBUTTONUP)
            {
                Button = BUTTON_RIGHT;
            }
            else if(Message == WM_MBUTTONDOWN ||
                    Message == WM_MBUTTONUP)
            {
                Button = BUTTON_MIDDLE;
            }
            else if(GET_XBUTTON_WPARAM(WParam) == XBUTTON1)
            {
                Button = BUTTON_4;
            }
            else
            {
                Button = BUTTON_5;
            }
            
            u32 Action;
            if(Message == WM_LBUTTONDOWN ||
               Message == WM_RBUTTONDOWN ||
               Message == WM_MBUTTONDOWN ||
               Message == WM_XBUTTONDOWN)
            {
                Action = BUTTON_PRESSED;
            }
            else
            {
                Action = BUTTON_RELEASED;
            }
            
            //NOTE: Captures for dragging if any button is pressed:
            u32 ButtonIndex;
            for(ButtonIndex = 0;
                ButtonIndex <= BUTTON_LAST;
                ++ButtonIndex)
            {
                if(GameInput->Buttons[ButtonIndex] == BUTTON_PRESSED)
                {
                    break;
                }
            }
            
            if(ButtonIndex > BUTTON_LAST)
            {
                Win32_SetCapture(Window);
            }
            
            GameInput->Buttons[Button] = Action;
            
            //NOTE: Releases capture after all buttons are released
            for(ButtonIndex = 0;
                ButtonIndex <= BUTTON_LAST;
                ++ButtonIndex)
            {
                if(GameInput->Buttons[ButtonIndex] == BUTTON_PRESSED)
                {
                    break;
                }
            }
            
            if(ButtonIndex > BUTTON_LAST)
            {
                Win32_ReleaseCapture();
            }
            
            if(Message == WM_XBUTTONDOWN ||
               Message == WM_XBUTTONUP)
            {
                return TRUE;
            }
            
            return 0;
        } break;
        
        case WM_MOUSEMOVE:
        {
            v2s32 CursorPos = V2s32_2x1(GET_X_LPARAM(LParam), GET_Y_LPARAM(LParam));
            
            if((PlatformState->CursorFlags & CURSOR_ENTERED_BIT) == 0)
            {
                win32_track_mouse_event TrackMouseEventStruct = {0};
                TrackMouseEventStruct.Size = sizeof(TrackMouseEventStruct);
                TrackMouseEventStruct.Flags = TME_LEAVE;
                TrackMouseEventStruct.WindowTrack = LoaderState->Window;
                
                Win32_TrackMouseEvent(&TrackMouseEventStruct);
                
                PlatformState->CursorFlags |= CURSOR_ENTERED_BIT;
            }
            
            if(PlatformState->CursorFlags & CURSOR_DISABLED_BIT)
            {
                if(PlatformState->CursorFlags & CURSOR_RAW_BIT ||
                   !LoaderState->CursorIsDisabled)
                {
                    break;
                }
                
                v2s32 DeltaPos = V2s32_Sub(CursorPos, LoaderState->PrevCursorPos);
                GameInput->CursorPos = V2s32_Add(GameInput->CursorPos, DeltaPos);
            }
            else
            {
                GameInput->CursorPos = CursorPos;
            }
            
            LoaderState->PrevCursorPos = CursorPos;
            
            return 0;
        } break;
        
        case WM_INPUT:
        {
            if((PlatformState->CursorFlags & CURSOR_RAW_BIT) == 0 ||
               !LoaderState->CursorIsDisabled)
            {
                break;
            }
            
            u32 Size;
            u32 RawInputSize = Heap_GetSize(LoaderState->RawInput);
            
            Win32_GetRawInputData((win32_raw_input_handle)LParam, RID_INPUT, NULL, &Size, sizeof(win32_raw_input_header));
            if(Size > RawInputSize)
            {
                Heap_Resize(&LoaderState->RawInput, Size);
            }
            
            Size = (u32)RawInputSize;
            if(Win32_GetRawInputData((win32_raw_input_handle)LParam, RID_INPUT,
                               LoaderState->RawInput, &Size, sizeof(win32_raw_input_header)) == U32_MAX)
            {
                Platform_ThrowError_DEBUG("Wil32: Failed to retrieve raw input data\n");
            }
            
            v2s32 DeltaPos;
            win32_raw_input *Data = (win32_raw_input*)LoaderState->RawInput;
            if(Data->Data.Mouse.Flags & MOUSE_MOVE_ABSOLUTE)
            {
                DeltaPos = V2s32_Sub(FORCE_CAST(Data->Data.Mouse.LastX, v2s32), LoaderState->PrevCursorPos);
            }
            else
            {
                DeltaPos = FORCE_CAST(Data->Data.Mouse.LastX, v2s32);
            }
            
            GameInput->CursorPos = V2s32_Add(GameInput->CursorPos, DeltaPos);
            LoaderState->PrevCursorPos = V2s32_Add(LoaderState->PrevCursorPos, DeltaPos);
        } break;
        
        case WM_MOUSELEAVE:
        {
            PlatformState->CursorFlags &= ~CURSOR_ENTERED_BIT;
            
            return 0;
        } break;
        
        case WM_MOUSEWHEEL:
        {
            GameInput->DeltaWheelY += (r32)GET_WHEEL_DELTA_WPARAM(WParam) / (r32)WHEEL_DELTA;
            
            return 0;
        } break;
        
        case WM_MOUSEHWHEEL:
        {
            GameInput->DeltaWheelX -= (r32)GET_WHEEL_DELTA_WPARAM(WParam) / (r32)WHEEL_DELTA;
            
            return 0;
        } break;
        
        case WM_ENTERSIZEMOVE:
        case WM_ENTERMENULOOP:
        {
            if(PlatformState->FocusState & FOCUS_FRAME)
            {
                break;
            }
            
            if(PlatformState->CursorFlags & CURSOR_DISABLED_BIT)
            {
                Platform_EnableCursor(LoaderState, PlatformState->CursorFlags);
            }
        } break;
        
        case WM_EXITSIZEMOVE:
        case WM_EXITMENULOOP:
        {
            if(PlatformState->FocusState & FOCUS_FRAME)
            {
                break;
            }
            
            if(PlatformState->CursorFlags & CURSOR_DISABLED_BIT)
            {
                Platform_DisableCursor(LoaderState, PlatformState->CursorFlags);
            }
        } break;
        
        case WM_SIZE:
        {
            if(LoaderState->CursorIsDisabled)
            {
                Platform_ClipCursor(LoaderState->Window);
            }
            
            PlatformState->Size = V2u32_2x1(LOW_16(LParam), HIGH_16(LParam));
            
            //TODO: Maximization
            
            PlatformState->UpdateFlags |= UPDATE_SIZE;
            if(WParam == SIZE_MINIMIZED)
            {
                PlatformState->WindowFlags |= WINDOW_MINIMIZED;
            }
            else
            {
                PlatformState->WindowFlags &= ~WINDOW_MINIMIZED;
            }
            if(WParam == SIZE_MAXIMIZED)
            {
                PlatformState->WindowFlags |= WINDOW_MAXIMIZED;
            }
            else
            {
                PlatformState->WindowFlags &= ~WINDOW_MAXIMIZED;
            }
            
            return 0;
        } break;
        
        case WM_MOVE:
        {
            if(LoaderState->CursorIsDisabled)
            {
                Platform_ClipCursor(LoaderState->Window);
            }
            
            return 0;
        } break;
        
        case WM_GETMINMAXINFO:
        {
            win32_min_max_info *MinMaxInfo = (win32_min_max_info*)LParam;
            s32 MaxSize = MAX(MinMaxInfo->MinTrackSize.X, MinMaxInfo->MinTrackSize.Y);
            MaxSize = MAX(MaxSize, 200);
            MinMaxInfo->MinTrackSize.X = MaxSize;
            MinMaxInfo->MinTrackSize.Y = MaxSize;
            
            return 0;
        } break;
        
        case WM_ERASEBKGND:
        {
            return TRUE;
        } break;
        
        case WM_CLOSE:
        case WM_DESTROY:
        {
            PlatformState->ExecutionState = EXECUTION_ENDED;
            
            return 0;
        } break;
    }
    
    return Win32_DefWindowProcW(Window, Message, WParam, LParam);
}

external void _API_ENTRY
Platform_Entry(void)
{
    platform_module PlatformModule = {0};
    platform_state *PlatformState = &PlatformModule.State;
    platform_exports *PlatformExports = &PlatformModule.Exports;
    loader_state LoaderState = {0};
    PlatformModule.State.LoaderState = &LoaderState;
    
    util_module UtilModule = {0};
    util_state *UtilState = &UtilModule.State;
    util_exports *UtilExports = &UtilModule.Exports;
    
    game_module GameModule = {0};
    game_state *GameState = &GameModule.State;
    game_input *GameInput = &GameModule.Input;
    
    Platform_InitFunctions(PlatformExports);
    
    win32_instance Instance = Win32_GetModuleHandleA(NULL);
    
    win32_window_class_w WindowClass = {0};
    WindowClass.Style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
    WindowClass.Instance = Instance;
    WindowClass.Cursor = Win32_LoadCursorW(Instance, IDC_ARROW_W);
    WindowClass.WindowCallback = Platform_WindowCallback;
    WindowClass.ClassName = L"VoxarcWindowClass";
    
    if(!Win32_RegisterClassW(&WindowClass))
    {
        Platform_ThrowError_DEBUG("Failed to register window class\n");
    }
    
    win32_rect WindowRect = (win32_rect){0, 0, 1280, 800};
    
    win32_window Window = Win32_CreateWindowExW(WS_EX_OVERLAPPEDWINDOW, WindowClass.ClassName, L"Voxarc",
                                                WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                                CW_USEDEFAULT, CW_USEDEFAULT,
                                                WindowRect.Right - WindowRect.Left,
                                                WindowRect.Bottom - WindowRect.Top,
                                                NULL, NULL, Instance, NULL);
    
    if(!Window)
    {
        Platform_ThrowError_DEBUG("Failed to create window\n");
    }
    
    Win32_SetPropA(Window, "PlatformModule", &PlatformModule);
    Win32_SetPropA(Window, "GameModule", &GameModule);
    
    //TODO: https://handmade.network/wiki/2918-timers_and_frames_per_second
    
    win32_device_context DeviceContext = Win32_GetDC(Window);
    Platform_InitOpenGLFunctions(PlatformExports, DeviceContext);
    Platform.Loaded = TRUE;
    
    PlatformState->AllocationUsed = 0;
    PlatformState->AllocationSize = MEBIBYTES(16);
    PlatformState->AllocationBase = Platform_AllocateMemory(PlatformState->AllocationSize);
    
    Platform_LoadUtilDll(PlatformExports, &UtilModule, &LoaderState.UtilDll);
    Platform_LoadGameDll(PlatformExports, UtilExports, &GameModule, &LoaderState.GameDll);
    
    u32 HeapSize = MEBIBYTES(1);
    ASSERT(PlatformState->AllocationUsed + HeapSize <= PlatformState->AllocationSize);
    LoaderState.UtilHeap = Heap_Create(PlatformState->AllocationBase, HeapSize);
    PlatformState->AllocationUsed += HeapSize;
    
    LoaderState.RawInput = Heap_Allocate(LoaderState.UtilHeap, 0);
    LoaderState.Window = Window;
    
    //NOTE: Threading
    #if 0
    //TODO: Thread percent worload utilized
    loader_thread_info ThreadInfos[7];
    platform_work_queue WorkQueue = {0};
    u32 InitialCount = 0;
    u32 ThreadCount = ARRAY_COUNT(ThreadInfos);
    WorkQueue.Entries = Heap_Allocate(LoaderState.UtilHeap, MAX_WORK_QUEUE_ENTRIES * sizeof(platform_work_queue_entry));
    WorkQueue.Semaphore = CreateSemaphoreExW(NULL, InitialCount, ThreadCount, NULL, 0, SEMAPHORE_ALL_ACCESS);
    
    for(u32 ThreadIndex = 0;
        ThreadIndex < ThreadCount;
        ++ThreadIndex)
    {
        loader_thread_info *ThreadInfo = ThreadInfos + ThreadIndex;
        ThreadInfo->Queue = &WorkQueue;
        
        u32 ThreadId;
        win32_handle Thread = CreateThread(NULL, 0, Platform_ThreadCallback, ThreadInfo, 0, &ThreadId);
        CloseHandle(Thread);
    }
    #endif
    
    PlatformState->ExecutionState = EXECUTION_RUNNING;
    PlatformState->FocusState = FOCUS_CLIENT;
    
    PlatformState->UpdateFlags |= UPDATE_SIZE;
    PlatformState->CursorFlags |= CURSOR_RAW_BIT;
    PlatformState->SupportFlags |= SUPPORT_RAW_INPUT_BIT;
    
    Win32_GetClientRect(Window, &WindowRect);
    PlatformState->Size = FORCE_CAST(WindowRect.Right, v2u32);
    
    PlatformState->LoaderState = &LoaderState;
    
    win32_large_integer PerformanceCounter;
    Win32_QueryPerformanceCounter(&PerformanceCounter);
    
    Rand_Init(PerformanceCounter.LowPart);
    Game_Init(PlatformState, GameState);
    
    //TODO: https://gafferongames.com/post/fix_your_timestep/
    while(PlatformState->ExecutionState != EXECUTION_ENDED)
    {
        #if _MODE_DEBUG
        {
            //TODO: Make this thread safe
            win32_filetime GameDllWriteTime = Platform_GetFileDataA(DLL_PATH("Game")).LastWriteTime;
            win32_filetime UtilDllWriteTime = Platform_GetFileDataA(DLL_PATH("Util")).LastWriteTime;
            if(Win32_CompareFileTime(&GameDllWriteTime, &LoaderState.GameDll.LastWritten) > 0)
            {
                Platform_UnloadGameDll(&GameModule, &LoaderState.GameDll);
                Platform_LoadGameDll(PlatformExports, UtilExports, &GameModule, &LoaderState.GameDll);
                FLAG_SET(PlatformState->UpdateFlags, UPDATE_RELOADED);
            }
            if(Win32_CompareFileTime(&UtilDllWriteTime, &LoaderState.UtilDll.LastWritten) > 0)
            {
                Platform_UnloadUtilDll(&UtilModule, &LoaderState.UtilDll);
                Platform_LoadUtilDll(PlatformExports, &UtilModule, &LoaderState.UtilDll);
            }
        }
        #endif
        
        if(PlatformState->ExecutionState == EXECUTION_PAUSED)
        {
            Win32_WaitMessage();
        }
        
        // Win32CreateWorkQueueEntry(&WorkQueue, Platform_ProcessMessages, Platform.S);
        Platform_ProcessMessages(PlatformState);
        
        //TODO: This hogs the CPU, put it to sleep
        if(PlatformState->ExecutionState == EXECUTION_RUNNING)
        {
            if((GameInput->Keys[KEYCODE_LEFT_ALT] == KEY_PRESSED ||
                GameInput->Keys[KEYCODE_RIGHT_ALT] == KEY_PRESSED) &&
               GameInput->Keys[KEYCODE_F4] == KEY_PRESSED)
            {
                PlatformState->ExecutionState = EXECUTION_ENDED;
                break;
            }
            
            if(LoaderState.CursorIsDisabled)
            {
                win32_rect Rect;
                Win32_GetClientRect(LoaderState.Window, &Rect);
                win32_point CenterPos = (win32_point){Rect.Right / 2, Rect.Bottom / 2};
                
                if(!V2s32_Equal(LoaderState.PrevCursorPos, FORCE_CAST(CenterPos, v2s32)))
                {
                    LoaderState.PrevCursorPos = FORCE_CAST(CenterPos, v2s32);
                    Win32_ClientToScreen(LoaderState.Window, &CenterPos);
                    Win32_SetCursorPos(CenterPos.X, CenterPos.Y);
                }
            }
            
            Game_Update(PlatformState, &GameModule);
            
            Win32_SwapBuffers(DeviceContext);
        }
    }
    
    Game_Cleanup(UtilState, GameState);
    Platform_FreeMemory(PlatformState->AllocationBase);
    ASSERT(GlobalAllocCounter == 0);
    
    Win32_ExitProcess(0);
}