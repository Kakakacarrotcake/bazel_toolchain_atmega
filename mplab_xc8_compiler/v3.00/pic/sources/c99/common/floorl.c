/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef floorl

long double floorl(long double x)
{
    return (long double)floorf((float)x);
}
