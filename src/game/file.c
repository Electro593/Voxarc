/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

internal string
File_Read(c08 *FileName,
          u64 Length,
          u64 Offset)
{
    file_handle FileHandle;
    Assert(Platform_OpenFile(&FileHandle, FileName, FILE_READ),
           "Invalid file name!");
    
    if(Length == 0) {
        Length = Platform_GetFileLength(FileHandle);
    }
    
    vptr Text = Stack_Allocate(Length);
    u64 BytesRead = Platform_ReadFile(FileHandle, Text, Length, Offset);
    string String = {BytesRead, Length, FALSE, Text};
    
    Platform_CloseFile(FileHandle);
    
    return String;
}