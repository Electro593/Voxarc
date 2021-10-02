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


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: JSON
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


//CREDIT: https://github.com/zserge/jsmn

local json_token *
JSON_AllocateToken(json_token **Tokens,
                   s32 *NextToken)
{
    u32 PrevSize = Heap_GetSize(*Tokens);
    
    if(*NextToken >= (s32)PrevSize / (s32)sizeof(json_token))
    {
        Heap_Resize(Tokens, PrevSize + (sizeof(json_token) * 64));
    }
    
    json_token *Token = *Tokens + *NextToken;
    Token->StartOffset = -1;
    Token->EndOffset = -1;
    Token->ParentToken = -1;
    Token->ChildCount = 0;
    
    (*NextToken)++;
    return Token;
}

internal json_token *
JSON_Parse(heap *Heap,
           str FileText)
{
    u32 FileSize = Str_Len(FileText) + 1;
    
    json_token *Tokens = Heap_Allocate(Heap, sizeof(json_token) * 64);
    s32 ParentToken = -1;
    s32 NextToken = 0;
    u32 TokenCount = 0;
    
    chr *C = (chr*)FileText;
    for(u32 CharIndex = 0;
        CharIndex < FileSize && *C != '\0';
        ++CharIndex, ++C)
    {
        switch(*C)
        {
            case '[':
            case '{':
            {
                ++TokenCount;
                
                json_token *Token = JSON_AllocateToken(&Tokens, &NextToken);
                if(ParentToken != -1)
                {
                    json_token *Parent = Tokens + ParentToken;
                    ASSERT(Parent->Type != JSONToken_Object);
                    
                    ++Parent->ChildCount;
                    Token->ParentToken = ParentToken;
                }
                
                Token->Type = (*C == '[') ? JSONToken_Array : JSONToken_Object;
                Token->StartOffset = CharIndex;
                ParentToken = NextToken - 1;
            } break;
            
            case ']':
            case '}':
            {
                ASSERT(NextToken > 0);
                json_token *Token = Tokens + NextToken - 1;
                json_token_type Type = (*C == ']') ? JSONToken_Array : JSONToken_Object;
                
                while(TRUE)
                {
                    if(Token->StartOffset != -1 && Token->EndOffset == -1)
                    {
                        ASSERT(Token->Type == Type);
                        Token->EndOffset = CharIndex + 1;
                        ParentToken = Token->ParentToken;
                        
                        break;
                    }
                    
                    if(Token->ParentToken == -1)
                    {
                        ASSERT(Token->Type == Type && ParentToken != -1);
                        
                        break;
                    }
                    
                    Token = Tokens + Token->ParentToken;
                }
            } break;
            
            case '\"':
            {
                u32 Start = CharIndex;
                
                ++CharIndex, ++C;
                for(;
                    CharIndex < FileSize && *C != '\0';
                    ++CharIndex, ++C)
                {
                    if(*C == '\"')
                    {
                        json_token *Token = JSON_AllocateToken(&Tokens, &NextToken);
                        Token->Type = JSONToken_String;
                        Token->StartOffset = Start + 1;
                        Token->EndOffset = CharIndex;
                        Token->ChildCount = 0;
                        Token->ParentToken = ParentToken;
                        
                        goto ParseStringExit;
                    }
                    
                    if(*C == '\\' && CharIndex + 1 < FileSize)
                    {
                        ++CharIndex,++C;
                        switch(*C)
                        {
                            case '\\':
                            case '\"':
                            case '\'':
                            case 'b':
                            case 't':
                            case 'f':
                            case 'r':
                            case 'n':
                            { } break;
                            
                            case 'u':
                            {
                                ++CharIndex, ++C;
                                
                                for(u32 Index = 0;
                                    Index < 4 && CharIndex < FileSize && C != '\0';
                                    ++Index)
                                {
                                    ASSERT(('0' <= *C && *C <= '9') ||
                                           ('A' <= *C && *C <= 'F') ||
                                           ('a' <= *C && *C <= 'f'));
                                    
                                    ++CharIndex, ++C;
                                }
                                
                                --CharIndex, --C;
                            } break;
                            
                            default:
                            {
                                STOP;
                            }
                        }
                    }
                }
                
                STOP;
                
                ParseStringExit:
                
                ++TokenCount;
                if(ParentToken != -1)
                {
                    ++Tokens[ParentToken].ChildCount;
                }
            } break;
            
            case ' ':
            case '\t':
            case '\r':
            case '\n':
            { } break;
            
            case ':':
            {
                ParentToken = NextToken - 1; // Current token (key) becomes parent of the next (value)
            } break;
            
            case ',':
            {
                if(ParentToken != -1 &&
                   Tokens[ParentToken].Type != JSONToken_Array &&
                   Tokens[ParentToken].Type != JSONToken_Object)
                {
                    ParentToken = Tokens[ParentToken].ParentToken;
                    
                    for(s32 Index = NextToken - 1;
                        Index >= 0;
                        --Index)
                    {
                        if(Tokens[Index].Type == JSONToken_Array ||
                           Tokens[Index].Type == JSONToken_Object)
                        {
                            if(Tokens[Index].StartOffset != -1 &&
                               Tokens[Index].EndOffset == -1)
                            {
                                ParentToken = Index;
                                
                                break;
                            }
                        }
                    }
                }
            } break;
            
            case '-':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                json_token_type PrimType = JSONToken_Integer;
                goto ParsePrimitive;
            case 't': // TRUE
            case 'f': // FALSE
                PrimType = JSONToken_Boolean;
                goto ParsePrimitive;
            case 'n': // NULL
                PrimType = JSONToken_Null;
            {
                ParsePrimitive:
                
                if(ParentToken != -1)
                {
                    json_token *Parent = Tokens + ParentToken;
                    ASSERT(Parent->Type != JSONToken_Object &&
                           (Parent->Type != JSONToken_String || Parent->ChildCount == 0));
                }
                
                u32 Start = CharIndex;
                
                for(;
                    CharIndex < FileSize && *C != '\0';
                    ++CharIndex, ++C)
                {
                    switch(*C)
                    {
                        case '.':
                        {
                            ASSERT(PrimType == JSONToken_Integer);
                            PrimType = JSONToken_Float;
                        } break;
                        
                        case '}':
                        case ']':
                        case ',':
                        case ' ':
                        case '\t':
                        case '\r':
                        case '\n':
                        {
                            goto ParsePrimitiveExit;
                        }
                    }
                    
                    ASSERT(32 <= *C && *C <= 127);
                }
                
                STOP;
                
                ParsePrimitiveExit:
                
                json_token *Token = JSON_AllocateToken(&Tokens, &NextToken);
                Token->Type = PrimType;
                Token->StartOffset = Start;
                Token->EndOffset = CharIndex;
                Token->ParentToken = ParentToken;
                --CharIndex, --C;
                
                ++TokenCount;
                if(ParentToken != -1)
                {
                    ++Tokens[ParentToken].ChildCount;
                }
            } break;
            
            default:
            {
                STOP;
            }
        }
    }
    
    for(s32 Index = NextToken - 1;
        Index >= 0;
        --Index)
    {
        ASSERT(Tokens[Index].StartOffset != -1 && Tokens[Index].EndOffset != -1);
    }
    
    Heap_Resize(&Tokens, sizeof(json_token) * TokenCount);
    
    return Tokens;
}

internal void
JSON_Print(json_write_data *WriteData,
           str FilePath)
{
    vptr FileHandle;
    
    if(Platform_OpenFile(&FileHandle, FilePath, FileMode_Write))
    {
        Platform_WriteFile(WriteData->Buffer, &FileHandle, Str_Len(WriteData->Buffer), 0);
    }
    
    Platform_CloseFile(&FileHandle);
}

internal void
JSON_CreateInit(heap *Heap,
                json_write_data *WriteData)
{
    WriteData->Tokens = Heap_Allocate(Heap, 0);
    WriteData->Buffer = Str_Create(NULL, NULL, 0);
    WriteData->ParentToken = -1;
}

internal void
JSON_CreateEnd(json_write_data *WriteData)
{
    Heap_Free(WriteData->Tokens);
    Str_Free(WriteData->Buffer);
}

local void
JSON_AddNewLine(str *Buffer,
                u32 NestLevel)
{
    str Line = Str_Create(NULL, "\n", NestLevel + 1);
    
    Mem_Set(Line + 1, '\t', NestLevel);
    Str_Cat(Buffer, Line);
    
    Str_Free(Line);
}

internal json_token *
JSON_CreateNew(json_write_data *WriteData,
               json_token_type Type,
               str Key)
{
    if(WriteData->ParentToken != -1)
    {
        json_token *ParentToken = WriteData->Tokens + WriteData->ParentToken;
        if(ParentToken->ChildCount > 0)
        {
            Str_CatC(&WriteData->Buffer, ",");
        }
        
        JSON_AddNewLine(&WriteData->Buffer, WriteData->NestLevel);
        
        ++ParentToken->ChildCount;
    }
    
    if(Key)
    {
        ASSERT(((json_token*)(WriteData->Tokens + WriteData->ParentToken))->Type == JSONToken_Object);
        
        json_token *KeyToken = JSON_AllocateToken(&WriteData->Tokens, &WriteData->NextToken);
        KeyToken->Type = JSONToken_String;
        KeyToken->ChildCount = 1;
        KeyToken->ParentToken = WriteData->ParentToken;
        
        WriteData->ParentToken = WriteData->NextToken - 1;
        
        u32 BufLen = Str_Len(WriteData->Buffer);
        Str_CatC(&WriteData->Buffer, "\"\": ");
        Str_Ins(&WriteData->Buffer, Key, BufLen + 1);
    }
    
    json_token *Token = JSON_AllocateToken(&WriteData->Tokens, &WriteData->NextToken);
    Token->Type = Type;
    Token->ParentToken = WriteData->ParentToken;
    Token->StartOffset = 0;
    
    return Token;
}

internal json_token *
JSON_CreateContainerStart(json_write_data *WriteData,
                          json_token_type Type,
                          str Key)
{
    json_token *Result;
    
    Result = JSON_CreateNew(WriteData, Type, Key);
    
    ++WriteData->NestLevel;
    WriteData->ParentToken = WriteData->NextToken - 1;
    
    return Result;
}

internal void
JSON_CreateContainerEnd(json_write_data *WriteData,
                        json_token_type Type)
{
    --WriteData->NestLevel;
    JSON_AddNewLine(&WriteData->Buffer, WriteData->NestLevel);
    
    json_token *Token = WriteData->Tokens + WriteData->ParentToken;
    
    while(Token->ParentToken != -1)
    {
        if(Token->StartOffset != -1 && Token->EndOffset == -1)
        {
            ASSERT(Token->Type == Type);
            
            Token->EndOffset = 0;
            
            if(WriteData->Tokens[Token->ParentToken].Type == JSONToken_String)
            { // Has a key
                WriteData->ParentToken = WriteData->Tokens[Token->ParentToken].ParentToken;
            }
            else
            {
                ASSERT(WriteData->Tokens[Token->ParentToken].Type == JSONToken_Array ||
                       WriteData->Tokens[Token->ParentToken].Type == JSONToken_Object);
                
                WriteData->ParentToken = Token->ParentToken;
            }
            
            break;
        }
        
        Token = WriteData->Tokens + Token->ParentToken;
    }
    
    Token->EndOffset = 0;
}

internal json_token *
JSON_CreateObjectStart(json_write_data *WriteData,
                       str Key)
{
    json_token *Result;
    
    Result = JSON_CreateContainerStart(WriteData, JSONToken_Object, Key);
    Str_CatC(&WriteData->Buffer, "{");
    
    return Result;
}

internal void
JSON_CreateObjectEnd(json_write_data *WriteData)
{
    JSON_CreateContainerEnd(WriteData, JSONToken_Object);
    Str_CatC(&WriteData->Buffer, "}");
}

internal json_token *
JSON_CreateArrayStart(json_write_data *WriteData,
                      str Key)
{
    json_token *Result;
    
    Result = JSON_CreateContainerStart(WriteData, JSONToken_Array, Key);
    Str_CatC(&WriteData->Buffer, "[");
    
    return Result;
}

internal void
JSON_CreateArrayEnd(json_write_data *WriteData)
{
    JSON_CreateContainerEnd(WriteData, JSONToken_Array);
    Str_CatC(&WriteData->Buffer, "]");
}

internal json_token *
JSON_CreateStr(json_write_data *WriteData,
               str Key,
               str Value)
{
    json_token *Result;
    
    Result = JSON_CreateNew(WriteData, JSONToken_String, Key);
    Result->EndOffset = 0;
    
    u32 BufLen = Str_Len(WriteData->Buffer);
    Str_CatC(&WriteData->Buffer, "\"\"");
    Str_Ins(&WriteData->Buffer, Value, BufLen + 1);
    
    s32 ParentTokenIndex = Result->ParentToken;
    if(Key)
    {
        json_token *ParentToken = WriteData->Tokens + ParentTokenIndex;
        ParentTokenIndex = ParentToken->ParentToken;
    }
    WriteData->ParentToken = ParentTokenIndex;
    
    return Result;
}

internal json_token *
JSON_CreateNum(json_write_data *WriteData,
               str Key,
               num Value)
{
    json_token *Result;
    
    json_token_type PrimType = JSONToken_None;
    switch(Value.Type)
    {
        case TYPE_U32:
        case TYPE_S32:
        {
            PrimType = JSONToken_Integer;
        } break;
        case TYPE_R32:
        {
            PrimType = JSONToken_Float;
        } break;
        default:
        {
            STOP;
        }
    }
    
    u32 Len = Str_Len(WriteData->Buffer);
    Result = JSON_CreateNew(WriteData, PrimType, Key);
    Len = Str_Len(WriteData->Buffer);
    Result->EndOffset = 0;
    
    str NumBuf = NULL;
    switch(Value.Type)
    {
        case TYPE_R32:
        {
            Str_Cat(&WriteData->Buffer, R32_To_Str(&NumBuf, Value.R32));
        } break;
        
        case TYPE_S32:
        {
            Str_Cat(&WriteData->Buffer, S32_To_Str(&NumBuf, Value.S32));
        } break;
        
        case TYPE_U32:
        {
            Str_Cat(&WriteData->Buffer, U32_To_Str(&NumBuf, Value.U32));
        } break;
        
        // case TYPE_STR:
        // { // For directly transferring a number string
        //     Str_Cat(&WriteData->Buffer, Value.NumStr);
        // } break;
        
        default:
        {
            STOP;
        }
    }
    Str_Free(NumBuf);
    
    s32 ParentTokenIndex = Result->ParentToken;
    if(Key)
    {
        json_token *ParentToken = WriteData->Tokens + ParentTokenIndex;
        ParentTokenIndex = ParentToken->ParentToken;
    }
    WriteData->ParentToken = ParentTokenIndex;
    
    return Result;
}

internal json_token *
JSON_CreateRaw(json_write_data *WriteData,
               str Text)
{
    json_token *Result;
    
    Result = JSON_CreateNew(WriteData, JSONToken_Raw, NULL);
    Result->EndOffset = 0;
    
    Str_Cat(&WriteData->Buffer, Text);
    
    return Result;
}