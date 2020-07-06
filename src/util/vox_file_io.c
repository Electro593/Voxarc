#include "vox_file_io.h"

local_func memory_handle *
VoxReadFile(memory_handle *Handle, c08 *FilePath)
{
    handle_pool *HandlePool = GetHandlePool(Handle);
    
    memory_handle *FileHandle = AllocateMemory(HandlePool, sizeof(vptr));
    OpenFile(FileHandle, FilePath);
    memory_handle *Text = AllocateMemory(HandlePool, GetFileSize(FileHandle));
    ReadDataFromFile(Text, FileHandle, 0);
    CloseFile(FileHandle);
    FreeMemory(FileHandle);
    return Text;
}

local_func config
ReadConfig(memory_handle *Handle, c08 *ConfigFilePath)
{
    //TODO: Implement type specifiers (U32:, V3S:, etc)
    
    handle_pool *HandlePool = GetHandlePool(Handle);
    
    config Config;
    SetMemory(&Config, 0, sizeof(Config));
    
    memory_handle *ConfigText = VoxReadFile(Handle, ConfigFilePath);
    memory_handle *Buffer = AllocateMemory(HandlePool, 256);
    memory_handle *TypeName = AllocateMemory(HandlePool, 256);
    
    u08 *Byte = (u08*)Buffer->Base;
    
    s32 I = 0;
    b08 Commented = FALSE;
    u08 *C = (u08*)ConfigText->Base;
    s32 TypeNameOffset = 0;
    while(*C != '\0')
    {
        if(*C == '#')
        {
            Commented = TRUE;
        }
        
        if(*C == '=' && !Commented)
        {
            CopyHandles(TypeName, Buffer);
            SetMemory(Buffer->Base, 0, Buffer->Size);
            Byte = (u08*)Buffer->Base;
            TypeNameOffset = I + 1;
            C++;
            continue;
        }
        if(*C =='\n')
        {
            if(!Commented)
            {
                if(String_EqualC(TypeName, "VisionBitsPerChannel"))
                {
                    Config.VisionBitsPerChannel = String_ToU32(Buffer->Base);
                }
                else if(String_EqualC(TypeName, "VisionChannelBinary"))
                {
                    Config.VisionChannelBinary = String_ToU32(Buffer->Base);
                }
                else if(String_EqualC(TypeName, "FOV"))
                {
                    Config.FOV = String_ToU32(Buffer->Base);
                }
                else if(String_EqualC(TypeName, "Sensitivity"))
                {
                    Config.Sensitivity = String_ToR32(Buffer->Base);
                }
                else if(String_EqualC(TypeName, "RenderDistance"))
                {
                    Config.RenderDistance = String_ToV3u32(Buffer->Base);
                }
                else if(String_EqualC(TypeName, "MoveLeft"))
                {
                    Config.MoveLeft = String_ToU32(Buffer->Base);
                }
                else if(String_EqualC(TypeName, "MoveForward"))
                {
                    Config.MoveForward = String_ToU32(Buffer->Base);
                }
                else if(String_EqualC(TypeName, "MoveRight"))
                {
                    Config.MoveRight = String_ToU32(Buffer->Base);
                }
                else if(String_EqualC(TypeName, "MoveBack"))
                {
                    Config.MoveBack = String_ToU32(Buffer->Base);
                }
                else if(String_EqualC(TypeName, "MoveUp"))
                {
                    Config.MoveUp = String_ToU32(Buffer->Base);
                }
                else if(String_EqualC(TypeName, "MoveDown"))
                {
                    Config.MoveDown = String_ToU32(Buffer->Base);
                }
            }
            SetMemory(Buffer->Base, 0, Buffer->Size);
            SetMemory(TypeName->Base, 0, TypeName->Size);
            Byte = (u08*)Buffer->Base;
            TypeNameOffset = I + 1;
            C++;
            Commented = FALSE;
            continue;
        }
        
        *Byte++ = *C++;
        while(*C != '\n' && IsSpace(*C))
        {
            C++;
        }
    }
    
    FreeMemory(ConfigText);
    FreeMemory(Buffer);
    FreeMemory(TypeName);
    
    return Config;
}

local_func u32
LoadShaders(memory_handle *Handle, c08 *VertexShaderFilePath, c08 *FragmentShaderFilePath)
{
    s32 Result = FALSE;
    s32 InfoLogLength;
    
    handle_pool *HandlePool = GetHandlePool(Handle);
    
    u32 VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    memory_handle *VertexShaderCode = VoxReadFile(Handle, VertexShaderFilePath);
    glShaderSource(VertexShaderID, 1, &(const c08*)VertexShaderCode->Base, 0);
    glCompileShader(VertexShaderID);
    
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    memory_handle *VertexShaderErrorMessage = AllocateMemory(HandlePool, InfoLogLength);
    glGetShaderInfoLog(VertexShaderID, InfoLogLength, 0, (c08*)VertexShaderErrorMessage->Base);
    if(!Result)
    {
        //TODO: Logging
    }
    FreeMemory(VertexShaderErrorMessage);
    FreeMemory(VertexShaderCode);
    
    u32 FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    memory_handle *FragmentShaderCode = VoxReadFile(Handle, FragmentShaderFilePath);
    glShaderSource(FragmentShaderID, 1, &(const c08*)FragmentShaderCode->Base, 0);
    glCompileShader(FragmentShaderID);
    
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    memory_handle *FragmentShaderErrorMessage = AllocateMemory(HandlePool, InfoLogLength);
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, 0, (c08*)FragmentShaderErrorMessage->Base);
    if(!Result)
    {
        //TODO: Logging
    }
    FreeMemory(FragmentShaderErrorMessage);
    FreeMemory(VertexShaderCode);
    
    u32 ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    memory_handle *ProgramErrorMessage = AllocateMemory(HandlePool, InfoLogLength);
    glGetProgramInfoLog(ProgramID, InfoLogLength, 0, (c08*)ProgramErrorMessage->Base);
    if(!Result)
    {
        //TODO: Logging
    }
    FreeMemory(ProgramErrorMessage);
    
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
    
    return ProgramID;
}

#if 0
local_func u32
LoadBMP(memory_handle *Handle, c08 *ImagePath)
{
    u08 Header[0x8A];
    u32 DataPos;
    u32 ImageSize;
    u32 Width;
    u32 Height;
    
    handle_pool *HandlePool = GetHandlePool(Handle);
    
    memory_handle *FileHandle = AllocateMemory(HandlePool, sizeof(vptr));
    OpenFile(FileHandle, ImagePath);
    memory_handle *ImageHeader = AllocateMemory(HandlePool, 0x8A);
    ReadDataFromFile(ImageHeader, FileHandle, 0);
    
    s32 BytesPerPixel = 4;
    
    Assert(*(c08*)ImageHeader->Base == 'B' && *((c08*)ImageHeader->Base + 1) == 'M');
    Assert(*((u08*)ImageHeader + 0x1E) == 3 && *((u08*)Header + 0x1C) == 8 * BytesPerPixel);
    DataPos = *(u32*)((u08*)ImageHeader->Base + 0x0A);
    ImageSize = *(u32*)((u08*)ImageHeader->Base + 0x22);
    Width = *(u32*)((u08*)ImageHeader->Base + 0x12);
    Height = *(u32*)((u08*)ImageHeader->Base + 0x16);
    
    if(ImageSize == 0) ImageSize = Width * Height * BytesPerPixel;
    if(DataPos == 0) DataPos = 0x8A;
    
    memory_handle *ImageData = AllocateMemory(HandlePool, ImageSize);
    ReadDataFromFile(ImageData, FileHandle, DataPos);
    CloseFile(FileHandle);
    
    u32 TextureID;
    glGenTextures(1, &TextureID);
    glBindTexture(GL_TEXTURE_2D, TextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_BGRA, GL_UNSIGNED_BYTE, ImageData->Base);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    return TextureID;
}
#endif