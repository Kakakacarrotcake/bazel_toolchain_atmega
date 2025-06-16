/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef remainderl

long double remainderl(long double x, long double y)
{
    return (long double)remainderf((float)x, (float)y);
}
