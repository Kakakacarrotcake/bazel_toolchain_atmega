/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef fabsl

long double fabsl(long double x)
{
    return (long double)fabsf((float)x);
}
