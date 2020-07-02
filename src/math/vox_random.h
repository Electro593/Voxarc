#ifndef MATH_VOX_RANDOM_H_

#include "util/vox_defines.h"

#define POLYNOMIAL_MASK32 0xB4BCD35C
#define POLYNOMIAL_MASK31 0x7A5BC2E3

typedef struct random
{
    u32 LFSR32;
    u32 LFSR31;
} random;

inline random
InitRand(u32 Seed)
{
    random Random;
    Random.LFSR32 = Seed % U32_MAX;                   // 32 Bit
    Random.LFSR31 = (Seed + 0x2F2F2F2F) % (U32_MAX / 2); // 31 Bit
    return Random;
}

inline u32
RandInt(random *Random)
{
    s32 FeedBack;
    u32 *LFSR32 = &Random->LFSR32;
    FeedBack = *LFSR32 & 1;
    *LFSR32 >>= 1;
    if(FeedBack == 1)
    {
        *LFSR32 ^= POLYNOMIAL_MASK32;
    }
    FeedBack = *LFSR32 & 1;
    *LFSR32 >>= 1;
    if(FeedBack == 1)
    {
        *LFSR32 ^= POLYNOMIAL_MASK32;
    }
    
    u32 *LFSR31 = &Random->LFSR31;
    FeedBack = *LFSR31 & 1;
    *LFSR31 >>= 1;
    if(FeedBack == 1)
    {
        *LFSR31 ^= POLYNOMIAL_MASK31;
    }
    
    return (*LFSR32 ^ *LFSR31) & 0xFFFF;
}

inline s32
RandRange(random *Random, s32 Min, s32 Max)
{
    return RandInt(Random) % (Max - Min) + Min;
}

#define MATH_VOX_RANDOM_H_
#endif