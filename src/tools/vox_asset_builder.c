#include "util/vox_defines.h"
#include "tools/vox_asset_builder.h"

/*
.vaai (Voxarc Asset Information) files are information for the texture atlas
.vaaa (Voxarc Asset Atlas) files are where the data is stored
*/

// Only one VAAI
/* assets.vaai
<File Header>
  Magic Number
  Version
  Type Count

<Type Table>
  All types:
    ID
    Asset Count
    Asset Offset

<Asset Table>
  All assets for each type:
    Tag Count
    Tag Offset
    Data Offset

<Tag Table>
  All tags for each asset:
    ID (Last 2 bits are data type)
      00 = U32, 01 = S32, 10 = R32
    Value

<Data Table>
  All assets for each type:
    (if Bitmap):
      Atlas ID
      Width
      Height
      OffsetX
      OffsetY
*/

// VAAA are titled by index
/* *.vaaa
<Data>
  Atlas of textures
*/

//TODO: Repack previously made VAAA's as well
local_func void
GenAssetPacks(s32 MaxTextureSize)
{
    size MemSize = Megabytes(256);
    vptr Base = ReserveMemory(MemSize);
    handle_pool *MemoryPool = CreateHandlePool(Base, MemSize);
    
    memory_handle *FileHandle = AllocateMemory(MemoryPool, sizeof(vptr));
    memory_handle *FindHandle = AllocateMemory(MemoryPool, sizeof(vptr));
    
    OpenFileType(FileHandle, FindHandle, FileType_Bitmap);
    while(*(vptr*)FileHandle->Base != NULL)
    {
        size FileSize = GetFileSize(FileHandle);
        memory_handle *FileData = AllocateMemory(MemoryPool, FileSize);
        ReadDataFromFile(FileData, FileHandle, 0);
        CloseFile(FileHandle);
        
        
        
        FreeMemory(FileData);
        OpenNextFile(FileHandle, FindHandle);
    }
    CloseFileType(FindHandle);
    FreeMemory(FindHandle);
    FreeMemory(FileHandle);
    
    ReleaseMemory(Base);
}