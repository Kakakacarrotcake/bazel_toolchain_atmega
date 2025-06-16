/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef fdiml

long double fdiml(long double x, long double y)
{
    return (long double)fdimf((float)x, (float)y);
}
