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

//SUBSECTION: Conversions

//TODO: Use the num type instead of individual u64, u32, etc?
//      A Num_ToStr()

internal str
U64_ToStr(str *Dest,
          u64 N)
{
    u32 NumLen = 0;
    u64 Num = N;
    while(Num)
    {
        Num /= 10;
        ++NumLen;
    }
    
    str Str = Str_Create(Dest, NULL, NumLen);
    
    do
    {
        Str[NumLen--] = '0' + (N % 10);
        N /= 10;
    }
    while(N);
    
    return Str;
}

internal str
U64_ToStr_Hex(str *Dest,
              u64 N)
{
    u32 NumLen = U64_MAX_CHARS_HEX - 1;
    
    //TODO: Test this
    str Str = Str_Create(Dest, "0x", NumLen);
    
    while(NumLen > 2) // Length of "0x"
    {
        if(N % 16 < 10)
        {
            Str[--NumLen] = '0' + (N % 16);
        }
        else
        {
            Str[--NumLen] = 'A' + (N % 16 - 10);
        }
        
        N /= 16;
    }
    
    return Str;
}