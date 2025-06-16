/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef atanl

long double atanl(long double x)
{
    return (long double)atanf((float)x);
}
