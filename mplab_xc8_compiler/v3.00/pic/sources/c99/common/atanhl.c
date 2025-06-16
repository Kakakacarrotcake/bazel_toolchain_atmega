/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef atanhl

long double atanhl(long double x)
{
    return (long double)atanhf((float)x);
}
