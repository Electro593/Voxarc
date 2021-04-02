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

global_var s32 GlobalAllocCounter;

internal win32_teb *
Win32_GetTEB(void)
{
    win32_teb *Result;
    
    Result = (win32_teb*)__readgsqword(OFFSET_OF(win32_nt_tib, Self));
    
    return Result;
}

internal win32_ldr_data_table_entry *
Win32_GetLoaderTableEntry(u32 Index)
{
    win32_ldr_data_table_entry *Result;
    
    win32_teb *TEB = Win32_GetTEB();
    win32_list_entry *Entry = TEB->ProcessEnvironmentBlock->Ldr->InMemoryOrderModuleList.Flink;
    while(Index--)
    {
        Entry = Entry->Flink;
    }
    Result = (win32_ldr_data_table_entry*)((u08*)Entry - sizeof(win32_list_entry));
    
    return Result;
}

internal win32_unicode_string
Win32_GetProcessPath(void)
{
    win32_unicode_string Result;
    
    win32_ldr_data_table_entry *VoxarcEntry = Win32_GetLoaderTableEntry(0);
    Result = VoxarcEntry->FullDllName;
    
    return Result;
}

internal void
Win32_InitFunctions(platform_callbacks *PlatformCallbacks)
{
    win32_ldr_data_table_entry *KernelEntry = Win32_GetLoaderTableEntry(2);
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
    GetProcAddress = (_type__GetProcAddress*)PTR_JUMP(Kernel32, GetProcAddressRVA);
    
    LoadLibraryA = (_type__LoadLibraryA*)GetProcAddress(Kernel32, "LoadLibraryA");
    win32_module User32 = LoadLibraryA("user32.dll");
    win32_module Gdi32 = LoadLibraryA("gdi32.dll");
    win32_module Opengl32 = LoadLibraryA("opengl32.dll");
    
#if defined(_VOX_DEBUG)
    OutputDebugStringA = (_type__OutputDebugStringA*)GetProcAddress(Kernel32, "OutputDebugStringA");
    if(!OutputDebugStringA)
    {
        ASSERT(FALSE);
    }
#endif
    
#if defined(_VOX_DEBUG)
    b08 WinReturnError = FALSE;
    
    #define PROC(Module, ReturnType, Name, ...) \
        Name = (_type__##Name*)GetProcAddress(Module, #Name); \
        if(!Name) \
        { \
            OutputDebugStringA("Function " #Name " was not found.\n"); \
            WinReturnError = TRUE; \
        }
        WIN32_PROCS
    #undef PROC
    
    if(WinReturnError)
    {
        ASSERT(FALSE);
    }
#else
    #define PROC(Module, ReturnType, Name, ...) \
        Name = (_type__##Name*)GetProcAddress(Module, #Name); \
        if(!Name) \
        { \
            //TODO: Do something else here
            ExitProcess(1); \
        }
        WIN32_PROCS
    #undef PROC
#endif
    
#if defined(_VOX_DEBUG)
    b08 GlReturnError = FALSE;
    
    #define PROC(ReturnType, Name, ...) \
        GL_##Name = PlatformCallbacks->GL_##Name = (_type__GL_##Name*)GetProcAddress(Opengl32, "gl" #Name); \
        if(!GL_##Name) \
        { \
            OutputDebugStringA("Function GL_" #Name " was not found.\n"); \
            GlReturnError = TRUE; \
        }
        GL_PLATFORM_PROCS
    #undef PROC
    
    if(GlReturnError)
    {
        ASSERT(FALSE);
    }
#else
    #define PROC(ReturnType, Name, ...) \
        GL_##Name = (_type__GL_##Name*)GetProcAddress(Opengl32, "gl" #Name); \
        if(!GL_##Name) \
        { \
            //TODO: Do something else here
            ExitProcess(1); \
        }
        GL_PLATFORM_PROCS
    #undef PROC
#endif
    
    #define PROC(ReturnType, Name, ...) \
        Platform_##Name = PlatformCallbacks->Platform_##Name = Win32_##Name;
    PLATFORM_CALLBACKS
    #undef PROC
    
    #define PROC(ReturnType, Name, ...) \
        PlatformCallbacks->Name = Name;
    PLATFORM_FUNCS
    #undef PROC
    
    win32_window_class_w DummyWindowClass = {0};
    DummyWindowClass.Style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
    DummyWindowClass.WindowCallback = DefWindowProcW;
    DummyWindowClass.Instance = GetModuleHandleW(0);
    DummyWindowClass.ClassName = L"VoxarcOGLDummyWindowClass";
    
    RegisterClassW(&DummyWindowClass);
    vptr DummyWindow = CreateWindowExW(0, DummyWindowClass.ClassName, L"VoxarcOGLDummy", 0,
                                       CW_USEDEFAULT, CW_USEDEFAULT,
                                       CW_USEDEFAULT, CW_USEDEFAULT,
                                       0, 0, DummyWindowClass.Instance, 0);
    
    vptr DummyDeviceContext = GetDC(DummyWindow);
    
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
    
    s32 PixelFormat = ChoosePixelFormat(DummyDeviceContext, &PixelFormatDescriptor);
    SetPixelFormat(DummyDeviceContext, PixelFormat, &PixelFormatDescriptor);
    vptr DummyRenderContext = wglCreateContext(DummyDeviceContext);
    wglMakeCurrent(DummyDeviceContext, DummyRenderContext);
    
    
#if defined(_VOX_DEBUG)
    b08 WglReturnError = FALSE;
    
    #define PROC(ReturnType, Name, ...) \
        WGL_##Name = (_type__WGL_##Name*)wglGetProcAddress("wgl" #Name); \
        if(!WGL_##Name) \
        { \
            OutputDebugStringA("Function WGL_" #Name " was not found.\n"); \
            WglReturnError = TRUE; \
        }
        WGL_PROCS
    #undef PROC
    
    if(WglReturnError)
    {
        ASSERT(FALSE);
    }
#else
    #define PROC(ReturnType, Name, ...) \
        WGL_##Name = (_type__WGL_##Name*)wglGetProcAddress("wgl" #Name); \
        if(!WGL_##Name) \
        { \
            //TODO: Do something else here
            ExitProcess(1); \
        }
        WGL_PROCS
    #undef PROC
#endif
    
    wglMakeCurrent(DummyDeviceContext, 0);
    wglDeleteContext(DummyRenderContext);
    ReleaseDC(DummyWindow, DummyDeviceContext);
    DestroyWindow(DummyWindow);
}

internal win32_opengl_render_context
Win32_InitOpenGLFunctions(platform_callbacks *PlatformCallbacks, win32_device_context DeviceContext)
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
    DescribePixelFormat(DeviceContext, PixelFormat, sizeof(PixelFormatDescriptor), &PixelFormatDescriptor);
    
    #if defined(_VOX_DEBUG)
    u32 DebugBit = WGL_CONTEXT_DEBUG_BIT_ARB;
    #else
    u32 DebugBit = 0;
    #endif
    
    SetPixelFormat(DeviceContext, PixelFormat, &PixelFormatDescriptor);
    s32 AttribList[] = {WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
                        WGL_CONTEXT_MINOR_VERSION_ARB, 6,
                        WGL_CONTEXT_FLAGS_ARB, DebugBit | WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
                        WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                        0};
    
    win32_opengl_render_context RenderContext = WGL_CreateContextAttribsARB(DeviceContext, 0, AttribList);
    wglMakeCurrent(DeviceContext, RenderContext);
    
#if defined(_VOX_DEBUG)
    b08 GlReturnError = FALSE;
    
    #define PROC(ReturnType, Name, ...) \
        GL_##Name = PlatformCallbacks->GL_##Name = (_type__GL_##Name*)wglGetProcAddress("gl" #Name); \
        if(!GL_##Name) \
        { \
            OutputDebugStringA("Function GL_" #Name " was not found.\n"); \
            GlReturnError = TRUE; \
        }
        GL_PROCS
    #undef PROC
    
    if(GlReturnError)
    {
        ASSERT(FALSE);
    }
#else
    #define PROC(ReturnType, Name, ...) \
        GL_##Name = PlatformCallbacks->GL_##Name = (_type__GL_##Name*)wglGetProcAddress("gl" #Name); \
        if(!GL_##Name) \
        { \
            //TODO: Do something else here
            ExitProcess(1); \
        }
        GL_PROCS
    #undef PROC
#endif
    
    return RenderContext;
}

#if defined(_VOX_DEBUG)
internal void
Win32_ThrowError_DEBUG(chr *Message)
{
    Win32_Print_DEBUG(Message);
    STOP;
    ExitProcess(1);
}

internal void
Win32_Print_DEBUG(chr *Message)
{
    OutputDebugStringA(Message);
}
#endif

internal void
Win32_LoadDll(win32_dll *DllData,
              chr *Path,
              chr *LockedPath,
              chr *LoadFuncName)
{
    ASSERT(DllData->DLL == NULL);
    
    CopyFileA(Path, LockedPath, FALSE);
    
    DllData->DLL = LoadLibraryA(LockedPath);
    if(DllData->DLL)
    {
        DllData->LastWritten = Win32_GetFileDataA(LockedPath).LastWriteTime;
        DllData->LoadFunc = GetProcAddress(DllData->DLL, LoadFuncName);
    }
}

internal void
Win32_UnloadDll(win32_dll *DllData)
{
    if(DllData->DLL)
    {
        FreeLibrary(DllData->DLL);
        DllData->DLL = NULL;
    }
}

internal void
Win32_LoadGameDll(win32_dll *GameDll,
                  platform_callbacks *PlatformCallbacks,
                  game_exports *Functions,
                  game_state *State)
{
    Win32_LoadDll(GameDll, GAME_DLL_PATH, GAME_DLL_LOCKED_PATH, GAME_DLL_LOAD_FUNC_NAME);
    Game_Load = (_type__Game_Load*)GameDll->LoadFunc;
    Game_Load(PlatformCallbacks, Functions, State);
    
    #define PROC(ReturnType, Name, ...) \
        Name = Functions->Name;
    GAME__EXPORTS
    #undef PROC
}

internal void
Win32_UnloadGameDll(win32_dll *GameDll,
                    game_exports *Functions,
                    game_state *State)
{
    Game_Unload(Functions, State);
    Win32_UnloadDll(GameDll);
}

internal void
Win32_InitKeyTables(platform_state *PlatformState)
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

internal void
Win32_UpdateKeyNames(platform_state *PlatformState)
{
    u08 State[256] = {0};
    Mem_Set(PlatformState->Keynames, 0, sizeof(PlatformState->Keynames));
    
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
        
        u32 VirtualKey = MapVirtualKeyW(Scancode, MAPVK_VSC_TO_VK);
        u32 Length = ToUnicode(VirtualKey, Scancode, State, Buffer, ARRAY_COUNT(Buffer), 0);
        if(Length == -1)
        {
            Length = ToUnicode(VirtualKey, Scancode, State, Buffer, ARRAY_COUNT(Buffer), 0);    
        }
        
        if(Length < 1)
        {   
            continue;
        }
        
        WideCharToMultiByte(CP_UTF8, 0, Buffer, 1, PlatformState->Keynames[Keycode], sizeof(PlatformState->Keynames[Keycode]), NULL, NULL);
    }
}

internal void
Win32_CreateWorkQueueEntry(platform_work_queue *WorkQueue,
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
    ReleaseSemaphore(WorkQueue->Semaphore, 1, 0);
}

internal b08
Win32_ExecuteWorkQueueEntry(platform_work_queue *WorkQueue)
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
Win32_FlushWorkQueue(platform_work_queue *WorkQueue)
{
    while(WorkQueue->ExecutedEntryCount < WorkQueue->CreatedEntryCount)
    {
        Win32_ExecuteWorkQueueEntry(WorkQueue);
    }
    
    WorkQueue->ExecutedEntryCount = 0;
    WorkQueue->CreatedEntryCount = 0;
}

// internal win32_find_data_w
// Win32_GetFileDataW(wstr FileName)
// {
//     win32_find_data_w Result;
    
//     win32_handle FindHandle = FindFirstFileW(FileName, &Result);
//     if(FindHandle != INVALID_HANDLE_VALUE)
//     {
//         FindClose(FindHandle);
//     }
//     else
//     {
//         Win32_ThrowError_DEBUG("Failed to find file\n");
//     }
    
//     return Result;
// }

internal win32_find_data_a
Win32_GetFileDataA(chr *FileName)
{
    win32_find_data_a Result;
    
    win32_handle FindHandle = FindFirstFileA(FileName, &Result);
    if(FindHandle != INVALID_HANDLE_VALUE)
    {
        FindClose(FindHandle);
    }
    else
    {
        Win32_ThrowError_DEBUG("Failed to find file\n");
    }
    
    return Result;
}

internal size
Win32_GetFileLength(vptr FileHandle)
{
    win32_large_integer Size;
    Size.QuadPart = 0;
    
    if(GetFileSizeEx(*(win32_handle*)FileHandle, &Size) == 0)
    {
        Win32_ThrowError_DEBUG("Failed to get size of file\n");
    }
    
    return Size.QuadPart;
}

internal void
Win32_CloseFile(vptr FileHandle)
{
    CloseHandle(*(win32_handle*)FileHandle);
}

internal void
Win32_CloseFileType(vptr FindHandle,
                    file_data *FileData)
{
    Heap_Free(FileData->FileName);
    FindClose(*(win32_handle*)FindHandle);
}

internal b08
Win32_OpenFile(vptr FileHandle,
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
    
    win32_handle GivenHandle = CreateFileA(FileName, DesiredAccess, FILE_SHARE_READ,
                                           NULL, CreationDisposition, FILE_ATTRIBUTE_NORMAL, NULL);
    
    u32 Error = GetLastError();
    if(Error == ERROR_PATH_NOT_FOUND)
    {
        u32 NestLevel = 0;
        u32 SlashIndex = 0;
        u32 FinalSlashIndex = Str_FindChar(FileName, '/', -1);
        
        do
        {
            SlashIndex = Str_FindChar(FileName, '/', NestLevel++);
            str DirStr = Str_Sub(NULL, FileName, 0, SlashIndex);
            
            CreateDirectoryA(DirStr, NULL);
            Str_Free(DirStr);
        }
        while(SlashIndex < FinalSlashIndex);
        
        GivenHandle = CreateFileA(FileName, DesiredAccess, FILE_SHARE_READ,
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
Win32_CountFilesOfTypeInDir(file_type FileType)
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
    
    win32_handle FindHandle = FindFirstFileA(Selector, NULL);
    if(FindHandle != INVALID_HANDLE_VALUE)
    {
        ++Result;
        
        while(FindNextFileA(FindHandle, NULL))
        {
            ++Result;
        }
        
        FindClose(FindHandle);
    }
    
    return Result;
}

internal b08
Win32_OpenFileType(vptr FileHandle,
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
    win32_handle GivenFindHandle = FindFirstFileA(Selector, &FindData);
    Str_Free(Selector);
    
    if(GivenFindHandle != INVALID_HANDLE_VALUE)
    {
        ASSERT(sizeof(win32_handle) == sizeof(vptr));
        *(win32_handle*)FindHandle = *(win32_handle*)(&GivenFindHandle);
        
        str FileNameFull = Str_Create(NULL, FileData->RelDir, 0);
        Str_CatC(&FileNameFull, FindData.FileName);
        Result = Win32_OpenFile(FileHandle, FileNameFull, FileMode_Read);
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
Win32_OpenNextFile(vptr FileHandle,
                   vptr FindHandle,
                   file_data *FileData)
{
    b08 Result = FALSE;
    win32_find_data_a FindData;
    
    ASSERT(FileData->FileName);
    
    if(FindNextFileA(*(win32_handle*)FindHandle, &FindData))
    {
        str FileNameFull = Str_Create(NULL, FileData->RelDir, 0);
        Str_CatC(&FileNameFull, FindData.FileName);
        Result = Win32_OpenFile(FileHandle, FileNameFull, FileMode_Read);
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
Win32_ReadFile(vptr Dest,
               vptr FileHandle,
               size Length,
               size Offset)
{
    win32_overlapped Overlapped = {0};
    Overlapped.Offset = (u32)((Offset >> 0) & U32_MAX);
    Overlapped.OffsetHigh = (u32)((Offset >> 32) & U32_MAX);
    u32 BytesRead;
    
    ASSERT(Dest);
    
    if(!ReadFile(*(vptr*)FileHandle, Dest, (u32)Length, &BytesRead, &Overlapped))
    {
        Win32_ThrowError_DEBUG("Failed to read data from file\n");
    }
    ASSERT(BytesRead == Length);
}

internal void
Win32_WriteFile(vptr Src,
                vptr FileHandle,
                size Length,
                size Offset)
{
    win32_overlapped Overlapped = {0};
    Overlapped.Offset = (u32)((Offset >> 0) & U32_MAX);
    Overlapped.OffsetHigh = (u32)((Offset >> 32) & U32_MAX);
    u32 BytesWritten;
    
    if(!WriteFile(*(vptr*)FileHandle, Src, (u32)Length, &BytesWritten, &Overlapped))
    {
        Win32_ThrowError_DEBUG("Failed to write data to file\n");
    }
    ASSERT(BytesWritten == Length);
}

internal vptr
Win32_AllocateMemory(size Size)
{
    ++GlobalAllocCounter;
    
    vptr MemoryBlock = VirtualAlloc(0, Size, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
    ASSERT(MemoryBlock);
    // vptr MemoryBlock = HeapAlloc(GetProcessHeap(), 0, Size);
    return MemoryBlock;
}

internal void
Win32_FreeMemory(vptr Base)
{
    --GlobalAllocCounter;
    
    VirtualFree(Base, 0, MEM_RELEASE);
}

internal void
Win32_ClipCursor(win32_window Window)
{
    win32_rect ClipRect;
    GetClientRect(Window, &ClipRect);
    ClientToScreen(Window, (win32_point*)&ClipRect.Left);
    ClientToScreen(Window, (win32_point*)&ClipRect.Right);
    ClipCursor(&ClipRect);
}

internal void
Win32_DisableCursor(win32_data *Win32Data,
                    cursor_flags CursorFlags)
{
    Win32Data->CursorIsDisabled = TRUE;
    
    win32_point CursorPos;
    GetCursorPos(&CursorPos);
    ScreenToClient(Win32Data->Window, &CursorPos);
    Win32Data->RestoreCursorPos = FORCE_CAST(CursorPos, v2s32);
    
    SetCursor(NULL);
    
    win32_rect WindowRect;
    GetClientRect(Win32Data->Window, &WindowRect);
    win32_point CenterPos = (win32_point){WindowRect.Right / 2, WindowRect.Bottom / 2};
    Win32Data->PrevCursorPos = FORCE_CAST(CenterPos, v2s32);
    ClientToScreen(Win32Data->Window, &CenterPos);
    SetCursorPos(CenterPos.X, CenterPos.Y);
    
    Win32_ClipCursor(Win32Data->Window);
    
    if(CursorFlags & CURSOR_RAW_BIT)
    {
        win32_raw_input_device RawInputDevice = {0};
        RawInputDevice.UsagePage = HID_USAGE_PAGE_GENERIC;
        RawInputDevice.Usage = HID_USAGE_GENERIC_MOUSE;
        RawInputDevice.Target = Win32Data->Window;
        
        if(!RegisterRawInputDevices(&RawInputDevice, 1, sizeof(win32_raw_input_device)))
        {
            Win32_ThrowError_DEBUG("Failed to register raw input device\n");
        }
    }
}

internal void
Win32_EnableCursor(win32_data *Win32Data,
                   cursor_flags CursorFlags)
{
    if(CursorFlags & CURSOR_RAW_BIT)
    {
        win32_raw_input_device RawInputDevice = {0};
        RawInputDevice.UsagePage = HID_USAGE_PAGE_GENERIC;
        RawInputDevice.Usage = HID_USAGE_GENERIC_MOUSE;
        RawInputDevice.Flags = RIDEV_REMOVE;
        RawInputDevice.Target = NULL;
        
        if(!RegisterRawInputDevices(&RawInputDevice, 1, sizeof(RawInputDevice)))
        {
            Win32_ThrowError_DEBUG("Failed to remove raw input device\n");
        }
    }
    
    Win32Data->CursorIsDisabled = FALSE;
    ClipCursor(NULL);
    
    Win32Data->PrevCursorPos = Win32Data->RestoreCursorPos;
    win32_point RestorePoint = FORCE_CAST(Win32Data->RestoreCursorPos, win32_point);
    ClientToScreen(Win32Data->Window, &RestorePoint);
    SetCursorPos(RestorePoint.X, RestorePoint.Y);
    
    SetCursor(LoadCursorW(NULL, IDC_ARROW_W));
}

internal void
Win32_ProcessMessages(vptr Parameter)
{
    platform_state *PlatformState = (platform_state*)Parameter;
    
    win32_message Message;
    while(PeekMessageW(&Message, ((win32_data*)PlatformState->PlatformData)->Window, 0, 0, PM_REMOVE))
    {
        if(Message.Message == WM_QUIT)
        {
            PlatformState->ExecutionState = EXECUTION_ENDED;
            break;
        }
        
        TranslateMessage(&Message);
        DispatchMessageW(&Message);
    }
}

internal u32 _API_ENTRY
Win32_ThreadCallback(vptr Parameter)
{
    win32_thread_info *ThreadInfo = (win32_thread_info*)Parameter;
    
    while(TRUE)
    {
        if(!Win32_ExecuteWorkQueueEntry(ThreadInfo->Queue))
        {
            WaitForSingleObjectEx(ThreadInfo->Queue->Semaphore, INFINITE, FALSE);
        }
    }
}

internal s64 _API_ENTRY
Win32_WindowCallback(win32_window Window,
                     u32 Message,
                     u64 WParam,
                     s64 LParam)
{
    platform_state *PlatformState = GetPropW(Window, L"PlatformState");
    
    if(!PlatformState)
    {
        return DefWindowProcW(Window, Message, WParam, LParam);
    }
    
    game_input *GameInput = GetPropW(Window, L"GameInput");
    win32_data *Win32Data = (win32_data*)PlatformState->PlatformData;
    
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
                    Win32_DisableCursor(Win32Data, PlatformState->CursorFlags);
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
                Win32_DisableCursor(Win32Data, PlatformState->CursorFlags);
            }
            
            PlatformState->ExecutionState = EXECUTION_RUNNING;
            
            return 0;
        } break;
        
        case WM_KILLFOCUS:
        {
            PlatformState->FocusState = FOCUS_NONE;
            
            if(PlatformState->CursorFlags & CURSOR_DISABLED_BIT)
            {
                Win32_EnableCursor(Win32Data, PlatformState->CursorFlags);
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
            Win32_UpdateKeyNames(PlatformState);
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
                Scancode = MapVirtualKeyW((u32)WParam, MAPVK_VK_TO_VSC);
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
                    s64 MessageTime = GetMessageTime();
                    win32_message Next;
                    if(PeekMessageW(&Next, NULL, 0, 0, PM_NOREMOVE))
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
                SetCapture(Window);
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
                ReleaseCapture();
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
                TrackMouseEventStruct.WindowTrack = Win32Data->Window;
                
                TrackMouseEvent(&TrackMouseEventStruct);
                
                PlatformState->CursorFlags |= CURSOR_ENTERED_BIT;
            }
            
            if(PlatformState->CursorFlags & CURSOR_DISABLED_BIT)
            {
                if(PlatformState->CursorFlags & CURSOR_RAW_BIT ||
                   !Win32Data->CursorIsDisabled)
                {
                    break;
                }
                
                v2s32 DeltaPos = V2s32_Sub(CursorPos, Win32Data->PrevCursorPos);
                GameInput->CursorPos = V2s32_Add(GameInput->CursorPos, DeltaPos);
            }
            else
            {
                GameInput->CursorPos = CursorPos;
            }
            
            Win32Data->PrevCursorPos = CursorPos;
            
            return 0;
        } break;
        
        case WM_INPUT:
        {
            if((PlatformState->CursorFlags & CURSOR_RAW_BIT) == 0 ||
               !Win32Data->CursorIsDisabled)
            {
                break;
            }
            
            u32 Size;
            u32 RawInputSize = Heap_GetSize(Win32Data->RawInput);
            
            GetRawInputData((win32_raw_input_handle)LParam, RID_INPUT, NULL, &Size, sizeof(win32_raw_input_header));
            if(Size > RawInputSize)
            {
                Heap_Resize(&Win32Data->RawInput, Size);
            }
            
            Size = (u32)RawInputSize;
            if(GetRawInputData((win32_raw_input_handle)LParam, RID_INPUT,
                               Win32Data->RawInput, &Size, sizeof(win32_raw_input_header)) == U32_MAX)
            {
                Win32_ThrowError_DEBUG("Wil32: Failed to retrieve raw input data\n");
            }
            
            v2s32 DeltaPos;
            win32_raw_input *Data = (win32_raw_input*)Win32Data->RawInput;
            if(Data->Data.Mouse.Flags & MOUSE_MOVE_ABSOLUTE)
            {
                DeltaPos = V2s32_Sub(FORCE_CAST(Data->Data.Mouse.LastX, v2s32), Win32Data->PrevCursorPos);
            }
            else
            {
                DeltaPos = FORCE_CAST(Data->Data.Mouse.LastX, v2s32);
            }
            
            GameInput->CursorPos = V2s32_Add(GameInput->CursorPos, DeltaPos);
            Win32Data->PrevCursorPos = V2s32_Add(Win32Data->PrevCursorPos, DeltaPos);
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
                Win32_EnableCursor(Win32Data, PlatformState->CursorFlags);
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
                Win32_DisableCursor(Win32Data, PlatformState->CursorFlags);
            }
        } break;
        
        case WM_SIZE:
        {
            if(Win32Data->CursorIsDisabled)
            {
                Win32_ClipCursor(Win32Data->Window);
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
            if(Win32Data->CursorIsDisabled)
            {
                Win32_ClipCursor(Win32Data->Window);
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
    
    return DefWindowProcW(Window, Message, WParam, LParam);
}

external void
WinMainCRTStartup(void)
{
    win32_data Win32Data = {0};
    
    platform_callbacks PlatformCallbacks = {0};
    platform_state PlatformState = {0};
    Win32_InitFunctions(&PlatformCallbacks);
    
    game_exports GameExports = {0};
    game_state GameState = {0};
    game_input GameInput = {0};
    Win32_LoadGameDll(&Win32Data.GameDll, &PlatformCallbacks, &GameExports, &GameState);
    
    u32 UtilHeapSize = MEBIBYTES(2);
    PlatformState.AllocationSize = MEBIBYTES(8);
    u08 *MemoryBase = Win32_AllocateMemory(PlatformState.AllocationSize + UtilHeapSize);
    PlatformState.AllocationBase = MemoryBase;
    
    Win32Data.UtilHeap = Heap_Create(MemoryBase, UtilHeapSize);
    Win32Data.RawInput = Heap_Allocate(Win32Data.UtilHeap, 0);
    
    win32_instance Instance = GetModuleHandleA(NULL);
    
    win32_window_class_w WindowClass = {0};
    WindowClass.Style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
    WindowClass.Instance = Instance;
    WindowClass.Cursor = LoadCursorW(Instance, IDC_ARROW_W);
    WindowClass.WindowCallback = Win32_WindowCallback;
    WindowClass.ClassName = L"VoxarcWindowClass";
    
    if(!RegisterClassW(&WindowClass))
    {
        Win32_ThrowError_DEBUG("Failed to register window class\n");
    }
    
    win32_rect WindowRect = (win32_rect){0, 0, 1280, 800};
    
    win32_window Window = CreateWindowExW(WS_EX_OVERLAPPEDWINDOW, WindowClass.ClassName, L"Voxarc",
                                          WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                          CW_USEDEFAULT, CW_USEDEFAULT,
                                          WindowRect.Right - WindowRect.Left,
                                          WindowRect.Bottom - WindowRect.Top,
                                          NULL, NULL, Instance, NULL);
    
    if(!Window)
    {
        Win32_ThrowError_DEBUG("Failed to create window\n");
    }
    
    //TODO: https://handmade.network/wiki/2918-timers_and_frames_per_second
    
    win32_device_context DeviceContext = GetDC(Window);
    Win32_InitOpenGLFunctions(&PlatformCallbacks, DeviceContext);
    
    Game_Load(&PlatformCallbacks, NULL, NULL);
    
    Win32Data.Window = Window;
    
    //NOTE: Threading
    #if 0
    //TODO: Thread percent worload utilized
    win32_thread_info ThreadInfos[7];
    platform_work_queue WorkQueue = {0};
    u32 InitialCount = 0;
    u32 ThreadCount = ARRAY_COUNT(ThreadInfos);
    WorkQueue.Entries = Heap_Allocate(Win32Data.UtilHeap, MAX_WORK_QUEUE_ENTRIES * sizeof(platform_work_queue_entry));
    WorkQueue.Semaphore = CreateSemaphoreExW(NULL, InitialCount, ThreadCount, NULL, 0, SEMAPHORE_ALL_ACCESS);
    
    for(u32 ThreadIndex = 0;
        ThreadIndex < ThreadCount;
        ++ThreadIndex)
    {
        win32_thread_info *ThreadInfo = ThreadInfos + ThreadIndex;
        ThreadInfo->Queue = &WorkQueue;
        
        u32 ThreadId;
        win32_handle Thread = CreateThread(NULL, 0, Win32_ThreadCallback, ThreadInfo, 0, &ThreadId);
        CloseHandle(Thread);
    }
    #endif
    
    PlatformState.ExecutionState = EXECUTION_RUNNING;
    PlatformState.FocusState = FOCUS_CLIENT;
    
    PlatformState.UpdateFlags |= UPDATE_SIZE;
    PlatformState.CursorFlags |= CURSOR_RAW_BIT;
    PlatformState.SupportFlags |= SUPPORT_RAW_INPUT_BIT;
    
    GetClientRect(Window, &WindowRect);
    PlatformState.Size = FORCE_CAST(WindowRect.Right, v2u32);
    
    PlatformState.PlatformData = &Win32Data;
    
    SetPropW(Window, L"PlatformState", &PlatformState);
    SetPropW(Window, L"GameInput", &GameInput);
    
    win32_large_integer PerformanceCounter;
    QueryPerformanceCounter(&PerformanceCounter);
    
    Rand_Init(PerformanceCounter.LowPart);
    Game_Init(&PlatformState, &GameState, &GameInput);
    
    //TODO: https://gafferongames.com/post/fix_your_timestep/
    while(PlatformState.ExecutionState != EXECUTION_ENDED)
    {
        #if defined(_VOX_DEBUG)
        {
            //TODO: Make this thread safe
            win32_filetime GameDllWriteTime = Win32_GetFileDataA(GAME_DLL_PATH).LastWriteTime;
            if(CompareFileTime(&GameDllWriteTime, &Win32Data.GameDll.LastWritten) > 0)
            {
                Win32_UnloadGameDll(&Win32Data.GameDll, &GameExports, &GameState);
                Win32_LoadGameDll(&Win32Data.GameDll, &PlatformCallbacks, &GameExports, &GameState);
            }
        }
        #endif
        
        if(PlatformState.ExecutionState == EXECUTION_PAUSED)
        {
            WaitMessage();
        }
        
        // Win32CreateWorkQueueEntry(&WorkQueue, Win32_ProcessMessages, PlatformState);
        Win32_ProcessMessages(&PlatformState);
        
        //TODO: This hogs the CPU, put it to sleep
        if(PlatformState.ExecutionState == EXECUTION_RUNNING)
        {
            if((GameInput.Keys[KEYCODE_LEFT_ALT] == KEY_PRESSED ||
                GameInput.Keys[KEYCODE_RIGHT_ALT] == KEY_PRESSED) &&
               GameInput.Keys[KEYCODE_F4] == KEY_PRESSED)
            {
                PlatformState.ExecutionState = EXECUTION_ENDED;
                break;
            }
            
            if(Win32Data.CursorIsDisabled)
            {
                win32_rect Rect;
                GetClientRect(Win32Data.Window, &Rect);
                win32_point CenterPos = (win32_point){Rect.Right / 2, Rect.Bottom / 2};
                
                if(!V2s32_Equal(Win32Data.PrevCursorPos, FORCE_CAST(CenterPos, v2s32)))
                {
                    Win32Data.PrevCursorPos = FORCE_CAST(CenterPos, v2s32);
                    ClientToScreen(Win32Data.Window, &CenterPos);
                    SetCursorPos(CenterPos.X, CenterPos.Y);
                }
            }
            
            Game_Update(&PlatformState, &GameState, &GameInput);
            
            SwapBuffers(DeviceContext);
        }
    }
    
    Game_Cleanup(&PlatformState, &GameState, &GameInput);
    Win32_FreeMemory(MemoryBase);
    ASSERT(GlobalAllocCounter == 0);
    
    ExitProcess(0);
}