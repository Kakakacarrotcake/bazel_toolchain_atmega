/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef asinhl

long double asinhl(long double x)
{
    return (long double)asinhf((float)x);
}
