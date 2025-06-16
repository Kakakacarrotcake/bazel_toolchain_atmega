/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef asinl

long double asinl(long double x)
{
    return (long double)asinf((float)x);
}
