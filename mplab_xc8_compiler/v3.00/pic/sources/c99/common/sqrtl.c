/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef sqrtl

long double sqrtl(long double x)
{
    return (long double)sqrtf((float)x);
}
