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

internal void
Rand_Init(u32 Seed)
{
    __Global->Rand.LFSR32 = Seed % U32_MAX;                      // 32 Bit
    __Global->Rand.LFSR31 = (Seed + 0x2F2F2F2F) % (U32_MAX / 2); // 31 Bit
}

//TODO: Rework this

internal u32
Rand_Int(void)
{
    s32 FeedBack;
    u32 LFSR32 = __Global->Rand.LFSR32;
    FeedBack = LFSR32 & 1;
    LFSR32 >>= 1;
    if(FeedBack == 1)
    {
        LFSR32 ^= POLYNOMIAL_MASK32;
    }
    FeedBack = LFSR32 & 1;
    LFSR32 >>= 1;
    if(FeedBack == 1)
    {
        LFSR32 ^= POLYNOMIAL_MASK32;
    }
    __Global->Rand.LFSR32 = LFSR32;
    
    u32 LFSR31 = __Global->Rand.LFSR31;
    FeedBack = LFSR31 & 1;
    LFSR31 >>= 1;
    if(FeedBack == 1)
    {
        LFSR31 ^= POLYNOMIAL_MASK31;
    }
    __Global->Rand.LFSR31 = LFSR31;
    
    return (LFSR32 ^ LFSR31) & 0xFFFF;
}

//NOTE: Max is exclusive, so Min <= Result < Max
internal s32
Rand_IntRange(s32 Min,
              s32 Max)
{
    s32 Result;
    
    ASSERT(Min != Max);
    
    Result = Rand_Int() % (Max - Min) + Min;
    
    return Result;
}