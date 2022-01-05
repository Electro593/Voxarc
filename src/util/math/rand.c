/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

internal void
Rand_Init(u32 Seed)
{
    UtilState->Rand.LFSR32 = Seed % U32_MAX;                      // 32 Bit
    UtilState->Rand.LFSR31 = (Seed + 0x2F2F2F2F) % (U32_MAX / 2); // 31 Bit
}

//TODO: Rework this

internal u32
Rand_Int(void)
{
    s32 FeedBack;
    u32 LFSR32 = UtilState->Rand.LFSR32;
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
    UtilState->Rand.LFSR32 = LFSR32;
    
    u32 LFSR31 = UtilState->Rand.LFSR31;
    FeedBack = LFSR31 & 1;
    LFSR31 >>= 1;
    if(FeedBack == 1)
    {
        LFSR31 ^= POLYNOMIAL_MASK31;
    }
    UtilState->Rand.LFSR31 = LFSR31;
    
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