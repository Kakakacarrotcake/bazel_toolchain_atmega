/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef sinhl

long double sinhl(long double x)
{
    return (long double)sinhf((float)x);
}
