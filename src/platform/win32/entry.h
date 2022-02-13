/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define FONTS_DIR    "assets\\fonts\\"
#define SHADERS_DIR  "assets\\shaders\\"
#define TEXTURES_DIR "assets\\textures\\"

typedef win32_handle file_handle;

internal void _Assert(c08 *File, u32 Line, c08 *Expression, c08 *Message);
internal b08 Platform_OpenFile(file_handle *FileHandle, c08 *FileName, file_mode OpenMode);
internal u64 Platform_GetFileLength(file_handle FileHandle);
internal u64 Platform_ReadFile(file_handle FileHandle, vptr Dest, u64 Length, u64 Offset);
internal u64 Platform_WriteFile(file_handle FileHandle, vptr Src, u64 Length, u64 Offset);
internal void Platform_CloseFile(file_handle FileHandle);