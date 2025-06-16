/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef lrintl

long lrintl(long double x)
{
    return lrintf((float)x);
}
