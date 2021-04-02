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

internal r32
U32_InvLerp(u32 Start,
            u32 End,
            u32 X)
{
    r32 Result;
    
    ASSERT(X >= Start && X <= End);
    ASSERT(End > Start);
    
    Result = (r32)(X - Start) / (r32)(End - Start);
    
    return Result;
}

internal u32
U32_DivUp(u32 Dividend,
          u32 Divisor)
{
    r32 Quotient = (r32)Dividend / (r32)Divisor;
    u32 Result = (u32)R32_Ceil(Quotient);
    
    return Result;
}

//SUBSECTION: Conversions

internal str
U32_ToStr(str *Dest,
          u32 N)
{
    u32 NumLen = 0;
    
    if(N == 0)
    {
        NumLen = 1;
    }
    else
    {
        u32 Num = N;
        while(Num)
        {
            Num /= 10;
            ++NumLen;
        }
    }
    
    str Str = Str_Create(Dest, NULL, NumLen);
    
    do
    {
        Str[--NumLen] = '0' + (N % 10);
        N /= 10;
    }
    while(N);
    
    ASSERT(NumLen == 0);
    
    return Str;
}