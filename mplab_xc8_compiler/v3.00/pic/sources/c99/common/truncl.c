/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef truncl

long double truncl(long double x)
{
    return (long double)truncf((float)x);
}
