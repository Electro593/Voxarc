/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef UTIL_JSON_H_
#define UTIL_JSON_H_

//SUBSECTION: Types

typedef enum json_token_type
{
    JSONToken_Invalid = -1, // Currently Unused
    JSONToken_None    =  0,
    JSONToken_Integer,   // Starts with '-' or '0'-'9'
    JSONToken_Float,     // Starts with '-' or '0'-'9', contains '.' or is greater than S32_MAX
    JSONToken_Boolean,   // Starts with 't' or 'f'
    JSONToken_Null,      // Starts with 'n'
    JSONToken_String,    // Starts and ends with ' " '
    JSONToken_Array,     // Starts with '[', ends with ']'
    JSONToken_Object,    // Starts with '{', ends with '}'
    JSONToken_Raw,       // Plain JSON text
} json_token_type;

typedef struct json_token
{
    json_token_type Type;
    s32 StartOffset;
    s32 EndOffset;
    s32 ParentToken;
    u32 ChildCount;
} json_token;

//TODO: Maybe something to make a tree structure?

typedef struct json_write_data
{
    json_token *Tokens;
    str Buffer;
    s32 NextToken;
    s32 ParentToken;
    u32 NestLevel;
} json_write_data;

//TODO: SIMD

//SUBSECTION: Functions

#define UTIL__JSON__EXPORTS \
    FUNC(void,        JSON_Print, json_write_data *WriteData, str FilePath) \
    FUNC(json_token*, JSON_Parse, heap *Heap, str FileText) \
    FUNC(void,        JSON_CreateInit, heap *Heap, json_write_data *WriteData) \
    FUNC(void,        JSON_CreateEnd, json_write_data *WriteData) \
    FUNC(json_token*, JSON_CreateObjectStart, json_write_data *WriteData, str Key) \
    FUNC(void,        JSON_CreateObjectEnd, json_write_data *WriteData) \
    FUNC(json_token*, JSON_CreateArrayStart, json_write_data *WriteData, str Key) \
    FUNC(void,        JSON_CreateArrayEnd, json_write_data *WriteData) \
    FUNC(json_token*, JSON_CreateNum, json_write_data *WriteData, str Key, num Value) \

#define UTIL__JSON__FUNCS \
    FUNC(json_token*, JSON_CreateStr, json_write_data *WriteData, str Key, str Value) \
    FUNC(json_token*, JSON_CreateRaw, json_write_data *WriteData, str Text) \

    // FUNC(void,        JSON_CreateFromTokens, json_write_data *WriteData, str Text, json_token *Tokens, u32 TokenCount) \

#endif