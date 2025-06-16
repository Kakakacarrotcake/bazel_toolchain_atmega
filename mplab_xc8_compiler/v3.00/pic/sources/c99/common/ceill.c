/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef ceill

long double ceill(long double x)
{
    return (long double)ceilf((float)x);
}
