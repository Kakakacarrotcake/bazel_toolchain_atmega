/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef sinl

long double sinl(long double x)
{
    return (long double)sinf((float)x);
}
