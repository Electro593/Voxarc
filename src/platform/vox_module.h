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

#if !defined(PLATFORM_VOX_MODULE_H_)
#define      PLATFORM_VOX_MODULE_H_



#if defined(_VOX_DEBUG)
    #define PLATFORM_CALLBACKS_DEBUG \
        PROC(void, ThrowError_DEBUG, chr *Message) \
        PROC(void, Print_DEBUG, chr *Message) \

    #define PROC(ReturnType, Name, ...) \
        typedef ReturnType _type__Platform_##Name(__VA_ARGS__); \
        global_var _type__Platform_##Name *Platform_##Name;
    PLATFORM_CALLBACKS_DEBUG
    #undef PROC
#else
    #define PLATFORM_CALLBACKS_DEBUG
#endif

#define PLATFORM_CALLBACKS \
    PLATFORM_CALLBACKS_DEBUG \
    \
    PROC(u64,  GetFileLength, vptr FileHandle) \
    PROC(void, CloseFile, vptr FileHandle) \
    PROC(void, CloseFileType, vptr FindHandle, file_data *FileData) \
    PROC(void, CreateWorkQueueEntry, platform_work_queue *WorkQueue, platform_work_queue_callback Function, vptr Parameter) \
    PROC(void, FlushWorkQueue, platform_work_queue *WorkQueue) \
    PROC(b08, OpenFile, vptr FileHandle, str FileName, file_mode FileMode) \
    PROC(u32,  CountFilesOfTypeInDir, file_type FileType) \
    PROC(b08, OpenFileType, vptr FileHandle, vptr FindHandle, file_data *FileData, file_type FileType, file_group FileGroup) \
    PROC(b08, OpenNextFile, vptr FileHandle, vptr FindHandle, file_data *FileData) \
    PROC(void, ReadFile, vptr Dest, vptr FileHandle, u64 Length, u64 Offset) \
    PROC(void, WriteFile, vptr Src, vptr FileHandle, u64 Length, u64 Offset) \
    PROC(vptr, AllocateMemory, u64 Size) \
    PROC(void, FreeMemory, vptr Base) \

#define PROC(ReturnType, Name, ...) \
    typedef ReturnType _type__Platform_##Name(__VA_ARGS__); \
    global_var _type__Platform_##Name *Platform_##Name;
PLATFORM_CALLBACKS
#undef PROC

#define PLATFORM_FUNCS \
    PLATFORM_FUNCS_GL \

struct platform_callbacks
{
    #define PROC(ReturnType, Name, ...) \
        _type__Platform_##Name *Platform_##Name;
    PLATFORM_CALLBACKS
    #undef PROC
    
    #define PROC(ReturnType, Name, ...) \
        _type__##Name *Name;
    PLATFORM_FUNCS
    #undef PROC
    
    #define PROC(ReturnType, Name, ...) \
        _type__GL_##Name *GL_##Name;
    GL_PROCS
    GL_PLATFORM_PROCS
    #undef PROC
};

#endif