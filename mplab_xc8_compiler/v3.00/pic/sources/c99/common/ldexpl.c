/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef ldexpl

long double ldexpl(long double x, int y)
{
    return (long double)ldexpf((float)x, y);
}
