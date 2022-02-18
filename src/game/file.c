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

internal assetpack
File_LoadAssetpack(c08 *FileName)
{
    assetpack Assetpack;
    Stack_Push();
    
    string FileDataStr = File_Read(FileName, 0, 0);
    u08 *FileCursor = (u08*)FileDataStr.Text;
    u64 BaseOffset = (u64)FileCursor;
    
    Assetpack.Header = (assetpack_header*)FileCursor;
    FileCursor += sizeof(assetpack_header);
    Assetpack.Registries = (assetpack_tag_registry*)FileCursor;
    FileCursor += sizeof(assetpack_tag_registry) * Assetpack.Header->TagRegistryCount;
    Assetpack.Tags = (assetpack_tag*)FileCursor;
    FileCursor += sizeof(assetpack_tag) * Assetpack.Header->TagCount;
    Assetpack.TagData = FileCursor;
    FileCursor += Assetpack.Header->TagDataSize;
    Assetpack.Assets = (assetpack_asset*)FileCursor;
    FileCursor += sizeof(assetpack_asset) * Assetpack.Header->AssetCount;
    Assetpack.Atlases = (assetpack_atlas*)FileCursor;
    FileCursor += sizeof(assetpack_atlas) * Assetpack.Header->AtlasCount;
    Assetpack.Data = FileCursor;
    
    for(u32 R = 0; R < Assetpack.Header->TagRegistryCount; R++) {
        assetpack_tag_registry *Registry = Assetpack.Registries+R;
        (u08*)Registry->Tags += BaseOffset;
        if(Assetpack.Registries[R].Type.Size > sizeof(vptr)) {
            assetpack_tag *Tag = Registry->Tags;
            for(u32 T = 0; T < Registry->TagCount; T++) {
                *(u08*)Tag += BaseOffset;
                (u08*)Tag += sizeof(assetpack_tag) + Tag->AssetCount * sizeof(assetpack_asset);
                for(u32 A = 0; A < Tag->AssetCount; A++)
                    (u08*)(Tag->Assets[A]) += BaseOffset;
            }
        }
    }
    
    for(u32 A = 0; A < Assetpack.Header->AtlasCount; A++)
        (u08*)Assetpack.Atlases[A].Data += BaseOffset;
    
    Stack_Pop();
    return Assetpack;
}

internal void
File_CreateAssetpack(c08 *FileName)
{
    Stack_Push();
    
    assetpack_header Header = {0};
    heap_handle *Registries;
    heap_handle *Tags;
    heap_handle *TagData;
    heap_handle *Assets;
    heap_handle *Atlases;
    heap_handle *Data;
    
    {
        stbtt_fontinfo FontInfo;
        u08 *FontData = File_Read("assets\\fonts\\cour.ttf", 0, 0).Text;
        stbtt_InitFont(&FontInfo, FontData, stbtt_GetFontOffsetForIndex(FontData, 0));
        r32 Scale = stbtt_ScaleForPixelHeight(&FontInfo, 36.0f);
        s32 Ascent, Descent, LineGap;
        stbtt_GetFontVMetrics(&FontInfo, &Ascent, &Descent, &LineGap);
        Ascent  = (s32)((r32)Ascent  * Scale);
        Descent = (s32)((r32)Descent * Scale);
        LineGap = (s32)((r32)LineGap * Scale);
        
        Header.AssetCount = 127-32 + 1;
        Heap_Resize(Assets, (127-32)*sizeof(assetpack_texture) + sizeof(assetpack_font));
        
        assetpack_font *FontAsset = (assetpack_font*)Assets->Data;
        FontAsset->AdvanceY = Ascent - Descent + LineGap;
        
        assetpack_texture *Asset = (assetpack_texture*)(Assets->Data+sizeof(assetpack_font));
        for(c08 Codepoint = ' '; Codepoint <= '~'; Codepoint++) {
            Asset-> = (assetpack_texture)(Assets->Data + );
        }
    }
    
    
    
    
    
    
    
    file_handle FileHandle;
    Platform_OpenFile(&FileHandle, FileName, FILE_WRITE);
    u64 Offset = 0;
    Platform_WriteFile(FileHandle, &Header, sizeof(assetpack_header), Offset);
    Offset += sizeof(assetpack_header);
    Platform_WriteFile(FileHandle, Registries->Data, Registries->Size, Offset);
    Offset += Registries->Size;
    Platform_WriteFile(FileHandle, Tags->Data, Tags->Size, Offset);
    Offset += Tags->Size;
    Platform_WriteFile(FileHandle, TagData->Data, TagData->Size, Offset);
    Offset += TagData->Size;
    Platform_WriteFile(FileHandle, Assets->Data, Assets->Size, Offset);
    Offset += Assets->Size;
    Platform_WriteFile(FileHandle, Atlases->Data, Atlases->Size, Offset);
    Offset += Atlases->Size;
    Platform_WriteFile(FileHandle, Data->Data, Data->Size, Offset);
    Platform_CloseFile(FileHandle);
    
    Stack_Pop();
}