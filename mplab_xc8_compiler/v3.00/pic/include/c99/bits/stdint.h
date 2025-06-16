typedef int16_t int_fast16_t;
typedef int32_t int_fast32_t;
typedef uint16_t uint_fast16_t;
typedef uint32_t uint_fast32_t;

#define INT_FAST16_MIN  INT16_MIN
#define INT_FAST32_MIN  INT32_MIN

#define INT_FAST16_MAX  INT16_MAX
#define INT_FAST32_MAX  INT32_MAX

#define UINT_FAST16_MAX UINT16_MAX
#define UINT_FAST32_MAX UINT32_MAX

#ifdef _PIC18
#define INTPTR_MIN      INT24_MIN
#define INTPTR_MAX      INT24_MAX
#define UINTPTR_MAX     UINT24_MAX
#else
#define INTPTR_MIN      INT16_MIN
#define INTPTR_MAX      INT16_MAX
#define UINTPTR_MAX     UINT16_MAX
#endif
#define PTRDIFF_MIN     INT16_MIN
#define PTRDIFF_MAX     INT16_MAX
#define SIZE_MAX        UINT16_MAX
