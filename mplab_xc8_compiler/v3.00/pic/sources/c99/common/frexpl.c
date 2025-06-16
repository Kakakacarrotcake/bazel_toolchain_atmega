/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef frexpl

long double frexpl(long double x, int *y)
{
    return (long double)frexpf((float)x, y);
}
