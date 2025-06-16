/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef cbrtl

long double cbrtl(long double x)
{
    return (long double)cbrtf((float)x);
}
