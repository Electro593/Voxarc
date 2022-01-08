/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <shared.h>

global b08 Running;
global u32 MainThreadID;

internal void
LoadWin32Funcs(void)
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
    
    #define PROC(Module, ReturnType, Name, ...) \
        Win32_##Name = (func_Win32_##Name*)Win32_GetProcAddress(Module, #Name);
    WIN32_FUNCS
    #undef PROC
}

internal s64 API_ENTRY
ServiceWindowCallback(win32_window ServiceWindow,
                      u32 Message,
                      s64 WParam,
                      s64 LParam)
{
    switch(Message) {
        case WM_CREATE_WINDOW: {
            win32_window_class_a *WindowClass = (win32_window_class_a*)WParam;
            win32_window MainWindow = Win32_CreateWindowExA(0, WindowClass->ClassName, "Voxarc",
                                                            WS_OVERLAPPED|WS_SYSMENU|WS_CAPTION|WS_VISIBLE,
                                                            CW_USEDEFAULT, CW_USEDEFAULT,
                                                            CW_USEDEFAULT, CW_USEDEFAULT,
                                                            NULL, NULL, WindowClass->Instance, NULL);
            return (s64)MainWindow;
        };
    }
    
    return Win32_DefWindowProcA(ServiceWindow, Message, WParam, LParam);
}

internal s64 API_ENTRY
MainWindowCallback(win32_window ServiceWindow,
                   u32 Message,
                   s64 WParam,
                   s64 LParam)
{
    if(Message==WM_QUIT) Win32_OutputDebugStringA("MainCallback: WM_QUIT\n");
    if(Message==WM_CLOSE) Win32_OutputDebugStringA("MainCallback: WM_CLOSE\n");
    if(Message==WM_DESTROY) Win32_OutputDebugStringA("MainCallback: WM_DESTROY\n");
    
    switch(Message) {
        case WM_CLOSE: {
            Win32_PostThreadMessageA(MainThreadID, Message, (s64)ServiceWindow, LParam);
        } return 0;
        
        // case WM_: {
        //     Win32_PostThreadMessageA(MainThreadID, Message, WParam, LParam);
        // } return 0;
    }
    
    return Win32_DefWindowProcA(ServiceWindow, Message, WParam, LParam);
}

external void API_ENTRY
Main(win32_window ServiceWindow)
{
    // loader_dll GameDLL;
    // Platform_LoadGameDLL(&GameDLL);
    
    win32_window_class_a WindowClass = {0};
    WindowClass.Callback = MainWindowCallback;
    WindowClass.Instance = Win32_GetModuleHandleA(NULL);
    WindowClass.Icon = Win32_LoadIconA(NULL, IDI_APPLICATION);
    WindowClass.Cursor = Win32_LoadCursorA(NULL, IDC_ARROW);
    WindowClass.Background = (win32_brush)Win32_GetStockObject(BRUSH_BLACK);
    WindowClass.ClassName = "VoxarcMainWindowClass";
    Win32_RegisterClassA(&WindowClass);
    
    win32_window Window = (win32_window)Win32_SendMessageA(ServiceWindow, WM_CREATE_WINDOW,
                                                           (s64)&WindowClass, 0);
    ASSERT(Window);
    
    //TODO: Better loop
    while(Running) {
        win32_message Message;
        while(Win32_PeekMessageA(&Message, NULL, 0, 0, PM_REMOVE)) {
            switch(Message.Message) {
                case WM_QUIT:
                case WM_CLOSE:
                case WM_DESTROY: {
                    if(Message.Message==WM_QUIT) Win32_OutputDebugStringA("Main: WM_QUIT\n");
                    if(Message.Message==WM_CLOSE) Win32_OutputDebugStringA("Main: WM_CLOSE\n");
                    if(Message.Message==WM_DESTROY) Win32_OutputDebugStringA("Main: WM_DESTROY\n");
                    Running = FALSE;
                } break;
                
                // case WM_SETFOCUS: {
                //     Platform->ExecutionState = Execution_Running;
                //     FLAG_SET(Platform->AudioState, AudioFlag_Enabled);
                //     Game->Mem_Set(Platform->Input.Keys, 0, sizeof(Platform->Input.Keys));
                // } break;
                
                // case WM_KILLFOCUS:
                // {
                //     Platform->ExecutionState = Execution_Paused;
                //     FLAG_RESET(Platform->AudioState, AudioFlag_Enabled);
                // } break;
                
                case WM_SYSKEYDOWN:
                case WM_SYSKEYUP:
                case WM_KEYDOWN:
                case WM_KEYUP:
                {
                    u08 ScanCode    = (Message.LParam >> 16) & 0xFF;
                    b08 IsExtended  = (Message.LParam >> 24) & 0x01;
                    b08 WasDown     = (Message.LParam >> 30) & 0x01;
                    b08 IsUp        = (Message.LParam >> 31) & 0x01;
                    
                    key_state KeyState;
                    if(IsUp == TRUE && WasDown == TRUE) KeyState = Key_Released;
                    else if(IsUp == FALSE && WasDown == FALSE) KeyState = Key_Pressed;
                    else KeyState = Key_Repeated;
                    
                    if(IsExtended)
                    {
                        // Exclude NumLock
                        if(ScanCode != 0x45)
                            ScanCode |= 0x80;
                    }
                    else
                    {
                        // Pause key
                        if(ScanCode == 0x45)
                            ScanCode = 0xFF;
                        // Alt + PrintScreen
                        else if(ScanCode == 0x54)
                            ScanCode = 0x80 | 0x37;
                    }
                    
                    Platform->Input.Keys[ScanCode] = KeyState;
                } break;
                
                default: {
                    STOP;
                }
            }
        }
        
        if(ExecutionState == Execution_Running)
        {
            if((Keys[ScanCode_AltLeft]  == Key_Pressed ||
                Keys[ScanCode_AltRight] == Key_Pressed) &&
               Keys[ScanCode_F4] == Key_Pressed)
            {
                ExecutionState = Execution_Stopped;
                break;
            }
        }
    }
    
    Win32_ExitProcess(0);
}

external void API_ENTRY
Entry(void)
{
    LoadWin32Funcs();
    
    win32_window_class_a WindowClass = {0};
    WindowClass.Callback = ServiceWindowCallback;
    WindowClass.Instance = Win32_GetModuleHandleA(NULL);
    WindowClass.ClassName = "VoxarcServiceWindowClass";
    Win32_RegisterClassA(&WindowClass);
    
    win32_window ServiceWindow = Win32_CreateWindowExA(0, WindowClass.ClassName, "VoxarcServiceWindow", 0,
                                                       CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                                                       NULL, NULL, WindowClass.Instance, NULL);
    
    Running = TRUE;
    Win32_CreateThread(NULL, 0, (func_Win32_ThreadCallback*)Main, ServiceWindow, 0, &MainThreadID);
    
    while(Running) {
        win32_message Message;
        Win32_GetMessageA(&Message, NULL, 0, 0));
        Win32_TranslateMessage(&Message);
        Win32_DispatchMessageA(&Message);
    }
}