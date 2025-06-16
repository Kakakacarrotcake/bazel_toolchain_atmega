/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef expm1

double expm1(double x)
{
    return (double)expm1f((float)x);
}
