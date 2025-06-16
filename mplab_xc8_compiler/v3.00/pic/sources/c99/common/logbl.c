/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef logbl

long double logbl(long double x)
{
    return (long double)logbf((float)x);
}
