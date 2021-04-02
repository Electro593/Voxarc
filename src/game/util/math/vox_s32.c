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

//NOTE: Returns -1 if N < 0, 1 if N > 0
internal s32
S32_Sign(s32 N)
{
    s32 Result;
    
    s32 Sign = N >> S32_SHIFT_SIGN; // -1 or 0
    Result = (Sign * 2) + 1;
    
    return Result;
}

internal s32
S32_Lerp(s32 A,
         s32 B,
         r32 Percent)
{
    s32 Result;
    
    Result = (s32)((r32)A + (Percent * (r32)(B - A)));
    
    return Result;
}

internal s32
S32_Abs(s32 N)
{
    s32 Result;
    
    s32 Mask = N >> S32_SHIFT_SIGN;
    Result = (N + Mask) ^ Mask;
    
    return Result;
}

//SUBSECTION: Conversions

internal str
S32_ToStr(str *Dest,
          s32 N)
{
    u32 NumLen = 0;
    b08 Negative = FALSE;
    
    if(N < 0)
    {
        N = -N;
        Negative = TRUE;
    }
    if(N == 0)
    {
        NumLen = 1;
    }
    else
    {
        s32 Num = N;
        while(Num)
        {
            Num /= 10;
            ++NumLen;
        }
    }
    
    str Str = Str_Create(Dest, NULL, NumLen + Negative);
    if(Negative)
    {
        Str[0] = '-';
    }
    
    do
    {
        Str[--NumLen + Negative] = '0' + (N % 10);
        N /= 10;
    }
    while(N);
    
    ASSERT(NumLen == 0);
    
    return Str;
}