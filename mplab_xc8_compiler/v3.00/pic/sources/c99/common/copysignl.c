/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef copysignl

long double copysignl(long double x, long double y)
{
    return (long double)copysignf((float)x, (float)y);
}
