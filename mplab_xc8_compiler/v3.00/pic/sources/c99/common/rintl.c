/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef rintl

long double rintl(long double x)
{
    return (long double)rintf((float)x);
}
