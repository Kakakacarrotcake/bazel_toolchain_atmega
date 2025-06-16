/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef log10l

long double log10l(long double x)
{
    return (long double)log10f((float)x);
}
