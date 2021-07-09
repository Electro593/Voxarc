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

#if !defined(TOOLS_VOX_JSON_H_)

typedef enum json_type
{
    JSON_Invalid,
    JSON_Number,
    JSON_String,
    JSON_Array,
    JSON_Object,
} json_type;

typedef struct json
{
    json_type Type;
    struct json *Next;
    struct json *Child;
    struct json *Parent;
    str Name;
    str String;
    r64 Number;
} json;

internal memory_handle *
JSONWriteInit(handle_pool *Pool,
              u64 Size)
{
    memory_handle *Result = AllocateHandle(Pool, Size);
    u32 *JSONBase = Result->Base;
    *JSONBase = sizeof(u32) + sizeof(json);
    
    json *JSON = (json*)((u08*)Result->Base + sizeof(u32));
    JSON->Type = JSON_Object;
    
    return Result;
}

internal json *
JSONGetRoot(memory_handle *Handle)
{
    return (json*)((u08*)Handle->Base + sizeof(u32));
}

internal json *
JSONGetArrayMember(json *Array, u32 Index)
{
    json *Result = Array->Child;
    for(;
        Result && Index > 0;
        --Index)
    {
        Result = Result->Next;
    }
    return Result;
}

internal json *
JSONGetObjectMember(json *Object, str Name)
{
    json *Iterator = Object->Child;
    json *Result = NULL;
    while(Iterator)
    {
        if(Str_EqualCC(Iterator->Name, Name))
        {
            Result = Iterator;
        }
        
        Iterator = Iterator->Next;
    }
    return Result;
}

internal json *
JSONAddNumberToObject(memory_handle *Memory,
                      json *Parent,
                      str Name,
                      r64 Number)
{
    u32 *Used = (u32*)Memory->Base;
    json *JSON = (json*)((u08*)Memory->Base + *Used);
    u32 NameLength = PointerCount(Name, chr) + 1;
    str NameBase = (str)((u08*)JSON + sizeof(json));
    
    Mem_Cpy(NameBase, Name, NameLength);
    
    JSON->Type = JSON_Number;
    JSON->Parent = Parent;
    JSON->Name = NameBase;
    JSON->Number = Number;
    
    *Used += sizeof(json) + NameLength;
    
    if(Parent->Child)
    {
        json *Child = Parent->Child;
        while(Child->Next)
        {
            Child = Child->Next;
        }
        Child->Next = JSON;
    }
    else
    {
        Parent->Child = JSON;
    }
    
    return JSON;
}

internal json *
JSONAddStringToObject(memory_handle *Memory,
                      json *Parent,
                      str Name,
                      str String)
{
    u32 *Used = (u32*)Memory->Base;
    json *JSON = (json*)((u08*)Memory->Base + *Used);
    u32 NameLength = PointerCount(Name, chr) + 1;
    u32 StringLength = PointerCount(String, chr) + 1;
    str NameBase = (str)((u08*)JSON + sizeof(json));
    str StringBase = NameBase + NameLength;
    
    Mem_Cpy(NameBase, Name, NameLength);
    Mem_Cpy(StringBase, String, StringLength);
    
    JSON->Type = JSON_String;
    JSON->Parent = Parent;
    JSON->Name = NameBase;
    JSON->String = StringBase;
    
    *Used += sizeof(json) + NameLength + StringLength;
    
    if(Parent->Child)
    {
        json *Child = Parent->Child;
        while(Child->Next)
        {
            Child = Child->Next;
        }
        Child->Next = JSON;
    }
    else
    {
        Parent->Child = JSON;
    }
    
    return JSON;
}

internal json *
JSONAddArrayToObject(memory_handle *Memory,
                     json *Parent,
                     str Name)
{
    u32 *Used = (u32*)Memory->Base;
    json *JSON = (json*)((u08*)Memory->Base + *Used);
    u32 NameLength = PointerCount(Name, chr) + 1;
    str NameBase = (str)((u08*)JSON + sizeof(json));
    Mem_Cpy(NameBase, Name, NameLength);
    
    JSON->Type = JSON_Array;
    JSON->Parent = Parent;
    JSON->Name = NameBase;
    
    *Used += sizeof(json) + NameLength;
    
    if(Parent->Child)
    {
        json *Child = Parent->Child;
        while(Child->Next)
        {
            Child = Child->Next;
        }
        Child->Next = JSON;
    }
    else
    {
        Parent->Child = JSON;
    }
    
    return JSON;
}

internal json *
JSONAddObjectToObject(memory_handle *Memory,
                      json *Parent,
                      str Name)
{
    u32 *Used = (u32*)Memory->Base;
    json *JSON = (json*)((u08*)Memory->Base + *Used);
    u32 NameLength = PointerCount(Name, chr) + 1;
    str NameBase = (str)((u08*)JSON + sizeof(json));
    Mem_Cpy(NameBase, Name, NameLength);
    
    JSON->Type = JSON_Object;
    JSON->Parent = Parent;
    JSON->Name = NameBase;
    
    *Used += sizeof(json) + NameLength;
    
    if(Parent->Child)
    {
        json *Child = Parent->Child;
        while(Child->Next)
        {
            Child = Child->Next;
        }
        Child->Next = JSON;
    }
    else
    {
        Parent->Child = JSON;
    }
    
    return JSON;
}

internal json *
JSONAddNumberToArray(memory_handle *Memory,
                     json *Parent,
                     r64 Number)
{
    u32 *Used = (u32*)Memory->Base;
    json *JSON = (json*)((u08*)Memory->Base + *Used);
    
    JSON->Type = JSON_Number;
    JSON->Parent = Parent;
    JSON->Number = Number;
    
    *Used += sizeof(json);
    
    if(Parent->Child)
    {
        json *Child = Parent->Child;
        while(Child->Next)
        {
            Child = Child->Next;
        }
        Child->Next = JSON;
    }
    else
    {
        Parent->Child = JSON;
    }
    
    return JSON;
}

internal json *
JSONAddStringToArray(memory_handle *Memory,
                     json *Parent,
                     str String)
{
    u32 *Used = (u32*)Memory->Base;
    json *JSON = (json*)((u08*)Memory->Base + *Used);
    u32 StringLength = PointerCount(String, chr) + 1;
    str StringBase = (str)((u08*)JSON + sizeof(json));
    
    Mem_Cpy(StringBase, String, StringLength);
    
    JSON->Type = JSON_String;
    JSON->Parent = Parent;
    JSON->String = StringBase;
    
    *Used += sizeof(json) + StringLength;
    
    if(Parent->Child)
    {
        json *Child = Parent->Child;
        while(Child->Next)
        {
            Child = Child->Next;
        }
        Child->Next = JSON;
    }
    else
    {
        Parent->Child = JSON;
    }
    
    return JSON;
}

internal json *
JSONAddArrayToArray(memory_handle *Memory,
                    json *Parent)
{
    u32 *Used = (u32*)Memory->Base;
    json *JSON = (json*)((u08*)Memory->Base + *Used);
    
    JSON->Type = JSON_Array;
    JSON->Parent = Parent;
    
    *Used += sizeof(json);
    
    if(Parent->Child)
    {
        json *Child = Parent->Child;
        while(Child->Next)
        {
            Child = Child->Next;
        }
        Child->Next = JSON;
    }
    else
    {
        Parent->Child = JSON;
    }
    
    return JSON;
}

internal json *
JSONAddObjectToArray(memory_handle *Memory,
                     json *Parent)
{
    u32 *Used = (u32*)Memory->Base;
    json *JSON = (json*)((u08*)Memory->Base + *Used);
    
    JSON->Type = JSON_Object;
    JSON->Parent = Parent;
    
    *Used += sizeof(json);
    
    if(Parent->Child)
    {
        json *Child = Parent->Child;
        while(Child->Next)
        {
            Child = Child->Next;
        }
        Child->Next = JSON;
    }
    else
    {
        Parent->Child = JSON;
    }
    
    return JSON;
}

internal memory_handle *
JSONReadText(memory_handle *Text,
             handle_pool *OutputPool)
{
    chr *C = (chr*)Text->Base;
    chr NameBuffer[256];
    chr ValueBuffer[256];
    
    EatSpace(&C);
    if(*C != '{')
    {
        return NULL;
    }
    memory_handle *Memory = JSONWriteInit(OutputPool, OutputPool->DataPool->Size);
    json *Root = JSONGetRoot(Memory);
    json *Current = Root;
    C++;
    
    while(TRUE)
    {
        EatSpace(&C);
        if(!*C)
        {
            return NULL;
        }
        
        if(Current->Type == JSON_Object && *C != '}')
        {
            if(*C++ != '\"')
            {
                return NULL;
            }
            if(!*C)
            {
                return NULL;
            }
            u32 Index = 0;
            while(*C != '\"')
            {
                NameBuffer[Index++] = *C++;
                if(!*C)
                {
                    return NULL;
                }
            }
            NameBuffer[Index] = 0;
            
            C++;
            EatSpace(&C);
            if(!*C || *C++ != ':')
            {
                return NULL;
            }
            EatSpace(&C);
            if(!*C)
            {
                return NULL;
            }
        }
        
        if(*C == '[')
        {
            if(Current->Type == JSON_Object)
            {
                Current = JSONAddArrayToObject(Memory, Current, NameBuffer);
            }
            else
            {
                Current = JSONAddArrayToArray(Memory, Current);
            }
            C++;
            continue;
        }
        else if(*C == '{')
        {
            if(Current->Type == JSON_Object)
            {
                Current = JSONAddObjectToObject(Memory, Current, NameBuffer);
            }
            else
            {
                Current = JSONAddObjectToArray(Memory, Current);
            }
            C++;
            continue;
        }
        
        if(Chr_IsBase10(*C) || *C == '-' || *C == '.')
        {
            u32 Index = 0;
            while(Chr_IsBase10(*C) || *C == '-' || *C == '.' || ToLower(*C) == 'e')
            {
                ValueBuffer[Index++] = *C++;
                if(!*C)
                {
                    return NULL;
                }
            }
            ValueBuffer[Index] = 0;
            
            r64 Number = Str_ToR64(ValueBuffer);
            if(Current->Type == JSON_Object)
            {
                JSONAddNumberToObject(Memory, Current, NameBuffer, Number);
            }
            else
            {
                JSONAddNumberToArray(Memory, Current, Number);
            }
        }
        else if(*C == '\"')
        {
            u32 Index = 0;
            C++;
            if(!*C)
            {
                return NULL;
            }
            while(*C != '\"')
            {
                ValueBuffer[Index++] = *C++;
                if(!*C)
                {
                    return NULL;
                }
            }
            ValueBuffer[Index] = 0;
            C++;
            
            if(Current->Type == JSON_Object)
            {
                JSONAddStringToObject(Memory, Current, NameBuffer, ValueBuffer);
            }
            else
            {
                JSONAddStringToArray(Memory, Current, ValueBuffer);
            }
        }
        
        EatSpace(&C);
        while(*C != ',')
        {
            if(!*C)
            {
                if(*((u08*)Text->Base + Text->Size - 1) == *C)
                {
                    return Memory;
                }
                return NULL;
            }
            if(*C == '}' || *C == ']')
            {
                Current = Current->Parent;
                C++;
                EatSpace(&C);
            }
        }
        C++;
    }
}

#define JSONWriteName \
{ \
    if(Current->Name) \
    { \
        u32 NameLength = PointerCount(Current->Name, chr); \
        *Cursor++ = '\"'; \
        Mem_Cpy(Cursor, Current->Name, NameLength); \
        Cursor += NameLength; \
        *Cursor++ = '\"'; \
        *Cursor++ = ':'; \
        *Cursor++ = ' '; \
    } \
}

#define JSONWriteTabs \
{ \
    for(u32 SpaceIndex = 0; \
        SpaceIndex < Depth * 4; \
        ++SpaceIndex) \
    { \
        *Cursor++ = ' '; \
    } \
}

internal u32
JSONConvertToText(memory_handle *Memory,
                  memory_handle *Output)
{
    json *Current = (json*)((u08*)Memory->Base + sizeof(u32));
    u08 *Cursor = Output->Base;
    u32 Depth = 0;
    
    while(Current)
    {
        JSONWriteTabs;
        JSONWriteName;
        
        switch(Current->Type)
        {
            case JSON_Object:
            {
                *Cursor++ = '{';
                
                if(Current->Child)
                {
                    ++Depth;
                    *Cursor++ = '\n';
                    Current = Current->Child;
                    continue;
                }
                else
                {
                    *Cursor++ = '}';
                }
            } break;
            case JSON_Array:
            {
                *Cursor++ = '[';
                
                if(Current->Child)
                {
                    ++Depth;
                    *Cursor++ = '\n';
                    Current = Current->Child;
                    continue;
                }
                else
                {
                    *Cursor++ = ']';
                }
            } break;
            case JSON_String:
            {
                u32 StringLength = PointerCount(Current->String, chr);
                
                *Cursor++ = '\"';
                Mem_Cpy(Cursor, Current->String, StringLength);
                Cursor += StringLength;
                *Cursor++ = '\"';
            } break;
            case JSON_Number:
            {
                r64 Number = Current->Number;
                if(!Number)
                {
                    *Cursor++ = '0';
                    break;
                }
                b08 IsNegative = Number < 0;
                if(IsNegative)
                {
                    *Cursor++ = '-';
                    Number = -Number;
                }
                chr Buffer[50] = {0};
                s64 Whole = (s64)Number;
                r64 Fraction = Number - Whole;
                
                u32 Index = 50;
                do
                {
                    Buffer[--Index] = (Whole % 10) + '0';
                    Whole /= 10;
                }
                while(Whole && Index);
                Mem_Cpy(Cursor, Buffer + Index, 50 - Index);
                Cursor += 50 - Index;
                
                if(Fraction)
                {
                    Mem_Set(Buffer, 0, 50);
                    
                    Index = 0;
                    *Cursor++ = '.';
                    while((s64)Fraction != Fraction &&
                          Index < 32)
                    {
                        Fraction *= 10;
                        *Cursor++ = "0123456789"[(s64)Fraction % 10];
                    }
                }
            } break;
        }
        
        while(!Current->Next)
        {
            if(!Current->Parent)
            {
                goto End;
            }
            
            Current = Current->Parent;
            *Cursor++ = '\n';
            --Depth;
            JSONWriteTabs;
            switch(Current->Type)
            {
                case JSON_Object:
                {
                    *Cursor++ = '}';
                } break;
                case JSON_Array:
                {
                    *Cursor++ = ']';
                } break;
            }
        }
        *Cursor++ = ',';
        *Cursor++ = '\n';
        Current = Current->Next;
    }
    End:
    
    return (u32)((u64)Cursor - (u64)Output->Base);
}

#undef JSONWriteName
#undef JSONWriteTabs

#define TOOLS_VOX_JSON_H_
#endif