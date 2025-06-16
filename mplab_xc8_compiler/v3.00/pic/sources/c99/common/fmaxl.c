/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef fmaxl

long double fmaxl(long double x, long double y)
{
    return (long double)fmaxf((float)x, (float)y);
}
