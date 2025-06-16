/* Route "" ("d") and "l" sizes of math functions to "f" versions */
#include <math.h>
#undef tanl

long double tanl(long double x)
{
    return (long double)tanf((float)x);
}
