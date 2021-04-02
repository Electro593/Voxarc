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

#if !defined(GAME_UTIL_VOX_JSON_H_)
#define      GAME_UTIL_VOX_JSON_H_

//SUBSECTION: Types

typedef enum json_token_type
{
    JSONToken_Invalid = -1, // Currently Unused
    JSONToken_None    =  0,
    JSONToken_Primitive, // Starts with 't' (true), 'f' (false), 'n' (null), or '-','0'-'9' (numbers)
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

//SUBSECTION: Functions

#define GAME_UTIL__JSON__EXPORTS \
    PROC(void,        JSON_CreateInit, heap *Heap, json_write_data *WriteData) \
    PROC(json_token*, JSON_CreateObjectStart, json_write_data *WriteData, str Key) \
    PROC(json_token*, JSON_CreateNum, json_write_data *WriteData, str Key, num Value) \
    PROC(void,        JSON_CreateObjectEnd, json_write_data *WriteData) \
    PROC(void,        JSON_Print, json_write_data *WriteData, str FilePath) \
    PROC(void,        JSON_CreateEnd, json_write_data *WriteData) \

#define GAME_UTIL__JSON__FUNCS \
    PROC(json_token*, JSON_Parse, heap *Heap, str FileText) \
    PROC(json_token*, JSON_CreateArrayStart, json_write_data *WriteData, str Key) \
    PROC(void,        JSON_CreateArrayEnd, json_write_data *WriteData) \
    PROC(json_token*, JSON_CreateStr, json_write_data *WriteData, str Key, str Value) \
    PROC(json_token*, JSON_CreateRaw, json_write_data *WriteData, str Text) \

    // PROC(void,        JSON_CreateFromTokens, json_write_data *WriteData, str Text, json_token *Tokens, u32 TokenCount) \

#endif