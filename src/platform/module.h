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

#ifndef PLATFORM_MODULE_H_
#define PLATFORM_MODULE_H_

#define PLATFORM__MODULE__EXPORTS \
    // FUNC(void _API_ENTRY, Platform_Entry, void) \

#define PLATFORM__LOADER__EXPORTS \
    FUNC(void, Platform_ThrowError_DEBUG, chr *Message) \
    FUNC(void, Platform_Print_DEBUG, chr *Message) \
    FUNC(u64,  Platform_GetFileLength, vptr FileHandle) \
    FUNC(void, Platform_CloseFile, vptr FileHandle) \
    FUNC(void, Platform_CloseFileType, vptr FindHandle, file_data *FileData) \
    FUNC(void, Platform_CreateWorkQueueEntry, platform_work_queue *WorkQueue, platform_work_queue_callback Function, vptr Parameter) \
    FUNC(void, Platform_FlushWorkQueue, platform_work_queue *WorkQueue) \
    FUNC(b08,  Platform_OpenFile, vptr FileHandle, str FileName, file_mode FileMode) \
    FUNC(u32,  Platform_CountFilesOfTypeInDir, file_type FileType) \
    FUNC(b08,  Platform_OpenFileType, vptr FileHandle, vptr FindHandle, file_data *FileData, file_type FileType, file_group FileGroup) \
    FUNC(b08,  Platform_OpenNextFile, vptr FileHandle, vptr FindHandle, file_data *FileData) \
    FUNC(void, Platform_ReadFile, vptr Dest, vptr FileHandle, u64 Length, u64 Offset) \
    FUNC(void, Platform_WriteFile, vptr Src, vptr FileHandle, u64 Length, u64 Offset) \
    FUNC(vptr, Platform_AllocateMemory, u64 Size) \
    FUNC(void, Platform_FreeMemory, vptr Base) \

#define PLATFORM__EXPORTS \
    PLATFORM__MODULE__EXPORTS \
    PLATFORM__LOADER__EXPORTS \
    PLATFORM__GL__EXPORTS \

#define PLATFORM__PROCS \
    PLATFORM__GL__PROCS \

#define PLATFORM__FUNCS \
    PLATFORM__EXPORTS \
    PLATFORM__LOADER__FUNCS \
    PLATFORM__GL__FUNCS \

#define FUNC(ReturnType, Name, ...) \
    typedef _API_PLATFORM ReturnType _type__##Name(__VA_ARGS__);
#define PROC(Lib, ReturnType, LibName, Name, ...) \
    typedef _API_PLATFORM ReturnType _type__##Name(__VA_ARGS__); \
    global _type__##Name *Name;
PLATFORM__EXPORTS
PLATFORM__PROCS
PLATFORM__LOADER__PROCS
#undef FUNC
#undef PROC

struct platform_exports
{
    #define FUNC(ReturnType, Name, ...) \
        _type__##Name *Name;
    #define PROC(Lib, ReturnType, LibName, Name, ...) \
        _type__##Name *Name;
    PLATFORM__EXPORTS
    PLATFORM__PROCS
    #undef FUNC
    #undef PROC
};
struct platform_module
{
    platform_exports Exports;
    platform_state State;
    b08 Loaded;
};
global platform_module Platform;

#if _MODULE_PLATFORM
    #define FUNC(ReturnType, Name, ...) \
        internal ReturnType Name(__VA_ARGS__);
    PLATFORM__FUNCS
    #undef FUNC
    
    #define FUNC(ReturnType, Name, ...) \
        external ReturnType Name(__VA_ARGS__);
    PLATFORM__MODULE__EXPORTS
    #undef FUNC
#else
    #define FUNC(ReturnType, Name, ...) \
        global _type__##Name *Name;
    PLATFORM__EXPORTS
    #undef FUNC
#endif

#endif