#ifndef VOX_DEFINES_H_

typedef       unsigned char       b08;
typedef                int        b32;

typedef                char       c08;
typedef const          char      cc08;
typedef       unsigned short      c16;
typedef const unsigned short     cc16;

typedef                char       s08;
typedef                short      s16;
typedef                int        s32;
typedef                long long  s64;

typedef       unsigned char       u08;
typedef const unsigned char      cu08;
typedef       unsigned short      u16;
typedef       unsigned int        u32;
typedef       unsigned long long  u64;

typedef                float      r32;
typedef                double     r64;

#ifdef _WIN64
typedef       unsigned long long  size;
#else
typedef       unsigned int        size;
#endif

typedef                void *     vptr;
typedef const          void *    cvptr;

#define IGNORE -1
#define FALSE 0
#define TRUE 1

#define S08_MIN    0x80
#define S16_MIN    0x8000
#define S32_MIN    0x80000000
#define S64_MIN    0x8000000000000000
#define S08_MAX    0x7F
#define S16_MAX    0x7FFF
#define S32_MAX    0x7FFFFFFF
#define S64_MAX    0x7FFFFFFFFFFFFFFF
#define U08_MAX  0xFF
#define U16_MAX 0xFFFF
#define U32_MAX 0xFFFFFFFF
#define U64_MAX 0xFFFFFFFFFFFFFFFF
#define R32_MAX 3.40282e38
#define R32_MIN -1.17549e38

#define local_var  static
#define local_func static
#define global_var static
#define global_func

#ifdef _VOX_DEBUG
#define Assert(Condition) if(!(Condition)) (*(int*)0 = 0)
#else
#define Assert(Condition)
#endif

#define Kilobytes(Count) ((Count) * 1024ULL)
#define Megabytes(Count) (Kilobytes(Count) * 1024ULL)
#define Gigabytes(Count) (Megabytes(Count) * 1024ULL)
#define Terabytes(Count) (Gigabytes(Count) * 1024ULL)

#define OffsetOf(Struct, Member) ((size_t)&(((Struct*)0)->Member))

#define Align16(Num) (((Num) + 15) & ~15)

#define FILENAME_FONTS "fonts"

#define VOX_DEFINES_H_
#endif