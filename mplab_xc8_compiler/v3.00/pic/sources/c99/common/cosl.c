/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef cosl

long double cosl(long double x)
{
    return (long double)cosf((float)x);
}
