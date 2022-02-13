/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

internal r32
R32_Floor(r32 N)
{
    if(N > 0) return (r32)((s32)N);
    if(N - (s32)N == 0) return N;
    return (r32)((s32)N - 1);
}

internal r32
R32_Ceil(r32 N)
{
    if(N < 0) return (r32)((s32)N);
    if(N - (s32)N == 0) return N;
    return (r32)((s32)N + 1);
}

internal r32
R32_Sqrt(r32 N)
{
    r32 Result;
    Result = R128_Sqrt_4(R128_Set_4x32(N,0,0,0)).R32[0];
    return Result;
}

internal r32
R32_Abs(r32 N)
{
    u32 Binary = FORCE_CAST(u32, N);
    Binary &= 0x7FFFFFFF;
    return FORCE_CAST(r32, Binary);
}