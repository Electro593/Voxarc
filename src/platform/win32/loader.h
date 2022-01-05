/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef PLATFORM_WIN32_LOADER_H_
#define PLATFORM_WIN32_LOADER_H_

#if _MODE_DEBUG
    #define DLL_PATH(Module) ("build/Voxarc_" Module ".dll")
    #define DLL_LOCKED_PATH(Module) ("build/Voxarc_" Module "_Locked.dll")
#else
    #define DLL_PATH(Module) ("Voxarc_" Module ".dll")
    #define DLL_LOCKED_PATH(Module) ("Voxarc_" Module "_Locked.dll")
#endif
#define DLL_LOAD_FUNC_NAME(Module) (Module "_Load")

typedef struct loader_thread_info
{
    platform_work_queue *Queue;
} loader_thread_info;

typedef struct loader_dll
{
    win32_module DLL;
    fptr LoadFunc;
    win32_filetime LastWritten;
} loader_dll;

typedef struct loader_state
{
    loader_dll GameDll;
    loader_dll UtilDll;
    win32_window Window;
    heap *UtilHeap;
    hvptr RawInput; //win32_raw_input
    v2s32 PrevCursorPos;
    v2s32 RestoreCursorPos;
    b08 CursorIsDisabled;
} loader_state;

#define PLATFORM__LOADER__FUNCS \
    FUNC(win32_teb*,                  Platform_GetTEB,                void) \
    FUNC(win32_ldr_data_table_entry*, Platform_GetLoaderTableEntry,   u32 Index) \
    FUNC(win32_unicode_string,        Platform_GetProcessPath,        void) \
    FUNC(void,                        Platform_InitFunctions,         platform_exports *PlatformExports) \
    FUNC(win32_opengl_render_context, Platform_InitOpenGLFunctions,   platform_exports *PlatformExports, win32_device_context DeviceContext) \
    FUNC(void,                        Platform_InitKeyTables,         platform_state *PlatformState) \
    FUNC(void,                        Platform_LoadDll,               loader_dll *DllData, chr *Path, chr *LockedPath, chr *LoadFuncName) \
    FUNC(void,                        Platform_UnloadDll,             loader_dll *DllData) \
    FUNC(void,                        Platform_LoadGameDll,           platform_exports *PlatformExports, util_exports *UtilExports, game_module *GameModule, loader_dll *GameDll) \
    FUNC(void,                        Platform_LoadUtilDll,           platform_exports *PlatformExports, util_module *UtilModule, loader_dll *UtilDll) \
    FUNC(void,                        Platform_UnloadGameDll,         game_module *GameModule, loader_dll *GameDll) \
    FUNC(void,                        Platform_UnloadUtilDll,         util_module *UtilModule, loader_dll *UtilDll) \
    FUNC(void,                        Platform_UpdateKeyNames,        platform_state *PlatformState) \
    FUNC(b08,                         Platform_ExecuteWorkQueueEntry, platform_work_queue *WorkQueue) \
    FUNC(win32_find_data_a,           Platform_GetFileDataA,          chr *FileName) \
    FUNC(void,                        Platform_ClipCursor,            win32_window Window) \
    FUNC(void,                        Platform_DisableCursor,         loader_state *LoaderState, cursor_flags CursorFlags) \
    FUNC(void,                        Platform_EnableCursor,          loader_state *LoaderState, cursor_flags CursorFlags) \
    FUNC(void,                        Platform_ProcessMessages,       platform_state *PlatformState) \
    FUNC(u32 _API_ENTRY,              Platform_ThreadCallback,        vptr Parameter) \
    FUNC(s64 _API_ENTRY,              Platform_WindowCallback,        win32_window Window, u32 Message, u64 WParam, s64 LParam) \

#endif