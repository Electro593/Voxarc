/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: S32 (bfs32)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


internal s32
Bfs32_GetComponent(bfs32 Bitfield,
                   u32 Size,
                   u32 Offset)
{
    s32 Result;
    
    ASSERT(Size > 0);
    ASSERT(Offset + Size <= S32_BITS);
    
    u32 ComponentMask = ((1 << Size) - 1);
    Result = (Bitfield >> Offset) & ComponentMask;
    
    u32 SignMask = BIT_GET(Result, Size - 1);
    SignMask = (u32)(-(s32)SignMask);
    
    Result |= (~ComponentMask & SignMask);
    
    return Result;
}

internal void
Bfs32_SetComponent(bfs32 *Bitfield,
                   s32 Data,
                   u32 Size,
                   u32 Offset)
{
    ASSERT(Size > 0);
    ASSERT(Offset + Size <= 32);
    
    u32 Mask = ((1 << Size) - 1) << Offset;
    Data <<= Offset;
    Data &= Mask;
    
    *Bitfield &= ~Mask;
    *Bitfield |= Data;
}

//NOTE: Layout: N0 ____N1____ ____N2____ ____N3____
internal bfs32_1x2_3x10
Bfs32_1x2_3x10(s32 N0,
               s32 N1,
               s32 N2,
               s32 N3)
{
    bfs32 Result;
    
    Bfs32_SetComponent(&Result, N3, 10,  0);
    Bfs32_SetComponent(&Result, N2, 10, 10);
    Bfs32_SetComponent(&Result, N1, 10, 20);
    Bfs32_SetComponent(&Result, N0,  2, 30);
    
    return Result;
}

internal v4s32
Bfs32_1x2_3x10_To_V4s32(bfs32_1x2_3x10 Bitfield)
{
    v4s32 Result;
    
    Result.X = Bfs32_GetComponent(Bitfield, 10,  0);
    Result.Y = Bfs32_GetComponent(Bitfield, 10, 10);
    Result.Z = Bfs32_GetComponent(Bitfield, 10, 20);
    Result.W = Bfs32_GetComponent(Bitfield,  2, 30);
    
    return Result;
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: U32 (bfu32)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


internal u32
Bfu32_GetComponent(bfu32 Bitfield,
                   u32 Size,
                   u32 Offset)
{
    u32 Result;
    
    ASSERT(Size > 0);
    ASSERT(Offset + Size <= 32);
    
    Result = Bitfield >> Offset;
    Result = Bitfield & ((1 << Size) - 1);
    
    return Result;
}

internal void
Bfu32_SetComponent(bfu32 *Bitfield,
                   u32 Data,
                   u32 Size,
                   u32 Offset)
{
    ASSERT(Size > 0);
    ASSERT(Offset + Size <= 32);
    
    u32 Mask = ((1 << Size) - 1) << Offset;
    Data <<= Offset;
    Data &= Mask;
    
    *Bitfield &= ~Mask;
    *Bitfield |= Data;
}