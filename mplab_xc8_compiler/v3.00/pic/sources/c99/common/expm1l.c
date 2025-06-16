/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef expm1l

long double expm1l(long double x)
{
    return (long double)expm1f((float)x);
}
